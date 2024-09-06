/**
 * Device interface for the CS1521 Geofencing Device
 *
 * This module implements a MODBUS master that is used to monitor
 * the device (the MODBUS slave) via a MODBUS-RTU uart interface
 *
 * Use  _Initialize at powerup
 *		_Open when you want to open the port
 *		_MessageAvailable, _ProcessMessage, and _DiscardMessage for incoming
 *		messages
 *		_Tasks for miscellaneous tasks
 *
 * Copyright (c) 2018 Control Solutions LLC. All Rights Reserved
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "app.h"
#include "cbuf.h"
#include "modbus.h"

//------------------------------//----------------------------------------------
// Definitions

// The number of dynamic MODBUS commands we can queue for processing.  This
// needs to be a power of 2, due to the way the CBUF macros work.
#define commandQ_SIZE 2

// The max application data unit is sized to hold approximately
// a 250-byte PDU plus the MBAP header
#define MAX_ADU_LENGTH ( 250 + 7 )

// Define a circular buffer to hold command transactions (compatible
// with the CBUF macros)
typedef struct
{
    uint8_t m_getIdx;
    uint8_t m_putIdx;
    uint8_t *m_entry[ commandQ_SIZE ];
} commandQ_t;

extern commandQ_t commandQ;

// Defines the standard MODBUS header (MBAP) used for dynamic transactions
typedef struct
{
    // Transaction ID which is just echoed back to the sender
    uint16_t transaction;

    // Protocol indicator, should always be zero
    uint16_t protocol;

    // 2 bytes indicating number of bytes following the first 6 in the message
    uint8_t lengthMsb;
    uint8_t lengthLsb;

    // Slave unit ID
    uint8_t unit;

    // MODBUS function code
    uint8_t function;
} MbapHeader_t;

//------------------------------//----------------------------------------------
// Global variables

//------------------------------//----------------------------------------------
// Public functions

/**
 * Powerup initialization.  Call once at system start
 */
bool DEVICE_Initialize( void );

/**
 * Task runner. Call this periodically in a task loop
 */
void DEVICE_Tasks( void );

/**
 * Open the device interface
 *
 * @return boolean true if no error
 */
bool DEVICE_Open( uint32_t baud );

/**
 * Close the device port
 */
void DEVICE_Close( void );

bool DEVICE_Send( MbapHeader_t *pMsg );

//------------------------------//----------------------------------------------
// Callback functions.  These need to be implemented by the application

/**
 * Success callback for dynamic command
 *
 * @param      pResponse  The response
 */
void DEVICECB_SendResponse( MbapHeader_t *pResponse );

/**
 * Callback upon receipt of status message
 *
 * @param      p  updated status
 */
void DEVICECB_StatusResponse( ModbusRtuPdu_t *pPdu, uint8_t len );

#endif
