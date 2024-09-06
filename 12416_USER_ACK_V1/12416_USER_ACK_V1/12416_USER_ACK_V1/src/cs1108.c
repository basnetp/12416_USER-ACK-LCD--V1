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
#include "main.h"
#include <xc.h>

//------------------------------//----------------------------------------------
// Definitions
//
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
//define ULTA_ID_ADD 0x46 or 0x47 it is confusing on code written below.

//------------------------------------//--------------------------------------//
// I2C Addresses used for communication with the controller
#define ULTA_ID_ADD 0x40    // ID address for ulta (read only)
#define ULTA_RES_ADD 0x42   // Ulta result address (summary of delux buttons)
#define ULTA_POT_ADD 0x44   // Ulta pot address
#define ULTA_BTN_ADD 0x46   // Ulta button (fwd, rev, lft, rght, stop, inout) address
#define ULTA_MOD_ADD 0x48   // Ulta mode address
#define AUX_ADD 0x4A        // Aux module write address
#define ULTA_MISC_ADD 0x4C  // Ulta miscellaneous address

// ID to indicate advanced interface board is present. 
// ID to indicate advanced interface board is present
// We return this to the controller when it reads the ULTA_ID_ADD (0x40)
#define ULTA_ID 0x03

// This value is written by the controller on powerup, to DLX_ADD
#define STARTUP_MAGIC_VALUE 0xC7

//------------------------------------//--------------------------------------//
// Global variables

volatile _ulta_wa CS1108_ultaStatus = {{0xff,0xff,0xff,0xff,0xff,0xff,0x11,0xff}};
volatile _i2c_ulta_btn_inputs_w CS1108_Buttonstatus = {0xff};

volatile CS_read_data RX_data = { { {{0x05,0x38}, 0},     //Fault code
                                    {{0x07,0xa3},0},        //Runtime High byte
                                    {{0x07,0xa2},0},        //Runtime Low byte
                                    {{0x07,0xa4},0},        //NoFloats, 
                                    {{0x07,0xa5},0},       //lowbatt Minutes
                                    {{0x07,0xa6},0}        //lowbatt Hours
                                  }}; 

volatile CS_write_data CS_reset_counters =   {{  {{0xB0,0x64,0}},         //HiRAM NoFloat
                                                {{0xB0,0x65,0}},          //HiRAM lowbatt min
                                                {{0xB0,0x66,0}},          //HiRAM lowbatt hours
                                                {{0x06,0xa4,0}},          //EEPROM NoFloats, 
                                                {{0x06,0xa5,0}},          //EEPROM lowbatt Minutes
                                                {{0x06,0xa6,0}}           //EEPROM lowbatt Hours
                                                 }}; 

uint8_t read_chargemode[2] = {0x05, 0x5F};

volatile I2CQueue_t I2C_rxQ;


//volatile bool FAULT_CODE_READ_EXT = false;
//volatile bool DISPLAY_ACK_DONE_EXT = false;

volatile bool CLEAR_COUNTER = false;
volatile bool DISPLAY_ACK_DONE = false;
volatile bool DATA_READ = false;
volatile bool READ_CHARGEMODE = false;
volatile bool horn_state_isr = 1;       //Zero when horn needs to be active
volatile uint8_t charge_mode;
volatile uint8_t ultabutton_read = 0;
//volatile unsigned long int i2c_interrupt_counter = 0; 
volatile bool I2C_ACTIVE = 1;
volatile uint8_t FLAG_UART = 0;
volatile bool REQUEST_SEATSWITCH_STATE = false;
volatile bool REQUEST_FAULT = false;
volatile bool inout_bit = 1;    //Indoor mode i 0, outdoor mode is 1. Normal use is outdoor mode. 
                                //Outside bound the unit should switch to Indoor mode

//RX_data.fault = {{0x01, 0x38}, 0};

//------------------------------------//--------------------------------------//
// Local Variables
static uint8_t data_count = 0;      //To keep count of the CS data sequence
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
    //CBUF_Init( I2C_rxQ );

    // initialize the hardware
    // ACKEN disabled; STREN disabled; GCEN disabled; SMEN disabled; DISSLW enabled; I2CSIDL disabled; ACKDT Sends ACK;
    // SCLREL Holds; RSEN disabled; IPMIEN disabled; A10M 7 Bit; PEN disabled; RCEN disabled; SEN disabled; I2CEN
    // enabled;
    
    I2C3CON = 0x9000;       //Changed SCLREL from hold to release
    // BCL disabled; P disabled; S disabled; I2COV disabled; IWCOL disabled;
    I2C3STAT = 0x00;

    I2C3ADD = ADDRESS>>1;//DLUX_BASE_ADDR;
    I2C3MSK = 0x000F;//ADDRESS;//0x00FF;

    /* SI2C3 - I2C3 Slave Events */
    // clear the interrupt flag
    IFS5bits.SI2C3IF = 0;
    // enable the interrupt
    IEC5bits.SI2C3IE = 1;
}

/* Use this function to cause an intermittent beep
* @param period of the beep in milliseconds
*/
 bool beep_interval(uint16_t period_ms)
{
    static bool val = 0; //bit value to be returned
    static TIMER_TICK counter = 0;
    
    if(TIMER_Since(counter)>period_ms)   //Flip the state every n milliseconds
        {
            val = val^1;
            counter = TIMER_Now();
            //printf("V:%d", val);
        }
    return val;
}



/*Drive the buzzer on the display board using data from multiple sources
* @param zero-th bit in i2c byte - that corresponds to reverse horn
* @param Membrane horn button state
* @param Current GPS boundary action value
*/
void update_horn( bool horn_level_I2C, bool horn_in, uint8_t GPS_bound_state )
{
    static bool GPS_horn_flag = 0;
    bool horn_level_GPIO = 0;
    bool horn_net_state = 0;
    
    //*******Set GPS state horn bit**********
    if(GPS_bound_state == 0)                // Inside boundary
        GPS_horn_flag = 0;
    else if(GPS_bound_state == 1)           // Outside warning boundary
        GPS_horn_flag = beep_interval(300);
    else if(GPS_bound_state == 2)           // Outside top boundary
        GPS_horn_flag = 1;  
    else
        GPS_horn_flag = 0;                  // Error prevention      
    //**************************************
    
    horn_level_GPIO = horn_in^1; //Pin is pulled high. So invert it
    
    //'OR' the states of i2c and GPIO
    horn_net_state = horn_level_GPIO | (horn_level_I2C^1) | GPS_horn_flag;
        
  //If not charging. In charging Disable horn
    if((!(CS1108_ultaStatus.misc_data.bits.chg_mode)))//&&(ui_state != sleeping))
    {
            if(horn_net_state)
                OPIN_HORN_OUT = true;
            else
                OPIN_HORN_OUT = false;   
    }
    
    else
        OPIN_HORN_OUT = false;   
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
    static bool DATA_REQUESTED = false;// DISPLAY_ACK_DONE = false, 
    //static bool DATA_READ = false;
    static bool address_index = 0;
    static uint8_t tx_index = 0;
    static uint8_t _prev_WData = 0;
    static uint8_t data_index = 0;
    
    //Increment this counter each timer the interrupt executes
    //This can be used to detect if the i2c communication stops abruptly. 
    //An example application of this is detecting when charging cord is pulled out on LCD display.
    
    //Flag to indicate i2c is working
    //I2C_ACTIVE = 1;
    //For debugging sync inssues, use the below
     // push the data into a buffer for debugging purposes. This is not
    // needed in production.
    // FLAG_UART = data_count;
    
    
    
    
    if( IS_ADDRESS )
    {
        // We save the address, because the next interrupt should be either a data read or write
        _address = data;

        if( IS_READ )
        {
            //CBUF_Push( I2C_rxQ, _address & 0xF0);
            //***Is Display i2c mode is ON***
            if( _address == 0x4F )     //(( _address & 0xF0) == 0x40 )
            {
                //CBUF_Push( I2C_rxQ, _address);
                // respond with the display ID
                //Only the first time, does the display need to send back the byte D7. 
                if(!DISPLAY_ACK_DONE)
                {
                        TRANSMIT_BYTE = 0xD7;//STARTUP_MAGIC_VALUE;;
                        DISPLAY_ACK_DONE = true;
                }
                else
                    TRANSMIT_BYTE = _prev_WData;
                    //TRANSMIT_BYTE = 0xFF;
            }
            
            //***If HHP mode is ON***
            else if((_address & 0xF0) == 0x80)
            {
                if(!DATA_READ)  
                {   DATA_REQUESTED = true;
                    TRANSMIT_BYTE = RX_data.i2c_data[data_index].addr_bytes[address_index];//RX_data.fault.addr_bytes[i];    //0x01;
                    address_index = address_index^1;
                }
                
                else if(CLEAR_COUNTER)      //Reset counter flag is active
                {
                    TRANSMIT_BYTE = CS_reset_counters.i2c_data[data_index].data_byte[tx_index];
                   // There is probably a more efficient way to do the below
                    if((tx_index == 2) && (data_index == 5))
                        CLEAR_COUNTER = false;
                    
                    else
                    {
                        if(tx_index<2)
                             tx_index++;
                        else
                        {                        
                            tx_index = 0;
                            if(data_index<5)
                                data_index++;
                            else
                                data_index = 0;
                        }
                    }
                }
                
                else if (READ_CHARGEMODE)
                {
                    TRANSMIT_BYTE = read_chargemode[address_index];
                    address_index = address_index^1;
                }
                
                else
                {   
                    tx_index = 0;
                    address_index = 0;
                    data_index = 0;
                    TRANSMIT_BYTE = 0xFF;
                }
            }
            else if(_address == 0x47)    //ULTA_BTN_ADD
            {
                TRANSMIT_BYTE = CS1108_Buttonstatus.byte;//0xff ^ (((uint8_t)(!inout_bit))<<6) ;
                data_count = 0;
            }
            
            else        //Slave address other than 4E/80
            {
                TRANSMIT_BYTE = 0xFF;
                data_count = 0;
            }
        }

        // release the clock
          RELEASE_SCL();
    }
    else //if DATA is being read/written
    {
        // DATA WRITE - Controller(Master) writes to Slave
        if( IS_WRITE )
        {
            
            if( _address == 0x4E )
            {
                //Flag to indicate i2c is working
                I2C_ACTIVE = 1;
                
                switch(data_count)
                {
                    case 0: CS1108_ultaStatus.spare1 = data; break;
                    
                    case 1: CS1108_ultaStatus.spare2 = data; break;
                    
                    case 2: CS1108_ultaStatus.b_value[(data & 0x07)] = (data & 0xE0)>>5; break; 
                    
                    case 3: 
                            if(data == 0xff)            //--> When blinking this value becomes 0xFF
                                CS1108_ultaStatus.b_value[5] = 0;
                            else
                                CS1108_ultaStatus.b_value[(data & 0x07)] = (data & 0xE0)>>5;
                            
                            horn_state_isr = data&0x01;  //--> Indicates reverse horn state
                            break;
                            
                    case 4: CS1108_ultaStatus.spare = data;
                    
                    default:break;
                }
                
                if(data_count == 4)
                    data_count = 0;
                else
                    data_count++;
                
                 _prev_WData = data;
                 
            }
            
            //within the address range (0x42..0x4C) --> ULTA
            else if ((_address >= ULTA_RES_ADD) && (_address <= ULTA_MISC_ADD)) 
            {
                if(_address == ULTA_MISC_ADD)
                    CS1108_ultaStatus.misc_data.byte = data;
                else if(_address == ULTA_BTN_ADD)
                    CS1108_Buttonstatus.byte = data;
                else
                {}
                //Reset sync variable
                data_count = 0;
            }
            
            //Programmer mode
            else if((_address & 0xF0) == 0x80)
            {
                 if(DATA_REQUESTED)
                 {
                     //Write the received data into a extern variable
                     RX_data.i2c_data[data_index].rx_data = data;
                     //DATA_REQUESTED = false;  //Fault code has been received. So toggle the flag
                     
                     //Cycle through CS parameters array
                     //if just fault is to be read, after one data RX reset flags
                        if((data_index<5)&&(!REQUEST_FAULT))
                        {
                            data_index++;
                        }
                         else
                        {
                           data_index = 0;
                           DATA_READ = true;
                           DATA_REQUESTED = false; 
                           REQUEST_FAULT = false;
                        }
                        //FAULT_CODE_READ_EXT = FAULT_CODE_READ;
                    
                 }
                 else if(READ_CHARGEMODE)
                 {
                     charge_mode = data;
                     //Charge mode read, hence toggle flag
                     READ_CHARGEMODE = false;
                 }
              data_count = 2;   
            }
            
            else
            {
              //data_count = 0;
              
            }
           
        
        }
        // our response
        //TRANSMIT_BYTE = 0xFF;

        // release the clock
        RELEASE_SCL();
       /*
        CBUF_Push( I2C_rxQ, I2C3STAT & 0x24 );
        CBUF_Push( I2C_rxQ, _address );
        if( IS_READ )
            CBUF_Push( I2C_rxQ, TRANSMIT_BYTE );
        else
        CBUF_Push( I2C_rxQ, data );
        */
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
}
   
   
