/**
 * Implements the UART Driver for PIC24
 *
 * See uart.h
 *
 * Copyright(c) 2018 Control Solutions LLC.  All Rights Reserved
 */

#include "uart.h"
#include "system.h"

//------------------------------//----------------------------------------------
// Definitions
//#define FCY SYS_CLK_FrequencyPeripheralGet()

//------------------------------//----------------------------------------------
// Global variables

#ifdef ENABLE_UART1

// Queues to store incoming and outgoing bytes
volatile Uart1RxQueue_t UART1_rxQ;
volatile Uart1TxQueue_t UART1_txQ;

#endif

#ifdef ENABLE_UART2

// Queues to store incoming and outgoing bytes
volatile Uart2RxQueue_t UART2_rxQ;
volatile Uart2TxQueue_t UART2_txQ;

#endif

//------------------------------//----------------------------------------------
// Local variables

#ifdef ENABLE_UART1

#define BRG1H_VAL 1
#define BRG1_DIV 4  // this is 16 when BRGH is 0, and 4 when it is 1, per datasheet
#define BAUDRATEREG1 ( FCY / ( UART1_BAUD * BRG1_DIV ) ) - 1
#define BAUD1_ACTUAL FCY / ( BRG1_DIV * ( BAUDRATEREG1 + 1 ) )
#define BAUD1_ERROR ( ( BAUD1_ACTUAL > UART1_BAUD ) ? BAUD1_ACTUAL - UART1_BAUD : UART1_BAUD - BAUD1_ACTUAL )
#define BAUD1_ERROR_PERCENT ( BAUD1_ERROR / UART1_BAUD ) * 100

#if( BAUDRATEREG1 ) > 255
#error "Calculated BAUDRATEREG1 value is out of range for current FCY."
#endif

/**
 * Powerup initialization of the UART driver
 *
 */
bool UART1_Initialize( void )
{
    // Circular buffer init & flush
    CBUF_Init( UART1_txQ );
    CBUF_Init( UART1_rxQ );

    //  Set the UART Baud Rate and Mode registers.

    U1BRG = BAUDRATEREG1;
    U1MODE = 0;
    U1MODEbits.BRGH = BRG1H_VAL;

    //  Clear the UART Status register.
    U1STA = 0;
    
    //Added 1/28/20
    U1STAbits.URXISEL = 0x0;//0b11   //UART interrupt generated whenever a character is received.
    
    //  Clear the TX Interrupt source flag bits.
    IFS0bits.U1TXIF = 0;
    //  Clear the RX Interrupt source flag bits.
    IFS0bits.U1RXIF = 0;    //Added 1/28 by V.V
    
    //  Set the Interrupt source priority. TX is priority level 2.
    IPC3bits.U1TXIP = 2;
    //  Set the Interrupt source priority. TX is priority level 2.
    IPC2bits.U1RXIP = 2;    //Added 1/28 by V.V
    
    //  Enable the UART Transmitter, Receiver.
    U1MODEbits.UARTEN = 1;

    // Enable Interrupts
    IEC0bits.U1RXIE = 1;

    //  Enable the UART.
    U1STAbits.UTXEN = 1;

    return true;
}

/**
 * Transmit a byte
 *
 */
void UART1_Put( uint8_t c )
{
    if( !CBUF_IsFull( UART1_txQ ) )
    {
        CBUF_Push( UART1_txQ, c );

        // kick the transmitter to pull data out of the queue and send it
        IEC0bits.U1TXIE = true;
    }
}

/**/
 void UART_Poll()
 {
    //Maybe wait 20ms?
 }

 void UART1_PutString( const char *str )
 {
   //char *p = (char *)buffer;
       //unsigned int i;

       while(*str != 0)//; i++ )
       {
           UART1_Put( *str++ );
       }
 }
 
/* 
// Returns true if the transmit queue is not full
// (there is room to write at least one more byte)
bool UART_IsPutReady( void )
{
    return true;
}
 
// Returns true if the receive queue is not empty
//(there is at least one byte waiting to be read)
bool UART_IsGetReady( void )
{
    return !(CBUF_IsFull( UART1_rxQ ));
}
*/
/**
 * ISR for transmit interrupts
 */
void __attribute__( ( __interrupt__, auto_psv ) ) _U1TXInterrupt( void )
{
    // If no more to send, disable interrupt
    if( CBUF_IsEmpty( UART1_txQ ) )
    {
        IEC0bits.U1TXIE = false;
        return;
    }

    // Clear the interrupt flag
    IFS0bits.U1TXIF = false;

    // Put more byte(s) into the FIFO
    while( !( U1STAbits.UTXBF == 1 ) && !CBUF_IsEmpty( UART1_txQ ) )
    {
        U1TXREG = CBUF_Pop( UART1_txQ );
    }
}

/**
 * ISR for receive interrupts
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{

    while( ( U1STAbits.URXDA == 1 ) )
    {
        // Read the register, which clears it
        uint8_t c = U1RXREG;

        if( !CBUF_IsFull( UART1_rxQ ) )
        {
            CBUF_Push( UART1_rxQ, c );
        }
    }

    IFS0bits.U1RXIF = false;
}

/**
 * ISR for UART errors
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _U1ErrInterrupt( void )
{
    if( ( U1STAbits.OERR == 1 ) )
    {
        U1STAbits.OERR = 0;
    }

    IFS4bits.U1ERIF = false;
}

#endif

#ifdef ENABLE_UART2

#define BRG2H_VAL 0
#define BRG2_DIV 16  // this is 16 when BRGH is 0, per datasheet
#define BAUDRATEREG2 ( FCY / ( UART2_BAUD * BRG2_DIV ) ) - 1
#define BAUD2_ACTUAL FCY / ( BRG2_DIV * ( BAUDRATEREG2 + 1 ) )
#define BAUD2_ERROR ( ( BAUD2_ACTUAL > UART2_BAUD ) ? BAUD2_ACTUAL - UART2_BAUD : UART2_BAUD - BAUD2_ACTUAL )
#define BAUD2_ERROR_PERCENT ( BAUD2_ERROR / UART2_BAUD ) * 100

#if( BAUDRATEREG2 ) > 255
#error "Calculated BAUDRATEREG2 value is out of range for current FCY."
#endif

/**
 * Powerup initialization of the UART driver
 *
 */
bool UART2_Initialize( void )
{
    // Circular buffer init & flush
    CBUF_Init( UART2_txQ );
    CBUF_Init( UART2_rxQ );

    //  Set the UART Baud Rate and Mode registers.

    U2BRG = BAUDRATEREG2;
    U2MODE = 0;
    U2MODEbits.BRGH = BRG2H_VAL;

    //  Clear the UART Status register.
    U2STA = 0;

    //  Clear the TX Interrupt source flag bits.
    IFS1bits.U2TXIF = 0;

    //  Set the Interrupt source priority. TX is priority level 2.
    IPC7bits.U2TXIP = 2;

    //  Enable the UART Transmitter.
    U2MODEbits.UARTEN = 1;

    // Enable Interrupts
    IEC1bits.U2RXIE = 1;
    //    IEC1bits.U2TXIE = 1;

    //  Enable the UART.
    U2STAbits.UTXEN = 1;

    return true;
}

/**
 * Transmit a byte
 *
 */
void UART2_Put( uint8_t c )
{
    // if no room in the buffer, wait
    while( CBUF_IsFull( UART2_txQ ) )
    {
    }

    if( !CBUF_IsFull( UART2_txQ ) )
    {
        CBUF_Push( UART2_txQ, c );

        // kick the transmitter to pull data out of the queue and send it
        IEC1bits.U2TXIE = true;
    }
}

void UART2_PutString( const char *str )
 {
   //char *p = (char *)buffer;
       //unsigned int i;

       while(*str != 0)//; i++ )
       {
           UART2_Put( *str++ );
       }
 }

/**
 * ISR for transmit interrupts
 */
void __attribute__( ( __interrupt__, auto_psv ) ) _U2TXInterrupt( void )
{
    // If no more to send, disable interrupt
    if( CBUF_IsEmpty( UART2_txQ ) )
    {
        IEC1bits.U2TXIE = false;
        return;
    }

    // Clear the interrupt flag
    IFS1bits.U2TXIF = false;

    // Put more byte(s) into the FIFO
    while( !( U2STAbits.UTXBF == 1 ) && !CBUF_IsEmpty( UART2_txQ ) )
    {
        U2TXREG = CBUF_Pop( UART2_txQ );
    }
}

/**
 * ISR for receive interrupts
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _U2RXInterrupt( void )
{

    while( ( U2STAbits.URXDA == 1 ) )
    {

        if( !CBUF_IsFull( UART2_rxQ ) )
        {
            // Read the register, which clears it
            uint8_t c = U2RXREG;

            CBUF_Push( UART2_rxQ, c );
        }
    }

    IFS1bits.U2RXIF = false;
}

/**
 * ISR for UART errors
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _U2ErrInterrupt( void )
{
    if( ( U2STAbits.OERR == 1 ) )
    {
        U2STAbits.OERR = 0;
    }

    IFS4bits.U2ERIF = false;
}

#endif
