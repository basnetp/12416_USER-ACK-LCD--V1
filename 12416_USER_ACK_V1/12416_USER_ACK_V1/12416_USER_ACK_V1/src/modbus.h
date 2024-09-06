/*
 * Copyright (c) 2015 Control Solutions LLC. All Rights Reserved
 */

#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "uart.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------//----------------------------------------------
// Definitions

#define MODBUS_READ_HOLDING_REGISTERS 0x03
#define MODBUS_READ_INPUT_REGISTERS 0x04
#define MODBUS_REPORT_SLAVEID 0x11

#define MODBUS_READ_OBJECT 0x43
#define MODBUS_WRITE_OBJECT 0x44

#define MODBUS_COMMAND 0x47

// biggest message we can send or receive
#define MODBUS_MAX_ADU 255
#define MODBUS_EOF_MS 10

typedef struct
{
    uint8_t id;
    uint8_t function;

    // allows access to variable length data field
    uint8_t data[ MODBUS_MAX_ADU - 2 ];

} ModbusRtuPdu_t;

typedef struct
{
    uint8_t id;
    uint8_t function;
    uint8_t bytes;
    uint8_t product;
    uint8_t run;
    uint8_t versionMajor;
    uint8_t versionMinor;
    uint8_t versionBuild;
    uint8_t serial[ 4 ];
    uint8_t productId[ 4 ];

} ModbusReportSlaveIdResponse_t;



typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_19200 = 19200,
    UART_BAUD_57600 = 57600,
    UART_BAUD_115200 = 115200
} UartBaud_e;

//------------------------------//----------------------------------------------
// Global variables

// buffer that collects the incoming message
extern uint8_t MODBUS_rxBuffer[ MODBUS_MAX_ADU ];
extern uint8_t MODBUS_rxBufferIndex;

//------------------------------//----------------------------------------------
// Public functions

/**
 * Powerup initialization
 *
 * Call once at system start
 */
void MODBUS_Initialize( void );

/**
 * Maintenance tasks loop
 *
 * Call periodically from main application loop
 */
void MODBUS_Tasks( void );

/**
 * Opens the UART port using the specified baud rate
 */
bool MODBUS_Open( UartBaud_e baud );

/**
 * Close the serial port
 */
void MODBUS_Close( void );

/**
 * Sends a MODBUS ADU
 *
 * @param id the MODBUS device ID
 * @param function the MODBUS function code
 * @param pData pointer to message data
 * @param length number of bytes in pData
 *
 * @return true always
 */
bool MODBUS_Send( uint8_t id, uint8_t function, uint8_t *pData, uint8_t length );

/**
 * Sends a MODBUS ADU from a buffer
 *
 * Loads the ADU into the outgoing UART buffer.  The buffer
 * should contain the whole message EXCEPT the crc, which will
 * be added by this function
 */
bool MODBUS_SendBuffer( uint8_t *pData, uint8_t length );

/**
 * Interface to check if there is a complete incoming MODBUS message
 *
 * This function reads from the UART, accumulates the ADU, and validates
 * the checksum and end of frame.
 * If it returns true, it means there is a complete MODBUS message in the
 * UART_rxBuffer
 */
bool MODBUS_MessageAvailable( void );

/**
 * Clear the incoming buffer
 *
 */
void MODBUS_DiscardMessage( void );

#endif
