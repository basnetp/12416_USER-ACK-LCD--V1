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
#define DM_OUTDOOR 1
#define DM_INDOOR 0
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



/*
 Defines the ULTA mode Button Byte
 */
typedef union {
    unsigned char byte;
    struct
    {
        unsigned : 1; // LSB
        unsigned : 1;
        unsigned : 1;
        unsigned : 1;
        unsigned : 1;
        unsigned stop : 1; // 0=Stop
        unsigned outdoor : 1; // 0=Indoor Mode
        unsigned reverse : 1; // MSB
    } bits;
} _i2c_ulta_btn_inputs_w;


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

//Circular buffer for debugging
#define I2C_rxQ_SIZE 128
typedef struct
{
    uint16_t m_getIdx;
    uint16_t m_putIdx;
    uint8_t m_entry[ I2C_rxQ_SIZE ];

} I2CQueue_t;



typedef struct {
    const uint8_t addr_bytes[2];
    uint8_t rx_data;
}read_packet;

typedef union{
    uint8_t data_byte[ 3 ];
    struct 
    {
        const uint8_t addr_bytes[2];
        const uint8_t tx_data;      //Zeros for reset
    };
}write_packet;

/*
 * struct to store data recieved from controller
 
 * These are the elements in the order they are stored in
 *            //1-fault, 
              //2-runtime_Hbyte,
              //3-runtime_Lbyte,
              //4-NoFloats, 
              //5-lowbatt Hbyte;
 *            //6-lowbatt Lbyte;
*/
typedef struct {
      read_packet i2c_data[6];
}CS_read_data; 

typedef struct {
      write_packet i2c_data[6];
}CS_write_data; 


//------------------------------//----------------------------------------------
// Global Variables

extern volatile I2CQueue_t I2C_rxQ;

// Contains the control information to be sent to the controller
// This structure is 'write only' for the application
extern volatile _ulta_wa CS1108_ultaStatus;
extern volatile _i2c_ulta_btn_inputs_w CS1108_Buttonstatus;
extern volatile CS_read_data RX_data; 
//extern volatile unsigned long int i2c_interrupt_counter;
extern volatile bool I2C_ACTIVE;
//extern volatile bool FAULT_CODE_READ_EXT;
//extern volatile bool DISPLAY_ACK_DONE_EXT;
extern volatile bool DISPLAY_ACK_DONE;
extern volatile bool DATA_READ;
extern volatile bool horn_state_isr;        //To transfer Horn state data from i2c to application. used in reverse horn/button horn beep
extern volatile bool CLEAR_COUNTER;
extern volatile bool READ_CHARGEMODE;
extern volatile uint8_t charge_mode;
extern volatile uint8_t FLAG_UART;
extern volatile bool REQUEST_SEATSWITCH_STATE;
extern volatile bool REQUEST_FAULT;
extern volatile bool inout_bit;
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
void update_horn( bool horn_level, bool horn_in, uint8_t GPS_bound_state);

#endif
