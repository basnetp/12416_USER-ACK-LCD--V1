 
/**
 * Implements a MODBUS-RTU Master for monitoring a CS1521 geofencing device
 *
 * OPERATION
 * The master continually polls the slave device.  It uses two types of
 * transactions - 'static' transactions and 'dynamic' transactions.
 *
 * A static transaction comes from a list of requests,
 * and are polled in round robin fashion as long as there are no dynamic transaction requests.
 *
 * A dynamic transaction is a 'one time' request.  Dynamic transactions have higher priority than static transactions,
 * and are serviced as soon as the next opportunity arises.
 *
 * DISCONNECT
 * Currently no detection of a disconnected board is implemented
 *
 * References:
 */
#include "app.h"
#include "device.h"
#include "modbus.h"
//#include "timer.h"
#include "system.h"
#include "string.h"
//------------------------------//----------------------------------------------
// Definitions

// undefine DEBUG for this module, to save space when debugging is
// not needed
#undef DEBUG

#define RX_BUFFER_SIZE 128

// modbus slave IDs for devices on the bus
#define ID_SLAVE 1

// timeout in ms for a modbus command
#define DEFAULT_TIMEOUT 100

// data Object IDs to read and/or write to the device
#define OBJ_CONFIG 0x00
#define OBJ_STATUS 0x01
#define OBJ_SATS 0x02

// Transaction response functions should be defined as RESPONSE_FUNC_t
typedef void ( *RESPONSE_FUNC_t )( ModbusRtuPdu_t *, uint8_t );

// Transaction failure functions should be defined as FAIL_FUNC_t
typedef void ( *FAIL_FUNC_t )();

// A structure to keep track of a transaction request (static or dynamic)
typedef struct
{
    // length of the buffer in .pdu
    uint8_t pduLength;

    // number of times to try the message.  we can lose a message
    // while starting up and syncing the tunneling sequence ID
    // or due to noise on the bus (for instance)
    uint8_t retries;

    // the function to call when we receive a response
    RESPONSE_FUNC_t pfnResponse;

    // the function to call if the transaction fails
    FAIL_FUNC_t pfnFail;

    // The pdu to send
    uint8_t *pdu;

} ModbusTransaction_t;

//------------------------------//----------------------------------------------
// Global variables

//------------------------------//----------------------------------------------
// Local variables

// collects the MODBUS data from the serial port
//static char _rxBuffer[ RX_BUFFER_SIZE ];
//static uint16_t _rxBufferIndex;

// keep track of when we started a transaction, for triggering timeouts
TIMER_TICK _transactionStarted;

//------------------------------//----------------------------------------------
// Local functions

// MODBUS response handlers for static messages
static void _HandleDynamicResponse( ModbusRtuPdu_t *pPdu, uint8_t len );
static void _HandleDynamicFailure( void );

//------------------------------//----------------------------------------------
// Global variables

// Storage for MODBUS transaction queue
uint8_t transaction[ commandQ_SIZE ][ MAX_ADU_LENGTH ];

// storage for the CBUF structure that forms the transaction queue
commandQ_t commandQ = {0, 0, {&transaction[ 0 ][ 0 ], &transaction[ 1 ][ 0 ]}};

//------------------------------//----------------------------------------------
// Local variables

// Points to the current transaction we are trying to complete via the
// SLAVE_COMMAND function code
static ModbusTransaction_t *_pCurrentTransaction;

// Holds the active dynamic transaction, if any
static ModbusTransaction_t _theDynamicTransaction = {
    // pduLength
    0,

    // retries
    3,

    // pfnResponse
    _HandleDynamicResponse,

    // pfnFail
    _HandleDynamicFailure,

    // pdu
    NULL};

// A PDU to read the status
static uint8_t _pduReadStatus[ 3 ] = {
    ID_SLAVE,
    MODBUS_READ_OBJECT,  // function code
    OBJ_STATUS,          // object is
};

// Transaction to retrieve the status
static ModbusTransaction_t _readStatus = {
    // pduLength
    sizeof( _pduReadStatus ),

    // retries
    0,

    // pfnResponse
    DEVICECB_StatusResponse,

    // pfnFail
    // no failure handling since we will just try again
    NULL,

    // the PDU
    _pduReadStatus};

// Define the static list of MODBUS commands to send.
// If there are no dynamic commands waiting, these commands
// are sent in round-robin fashion
#define NUM_STATIC_COMMANDS 1

static ModbusTransaction_t *_staticCommands[ NUM_STATIC_COMMANDS ] = {&_readStatus};

// Keep track of which static command is being processed
static uint8_t _staticCommandIndex;

//------------------------------//----------------------------------------------
// Local functions

/**
 * @brief      Retrieve a pointer to the next command from the queue
 *
 *             Does not remove the command from the queue, and space in the
 *             queue is not released by this function.
 *
 * @return     The next command or NULL if there aren't any
 */
static MbapHeader_t *_GetNextCommand( void )
{
    if( !CBUF_IsEmpty( commandQ ) )
    {
        uint8_t **pp = CBUF_GetPopEntryPtr( commandQ );

        // *pp points to a flat buffer of length MAX_ADU_LENGTH,
        // containing
        // a MODBUS message
        return (MbapHeader_t *)*pp;
    }

    // no more commands
    return NULL;
}

/**
 * @brief      Gets the next dynamic transaction from the queue
 *
 * @return     The next dynamic transaction or NULL if none.
 */
static ModbusTransaction_t *_GetNextDynamicCommand( void )
{

    MbapHeader_t *p = _GetNextCommand();

    if( p )
    {
        _theDynamicTransaction.pduLength = p->lengthLsb;
        _theDynamicTransaction.pdu = &( p->unit );

        // TRACE( ( "DynamicNext: len %u fn %u \r\n", p->lengthLsb, p->function ) );

        return &_theDynamicTransaction;
    }

    return NULL;
}

/**
 * @brief      Gets the next static transaction from the queue
 *
 * @return     The next static transaction or NULL if none.
 */
static ModbusTransaction_t *_GetNextStaticCommand( void )
{
    ModbusTransaction_t *p = _staticCommands[ _staticCommandIndex ];
    
    
    _staticCommandIndex++;
    if( _staticCommandIndex >= NUM_STATIC_COMMANDS )
    {
        // skip command zero after the first time (read slave id just once )
        _staticCommandIndex = 0;
    }

    return p;
}

/**
 * @brief      Handles an incoming PDU in response to a Dynamic transaction
 *
 * @param      pPdu  The pdu
 * @param[in]  len   The length of the buffer
 */
static void _HandleDynamicResponse( ModbusRtuPdu_t *pPdu, uint8_t len )
{
    MbapHeader_t *p = _GetNextCommand();

    // Make sure there is a command to respond to, and the incoming
    // length is at least long enough for an id and function code
    if( p && len > 1 )
    {

        // TRACE( ( "Dynamic: %u %u %u\r\n", len, pPdu->id, pPdu->function ) );

        if( pPdu->id == p->unit && pPdu->function == p->function )
        {

            // pointer to data after the header
            uint8_t *pData = (uint8_t *)p + sizeof( MbapHeader_t );

            // length of message is length of buffer minus unit id, function code
            p->lengthMsb = 0;
            p->lengthLsb = len - 2;
            uint8_t *pSrc = &pPdu->data[ 0 ];

            memcpy( pData, pSrc, len - 2 );

            DEVICECB_SendResponse( p );

            CBUF_AdvancePopIdx( commandQ );
        }
        else
        {
            //TRACE( ( "unmatched dynamic pdu\r\n" ) );
        }
    }
}

static void _HandleDynamicFailure( void )
{
    MbapHeader_t *p = _GetNextCommand();

    // If there is a current command
    if( p )
    {
        // pointer to data after the header
        uint8_t *pData = (uint8_t *)p + sizeof( MbapHeader_t );

        // TRACE( ( "Failing Dynamic.\r\n" ) );

        // add error bit to the function code
        p->function |= 0x80;
        // add error code (slave error)
        *pData = 0x04;

        // length of message is unit id, function code, error code
        p->lengthMsb = 0;
        p->lengthLsb = 3;

        // Respond to the client
        DEVICECB_SendResponse( p );

        CBUF_AdvancePopIdx( commandQ );
    }
}

/**
 * @brief      Looks for the next command (static or dynamic) and sends it
 */
static void _SendNextCommand( void )
{
    // If no current transaction, get the next one from the dynamic queue
    if( NULL == _pCurrentTransaction )
    {
        _pCurrentTransaction = _GetNextDynamicCommand();
    }

    // If still no current transaction, get the next one from the static queue
    if( NULL == _pCurrentTransaction )
    {
        _pCurrentTransaction = _GetNextStaticCommand();
    }

    if( _pCurrentTransaction )
    {
        // write the ADU for the current transaction, which might be
        // a repeat or a new command
        MODBUS_SendBuffer( _pCurrentTransaction->pdu, _pCurrentTransaction->pduLength );
        _transactionStarted = TIMER_Now();
    }
}

/**
 * Handles an incoming message
 *
 * @param pPdu pointer to the MODBUS PDU
 * @param len number of bytes in tunneled PDU, including the 2-byte checksum
 */
static void _HandleMessage( ModbusRtuPdu_t *pPdu, uint8_t len )
{

    // If we have an active transaction
    if( _pCurrentTransaction )
    {

        // If the message matches our current transaction, process it
        if( _pCurrentTransaction->pdu[ 1 ] == ( pPdu->function & 0x7F ) && _pCurrentTransaction->pdu[ 0 ] == pPdu->id )
        {
            // if there's a handler, call it with the response PDU
            if( _pCurrentTransaction->pfnResponse )
            {
                _pCurrentTransaction->pfnResponse( pPdu, len );
            }
            //else
            //    printf("X");

            // we are done with this transaction
            _pCurrentTransaction = NULL;
        }
    }
}

//------------------------------//----------------------------------------------
// Public functions

/**
 * Powerup initialization, called once at system start
 *
 * @return boolean true
 */
bool DEVICE_Initialize( void )
{

    CBUF_Init( commandQ );

    // Don't open the port here, wait until _Open())
    return true;
}

/**
 * Maintenance tasks
 *
 * Called periodically from the main task loop
 */
void DEVICE_Tasks( void )
{
    if( MODBUS_MessageAvailable() )
    {
        _HandleMessage( (ModbusRtuPdu_t *)MODBUS_rxBuffer, MODBUS_rxBufferIndex );
        //printf("X");
        MODBUS_DiscardMessage();
    }

    else if( _pCurrentTransaction && TIMER_Since( _transactionStarted ) > COMMAND_TIMEOUT )
    {
        // no response...
        if( _pCurrentTransaction->retries > 0 )
        {
            _pCurrentTransaction->retries--;
            _SendNextCommand();
        }
        else
        {
            // we are out of retries; fail
            if( _pCurrentTransaction->pfnFail )
            {
                _pCurrentTransaction->pfnFail();
                _pCurrentTransaction = NULL;
            }
        }
    }

    else if( NULL == _pCurrentTransaction )
    {
        if(TIMER_Since( _transactionStarted ) > COMMAND_TIMEOUT)
        {// start the next transaction every 1000ms
            _SendNextCommand();
        }
    }
}

/**
 * Activates this module and opens the UART port
 *
 *
 * @return boolean true on success
 */
bool DEVICE_Open( uint32_t baud )
{
    MODBUS_Open( (UartBaud_e)baud );

    // There is no current transaction
    _pCurrentTransaction = NULL;

    // index of the first static command to send
    _staticCommandIndex = 0;

    return true;
}

/**
 * Sends a dynamic MODBUS request to the controller
 *
 *
 * @return boolean true if there is room in the queue
 */
bool DEVICE_Send( MbapHeader_t *pMsg )
{
    uint16_t length = (uint16_t)pMsg->lengthMsb << 8;
    length |= pMsg->lengthLsb;

    length += 6;  // to account for MBAP header bytes in excess of stated length

    if( !CBUF_IsFull( commandQ ) )
    {
        // retrieve a pointer to the next queue entry but
        // don't advance the push index so the consumer doesn't
        // realize the data is there yet.
        // Basically this is a 'reservation' on the queue entry
        // so we can collect data until the message is complete.

        // (this is a pointer to a pointer)
        uint8_t **pp = CBUF_GetPushEntryPtr( commandQ );

        // Copy the message data to the queue
        memcpy( *pp, pMsg, length );

        // advance the push index so the consumer knows the
        // message is ready to handle
        CBUF_AdvancePushIdx( commandQ );

        return true;
    }

    return false;
}

/**
 * Deactivates this module
 *
 *
 */
void DEVICE_Close( void )
{
    // Release the UART port
    MODBUS_Close();
}
