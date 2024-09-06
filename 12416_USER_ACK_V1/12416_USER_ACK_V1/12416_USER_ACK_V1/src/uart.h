/**
 * UART Driver for PIC24
 *
 * The receive and transmit queues are managed as circular buffers.  The
 * size of the buffers must be defined via macros in the 'app.h'.
 *
 * The size macro names have to match the queue variable names plus _SIZE
 * because of the way the CBUF library works.
 * It requires that the circular buffer size be a power of two, and the
 * size of the buffer needs to smaller than the index. So an 8 bit index
 * supports a circular buffer upto ( 1 << 7 ) = 128 entries, and a 16 bit index
 * supports a circular buffer upto ( 1 << 15 ) = 32768 entries.
 *
 * Copyright (c) 2015 Control Solutions LLC. All Rights Reserved
 */

#ifndef _UART_H_
#define _UART_H_

//#include "../12416_Rev_D_100.X/src/app.h"
#include "app.h"
#include "cbuf.h"

//------------------------------//----------------------------------------------
// Definitions

#ifdef ENABLE_UART1
// define the queues to hold receive and transmit bytes
typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ UART1_txQ_SIZE ];

} Uart1TxQueue_t;

typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ UART1_rxQ_SIZE ];

} Uart1RxQueue_t;

extern volatile Uart1RxQueue_t UART1_rxQ;
extern volatile Uart1TxQueue_t UART1_txQ;

// Returns true if the transmit queue is not full
// (there is room to write at least one more byte)
#define UART1_IsPutReady() ( !CBUF_IsFull( UART1_txQ ) )

// Returns true if the receive queue is not empty
//(there is at least one byte waiting to be read)
#define UART1_IsGetReady() ( !CBUF_IsEmpty( UART1_rxQ ) )

// Returns the number of bytes in the transmit queue
#define UART1_TxLen() ( CBUF_Len( UART1_txQ ) )

// Returns the number of bytes in the receive queue
#define UART1_RxLen() ( CBUF_Len( UART1_txQ ) )

/**
 * Initializes the console
 *
 * This initializes / clears the Console
 *
 */
bool UART1_Initialize( void );

/**
 * Send a character out to the console.
 *
 * @param c the byte to be output
 */
void UART1_Put( uint8_t c );

/**
 * Send a string out to the console.
 *
 * @param str the string to be output
 */
void UART1_PutString( const char *str );


/**
 * Retrieve the received byte from the UART
 *
 * Call this when CONSOLE_IsGetReady() returns true
 *
 * @return byte retrieved from UART
 */
#define UART1_Get() CBUF_Pop( UART1_rxQ )

/*
 UART functions added from Arduino GPS sample code
 */
void UART_Poll(void);
//bool UART_IsPutReady( void );
//bool UART_IsGetReady( void );


#endif

#ifdef ENABLE_UART2
// define the queues to hold receive and transmit bytes
typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ UART2_txQ_SIZE ];

} Uart2TxQueue_t;

typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ UART2_rxQ_SIZE ];

} Uart2RxQueue_t;

extern volatile Uart2RxQueue_t UART2_rxQ;
extern volatile Uart2TxQueue_t UART2_txQ;

// Returns true if the transmit queue is not full
// (there is room to write at least one more byte)
#define UART2_IsPutReady() ( !CBUF_IsFull( UART2_txQ ) )

// Returns true if the receive queue is not empty
//(there is at least one byte waiting to be read)
#define UART2_IsGetReady() ( !CBUF_IsEmpty( UART2_rxQ ) )

// Returns the number of bytes in the transmit queue
#define UART2_TxLen() ( CBUF_Len( UART2_txQ ) )

// Returns the number of bytes in the receive queue
#define UART2_RxLen() ( CBUF_Len( UART2_txQ ) )

/**
 * Initializes the console
 *
 * This initializes / clears the Console
 *
 */
bool UART2_Initialize( void );

/**
 * Send a character out to the console.
 *
 * @param c the byte to be output
 */
void UART2_Put( uint8_t c );

/**
 * Send a string out to the console.
 *
 * @param str the string to be output
 */
void UART2_PutString( const char *str );

/**
 * Retrieve the received byte from the UART
 *
 * Call this when CONSOLE_IsGetReady() returns true
 *
 * @return byte retrieved from UART
 */
#define UART2_Get() CBUF_Pop( UART2_rxQ )

#endif

#endif
