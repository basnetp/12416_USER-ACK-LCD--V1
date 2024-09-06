/**
 *
 * CS1108-type controller I2C Interface
 *
 * The controller acts as the I2C master. The basic sequence of events is
 * as follows:
 *      Upon powerup/wake, the controller writes 0xC7 to the DLX_ADD.
 *
 *      The controller then reads from DLX_ADD, to see what kind of display
 *      is present (eg. DELUXE, ULTA) (see dethead.asm in the controller
 *      code).  Provisions
 *      are made for the controller to determine if a programmer is connected
 *      as well as an intelligent display.
 *
 * The CS1108 runs an 8-phase loop to poll the external devices. This loop
 * runs each ~10ms.  In ULTA mode, the loop looks like this:
 *
 *      PHASE       CS1108 Ref          Type    Addr        Desc
 *       0          lblnk_dsp           W       4E          Left blinker status
 *       1          rblnk_dsp           W       4E          Right blinker status
 *       2          pre_fg_tsks                             N/A
 *       3          prgmer              R       80          Poll handheld prgmr
 *       4          spd_dsp             W       4E          Speed Display
 *       5          bat_dsp             W       4E          Battery Display
 *       6          wr_dsp              W       4E          Row id for next phase
 *       7          ifc_tsks            varies  42-4C       see below
 *
 * Each time through phase 7, the controller accesses a register from the
 *  following list (meaning this sub-loop runs at 1/8 the speed of the outer
 *  loop above.
 *
 *      Step        Register            Type    Addr
 *       0          ULTA_RES            R       (0x42)
 *       1          ULTA_POT            R       (0x44)
 *       2          ULTA_BTN            R       (0x46)
 *       3          ULTA_MOD            R       (0x48)
 *
 *       4          ULTA_BTN            W       (0x46)
 *       5          ULTA_MOD            W       (0x48)
 *       6          AUX                 W       (0x4A)
 *       7          ULTA_MISC           W       (0x4C)
 *
 *
 **/
#include "cs1108.h"
#include "app.h"
#include "cbuf.h"
#include "system.h"
#include <string.h>
#include <xc.h>

//------------------------------//----------------------------------------------
// Definitions

// Define this to enable module debugging code
// Leave it undefined to save memory and execution cycles
#define MODULE_DEBUG

#define DLUX_BASE_ADDR 0x4E
#define ULTA_BASE_ADDR 0x40
#define HHP_ADDR 0x80

#define IS_ADDRESS ( !I2C3STATbits.D_A )
#define IS_DATA ( I2C3STATbits.D_A )
#define IS_READ ( I2C3STATbits.R_W )
#define IS_WRITE ( !I2C3STATbits.R_W )

// Release SCL clock hold
#define RELEASE_SCL() I2C3CONbits.SCLREL = 1

#define TRANSMIT_BYTE I2C3TRN
#define RECEIVE_BYTE I2C3RCV

//------------------------------------//--------------------------------------//
// I2C Addresses used for communication with the controller
#define ULTA_ID_ADD 0x40    // ID address for ulta (read only)
#define ULTA_RES_ADD 0x42   // Ulta result address (summary of delux buttons)
#define ULTA_POT_ADD 0x44   // Ulta pot address
#define ULTA_BTN_ADD 0x46   // Ulta button (fwd, rev, lft, rght, stop, inout) address
#define ULTA_MOD_ADD 0x48   // Ulta mode address
#define AUX_ADD 0x4A        // Aux module write address
#define ULTA_MISC_ADD 0x4C  // Ulta miscellaneous address

// ID to indicate advanced interface board is present
// We return this to the controller when it reads the ULTA_ID_ADD (0x40)
#define ULTA_ID 0x03

// This value is written by the controller on powerup, to DLX_ADD
#define STARTUP_MAGIC_VALUE 0xC7

//------------------------------------//--------------------------------------//
// Global variables

volatile _ulta_wa CS1108_ultaStatus;

volatile CS_read_data RX_data = {   {{0x05, 0x38}, 0},  //Fault code
                                    {{0x07,0xa3},0},    //Runtime High byte
                                    {{0x07,0xa2},0},          //Runtime Low byte
                                    {{0,0},0},
                                    {{0,0},0}
                                }; 

volatile I2CQueue_t I2C_rxQ;

//volatile bool FAULT_CODE_READ_EXT = false;
//volatile bool DISPLAY_ACK_DONE_EXT = false;

volatile bool DISPLAY_ACK_DONE = false;
volatile bool FAULT_CODE_READ = true;


//RX_data.fault = {{0x01, 0x38}, 0};

//------------------------------------//--------------------------------------//
// Local Variables

static uint8_t _address = 0;

//------------------------------------//--------------------------------------//
// Local functions

//------------------------------------//--------------------------------------//
// Public functions

/**
 * Module power-up initialization
 */
void CS1108_Initialize( uint8_t ADDRESS )
{
    CBUF_Init( I2C_rxQ );

    // initialize the hardware
    // ACKEN disabled; STREN disabled; GCEN disabled; SMEN disabled; DISSLW enabled; I2CSIDL disabled; ACKDT Sends ACK;
    // SCLREL Holds; RSEN disabled; IPMIEN disabled; A10M 7 Bit; PEN disabled; RCEN disabled; SEN disabled; I2CEN
    // enabled;
    //Updated: SCLREL changed bit value to Zero
    I2C3CON = 0x8000;       //Previously 0x9000
    // BCL disabled; P disabled; S disabled; I2COV disabled; IWCOL disabled;
    I2C3STAT = 0x00;

    I2C3ADD = ADDRESS;//DLUX_BASE_ADDR;
    I2C3MSK = 0x00FF;

    /* SI2C3 - I2C3 Slave Events */
    // clear the interrupt flag
    IFS5bits.SI2C3IF = 0;
    // enable the interrupt
    IEC5bits.SI2C3IE = 1;
}

void CS1108_Tasks( void )
{
}

/*
 * High priority ISR
 *
 * This routine is optimized for I2C communication with the CS1108 slave
 * which has tight timing requirements, notably on the read-from-address
 * state.
 *
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _SI2C3Interrupt( void )
{

    // Read the incoming I2C data byte
    uint8_t data = RECEIVE_BYTE;
    static bool FAULT_CODE_REQUESTED = false;// DISPLAY_ACK_DONE = false, FAULT_CODE_READ = false;
    static bool i = 0;
    static uint8_t _prev_WData = 0;
    
    if( IS_ADDRESS )
    {
        // We save the address, because the next interrupt should be either a data read or write
        _address = data;

        if( IS_READ )
        {
            if( _address == 0x4F )
            {
                // respond with the display ID
                //Only the first time, does the display need to send back the byte D7. 
                if(!DISPLAY_ACK_DONE)
                {
                    TRANSMIT_BYTE = 0xD7;
                    DISPLAY_ACK_DONE = true;
                    //DISPLAY_ACK_DONE_EXT = DISPLAY_ACK_DONE;
                }
               
                else
                    TRANSMIT_BYTE = _prev_WData;
                    //TRANSMIT_BYTE = 0xFF;
                
            }
            else if( (_address & 0xF0) == 0x80)
            {
                if(!FAULT_CODE_READ)
                {   FAULT_CODE_REQUESTED = true;
                    TRANSMIT_BYTE = RX_data.fault.addr_bytes[i];//RX_data.fault.addr_bytes[i];    //0x01;
                    //TRANSMIT_BYTE = RX_data.fault.addr_byte2; //0x38; 
                    // FAULT_CODE_READ = true;   //Do this after receiving data back
                    //if()
                    i = ~i;
                }
                else
                    TRANSMIT_BYTE = 0xFF;
                
            }
            else
            {
                TRANSMIT_BYTE = 0xFF;
            }
        }

        // release the clock
        //if(((_address&0xF0) == 0x40) || ((_address&0xF0) == 0x80))
          RELEASE_SCL();
    }
    else
    {
        // DATA
        if( IS_WRITE )
        {
            
            if( 0x40 == ( _address & 0xF0 ) )
            {
                // A write in the ULTA/DLUX range
                // Save the byte in our array (index 0..7)
                CS1108_ultaStatus.b_value[ ( _address >> 1 ) & 0x07 ] = data;
            }
        
        
        else if((_address & 0xF0) == 0x80)
        {
             if(FAULT_CODE_REQUESTED)
             {
                 //Write the received data into a extern variable
                 RX_data.fault.rx_data = data;
                 FAULT_CODE_REQUESTED = false;  //Fault code has been received. So toggle the flag
                 FAULT_CODE_READ = true;
                 //FAULT_CODE_READ_EXT = FAULT_CODE_READ;
             }
        }
            _prev_WData = data;
        }
        // our response
        //TRANSMIT_BYTE = 0xFF;

        // release the clock
        RELEASE_SCL();
        
        CBUF_Push( I2C_rxQ, I2C3STAT & 0x24 );
        CBUF_Push( I2C_rxQ, _address );
        if( IS_READ )
            CBUF_Push( I2C_rxQ, TRANSMIT_BYTE );
        else
        CBUF_Push( I2C_rxQ, data );
    }

    // clear the slave interrupt flag
    IFS5bits.SI2C3IF = 0;

    // push the data into a buffer for debugging purposes. This is not
    // needed in production.
    /*
    CBUF_Push( I2C_rxQ, I2C3STAT & 0x24 );
    CBUF_Push( I2C_rxQ, _address );
    CBUF_Push( I2C_rxQ, data );
    */
    //FAULT_CODE_READ_EXT = FAULT_CODE_READ;
}
