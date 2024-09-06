/**
 * I2C Interface for CS1108-type motor controllers
 *
 * Implements an I2C slave interface to the controller.
 *
 *
 * Copyright (c) 2015 Control Solutions LLC.  All rights reserved.
 */

#ifndef CS1108_H
#define CS1108_H

#include <stdbool.h>
#include <stdint.h>

//------------------------------------//--------------------------------------//
// Definitions

//------------------------------------//--------------------------------------//
// Types

/**
 * Defines the ULTA mode MISC word (0x4C)
 */
typedef union {
    unsigned char byte;
    struct
    {
        unsigned temp_led : 1;   // Active high for LED on
        unsigned indoor : 1;     // Active high for indoor
        unsigned low_volts : 1;  // Active high for low volts
        unsigned brk_rel : 1;    // Active high for brk_rel
        unsigned q_stopn : 1;    // Active high for quick stop
        unsigned chg_mode : 1;   // Active high for charge mode
        unsigned reverse : 1;
        unsigned : 1;  // MSB
    } bits;
} _i2c_misc_w;

/**
 * Defines the array that is WRITTEN to the display in ULTA mode
 */
typedef union {
    uint8_t b_value[ 8 ];
    struct
    {
        uint8_t spare1;
        uint8_t spare2;
        uint8_t spare3;
        uint8_t led_data;
        uint8_t mode_data;
        uint8_t aux_data;
        _i2c_misc_w misc_data;
        uint8_t spare;
    };
} _ulta_wa;

#define I2C_rxQ_SIZE 128
typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ I2C_rxQ_SIZE ];

} I2CQueue_t;

typedef struct {
    uint8_t addr_bytes[2];
    //uint8_t addr_byte2;
    uint8_t rx_data;
}read_packet;

typedef struct {
      read_packet fault, 
              runtime_Hbyte,
              runtime_Lbyte,
              NoFloats, 
              lowbatt;
}CS_read_data; 



//------------------------------//----------------------------------------------
// Global Variables

extern volatile I2CQueue_t I2C_rxQ;

// Contains the control information to be sent to the controller
// This structure is 'write only' for the application
extern volatile _ulta_wa CS1108_ultaStatus;

extern volatile CS_read_data RX_data; 

//extern volatile bool FAULT_CODE_READ_EXT;
//extern volatile bool DISPLAY_ACK_DONE_EXT;
extern volatile bool DISPLAY_ACK_DONE;
extern volatile bool FAULT_CODE_READ;
//------------------------------//----------------------------------------------
// Public functions

/**
 * Powerup initialization for this module
 *
 * Should be called at system startup.  It sets up the hardware, data,
 * and starts the interface running.
 *
 */
void CS1108_Initialize( uint8_t ADDRESS );

/**
 * Task runner
 */
void CS1108_Tasks( void );

#endif
