

/**
 * 
 * This code has been updated by Prem Basnet.
 * It is firmware for 12416 PCB board that has PIC24......110 controller not PIC24.....206
 * LCD NEW REDESIGN-Blue board
 * PREM BASNET
 * Date:7/28/2021 
 * Tested: Fault code, battery info, and controller I2C, LED, HORN, Smart sensor icon !!DONE    Successful 
 * Need to be tested:  Smart Track and Smart sensor messaging  functionality, Charging behavior  -DONE!! Successful 
 * 
 * Amigo LCD display board firmware
 * This Program is Compatible to the AMIGO ST BOARD PN:12596!!!!!!!!!
 * Testing in progress-PREM BASENT
 * Date:8/24/2021 WORKED GREAT!!!
 * Need to test it again to verify-DONE!!
 * -Prem Basnet
 * 
 

 * 
 * 
 * Copyright (c) 2019 Amigo Mobility International Inc. All Rights Reserved.
 * Description: This is the first prototype code for Amigo display. 
 *              The Splashscreen loads first, followed by the Disclaimer.
 *              After this, loads the UI. 
 *              
 * 11/7/2019: 
 * In order to work with the CS's latest revision of the board the code had to be
 * altered to work with a system that's always powered. Compare this to older displays 
 * where display board would lose power with the Keyswitch turning off.
 * For this to work, Change notification interrupts were implemented on the pin RB8
 * which was connected to the keyswitch signal. This interrupt was used to force the
 * board to go to sleep if the keyswitch was turned off. The sleep feature was also 
 * used to turn off the board when charger was unplugged.
 * 
 * 11/9/19
 * The display auto detects charging and turns on when plugged in. It also turns off 
 * when unplugged.
 * 11/20/19
 * The reverse horn now works also when splashscreen/disclaimer screens are being displayed.
 * The delays were modified for this.
 * 
 * <
 * Rev 2_4   - First fully working code for the LCD display
 * Rev 2_4_1 - Added a timer feature to add sub-levels below 15%
 * Rev 2_4_2 - Added Watchdog timer
 *             Added Smart-track boundary footer messages, horn indication, Drive mode switching
 *             TODO: Improve modularity. Move more functions out of main.c
 * 2/14/2020 -  Added the object detection message from the smart sensor to the screen.
 * 
 * rev:  Added ultrasonic icon and priority of GPS messages over ultrasonic sensor in footer message.-Basnetp
 * 11/10/2020 - Working on updating GPS data first so that cart will not move when power cycled. -Basnetp  DONE!! 
 * > 12/12/2020  - Updated Battery percentage so that false battery percentage reading has been omitted.-Basnetp
 * 3/5/2021  - changed SS and ST icons. "5 hour remaining " text removed and displayed "CART NEEDS CHARGING"
 * 
 * //5/5/3031  - Updated GPS status variable so that it will not initialize garbage value when not connected. (fixed problem
 *  of not showing obstacle detected message when gps not connected. -Basnetp
 * 
 * 
 * * //Date: 10/21/2022             Updating LED behaviour based on Gary
 * Check email:
 * 
 * Hi Prem, 

After much discussion with Al, Beth, and Mike L., this is what we would like to proceed with: 

		Plugged in (on or off)	Unplugged (on)	Unplugged (off)	 	
State of charge	 	 	 	 	 	
100%	Charge Light	Solid Green	Solid Green	Display OFF (No message)
LED OFF	 	
	LCD MSG	Fully Charged	No Message		 	
95%	Charge Light	Solid Green	Solid Green		 	
	LCD MSG	Charging	No Message		 	
80%	Charge Light	Solid Green	Solid Green		 	
	LCD MSG	Charging	No Message		 	
65%	Charge Light	Solid Red	Solid Red		 	
	LCD MSG	Charging	Charge Soon		 	
50%	Charge Light	Solid Red	Solid Red		 	
	LCD MSG	Charging	Charge Soon		 	
30%	Charge Light	Solid Red	Solid Red		 	
	LCD MSG	Charging	Charge Soon		 	
15%	Charge Light	Solid Red	Solid Red		 	
	LCD MSG	Charging	Charge Soon		 	
Note: If unit is unplugged and keyswitch is ON and unit is inactive for 25 minutes then cart goes to sleep mode.  Could be extended up to 240 minutes.	 	
		
		

Please let me know the estimated timeframe to have a prototype ready with these setting for review.  

Thanks,
Gary
 * 
 * 
 * 
 * date:11/2/2022
 *  Adding dual sensor  messaging with led update 
 * Testing needs to be done
 * this code works with 12596 ST board only
 * 
 * 
 * //Updating for SONIEL charger for Float mode Timer:://5-5-2023
 * /////////////////////
 * //This needs to get adjusted in case of trouble finding float mode:
 *  based on data retrieved from TID:650
 * //65% = 3 hour 30 min
 * //80% = 25 min
 * //95% = 6 min
 * 
 * ////////////////////
 * 
 * 
 * 
 * 
 * 3-27-2024
 * Updating Code to Fix 'DISPLAY ON' feature while charging unit with KS OFF
 * testing done!! ECN:240433
 * TID:851
 * 
 * rev beep interuption with ST fixed!!
 * 
 * REV G
 * Charge indicator added
 * While charging:
 * <80% RED
 * >80% GREEN
 * 
 * While Not Charging
 * LED OFF
 * 
 * Date: 8/27/2024 
 * ############################
 * ############################
 * 
 * 
 *~~~~~~~~~~~~ USER ACK FEATURE!!!!!!~~~~~~~~~~~~~~~~~~~~~
 * 
 * BENCH TESTING DONE uses stop bit to stop unit until horn is pressed!!
 * prem 9-5-2024 
 * ############################
 * ############################
 * 
 * !
 * 
 * 
 * 
 * 
 * //this is a testing code for user ack feature

 * 
 * @private
 */

#include "cs1108.h"
#include "main.h"
#include "gfx/gfx.h"
#include "internal_resource_main.h"
#include "internal_resource_main_color_defines.h"
#include "nvm_w25.h"
#include "system.h"
#include "uart.h"
#include "device.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//------------------------------//----------------------------------------------
// Definitions

//#define DEBUG

#define APP_WaitUntilFinished( x ) while( !x )  //Used to run GFX functions if needed

#define APP_SCREEN_DELAY_MS ( 1000 )
#define I2C_TIMEOUT (2000)

#define INFO_FONT (GFX_RESOURCE_HDR *)&Arial_Bold_14

#define DLUX_BASE_ADDR ((uint8_t)0x4E)
#define ULTA_BASE_ADDR ((uint8_t)0x40)
#define HHP_ADDR ((uint8_t)0x80)
#define ULTA_DRIVEMODE_BIT 6
#define GPS_DATAPACKET_SIZE 25
#define ULTA_QSTOP_BIT 4


//#define ON true
//#define OFF false


//Uncomment below if front facing indicator lights are enabled
#define ff_LEDs_enabled
#define SmartSensor_enabled

//#define SMART_SENSOR_PIN IPIN_BUTTON_2  //100 pin and 64 pin works with same wiring i.e black wire on right side and 
//red wire on left side. R
//Red wire-R39-Smart Sensor Persistent ICON-this is button 2 
//Black wire-R38 -Smart sensor OBASTACLE DETECTION MESSAGE-this is button 1 

#define SMART_SENSOR_PERSISTENT_PIN IPIN_BUTTON_2  
#define SMART_SENSOR_PIN IPIN_BUTTON_1  

//------------------------------//----------------------------------------------
// Global variables

//------------------------------//----------------------------------------------
// Local variables

static unsigned int _cntrl_runtime = 23;
static uint8_t _fault_code_local = 0;
static uint8_t _chg_NoFloats = 0;
static unsigned int _Low_batt = 0;
bool FLAG_I2C_ACTIVE = true;
bool ACK_DONE = false;
bool _seat_switch_state=  OFF;
volatile uint8_t ver3 = 0, ver2 = 0, ver1 = 0;
bool flag_lowbatt = false;//__attribute__((persistent));
char lowbatt_state __attribute__((persistent));
bool SS_obstacle_detected = false, SS_detected = false;
static uint8_t SS_Equipped = 0;


volatile bool FLAG_ST_ACK_RECD = false;

//Initialize the enum
volatile UI_STATE ui_state = create_normal;
/* UART VARIABLES*/

uint8_t incoming_bytes[5];
static TIMER_TICK _timer_uart_timeout = 0; 

 bool FLAG_FLOAT_DETECT_95 = false;
 static TIMER_TICK FloatMode_timer_95 = 0;
 
 bool FLAG_FLOAT_DETECT_80 = false;
 static TIMER_TICK FloatMode_timer_80 = 0;
 
 bool FLAG_FLOAT_DETECT_65 = false;
 static TIMER_TICK FloatMode_timer_65 = 0;
 
// bool FLAG_USER_ACK = false;
 
//***********************************************************
/****************************Enums**************************/

//Enum for charging stage. 
typedef enum
{
    NOT_CHARGING = 0, PRECHARGE = 1, BULK_CHARGE = 2, BALANCING = 4, FLOAT = 8
}CHARGE_TYPE;

CHARGE_TYPE CHARGE_STAGE = NOT_CHARGING;


//Different footers
typedef enum
{
    FOOTER_NONE = 0, FOOTER_FAULT = 1, FOOTER_CHARGING = 2, FOOTER_FULL_CHARGE = 3,
    FOOTER_GPS_WARN = 4, FOOTER_GPS_STOP = 5, FOOTER_SMARTSENSOR_MSG = 6, 
    FOOTER_BRAKERLS_OPEN = 7,  FOOTER_LOWCHARGE = 8
}FOOTER_TYPE;


// Action to take when the boundary is violated
typedef enum
{

  GEO_NO_ACTION = 0,
  GEO_WARNING = 1,
  GEO_STOP = 2

} GeoBoundaryAction_e;

//volatile GeoBoundaryAction_e curr_GPS_state = GEO_NO_ACTION;  
volatile GeoBoundaryAction_e curr_GPS_state  __attribute__((persistent)); 


//volatile GeoBoundaryAction_e curr_GPS_state =CS1108_Buttonstatus.byte ;

volatile bool GPS_fix = false;
volatile uint8_t GPS_sats = 0;
volatile uint32_t GPS_ehpe = 0;
volatile uint16_t GPS_BoundViolated = 0;
volatile bool GPS_error = false;
volatile bool GPS_update_received = false;
volatile uint8_t GPS_check=1  ;//initiallize??basnetp
// Number of hardware channels supported by the receiver
//#define GEO_NUM_CHANNELS 12

// Indicates that no boundary has been violated
//#define GEO_NO_BOUNDARY 0x00

// MODBUS message definition - requests slave identity
const uint8_t _requestSlaveId[] = {
    0,                     // transaction MSB;
    0,                     // transaction LSB;
    0,                     // protocol MSB;
    0,                     // protocol LSB;
    0,                     // lengthMsb;
    2,                     // lengthLsb;
    1,                     // unit;
    MODBUS_REPORT_SLAVEID  // function;
};

// structure that holds the GPS/geofence status
typedef struct
{
    int32_t latitude;
    int32_t longitude;
    uint8_t sats;
    uint8_t fixValid;   //previously bool
    uint32_t ehpe;

    // Store satellite signal statistics
    uint8_t cnoMin;
    uint8_t cnoMax;
    uint8_t cnoAvg;

    // bitmap which shows boundaries that are violated
    uint16_t boundaryViolated;
    
    // The current GeoBoundaryAction_e that is in effect
    uint8_t boundaryAction;
    
    
} GeoStatus_t;

//*******************************************************


//*******************************************************
//*************************STRINGS***********************

//Character constants
GFX_XCHAR disclaimer_main_18point[] = {'F','O','R',' ','Y','O','U','R',' ','S','A','F','E','T','Y',':',0xa,0x0a,
                                'T','h','i','s',' ','c','a','r','t',' ','i','s',' ','f','o','r',' ','i','n','d','o','o','r',' ','u','s','e',' ','o','n','l','y', '.', 0x0a,                     
                                'S','t','a','y',' ','a','t',' ','l','e','a','s','t',' ','3',' ','f','e','e','t',' ','a','w','a','y',' ','f','r','o','m',0x0a, 
                                'p','e','o','p','l','e',' ','a','n','d',' ','t','u','r','n',' ','c','a','r','t',' ','o','f','f',' ','w','h','e','n',0x0a,
                                's','t','o','p','p','e','d',' ','i','n',' ','c','h','e','c','k','o','u','t',' ','l','a','n','e','.','B','e','f','o','r','e',0x0a,
                                'b','a','c','k','i','n','g',' ','u','p',',','p','l','e','a','s','e',' ','t','u','r','n',' ','a','r','o','u','n','d',' ','t','o',0x0a,
                                's' ,'e','e',' ','i','f',' ','a','n','y','o','n','e',' ','i','s',' ','b','e','h','i','n','d',' ','y','o','u','.','Y','o','u',0x0a,
                                'a','r','e',' ','r','e','s','p','o','n','s','i','b','l','e',' ','f','o','r',' ','y','o','u','r',' ','d','r','i','v','i','n','g','.',0x0a,
                                 'P','r','e','s','s',' ','h','o','r','n',' ','t','o',' ','a','c','k','n','o','w','l','e','d','g','e','.',0 
                                };
GFX_XCHAR amigo_address[] = {'A','m','i','g','o',' ','M','o','b','i','l','i','t','y',' ','I','n','t','e','r','n','a','t','i','o','n','a','l',',',' ','I','n','c','.',0x0a,
                                '6','6','9','3',' ','D','i','x','i','e',' ','H','i','g','h','w','a','y',0x0a,
                                'B','r','i','d','g','e','p','o','r','t',',',' ','M','I',' ','4','8','7','2','2', 0
                                };

GFX_XCHAR battlevel_num[] = {'9','0',0};//{'9', '0',0}; 
GFX_XCHAR battlevel_time[] = {'~',' ','6','h',' ', '3', '0', 'm',' ','l','e','f','t',0};
GFX_XCHAR perc_sign[] = {'%',0};
GFX_XCHAR emergency_text[] = {'E','M','E','R','G','E','N','C','Y',0};


GFX_XCHAR status_reverse_mode[] = {'R','E','V','E','R','S','E',' ','O','N','L','Y',' ','M','O','D','E',' ','A','C','T','I','V','E',0};
//GFX_XCHAR batt_life_text[] = {'B','a','t','t','e','r','y',' ','l','i','f','e',0};
GFX_XCHAR batt_life_text[] = {' ',' ','B',' ',' ',' ',' ','A',' ',' ',' ',' ','T',' ',' ',' ',' ','T',' ',' ',' ',' ','E',' ',' ',' ',' ','R',' ',' ',' ',' ','Y',0};
//GFX_XCHAR batt_time_left[] = {'6',' ',' ',' ','H',' ','O',' ','U',' ','R',' ','S',' ',' ',' ','L',' ','E',' ','F',' ','T',0};

/*
 Fault code strings
 */
const GFX_XCHAR FAULTCODE[14][30] = 
{
    {'1',':',' ','E','M','B','R','A','K','E',' ','S','H','O','R','T',0},    //11
    {'2',':',' ','E','M','B','R','A','K','E',' ','O','P','E','N',0},        //12
    {'3',':',' ','M','O','T','O','R',' ','S','H','O','R','T',0},            //21
    {'4',':',' ','M','O','T','O','R',' ','O','P','E','N',0},                //22
    {'8',':',' ','P','O','W','E','R',' ','S','H','O','R','T','A','G','E',0},//23
    {'6',':',' ','O','V','E','R',' ','T','E','M','P',0},                    //31
    {'7',':',' ','C','H','A','R','G','E','R',' ','E','R','R','O','R',0},    //41
    {'7',':',' ','C','H','A','R','G','E','R',' ','E','R','R','O','R',0},    //42
    {'8',':',' ','F','E','T',' ','S','H','O','R','T',0},                    //51
    {'8',':',' ','F','E','T',' ','S','H','O','R','T',0},                    //52
    {'5',':',' ','T','H','R','O','T','T','L','E',' ','F','A','I','L',0},    //53
    {'8',':',' ','F','I','R','M','W','A','R','E',0},                        //61
    {'8',':',' ','B','A','D',' ','P','W','M'},                              //62,63
    {'F','A','U','L','T',' ',':',' ','D','I','S','P','L','A','Y',' ','F','W'}
};

//GFX_XCHAR faultcode_throttle[] = {'5','3',':','T','h','r','o','t','t','l','e',' ','F','a','u','l','t',0};
//GFX_XCHAR faultcode_brakeopen[] = {'2','1',':','B','R','A','K','E',' ','O','P','E','N',0};

GFX_XCHAR CHARGING_TEXT[] = {'C','H','A','R','G','I','N','G',0};
GFX_XCHAR CHARGE_FULL_TEXT[] = {'F','U','L','L','Y',' ','C','H','A','R','G','E','D',0};
GFX_XCHAR SEAT_SWITCH_OPEN[] = {'U','s','e','r',' ','m','u','s','t',' ','b','e',' ','s','e','a','t','e','d',0};
GFX_XCHAR GPS_warning_msg[] = {"APPROACHING STORE BOUNDARY"};//Yellow color footer
GFX_XCHAR GPS_stop_msg[] = {"RETURN CART TO STORE"};//Red color footer
GFX_XCHAR SMARTSENSOR_msg[] = {"OBSTACLE DETECTED"};

//GFX_XCHAR LOW_BATT_TEXT[] = {'L','O','W',' ','B','A','T','T','E','R','Y',' ','-',' ','P','l','e','a','s','e',' ','c','h','a','r','g','e',0};
//GFX_XCHAR SS_DETECT[] = {"Obstacle detected"};    //Smart-sensor obstacle detected
GFX_XCHAR LOW_BATT_TEXT[] = {"CHARGE SOON"};




//------------------------------//----------------------------------------------
// Local functions

/*******************************************************************************
 * @brief      Create the second UI screen. 
 *
 * Create the screen, print the disclaimer text.
 *
 * @return     none
 ******************************************************************************/
void create_disclaimer(void)
{
    GFX_ColorSet(DISCLAIMER_BKGCLR);
    GFX_ScreenClear();
    GFX_ColorSet(DISCLAIMER_TXTCLR);
    GFX_FontSet(DISCLAIMER_FONT);
    GFX_TextStringBoxDraw(5,5,310,230,disclaimer_main_18point, 0, GFX_ALIGN_CENTER);
}

/**'****************************************************************************
 * Function:   fetch_index  
 * ---------------------------------
 * Convert the faultcode into the index for the array where the strings
 *  are stored.
 * 
 * faultcode: Take as input faultcode read from the controller
 * 
 * @return     index
 ******************************************************************************/

 uint8_t fetch_index(int faultcode)
{
    switch(faultcode)
    {
        case 0x11: return 0;    //Embrake short
        case 0x12: return 1;    //Embrake open
        case 0x21: return 2;    //Motor short
        case 0x22: return 3;    //Motor open
        case 0x23: return 4;    //Power shortage
        case 0x31: return 5;    //Over Temp
        case 0x41: return 6;    //Charger is supplying too much voltage
        case 0x42: return 7;    //Charger not dropiing back to Float mode
        case 0x51: return 8;    //Controller Fets shorted
        case 0x52: return 9;    //Regen brake FETs shorted
        case 0x53: return 10;   //Throttle fault
        case 0x61: return 11;   //Bad controller s/w rev
        case 0x62: return 12;   //Bad PWM
        case 0x63: return 12;   //Bad PWM
        default: return 13;         //Function should never reach here. If it does its
                                    //a bug in the display firmware. 
        
        
    }
}

 //Use the battery % or charge level to get the Hours remaining
 //This data was derived from tests done by Amigo Testing dpt.
 //TODO: Return zero for all levels where the hour should not be displayed
 uint8_t calc_hours_left(uint8_t batt_level)
 {
     switch(batt_level)
     {
         case 1: return 1;
         case 5: return 2;
         case 8: return 3;
         case 12: return 4;
         case 15: return 5; //when unit is about to die it shouldn't show 5 hours left.
         case 30: return 6;
         case 50: return 7;
         case 65: return 8;
         case 80: return 9;
         case 100: return 12;
         default: return 0; 
     }
             
     //Control should never reach here.
     return 0;
 }
 
// Update the hours left text 
 void update_hour_display(uint8_t hours_left)
{
    static uint8_t prev_hours_left = 0;
    
    char hour_left_text[40];
    
    if(hours_left != prev_hours_left)
    {
        
        
        if(hours_left<2)   //under than 2 hours there is no time left display
        {
            //Clear the area.
            GFX_ColorSet(DRIVE_BKGCLR); 
            APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_num_pos_y,batt_bar_start_x+(14*14),batt_num_pos_y+17));
        }
        
        else if(hours_left>=2 && hours_left<=5){
            
             //Clear the area.
            GFX_ColorSet(DRIVE_BKGCLR); 
            APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_num_pos_y,batt_bar_start_x+(14*14),batt_num_pos_y+17));//14*14
             sprintf( hour_left_text,"CART NEEDS CHARGING");
            GFX_ColorSet(BLUE_GREEN);   
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
            APP_WaitUntilFinished(GFX_TextStringBoxDraw(batt_bar_start_x,batt_num_pos_y ,177,20,hour_left_text,0, GFX_ALIGN_CENTER));
        }
            
        else
        {
            //Clear the text first
            GFX_ColorSet(DRIVE_BKGCLR); 
            APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_num_pos_y,batt_bar_start_x+(14*14),batt_num_pos_y+17)); //14*14
            //Generate new text
            sprintf( hour_left_text,
                 "%d HOURS REMAINING",
                 hours_left);
            GFX_ColorSet(BLUE_GREEN);   
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
            APP_WaitUntilFinished(GFX_TextStringBoxDraw(batt_bar_start_x,batt_num_pos_y ,177,20,hour_left_text,0, GFX_ALIGN_CENTER));
        }//Time left Margins
        
   
        prev_hours_left = hours_left;
    }
    
    
}
 
 
/**
 * @brief      Check the status of the Horn membrane button 
 *
 * Horn is Active Low. Hence a GPIO read gives 0 when horn button is pushed.
 *
 * @return     true if pushed, false else
 */
bool check_horn(void)
{
    //return 1 if Horn button is being pressed, 0 if not.
    //NOTE: IPIN_HORN_IN is 0 when membrane button is pressed and vice-versa. 
    if(!IPIN_HORN_IN)
        return true;
    else
        return false;   
}


/*Function to convert battery charge level to battery bars to be drawn on the LCD*/
void draw_batt_bars(uint8_t batt_perc)
{
    static uint8_t prev_batt_level = 20; //Used to prevent the redrawing unless there is a change.
    static uint8_t BAR_COLOR = GREEN;
    
    if(batt_perc != prev_batt_level)
    {
        
        
        
        //NOTE: Every often, it seems the batt-level value is something other than 1<=x<=6. Hence the whole bar will disappear. Need to correct this bug inside the interrupt
        //Clear all bars first
       // if((batt_perc<7) && (batt_perc>0))
        //{
            GFX_ColorSet(BLIZZARD_BLUE);//(DRIVE_BKGCLR);
            APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                        batt_bar_start_x+6*batt_bar_width+5*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
        //}
        
        switch(batt_perc)
        {
            
            case 100: 
            case 95: BAR_COLOR = GREEN; break;
            case 80: BAR_COLOR = GREEN; break;
            case 65: BAR_COLOR = SUNGLOW; break;
            case 50: BAR_COLOR = SUNGLOW; break;
            case 30: BAR_COLOR = RED_ORANGE; break;
            case 15: 
            case 12:
            case 8:
            case 5:
            case 1:  BAR_COLOR = RED_ORANGE; break;
            default:  /*BAR_COLOR = RED_ORANGE;//BLIZZARD_BLUE;*/ break;
                
        }
        
        GFX_ColorSet(BAR_COLOR);
        
            switch(batt_perc)
        {
            case 100: 
            case 95:
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x+5*(batt_bar_width+batt_bar_spacing),batt_bar_start_y,
                        batt_bar_start_x+6*batt_bar_width+5*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
                

            case 80: 
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x+4*(batt_bar_width+batt_bar_spacing),batt_bar_start_y,
                        batt_bar_start_x+5*batt_bar_width+4*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
                
            case 65: 
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x+3*(batt_bar_width+batt_bar_spacing),batt_bar_start_y,
                        batt_bar_start_x+4*batt_bar_width+3*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
                            
            case 50: 
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x+2*(batt_bar_width+batt_bar_spacing),batt_bar_start_y,
                        batt_bar_start_x+3*batt_bar_width+2*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
                
            case 30: 
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x+batt_bar_width+batt_bar_spacing,batt_bar_start_y,
                        batt_bar_start_x+2*batt_bar_width+batt_bar_spacing,batt_bar_start_y+batt_bar_height));
                
            case 15:
            case 12:
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                        batt_bar_start_x+batt_bar_width,batt_bar_start_y+batt_bar_height));
                
            case 8:
            case 5:
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                        batt_bar_start_x+(batt_bar_width/2),batt_bar_start_y+batt_bar_height));
                
            case 1:
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                        batt_bar_start_x+(batt_bar_width/4),batt_bar_start_y+batt_bar_height));
                //********//Need to add 12, 8, 5 1 levels
                
                break;
    
            default: break;        //retain whatever is on the display
        }
        
        prev_batt_level = batt_perc;
    }
    
    
}


//Charging animation
void animate_batt_bars(uint8_t batt_lvl)
{
    //static uint8_t batt_count = 1;
    static TIMER_TICK prevTick = 0;
    static uint8_t curr_x = batt_bar_start_x;
    static uint8_t start_x = batt_bar_start_x;
    const uint8_t max_x = batt_bar_start_x+6*batt_bar_width+5*batt_bar_spacing;
//    static uint8_t color = 0;
    
    //Update the bar cout every 5 milliseconds
    if((TIMER_Now() - prevTick)>5)
        {
            //switch between 3 colors after every one cycle
            if(curr_x>=max_x)
            {
                //bring the X value back to start
                curr_x = start_x;
                //clear the line
                GFX_ColorSet(BLIZZARD_BLUE);//(DRIVE_BKGCLR);
                    APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                                batt_bar_start_x+6*batt_bar_width+5*batt_bar_spacing,batt_bar_start_y+batt_bar_height));
            //Color scheme of animated bars were changed
                    /*
                if(color<2)
                    color = color+1;
                else
                    color = 0;
 * */
            }
            else
            {
                curr_x++;

                switch(batt_lvl)
                {
                  case 1:
                  case 5:
                  case 8:
                  case 12:
                  case 15: 
                  case 30:GFX_ColorSet(RED_ORANGE); break;
                  
                  case 50:
                  case 65:GFX_ColorSet(SUNGLOW); break;
                    
                  case 80:
                  case 95:
                  case 100: GFX_ColorSet(GREEN); break;
                  
                  default: break;
                }
                APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_bar_start_x,batt_bar_start_y,
                                curr_x,batt_bar_start_y+batt_bar_height));

        }
        
        
    prevTick = TIMER_Now();
    }
        
}



/**
 * @brief      Display debug battery info on Logo splash screen 
 *
 * If the user presses the horn button
 *
 * @return     true if pushed, false else
 */
void display_battery_info(unsigned int *runtime, uint8_t *chgNoFloats, unsigned int *lowbatt)
{
    
    //If the user has pushed down on the horn button while powering up the unit
    //the splashscreen will have info related to battery on the top right corner.
    //This is mainly for technicians to warranty battery?
     char s[ 70 ];

    // can fit about 6 lines on screen
    sprintf( s,
             "Runtime: %d\nNoFloat: %d\nLowBatt: %d",
             *runtime, *chgNoFloats, *lowbatt);
    
    GFX_FontSet(&Arial_Narrow_12);
    GFX_ColorSet(BLACK);
    APP_WaitUntilFinished(GFX_TextStringBoxDraw(batt_counters_start_x,batt_counters_start_y,
                                100,15*3,s, 0, GFX_ALIGN_LEFT));
}



/**
 * @brief      Create the footer on UI
 *
 * This will be called inside the UI FSM. 
 * Function arguments: Current UI state, fault code, pointer to i2c data structure 
 * @return     None
 */
void update_footer(char DRIVE_STATE, uint8_t fault, uint8_t batt_level, _ulta_wa * cs_data, uint8_t GPS_cond)
{
    static uint8_t curr_footer = 0;
    static uint8_t prev_footer = 0;
    
    //TODO: Need to change the footer states to enums instead of numbers
    
    
    switch(DRIVE_STATE)
    {
        case create_normal:
        
            
            if(fault != 0)            //If Controller sends a fault code to the display
            {
                curr_footer = FOOTER_FAULT;//3;;
                if(curr_footer != prev_footer)
                {
                    GFX_ColorSet(FOOTER_CLR_RED);
                    APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                    GFX_ColorSet(BLACK);
                    GFX_FontSet(UI_FOOTER_FONT);
                    APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,FAULTCODE[fetch_index(fault)],0, GFX_ALIGN_CENTER));
                }  
            }
             else                   //Normal operation. No fault.                                 
            {
                curr_footer = FOOTER_NONE;//0;
             if(curr_footer != prev_footer)
             {
                 GFX_ColorSet(DRIVE_BKGCLR);
                 APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
             }
            }
            
            break;
        case display_normal:               
             if(fault != FOOTER_NONE)//0)         //RETAIN FAULT CODE MESSAGE
             {}
             else if(GPS_cond > 0)
             {
                 if(GPS_cond == (uint8_t)GEO_WARNING)   //--> GPS outside warning bound
                 {
                   
                    curr_footer = FOOTER_GPS_WARN;
                    if(curr_footer != prev_footer)
                    {
                        GFX_ColorSet(FOOTER_CLR_YELLOW);
                        APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                        GFX_ColorSet(BLACK);
                        GFX_FontSet(UI_FOOTER_FONT);
                        APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,GPS_warning_msg,0, GFX_ALIGN_CENTER));
                   }
                
                 }
                 else if (GPS_cond == (uint8_t)GEO_STOP)    //--> GPS state outside STOP bound
                     
                 {
                   
                    curr_footer = FOOTER_GPS_STOP;
                    if(curr_footer != prev_footer)
                    {
                        GFX_ColorSet(FOOTER_CLR_YELLOW);
                        APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                        GFX_ColorSet(BLACK);
                        GFX_FontSet(UI_FOOTER_FONT);//UI_FOOTER_FONT_2);
                        APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,GPS_stop_msg,0, GFX_ALIGN_CENTER));
                  
                    }
                 }
                 
                
             }
             
#ifdef SmartSensor_enabled
            
//            if(GPS_cond > 0 || fault != FOOTER_NONE)//if Fault message or GPS Warning or Stop message is active.
//            {}
             
         
            if(GPS_cond == (uint8_t)GEO_WARNING || GPS_cond == (uint8_t)GEO_STOP || fault != FOOTER_NONE)//if Fault message or GPS Warning or Stop message is active.
            {}
             else if (SS_obstacle_detected == true)
             {
                 
               
                 
                   
                curr_footer = FOOTER_SMARTSENSOR_MSG;
                //display_SS_info();
                if(curr_footer != prev_footer)
                    {
                        GFX_ColorSet(FOOTER_CLR_YELLOW);
                        APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                        GFX_ColorSet(BLACK);
                        GFX_FontSet(UI_FOOTER_FONT_3);
                        APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,SMARTSENSOR_msg,0, GFX_ALIGN_CENTER));
                    }
                
             
             }  
             
#endif                         
           // else if(batt_level <= 30)                        //Charge level <= 30%
         else if(batt_level <= 65)                        //Charge level <= 65% diplay CHARGE SOON

            {
                curr_footer = FOOTER_LOWCHARGE;//5;
                if(curr_footer != prev_footer)
                {
                    GFX_ColorSet(FOOTER_CLR_RED);//(253,238,57));   //yellow
                    APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                    GFX_ColorSet(BLACK);
                    GFX_FontSet(UI_FOOTER_FONT);
                    //GFX_FontSet(UI_FOOTER_FONT_3);
                    APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,LOW_BATT_TEXT ,0, GFX_ALIGN_CENTER));
                }
            }    
             
            else                                            //Normal operation
            {                    
                curr_footer = FOOTER_NONE;//0;
                if(curr_footer != prev_footer)  
                {   //Clear the footer area    
                    GFX_ColorSet(DRIVE_BKGCLR);
                    APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                }
                
            }
            break;

        case create_charging:
                curr_footer = FOOTER_NONE;
                break;
                
        case display_charging:         
                curr_footer = FOOTER_CHARGING;
            if(prev_footer != curr_footer)
            {    
                GFX_ColorSet(FOOTER_CLR_GREEN);   
                APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                GFX_ColorSet(BLACK);
                GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
                APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,CHARGING_TEXT,0, GFX_ALIGN_CENTER));
            }
            
           
            break;
        
        case fully_charged:
                    curr_footer = FOOTER_FULL_CHARGE;
                    if(prev_footer != curr_footer)
                    {
                        GFX_ColorSet(FOOTER_CLR_FULLCHARGE);   //yellow
                        APP_WaitUntilFinished(GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y));
                        GFX_ColorSet(BLACK);
                        GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
                        APP_WaitUntilFinished(GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,CHARGE_FULL_TEXT,0, GFX_ALIGN_CENTER));
                    }
            
        default:    //Do nothing
            break;
    }
    //Switch case to decide color for the message bar goes here.
    prev_footer = curr_footer;
    }

/**
 * @brief      Create the static symbols on UI
 *
 * This will be called while creating a new screen to display logos/shapes
 *
 * @return     None
 */
void create_static_symbols()
{
    //GFX_ImageDraw(amigo_tinylogo_posX,amigo_tinylogo_posY, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_AMIGOBW);
    GFX_ColorSet(BLACK);
     //Battery life description
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
    //Sizeof returns bytes. GFX_CHAR seems to use 2 bytes per char. so divide by 2 to find # of char.
    GFX_TextStringBoxDraw(batt_caption_start_x,batt_caption_start_y ,strlen(batt_life_text)*12,20,batt_life_text,0, GFX_ALIGN_LEFT);   
    //Battery logo
    GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATT_NOSPARK);
    
}
/*void create_sensor_symbol()
{
    //GFX_ImageDraw(amigo_tinylogo_posX,amigo_tinylogo_posY, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_AMIGOBW);
    GFX_ColorSet(BLACK);
     //Battery life description
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
    //Sizeof returns bytes. GFX_CHAR seems to use 2 bytes per char. so divide by 2 to find # of char
    GFX_TextStringBoxDraw(batt_caption_start_x,batt_caption_start_y ,strlen(batt_life_text)*12,20,batt_life_text,0, GFX_ALIGN_LEFT);   
    //Battery logo
    GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATT_NOSPARK);
    
}
*/

uint8_t calc_battery_perc(volatile uint8_t battery_level)
{
    //Making changes to get stable data-Basnetp
    
    //static uint8_t batt_val = 100;
    static uint8_t batt_val  __attribute__((persistent));
    static TIMER_TICK lowbatt_timer = 0;// __attribute__((persistent)); //This is to add more resolution below 15%

    //static uint8_t prev_batt_val  
   // batt_val=prev_batt_val;
    if(!(0<batt_val<101)){
        batt_val=100;
    }
    
    
    //CS controller sends battery level value like below
    //i2c data         Actual level /no.of bars in old display
    //1                 6
    //2                 5
    //3                 4
    //4                 3
    //5                 2
    //6                 1
    
    switch(battery_level)
    {
        case 0: //flag_lowbatt = false;
                break;          //Return previous value
        
        
        case 1:             
            flag_lowbatt = false; lowbatt_state = 1;
            //If the units are charging, don't show 100 until float mode hits.
            if((ui_state == create_charging)||(ui_state == display_charging))
                batt_val = 95;
                
            else
                batt_val = 100; 
            
            break;
        
        case 2:  flag_lowbatt = false; lowbatt_state = 1; batt_val = 80;  break;  //83
        case 3: flag_lowbatt = false; lowbatt_state = 1; batt_val = 65;  break;  //66
        case 4: flag_lowbatt = false; lowbatt_state = 1; batt_val = 50;  break;
        case 5: flag_lowbatt = false; lowbatt_state = 1; batt_val = 30;  break;  //33
        case 6: 
               
         
            if((ui_state == display_normal) || (ui_state == create_normal))
            {
                //The below would only execute the first time the charge drops to 15%
                if(flag_lowbatt == false)        
                {
                    flag_lowbatt = true;
                    lowbatt_timer = TIMER_Now();  //time_when_level_hits_15
                }
                
                //Added time based beyond the 15% mark
                //Each level is 45min each
                
                
                if(lowbatt_state < 6 && lowbatt_state > 0)
                {
                    switch(lowbatt_state)
                    {
                        case 1: 
                            if(TIMER_Since(lowbatt_timer) < 2700000)//(45min * 60sec * 1000ms)) //45 mins
                            {   batt_val = 15;
                                lowbatt_state = 1;
                            }
                                    else
                            {
                                lowbatt_state = 2;
                                lowbatt_timer = TIMER_Now();
                            }
                            break;
                            
                        case 2: if(TIMER_Since(lowbatt_timer) < 2700000)
                            {
                                batt_val = 12;
                                lowbatt_state = 2;
                            }
                            else
                            {
                                lowbatt_state = 3;
                                lowbatt_timer = TIMER_Now();
                            }
                            break; 
                            
                        case 3: if(TIMER_Since(lowbatt_timer) < 2700000)
                            {
                                batt_val = 8;
                                lowbatt_state = 3;
                            }
                            else
                            {
                                lowbatt_state = 4;
                                lowbatt_timer = TIMER_Now();
                            }
                            break;
                            
                        case 4: if(TIMER_Since(lowbatt_timer) < 2700000)
                            {
                                batt_val = 5;
                                lowbatt_state = 4;
                            }
                            else
                            {    
                                lowbatt_state = 5;
                                lowbatt_timer = TIMER_Now();
                            }

                            break;
                            
                        case 5: 
                                batt_val = 1; 
                                lowbatt_state = 5;
                                break;
                        
                        default: break;
                    }
                }
                else
                {
                    lowbatt_state = 1;
                    batt_val = 15;
                }
                
            
            //DEBUG - Display seconds counter on topright corner. 
            //Uncomment below block only for Debugging purposes
            //***********
            /*temp = (TIMER_Since(lowbatt_timer))/(1000);//To display minutes);    
            sprintf( s,"%d-%d-%d",temp, flag_lowbatt, lowbatt_state);
            GFX_ColorSet(DRIVE_BKGCLR);
            APP_WaitUntilFinished(GFX_RectangleFillDraw(200,6,239,20));
            GFX_FontSet(&Arial_Bold_14);
            GFX_ColorSet(BLACK);
            APP_WaitUntilFinished(GFX_TextStringBoxDraw(200,6,80,20, s, 0, GFX_ALIGN_LEFT));//batt_font_size_x*(sizeof(batt))
            *///***********
            }
            
            else        //If in charging state
            {
                batt_val = 15;
            }
            
           
                break;  //16
            
        default: batt_val = 0;  break;
    }
    
    return batt_val;
    
}

/**
 * @brief      Update the numeric battery level value on UI
 *
 * Use data from i2c to update battery level
 *
 * @return     None
 */
void update_battlevel_num(uint8_t battery_level)
{
    char batt[5];
    static uint8_t prev_val = 7;
    //static uint8_t prev_state = 0;
    
    //Execute below only if the value is not a repetition
    if((battery_level != prev_val))// || (prev_state != ui_state))
    {
        sprintf(batt, "%d",battery_level);
        
        //Clear the font area before updating. This avoids overwrites on display
        GFX_ColorSet(DRIVE_BKGCLR);
        APP_WaitUntilFinished(GFX_RectangleFillDraw(batt_num_pos_x,batt_num_pos_y,batt_num_pos_x+batt_font_size_x*3,batt_num_pos_y+batt_font_size_y));
        
        //Battery level numeric 
        GFX_ColorSet(BLACK);
        GFX_FontSet(UI_BATT_FONT);
        APP_WaitUntilFinished(GFX_TextStringBoxDraw(batt_num_pos_x,batt_num_pos_y,batt_font_size_x*3,batt_font_size_y, batt, 0, GFX_ALIGN_CENTER));//batt_font_size_x*(sizeof(batt))
        
        ///Print smaller percentage
        GFX_FontSet(UI_FOOTER_FONT);
        APP_WaitUntilFinished(GFX_TextStringBoxDraw(perc_pos_x,perc_pos_y,perc_font_size_x, perc_font_size_y, "%", 0, GFX_ALIGN_LEFT));
        
        //Below lines are moved here from Batt hour function so its only executed every time the batt num
        //text entry is cleared
        GFX_ColorSet(GRAY);//(BLUE_GREEN);
        GFX_LineStyleSet(GFX_LINE_STYLE_THIN_SOLID);//(GFX_LINE_STYLE_THIN_DASHED);
        APP_WaitUntilFinished(GFX_LineDraw(batt_bar_start_x,batt_num_pos_y+20,batt_bar_start_x+177,batt_num_pos_y+20));
        
    //prev_state = ui_state;
    prev_val = battery_level;
    }
    
    
}

//Manually activate the sequence to put display pcb to sleep.
void display_sleeps()
{
    LED_BACKLIGHT_OUT = 0;
    //Disable interrupts
    IEC0bits.T3IE = 0; //Timer
    //Disable i2c interrupts
    //IEC5bits.SI2C3IE = 0; //Disabled to enable background i2c comm. To detect charging
    //Disable GFX controller Interrupt
    IEC6bits.GFX1IE = 0;
    //Disable GFX Module
    G1CON1bits.G1EN = 0;
    //Turn OFF horn
    OPIN_HORN_OUT = 0; 
    //Turn off front facing LEDs
    OPIN_LED_1 = 0;
    OPIN_LED_2 = 0;
    SS_Equipped==0;//
    curr_pwr_state = DISP_OFF;
    ui_state = sleeping;
    //Go to sleep
    asm("PWRSAV #0");
}

//Function to beep the horn for user denoted time in milliseconds
void beep_horn(uint16_t time_ms)
{
    //Turn horn ON
    OPIN_HORN_OUT = ON;
    //delay for as long as user wants
    __delay_ms(time_ms);
    //Turn horn off        
    OPIN_HORN_OUT = OFF;
}


//Display GPS icon when Smart-track is detected
//While debugging, uncomment the sprintfs to see GPS data.
void display_ST_info()
{
    static bool icon_updated = false, prev_fix = false;
    char s[ 60 ];
    
    //Print on screen
    //if((FLAG_ST_ACK_RECD) && 
    if(!icon_updated)
    {
        GFX_ImageDraw(gps_icon_posX, gps_icon_posY, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_GPS_ICON);
       // GFX_ColorSet(BLACK);
       // APP_WaitUntilFinished(GFX_CircleDraw(300,30,8));
        icon_updated = true;
        
        
        //GFX_ColorSet(BLACK);
       // GFX_FontSet(&Arial_Bold_14);
       // APP_WaitUntilFinished(GFX_TextStringBoxDraw(gps_icon_posX,gps_icon_posY+gps_icon_height,
        //gps_icon_width,18,"GPS",0,GFX_ALIGN_CENTER));//(260,5, 100, 15*6, s, 0, GFX_ALIGN_LEFT)); 
            
    }
   // GPS_fix = 1;
//    if( (FLAG_ST_ACK_RECD) && 
//    if(prev_fix != GPS_fix) 
//    {
//        if(GPS_fix == true) //If fix detected, draw circle
//        {
//            GFX_ColorSet(BLACK);
//            APP_WaitUntilFinished(GFX_CircleFillDraw(300,30,8));
//        }
//        else                //If no fix, clear the circle
//        {
//            GFX_ColorSet(WHITE);
//            APP_WaitUntilFinished(GFX_CircleFillDraw(300,30,8));
//            GFX_ColorSet(BLACK);
//            APP_WaitUntilFinished(GFX_CircleDraw(300,30,8));
//           // GFX_CircleFillDraw(300,30,8);
//        }
//         prev_fix = GPS_fix;
//    }
    
  //Uncomment below block only while debugging  
    /*
    if(GPS_update_received)
        {
            //Clear area
            GFX_ColorSet(WHITE);
            APP_WaitUntilFinished(GFX_RectangleFillDraw(10,10,190,40))//(260,5,320,80));
                    
            GPS_update_received = false;
            //Write GPS debug info on screen
            sprintf(s, "Fix: %u, Sats: %u\n\rBA: %u, BV: %u, Err: %u", GPS_fix, GPS_sats, (uint8_t)curr_GPS_state, GPS_BoundViolated, GPS_error);
            
            //Update values    
            GFX_ColorSet(BLACK);
            GFX_FontSet(&Arial_Narrow_12);
            APP_WaitUntilFinished(GFX_TextStringBoxDraw(10,10,190,15*2,s,0,GFX_ALIGN_LEFT));//(260,5, 100, 15*6, s, 0, GFX_ALIGN_LEFT)); 
            
        }
    */
        
}
/*void display_SS_info()
{
    static bool icon_updated = false, prev_fix = false;
    char s[ 60 ];
    
    //Print on screen
    //if((FLAG_ST_ACK_RECD) && 
    if(!icon_updated)
    {
        GFX_ImageDraw(ss_icon_posX, ss_icon_posY, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_SS_ICON);
        //GFX_ColorSet(BLACK);
        //APP_WaitUntilFinished(GFX_CircleDraw(300,30,8));
        icon_updated = true;
        //GFX_ColorSet(BLACK);
      //  GFX_FontSet(&Arial_Bold_14);
        APP_WaitUntilFinished(GFX_TextStringBoxDraw(ss_icon_posX,ss_icon_posY+ss_icon_height,
        ss_icon_width,18,"SS",0,GFX_ALIGN_CENTER));//(260,5, 100, 15*6, s, 0, GFX_ALIGN_LEFT)); 
            
    }
}
*/
/**
 * @brief      State machine for UI screen
 *
 * This will be called inside the Main periodically
 *
 * @return     None
 */
void UI_state(uint8_t fc, volatile _ulta_wa * cs_data, uint8_t GPS_state)//uint8_t batt, uint8_t speed)
{
    //static enum UI_STATE ui_state = create_normal;
    static TIMER_TICK prevTick; 
    uint8_t batt_perc = calc_battery_perc(cs_data->aux_data);
    
    switch(ui_state)
    {
        
        case create_normal:
            // Draw the main user interface screen. 
            GFX_ColorSet(DRIVE_BKGCLR);
            GFX_ScreenClear();           
            update_battlevel_num(batt_perc);
            update_footer(ui_state, fc, batt_perc, cs_data, GPS_state);
            create_static_symbols();
            draw_batt_bars(0);
            update_hour_display(calc_hours_left(batt_perc));
            ui_state = display_normal;
            FLAG_CHARGING = false;
            prevTick = TIMER_Now()-800;
            break;
            
        case display_normal:    
            if(TIMER_Now() - prevTick > 1000)
            {
                update_battlevel_num(batt_perc);
                draw_batt_bars(batt_perc);
               // if(FLAG_ST_ACK_RECD) 
                if(IPIN_GPS_ENABLE)   
                    display_ST_info();
                update_footer(ui_state, fc, batt_perc, cs_data, GPS_state);       //For Quickstop, ST, SS detection message
                update_hour_display(calc_hours_left(batt_perc));
#ifdef ff_LEDs_enabled 
                update_frontfacing_LED(batt_perc, false);
#endif
                if(FLAG_I2C_ACTIVE)
                {
                    if(cs_data->misc_data.bits.chg_mode)      //if charging bit is ON - Active High
                        ui_state = create_charging;
                    else if(curr_pwr_state == DISP_OFF)
                        ui_state = sleeping;
                    else    
                        ui_state = display_normal;
                }
                else //if(!FLAG_I2C_ACTIVE)
                {
                    
#ifdef DEBUG
            printf("slp1..");
#endif
                    curr_pwr_state  = DISP_OFF;
                    ui_state = sleeping;
                    display_sleeps();
                }
                prevTick = TIMER_Now();
            }

            break;
            
        case create_charging:
            
            update_footer(ui_state, fc, batt_perc, cs_data, 0);
            update_battlevel_num(batt_perc);
            update_hour_display(0);
            GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATT_SPARK);
            //update_batt_icon(ui_state,  cs_data);
            ui_state = display_charging;
            FLAG_CHARGING = true;
            FloatMode_timer_80=0;
            FloatMode_timer_95=0;
            FloatMode_timer_65=0;
            FLAG_FLOAT_DETECT_95= false;
            FLAG_FLOAT_DETECT_80= false;
            FLAG_FLOAT_DETECT_65= false;
#ifdef DEBUG
            printf("chg2");
#endif
            //prevTick = TIMER_Now();
            break;
            
        case display_charging:
            update_footer(ui_state, fc, batt_perc, cs_data, 0);
            update_battlevel_num(batt_perc);
            update_hour_display(0);
#ifdef ff_LEDs_enabled 
            update_frontfacing_LED(batt_perc, true);
#endif
            //animate_batt_bars();
            if(FLAG_I2C_ACTIVE)
            {
                FLAG_CHARGING = true;
                
                
                //Check if you pass 5 minute past this mode and display fully charged-prem
                //
                //worked for 5 A charger 
                 if(batt_perc==65 && FLAG_FLOAT_DETECT_65 == false){
                   
                    FloatMode_timer_65=TIMER_Now();
                     FLAG_FLOAT_DETECT_65=true;
                    FLAG_FLOAT_DETECT_80=false;
                    FLAG_FLOAT_DETECT_95=false;
                }
                
                 else if(batt_perc==80 && FLAG_FLOAT_DETECT_80 == false){
                   
                    FloatMode_timer_80=TIMER_Now();
                     FLAG_FLOAT_DETECT_80=true;
                     FLAG_FLOAT_DETECT_65=false;
                    FLAG_FLOAT_DETECT_95=false;
                }
                
                
                //below worked for 3 A charger
                
                else if(batt_perc==95 && FLAG_FLOAT_DETECT_95 == false){
                   
                    FloatMode_timer_95=TIMER_Now();
                     FLAG_FLOAT_DETECT_95=true;
                     FLAG_FLOAT_DETECT_65=false;
                    FLAG_FLOAT_DETECT_80=false;
                }
              
                //
                // mathi ko chalena vane you logic use garda hunxa hola
                //Keep reseting timer untill condition meets
                // if(FLAG_FLOAT_DETECT_95==false){ FloatMode_timer_95=TIMER_Now(); }
                // if(FLAG_FLOAT_DETECT_85==false){ FloatMode_timer_85=TIMER_Now();    }
                // if(FLAG_FLOAT_DETECT_65==false){ FloatMode_timer_65=TIMER_Now();    }
                 
                
                
               
               
             
                //Check if balancing or float mode. Then device is fully charged
               // if((CHARGE_STAGE == FLOAT)||(CHARGE_STAGE == BALANCING)||(FloatMode_timer >= 120000)) 
        // if(FloatMode_timer >= 30000)
                
                //Trying 65- 5 min 80 - 3 min and 95 2 minute for test
                //actual will be 65% - 3 hr 30 min    80% - 25 min   65% 3 hour 30 min
   // ((FLAG_FLOAT_DETECT_95=true) && (TIMER_Since(FloatMode_timer_95) > 120000))
                
                
       // if((TIMER_Since(FloatMode_timer_95) > 120000)||(CHARGE_STAGE == FLOAT)||(CHARGE_STAGE == BALANCING )|| (TIMER_Since(FloatMode_timer_80) > 180000)||(TIMER_Since(FloatMode_timer_65) > 300000))//(45min * 60sec * 1000ms)) //45 mins
              //   //Need to make 3 h 30 min for 65% , 25 min fr 80% and 6 min for 95% floating 
          if(((FLAG_FLOAT_DETECT_95==true) && (TIMER_Since(FloatMode_timer_95) > 360000))||(CHARGE_STAGE == FLOAT)||(CHARGE_STAGE == BALANCING )|| ((FLAG_FLOAT_DETECT_80==true) && (TIMER_Since(FloatMode_timer_80) > 1500000))||((FLAG_FLOAT_DETECT_65==true) && (TIMER_Since(FloatMode_timer_65) > 12600000)))//////////(45min * 60sec * 1000ms)) //45 mins
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
              /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
                {
                    //FLAG_CHARGING = FULLY_CHARGED;
                    ui_state = fully_charged;
                    beep_horn(200);
                    update_battlevel_num(0);
                    draw_batt_bars(95);
                   // FLAG_FLOAT_DETECT_65=false;
                   // FLAG_FLOAT_DETECT_80=false;
                   // FLAG_FLOAT_DETECT_95=false;

                }
                else// if ((CHARGE_STAGE == 1) || (CHARGE_STAGE == 2)) 
                {
                    
                    animate_batt_bars(batt_perc);
                }
            }
                  
            else// if(!FLAG_I2C_ACTIVE)
            {
                
#ifdef DEBUG
            printf("Slp3..");
#endif
                FLAG_CHARGING = false;
                FLAG_I2C_ACTIVE = false;
                I2C_ACTIVE = false;
                curr_pwr_state  = DISP_OFF;
                ui_state = sleeping;
                DISPLAY_ACK_DONE = false;
                display_sleeps();
                
            }

            
            break;
            
        case fully_charged:
                update_footer(ui_state, fc, batt_perc, cs_data, 0);
                update_battlevel_num(100);
                update_hour_display(12);
                draw_batt_bars(100);
#ifdef ff_LEDs_enabled 
                //update_frontfacing_LED(batt_perc, false);
                update_frontfacing_LED(100, true);
#endif
                FLAG_CHARGING = true;
                
                //If charger is unplugged, i2c comm stops, put display to sleep
                if(!FLAG_I2C_ACTIVE)    
                {
                    //FLAG_CHARGING = false;
                    //asm("RESET"); //Reset and wait in 
                    //GONE_TO_SLEEP = true;
                    FLAG_CHARGING = false;
                    FLAG_I2C_ACTIVE = false;
                    I2C_ACTIVE = false;
                    DISPLAY_ACK_DONE = false;
                    ui_state = sleeping;
                    curr_pwr_state  = DISP_OFF;
                    display_sleeps();
                }
                
             break;
        
        case sleeping:
            
            OPIN_HORN_OUT = 0;  //Turn OFF Horn in-case boundary violation is causing horn to beep
                                //in sleep
            //check i2c for charging bit indication to turn ON by reset
            //FLAG_CHARGING = false;
            //if charger is unplugged, controller turns off. 
            //So next time controller turns on, the display needs to be reset
            if(I2C_ACTIVE == true)
            {
                //beep_horn(200);
#ifdef DEBUG
            printf("Rst");
#endif
                    curr_pwr_state  = DISP_ON;
                    asm("RESET");
            }
            else
            {
              //  GONE_TO_SLEEP = true;
#ifdef DEBUG
            printf("S");
#endif
                curr_pwr_state  = DISP_OFF;
                // asm("PWRSAV #0");
            }
            break;
            
        default: break;    
            
    }
    
}

/*
 * Handle the Graphics elements on the first screen after boot-up
*/
void create_splashscreen()
{
    // Splashscreen with Amigo Logo
    APP_WaitUntilFinished( GFX_ImageDraw( 0, 0, APP_INTERNAL_FLASH_SPLASH_IMAGE) );
    //print address text
    GFX_ColorSet(BLACK);
    GFX_FontSet(LOGO_CAPTION_FONT);
    APP_WaitUntilFinished(GFX_TextStringBoxDraw(20,180,280,17*3,amigo_address, 0, GFX_ALIGN_CENTER));    
}

/*
 Function to clear the values of the CS Motor controller parameters NoFloat, Lowbatt 
 */
void clear_counters()
{   
    
    CLEAR_COUNTER = true;
    CS1108_Initialize(HHP_ADDR);        //Switch to programmer mode
    while(CLEAR_COUNTER == true);       //Stay here till i2c transactions to clear the values are done
    CS1108_Initialize(DLUX_BASE_ADDR);  //Switch to Display mode
    char reset_text[] = "COUNTERS RESET";
    GFX_ColorSet(TORCH_RED);
    GFX_FontSet(LOGO_CAPTION_FONT);
    APP_WaitUntilFinished(GFX_TextStringBoxDraw(110,40,17*sizeof(reset_text),17,reset_text,0, GFX_ALIGN_LEFT));    

}

/*
 * Function to dynamically read fault code state from the motor controller
 *  This function is used to update the error code every 3 seconds
*/
uint8_t update_fault(uint8_t current_fault)
{
 uint8_t new_fault = current_fault;
 
 if(current_fault == 0)
 {
     //Toggle flag that updates when data was read from controller
     DATA_READ = false;
     REQUEST_FAULT = true;
     //Switch i2c address to HHP mode
     CS1108_Initialize(HHP_ADDR);
    while(REQUEST_FAULT){};
    new_fault = RX_data.i2c_data[0].rx_data;
    DATA_READ = true;
    
     //switch back to display mode
     CS1108_Initialize(DLUX_BASE_ADDR);

 }     
     //return the new fault code
return new_fault;

}




//The below function reads the status of the seatswitch
//This function is needed on S-4 controllers since Seatswitch bit on ULTA doesnot
//correspond exactly to physical state of the seatswitch. This is due to UL safety rules
//So seatswitch is connected also to ANin port of 18 pin connector. ANin is read 
//using HHP mode to determine state of seatswitch.
//Below functionality was disabled upon Beth's direction until further notice
/*
bool update_seatswitch()
{
    bool seatswitch_state = OFF;
    //Set flag to let ISR know seatswitch is to be read
    REQUEST_SEATSWITCH_STATE = true;
    //Switch i2c address to HHP mode
    CS1108_Initialize(HHP_ADDR);
    //Wait till ISR finishes reading and toggles the flag
    while(!REQUEST_SEATSWITCH_STATE){};
    
    seatswitch_state = 1;  //Replace with data read from i2c
    
    //switch back to display mode
     CS1108_Initialize(DLUX_BASE_ADDR);
    
     return seatswitch_state;
}
*/

/*
 * Read charge mode bit from motor controller. 
 * A timeout was added to prevent infinite loop
 * Charge mode can be one of five values from enum CHARGE_TYPE
 */
uint8_t update_chargemode()
{
    TIMER_TICK tick_local = TIMER_Now();
    
   //Switch i2c address to HHP mode
     CS1108_Initialize(HHP_ADDR);
      
     READ_CHARGEMODE = true;
     //Wait till this flag is toggled in i2c interrupt after reading
     while(READ_CHARGEMODE)
     {
          ClrWdt(); //Clear watchdog
         if(TIMER_Since(tick_local)> I2C_TIMEOUT) //if i2c is non-responsive
         {
             charge_mode = 0;
             break;
         }
             
     }
    
      //switch back to display mode
     CS1108_Initialize(DLUX_BASE_ADDR);
               
     return charge_mode;
}

/*
 * Function to wait until Motor controller reads Display Device ID
 * A timeout was later added to this, to prevent infinite loop
 * @param pointer to flag that return 1 
 */
bool wait_for_deviceID_request(volatile bool *flag)
{
    TIMER_TICK _timer_func = TIMER_Now();
    
    while(!(*flag))
    {
        ClrWdt(); //Clear watchdog
        if(TIMER_Since(_timer_func) > I2C_TIMEOUT)
        {
            *flag = true;
            if(I2C_ACTIVE == 1)
                return true;    //i2c detected. Software reset so ACK was not detected 
            else
                return false; //Returns false since Timeout occured. No i2c detected. 
            //break;
        }
         
              
    } 
    
    return true;        //ACK was detected
}

/*
 * Switch the i2c slave address of the LCD display board
 * @param desired i2c slave address
*/
void switch_slave_address(uint8_t address)
{
    //Disable i2c interrupts
    IEC5bits.SI2C3IE = 0;
    //Turn ON HHP mode to read parameters
    CS1108_Initialize(address);
    //Re-enable i2c interrupts
    IEC5bits.SI2C3IE = 1;
}


/*
 Function to read parameter values from CS motor controllers
 *
 * @param Flag that sets when all the parameters have been read from the controller 
 * @param pointer to controller runtime
 * @param pointer to fault code value
 * @param pointer to NoFloat counter value
 * @param pointer to Lowbatt counter value
 */
bool read_cs_parameters(volatile bool *flag, unsigned int *cntrl_runtime, uint8_t *fault, 
                            uint8_t *NoFloat, unsigned int *Lowbatt)
{
    TIMER_TICK _timer_func = TIMER_Now();
    
    while(!(*flag))
    {    
         ClrWdt(); //Clear watchdog
        if(TIMER_Since(_timer_func) > I2C_TIMEOUT)
        {
            *flag = true;       //Reset flag state
            return false;       //Timeout occured. Return false 
            break; 
        }
             
    }        
    
    //Save data read onto local variables
    *cntrl_runtime = (RX_data.i2c_data[1].rx_data<<8)|(RX_data.i2c_data[2].rx_data);
    *fault = RX_data.i2c_data[0].rx_data;
    *NoFloat = RX_data.i2c_data[3].rx_data;
    *Lowbatt = (RX_data.i2c_data[4].rx_data) + (RX_data.i2c_data[5].rx_data)*60;        //in minutes
    
    *flag = true; 
    return true;        //return true to denote read successful
}   

/*
 If user presses horn button at Spalshscreen, display the values of the parameters read 
 * from CS1108 controller at startup
 * 
 * @param Flag that returns true if user pressed the horn button
 * @param pointer the controller runtime value
 * @param pointer to NoFloatCharge counter value
 * @param pointer to low battery counter value
 */
void display_cs_parameters( bool *flag, unsigned int *_cntrl_runtime, uint8_t *_chg_NoFloats, unsigned int *_Low_batt)
{
    //Check if the user is pressing the horn button on membrane
    //To do - May be put these in a box
    if(check_horn())
    {
      *flag = true;  
      display_battery_info(_cntrl_runtime, _chg_NoFloats, _Low_batt);   //If button is pressed, display debug info.
      //__delay_ms( APP_SCREEN_DELAY_MS * 0.1 );
      while(check_horn()) //Stay here until user releases the Horn button
      {
           ClrWdt(); //Clear watchdog
      };       
       //reset_sequence();
    }
}

/*
 * Function to debouce the input from the push membrane horn button. 
 * Debounce period = 100ms
 */
bool debounce_horn()
{
    bool debounced_horn_in  = 1;
    static bool raw_horn_prev = 1;
    bool raw_horn_in =  1;
    static TIMER_TICK tick_deb = 0;
    
    raw_horn_in =  IPIN_HORN_IN;
    
    if(raw_horn_in == raw_horn_prev)
    {
        if(TIMER_Since(tick_deb) > 100)     //Debounce period of 100 ms
        {
            debounced_horn_in = raw_horn_in;
        }
    }
    else
    {
        raw_horn_prev = raw_horn_in;
        debounced_horn_in = 1;          //Off
        tick_deb = TIMER_Now();
    }
    return debounced_horn_in;
}



/*
 * Replaces delay_ms which was blocking and did not allow horn function
 * Below function is still blocking, but it allows horn
 * @param pointer to the bool that has the state of the i2c LSB
 * @param pointer to the debounced state of the horn button
 * @param delay in milliseconds
*/


void delay_with_Revhorn(bool *i2c_horn_bit, bool *membrane_horn_state, uint16_t delay_ms)
{
    TIMER_TICK _timer = 0; 
    
    _timer = TIMER_Now();
    //__delay_ms( APP_SCREEN_DELAY_MS * 1 )
    while(TIMER_Since(_timer)<delay_ms)
    {       
             ClrWdt(); //Clear watchdog
            *membrane_horn_state = debounce_horn();
            update_horn(*i2c_horn_bit, *membrane_horn_state, 0);
          //  if(FLAG_ST_ACK_RECD){ update_driveMode(curr_GPS_state);}
       
    }
}




/*
 * Function to update the Dual color LEDs for charging indication
 * Green when batt percentage is above 80
 * Red when 30 or below
 * @param battery percentage level
 * @param flag that indicates when its charging
 */

void update_frontfacing_LED(int batt_perc, bool charging)
{
    static TIMER_TICK _timer_LED = 0;//TIMER_Now(); 
    //static LED_state_red = 0, LED_state_grn = 0;
    static unsigned char counter = 0;
    if(!charging){
        //Turn off both 
            OPIN_LED_2 = 0;  
            OPIN_LED_1 = 0;
        
    }
    else
    {
        if(batt_perc>65) // solid green light  For 80% 95% and 100%
        {
            //Turn ON Green
            OPIN_LED_2 = 1;
            //Turn OFF Red
            OPIN_LED_1 = 0;
        }
        
//        else if((batt_perc<95)& (batt_perc>50)) // flashing green
//        {
//            OPIN_LED_2 = 1;
//            __delay_ms(100); //50ms delay
//            OPIN_LED_2 = 0;
//        }
        
       else if((batt_perc>5) & (batt_perc<=65)) // solid red light for 15% 30% 50% and 65%
        {
            //Turn ON red
            OPIN_LED_1 = 1;
            //Turn Off Green
            OPIN_LED_2 = 0;
            
        }
 //
    //
    //
    
        else
        {
            //Turn off both //Ideally this should not happen during display ON
            OPIN_LED_2 = 0;  
            OPIN_LED_1 = 0;
               
        }
   
    
    
  }//end of charging
//    else    //if charging
//    {
//       /* if((CHARGE_STAGE == FLOAT)||(CHARGE_STAGE == BALANCING)) 
//        {
//            OPIN_LED_1 = 0;
//            //__delay_ms(50); //50ms delay
//            OPIN_LED_2 = 0;
//        }
//        else
//        {*/
//            //While charging toggle both lights
//            if(TIMER_Since(_timer_LED)>1000)
//            {
//                switch((counter++)%2)
//                {
//                    case 0: OPIN_LED_1 = 1;
//                            __delay_ms(50); //50ms delay
//                            OPIN_LED_2 = 0;
//                            break;
//
//                    case 1: OPIN_LED_1 = 0;//
//                            __delay_ms(50); //50ms delay
//                            OPIN_LED_2 = 1;
//                            break;    
//                }   
//            _timer_LED = TIMER_Now();
//            }
//        //}
//    }
}//End of  update_frontfacing_LED







//*****************************************************************************
#ifdef SmartSensor_enabled

void init_smartsensor_detection()
{
    //Declare the pin as digitial input
    //This is done already in system.c
    
    //activate internal Pull-down/pull-down?
}

//Smart sensor can be connected to LCD display via one of the four pins on the membrane header.
// The output of the sensor can be made active high that the scooter detects smart-sensor.
//The input pin on the LCD board would need to be pulled down??
void detect_smartsensor()
{
    //For the first second or so, pin is pulled high at the Interface board.
    if(!SMART_SENSOR_PIN)
    {
        SS_detected = true;
        //beep_horn(200);
       // beep_horn(100);
    }
    
//return 1;    
}
//Basnsetp Persistent icon moved to another pin-7/21/2021
void Check_SmartSensorEnabled(){
    
    if(SMART_SENSOR_PERSISTENT_PIN==1){
        
        GFX_ImageDraw(ultrasonic_icon_pos_x, ultrasonic_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_ULTRASONIC); //Display smart sensor Icon
        SS_Equipped=1;
    }
    
    else{
           GFX_ImageDraw(ultrasonic_icon_pos_x, ultrasonic_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_NO_ULTRASONIC); //Disable smart sensor Icon
            SS_Equipped=0;
    }
   
}

void update_smartsensor_status()
{
    //if(!SS_detected)
     //   detect_smartsensor();
    //else
   // if(SS_detected)
   // {
        if(SMART_SENSOR_PIN==1)
        {
            SS_obstacle_detected = true;
           // GFX_ImageDraw(ultrasonic_icon_pos_x, ultrasonic_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_ULTRASONIC); //Display smart sensor Icon

            
             //beep_horn(200);
        }
        else
        {
            SS_obstacle_detected = false;
           // GFX_ImageDraw(ultrasonic_icon_pos_x, ultrasonic_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_NO_ULTRASONIC); //Disable smart sensor Icon

             //beep_horn(200);
        }
   // }
    
}
#endif

//******************GPS Board functions****************

/**
 * callback for dynamic command
 *
 * Here's where the result of DEVICE_Send() winds up.
 * The result may be an error code, or success.
 *
 * @param      pResponse  Pointer to the response
 */
void DEVICECB_SendResponse( MbapHeader_t *pResponse )
{
    char buf[ 80 ];
    uint8_t *pData = (uint8_t *)&pResponse[ 1 ];

    if( pResponse->function & 0x80 )
    {
        // an error occurred
        //sprintf( buf, "MB ERR: %u, %u\r\n", pResponse->function & 0x7F, pData[ 0 ] );
        //printf( "%s", buf );
        GPS_error = true;       //Set the global variable so the entire code knows when there's an error
    }
    else
    {
        GPS_error = false;
        switch( pResponse->function )
        {
            case MODBUS_REPORT_SLAVEID:
            {
                //sprintf( buf, "\n***\nDetected: %u, %u, Version: %u.%u.%u\r\n", pData[ 1 ], pData[ 2 ], pData[ 3 ], pData[ 4 ],
                        // pData[ 5 ] );
                ver3 = pData[ 3 ]; ver2 = pData[ 4 ]; ver1 = pData[ 5 ];  
                
                if((uint8_t)ver3 >= 3)
                {
                    FLAG_ST_ACK_RECD = true;
                    //beep_horn(200);
                }
                //UART2_PutString(buf);
                
                CBUF_Flush(UART1_rxQ);  //Empty the uart receive buffer
                break;
            }

            case MODBUS_COMMAND:
            {
                if( 0 == pData[ 1 ] )
                {
                    //sprintf( buf, "Command: %u SUCCESS\r\n", pData[ 0 ] );
                }
                else
                {
                    //sprintf( buf, "Command: %u FAILED %u\r\n", pData[ 0 ], pData[ 1 ] );
                }
                //printf( "%s", buf );
                break;
            }

            default:
                break;
        }
    }
}

/**
 * Handle the response to the status query
 *
 * This updates our record of the system status and reports it
 * out.  It is called when status is polled from the device.
 *
 * @param pPdu pointer to the MODBUS PDU
 * @param len number of bytes in tunneled PDU, including the 2-byte checksum
 */
void DEVICECB_StatusResponse( ModbusRtuPdu_t *pPdu, uint8_t len )
{
    //ModbusRtuPdu_t mock_data[] = {0x01,0x43,0x14,0x56,0x15,0xD6,0x19,0x9E,0x12,0x05,0xCE,0x0A,00,0x8C,0x05,0x00,0x00,0x18,0x29,0x20,0x03,0x00,0x00,0x85,0x4F};
    //char s[70];
    // the number of data bytes returned is the length minus 2-byte checksum
    // minus 2-byte PDU header.  Test the length to make sure it's all there
    if( len >= GPS_DATAPACKET_SIZE)
    {
        GPS_update_received = true;
        // the stuff we care about starts at the third byte
        uint8_t *p = &pPdu->data[1];//  &mock_data->data[1];
        
        curr_GPS_state = (GeoBoundaryAction_e)p[19];//p[20];
        GPS_fix = (bool)((p[9] == 0)?0:1);     
        GPS_sats = p[8];
        GPS_BoundViolated = (uint16_t)(  1*((p[17])&0x0f) + 16*((p[17])&0xf0) +
                256*((p[18])&0xf0) + 4096*((p[18])&0xf0) );
        GPS_error = false;
        
        //sprintf(s, "Fix: %u, Sats: %u, BV: %u, EHPE: %u, Lat: %d, Long: %d\n", 
       //         p->fixValid, p->sats, p->boundaryAction, p->ehpe, p->latitude, p->longitude);
        
       // UART2_PutString(s);
        
    }
}
//*****************************************************


/*
 Set the Drive mode based on GPS boundary state
 * By default, the carts are in Outdoor mode. Indoor modes are programmed to have Zero FWD speed.
 * So when unit switches to Indoor mode upon violating boundary, user wont be able to drive Forward
 * They will have to reverse the unit until it is back inside the boundary. 
 * In the below function, boundary action is monitored. And the 6th bit of Ulta Button byte is
 * set correspondingly to switch between Outdoor and Indoor mode
 * TODO: In Indoor mode, if someone power cycles unit, until code enters main, the unit is able to move.
 *      Maybe use persistent variables for this? 
 */

void update_driveMode(GeoBoundaryAction_e GPS_bound)
{
    //static GeoBoundaryAction_e prev_GPS_bound = 4;
    
   // if(FLAG_ST_ACK_RECD)  //Check if GPS has been detected
    //{
if(IPIN_GPS_ENABLE) {
        //if(GPS_bound != prev_GPS_bound)
        //{
            switch(GPS_bound)
            {
                case GEO_NO_ACTION: //Inside boundary
                        CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_DRIVEMODE_BIT;  //set 6th bit
                    break;
                case GEO_WARNING: //Crossed warning boundary
                        CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_DRIVEMODE_BIT;  //Set 6th bit
                    break;
                case GEO_STOP: //Crossed STOP boundary. Switch to Indoor mode
                        CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_DRIVEMODE_BIT);   //Clear 6th bit
                    break;
                default: //inout_bit = DM_OUTDOOR;
                        CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_DRIVEMODE_BIT;//0xff;

            }
          //  prev_GPS_bound = GPS_bound;
        //}
    }
   else
      CS1108_Buttonstatus.byte |= DM_OUTDOOR<<6;//CS1108_Buttonstatus.byte = 0xff;

}

//Added for Amigo ST board-Prem_Basnet

void send_cmd_uart(char funct_code){
    
    int i = 0;
    if(funct_code == 0x45){
        char uart_cmd[3] = {0x03, 0x45, 0xFF};        
        for(i = 0; i < sizeof(uart_cmd); i++){            
            UART1_Put(uart_cmd[i]);            
        }        
    }  
    
}

void  wait_and_read_response(){
    
    int index = 0;    
    _timer_uart_timeout = TIMER_Now();
    
    while(TIMER_Since(_timer_uart_timeout) < 1000 && index < sizeof(incoming_bytes)){
        if(UART1_IsGetReady()){
            incoming_bytes[index] = UART1_Get();
            index++;
        }
    }
}

int main( void )
{
     ClrWdt(); //Clear watchdog
     //Added for gps state stability - when ST is not connected, this variable reads some garbage value because it was not initialized-basnetp 5/5/2021
     if(curr_GPS_state!=GEO_NO_ACTION && curr_GPS_state!=GEO_WARNING && curr_GPS_state!=GEO_STOP ) 
     {
       curr_GPS_state=GEO_NO_ACTION;  
     }
     
     
     //this code is working fine but still needs testing!!!!!!
     
     
     DEVICE_Tasks();
    //Local variables
    ui_state = create_normal;
    bool FLAG_INFO_DISPLAYED = false;   //If user pressed horn at startup
    bool FLAG_USER_ACK= false;
    bool deb_horn_in = 1;
    TIMER_TICK _timer_main = 0, _timer_i2c = 0, _timer_main2 = 0; 
   TIMER_TICK _timer_user_ack = 0;
    TIMER_TICK _timer_battery_check = 0;
    // initialize hardware components and peripherals
    SYSTEM_Initialize();
    
    
    // Motor controller interface
    CS1108_Initialize(DLUX_BASE_ADDR);
//    DEVICECB_SendResponse();
    DEVICE_Tasks();
    //if(FLAG_ST_ACK_RECD)
        update_driveMode(curr_GPS_state);
    //////////////////////////////////////edit by prem
//         GPS_check = CS1108_Buttonstatus.byte<<ULTA_DRIVEMODE_BIT & 1 ; //update latest GPS state.//testing by basnetp
//         if(GPS_check==0){
//                curr_GPS_state= GEO_STOP;
//              //  CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_DRIVEMODE_BIT); 
//         }
//         else {
//                  curr_GPS_state=GEO_NO_ACTION;
//               //CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_DRIVEMODE_BIT;  //Set 6th bit
//         }
//
        // DEVICE_Tasks();
//if(FLAG_ST_ACK_RECD)
        //update_driveMode(curr_GPS_state);
    
    // Init graphics engine
    GFX_Initialize();
    //////////////////////////////////////////////////////
    // Serial port
    //UART1_Initialize();   //This is done inside DEVICE_initialize. Used for GPS comm.
    //UART2_Initialize();   //Use for debugging. TODO: Need to update system.c with HW config for this
    
    //Set color palette for LCD
    GFX_PaletteSet( (GFX_RESOURCE_HDR *)&CrayolaByHue, 0, 255 );

//#ifdef SmartSensor_enabled
    //Smart-sensor det
   // detect_smartsensor();
//#endif    
    //Stay here until data is read from controller or timeout reaches.
    //ACK_DONE denotes whether successful i2c comm. was initiated.
    //There is a timeout of 2 seconds
    ACK_DONE = wait_for_deviceID_request(&DISPLAY_ACK_DONE);
    
    //Initialize UART1 for GPS board comm.
    DEVICE_Initialize();
    //Set board rate for GPS comm.
    DEVICE_Open( 19200 );
    
    // Query the device to see if it is there
    CBUF_Flush(UART1_txQ);  //Empty the uart transmit buffer
    
    
   // DEVICE_Send( (MbapHeader_t *)_requestSlaveId ); //Send slaveID request to TX buffer
 //33333333333333333333333333333333333333333333333333333333   
//     if ( IPIN_GPS_ENABLE )  
//    {
//  
//        send_cmd_uart(0x45);        
//        wait_and_read_response();  //Timeout = 500ms      
//        if(incoming_bytes[0] == 0x01 && incoming_bytes[1] == 0x45 && incoming_bytes[4] == 0xFF){            
//            if(incoming_bytes[2]){ //if I have a fix
//                
//                GPS_fix = true;
//                curr_GPS_state = incoming_bytes[3];
//            }
//            else{
//                GPS_fix = false;
//                curr_GPS_state = 0;
//            }
//        }
//        else{
//            GPS_fix = false;
//            curr_GPS_state = 0;
//        }        
//        update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated
//        
//      
//        
//    }
//    
 /////333333333333333333333333333333333333333333   
    /////////////////////////////////////////////////////////////////////After I2c communication Initiation: FOR GPS -PREM
    //Trying to make gps activate when power cycled
    ///////////////////////////////////////////////////////////
   
     //switch_slave_address(DLUX_BASE_ADDR);
    // GeoBoundaryAction_e curr_GPS_state =CS1108_Buttonstatus.byte & 1 ; //update latest GPS state.//testing by basnetp    
//DEVICE_Tasks();
//update_driveMode(curr_GPS_state);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Draw the first screen with Amigo logo
    create_splashscreen();
    //DEVICE_Tasks();
    /////////////////////////////////////
//     _timer_i2c = TIMER_Now();
//     while(TIMER_Since(_timer_i2c) < 500)
//            {
//                if (I2C_ACTIVE == 1)
//                {
//                    FLAG_I2C_ACTIVE = true;
//                    I2C_ACTIVE = 0;
//                }
//                else
//                    FLAG_I2C_ACTIVE = false;
//
//
//                _timer_i2c = TIMER_Now();
//            }    
       // GeoBoundaryAction_e curr_GPS_state =CS1108_Buttonstatus.byte & 1 ;
        //Call the UI FSM function to update state
       // UI_state(_fault_code_local, &CS1108_ultaStatus, (uint8_t)curr_GPS_state);//batt_level, speed_level ); 
    delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 100);
    //__delay_ms( 100);
    // if(FLAG_ST_ACK_RECD)
    //update_driveMode(curr_GPS_state);
    deb_horn_in = debounce_horn();    //Read debounced horn IO pin state
            //Update the Horn state
            update_horn(horn_state_isr, deb_horn_in, (uint8_t)curr_GPS_state);
            
            
#ifdef DEBUG
   printf("CM %d",CS1108_ultaStatus.misc_data.bits.chg_mode);
#endif  
    
    //If keyswitch is in OFF position and Not charging, turn Off and wait for CN interrupt
    if((KSWITCH_IN)&&(!CS1108_ultaStatus.misc_data.bits.chg_mode)) //Replace the i2c bit with a PERSISTENT variable?
    {   
        
        
        curr_pwr_state  = DISP_OFF;
        //beep_horn(300);
        ui_state = sleeping;
       I2C_ACTIVE = false;//try turning it off to see if it fix this charging glitch!!!!!!if it does not help move display sleep function inside while(1) below UI STATE)
      // 
      ////Disable GFX Module
       //display_sleeps();
         LED_BACKLIGHT_OUT = 0;
         IEC6bits.GFX1IE = 0;
         G1CON1bits.G1EN = 0;
         
         //Turn OFF horn
    OPIN_HORN_OUT = 0; 
    //Turn off front facing LEDs
    OPIN_LED_1 = 0;
    OPIN_LED_2 = 0;
         
    }
 
   
    else//else turn On
    {
        SYSTEM_Backlight( 100 );
        curr_pwr_state  = DISP_ON;
       CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_QSTOP_BIT);  //activate indoor mode
         if(CS1108_ultaStatus.misc_data.bits.chg_mode)
            {FLAG_CHARGING= true;}
    }
   //OPIN_LED_1 = 1;
  // OPIN_LED_2 = 0;
   
//   while(1){
//       if(KSWITCH_IN)
//    {   
//        curr_pwr_state  = DISP_OFF;
//        OPIN_LED_1 = 1;
//        OPIN_LED_2 = 1;
//    }
//   
//   
//    else//else turn On
//    {
//        curr_pwr_state  = DISP_OFF;
//        OPIN_LED_1 = 1;
//        OPIN_LED_2 = 0;
//    } 
//       
//       
//       
//   }
   //Delay functions were replaced with delays where the horn bit is checked
   //This was so reverse horn feature would be present at bootup.
    //__delay_ms( APP_SCREEN_DELAY_MS*1);//_timer_main = TIMER_Now();
   delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 1000);
   
    

#ifdef DEBUG    
   // printf("%d",CS1108_ultaStatus.misc_data.bits.chg_mode); 
#endif 
    
    ClrWdt(); //Clear watchdog
    DEVICE_Tasks();
    
    //******************************Battery parameter display*******************************
    /*
     * This functionality was added on Walmart's request. Essentially when a user holds down
     * the horn button at the splash-screen, the display proceeds to read a few battery related
     * parameters from the CS controller. After which these values are displayed. This can be used a
     * service tech while deciding whether to warranty a battery.
     * If the tech decides to swap the batteries, it is recommended to reset some of the parameters.
     * This can be done as follows. After the parameters values are being displayed as mentioned above, 
     * release the hron button for a moment and press it again. Keep holding it. 
     * You will see a red circle on the bottom right corner that indicates the sequence has been activated. 
     * If the user goes on to hold the horn button for 30 seconds, a display board will proceed to 
     * reset the values and display a message on the LCD for the same. After this execution will move onto
     * the next screen i.e. disclaimer.
     */
    
    //switch to programmer mode
    switch_slave_address(HHP_ADDR);
    //Read a select few values from CS1108. 
    read_cs_parameters(&DATA_READ, &_cntrl_runtime, &_fault_code_local, 
                            &_chg_NoFloats, &_Low_batt);
    //switch to display mode
    switch_slave_address(DLUX_BASE_ADDR);
    
    
    ////
    
   // CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_DRIVEMODE_BIT);  //activate indoor mode
    
    
    ////
    //If user presses horn button on membrane, display the values.
    display_cs_parameters(&FLAG_INFO_DISPLAYED,&_cntrl_runtime, &_chg_NoFloats, &_Low_batt);
     ClrWdt(); //Clear watchdog
    //if horn is pressed, add an extra delay before switching screen
    if(FLAG_INFO_DISPLAYED)
    {
        delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 1000);
    }
    
   if ((FLAG_INFO_DISPLAYED) && (check_horn()))
   {
       //Draw a red dot on bottom right
       GFX_ColorSet(RED);
       GFX_LineStyleSet(GFX_LINE_STYLE_THIN_SOLID);
       //Draw a red circle on bottom right when horn button is pushed
        APP_WaitUntilFinished(GFX_CircleDraw(310,230,5))
       _timer_main = TIMER_Now();
     while(check_horn()) 
     {
        ClrWdt(); //Clear watchdog
         if(TIMER_Since(_timer_main)> 3000)//If no user button press for more than 2 seconds move to disclaimer.
        {
            clear_counters();
            continue;
        }
     }
   }
    //*************************************************************************

       //Switch to the second screen that displays the User  disclaimer.
    create_disclaimer();
    
    //date: 8/26/2024 updating display declimer to disable unit and wait for user ack signal
    delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 2000);
    
    ///////////////////////////////////////
    //////////////////////////////////////
   // while(!(check_horn()) )
    _timer_user_ack== TIMER_Now();
    
   while(!(check_horn()) && (!(CS1108_ultaStatus.misc_data.bits.chg_mode)) && (!KSWITCH_IN)&& (TIMER_Since(_timer_user_ack)<1500000))//25 min sleep time 3 min-180000
        
    //&& (!(CS1108_ultaStatus.misc_data.bits.chg_mode))) 
     {
       // CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_DRIVEMODE_BIT);  //activate indoor mode
         ClrWdt(); //Clear watchdog
       //Wait untill user ack it or unit is charging
         //testing in progress
         
                    if(!FLAG_I2C_ACTIVE)
                           {
                               curr_pwr_state  = DISP_OFF;
                               ui_state = sleeping;
                               display_sleeps();
                           }
         
    }
    delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 100);  
     CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_QSTOP_BIT;//go to outdoor mode
      
     // CS1108_Buttonstatus.byte &= ~(!DM_INDOOR<<ULTA_DRIVEMODE_BIT);  
      delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 100); 
   // update_driveMode(curr_GPS_state);
    FLAG_USER_ACK= true;
    /////////////////////////////////////////
    /////////////////////////////////////////
    
 
    
    DEVICE_Tasks();
    
    
  // if(FLAG_ST_ACK_RECD)
  //      update_driveMode(curr_GPS_state);
    
  //  delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 2000);    //Changed from 4s to 6s on Beth' request
            // DEVICE_Tasks();
            // if(FLAG_ST_ACK_RECD)
            //    update_driveMode(curr_GPS_state);
  //  delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 2000);
        // DEVICE_Tasks();
        // if(FLAG_ST_ACK_RECD)
        //   update_driveMode(curr_GPS_state);
  //  delay_with_Revhorn(&horn_state_isr, &deb_horn_in, 2000);
    //
    //Initialise all time variables before entering main
    _timer_main = TIMER_Now();

    _timer_i2c = TIMER_Now();
    
    _timer_main2 = TIMER_Now();
    
    _timer_battery_check=TIMER_Now();
    // ClrWdt(); //Clear watchdog
     
  //  CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_DRIVEMODE_BIT;//go to outdoor mode
CS1108_Buttonstatus.byte |= DM_OUTDOOR<<ULTA_QSTOP_BIT;//go to outdoor mode
      
     while( 1 )
    {
       
            if(TIMER_Since(_timer_i2c) > 500)
            {
                if (I2C_ACTIVE == 1)
                {
                    FLAG_I2C_ACTIVE = true;
                    I2C_ACTIVE = 0;
                }
                else
                    FLAG_I2C_ACTIVE = false;


                _timer_i2c = TIMER_Now();
            }
        
        
        //Call the UI FSM function to update state
        UI_state(_fault_code_local, &CS1108_ultaStatus, (uint8_t)curr_GPS_state);//batt_level, speed_level );    
        //Update only when not in sleep mode
        if((ui_state != sleeping)&&(curr_pwr_state != DISP_OFF))
        {
            deb_horn_in = debounce_horn();    //Read debounced horn IO pin state
            //Update the Horn state
            update_horn(horn_state_isr, deb_horn_in, (uint8_t)curr_GPS_state);
        }
        
#ifdef DEBUG
        if(prev_state_w != ui_state)  
        {
            printf(" S: %d ", ui_state);
            prev_state_w = ui_state;
        }
#endif
        
      
      
       //if(!FLAG_INFO_DISPLAYED) 
       //{    
        
        /* 
        if(((TIMER_Now() - _timer_main2) > 1000) && (!FLAG_INFO_DISPLAYED))
        {
            if(ui_state != display_charging)
            {
                _fault_code_local = update_fault(_fault_code_local);
                //_seat_switch_state = update_seatswitch();
            }
            _timer_main2 = TIMER_Now();
        }    
        */
          //While charging the HHP will have issues talking to the display
        //because the charge stage is read from controller in HHP mode
        //Dont connect the HHP in charging mode. To be able to connect HHP in charging, 
        // you have to activate debug mode by pressing horn button at startup &  display parameters
        //Check charging stage 
        if((TIMER_Since(_timer_main) > 5000) && (!FLAG_INFO_DISPLAYED))
         {
             if((ui_state == display_charging)&&(FLAG_I2C_ACTIVE))//&&(!FLAG_INFO_DISPLAYED))
                 {
#ifdef DEBUG                 
                     printf("CS:");
#endif
                     //Every few seconds read the RAM location 0x5F to get the charge mode
                     CHARGE_STAGE = update_chargemode();
#ifdef DEBUG
                     printf("%d", CHARGE_STAGE);
#endif
                 }

                  _timer_main = TIMER_Now();
              
         }
    
//    // Auto-detect when a GPS module is connected.
//    // If in the first 5 seconds, GPS board acknowledgement is not received, 
//    // dont execute GPS related funcs.
//   //if ( (TIMER_Since(_timer_main2) < 5000) || (FLAG_ST_ACK_RECD == true) ) 
//        
//      if ( (TIMER_Since(_timer_main2) > 5000) || (FLAG_ST_ACK_RECD == true) )  
//    
//       
//       // if (FLAG_ST_ACK_RECD ) 
//    {
//        DEVICE_Tasks();                             //Transmit any GPS commands in buffer
//        update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated
//    }
  ///////////////////////////////
//        if  (FLAG_ST_ACK_RECD == true )  
//        {
//          DEVICE_Tasks();                             //Transmit any GPS commands in buffer
//          update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated
//         }
//        
   //Added new function for ST detection:
        
        
        if ( IPIN_GPS_ENABLE && (TIMER_Since(_timer_main2) > 4000) )  
    {
  
        send_cmd_uart(0x45);        
        wait_and_read_response();  //Timeout = 500ms      
        if(incoming_bytes[0] == 0x01 && incoming_bytes[1] == 0x45 && incoming_bytes[4] == 0xFF){            
            if(incoming_bytes[2]){ //if I have a fix
                
                GPS_fix = true;
                curr_GPS_state = incoming_bytes[3];
                update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated

            }
            else{
                GPS_fix = false;
                curr_GPS_state = 0;
                update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated

            }
        }
        else{
            GPS_fix = false;
            curr_GPS_state = 0;
        }        
       // update_driveMode(curr_GPS_state);  //Switch Drive mode if Bound condition is violated
        
        _timer_main2 = TIMER_Now();
        
    }     
      
     #ifdef SmartSensor_enabled
    //Read Smart-sensor status on pin RF7
    if((ui_state != sleeping)&&(curr_pwr_state != DISP_OFF)){
        update_smartsensor_status();
         if(SS_Equipped==0){  //Do not scan once it gets persistence signal.
            
            Check_SmartSensorEnabled();  
        }
    }//end of if ui state!=sleeping or curr pwr state!=disp off
        
#endif     
    ClrWdt(); //Clear watchdog;    
  
     }   //end of while(1);
}   //End of main())



/**
 * Hook for C library printf function(s).
 *
 * Prints the message to the serial port
 */
int __attribute__( ( __section__( ".libc.write" ) ) ) write( int handle, void *buffer, unsigned int len )
{

    char *p = (char *)buffer;
    unsigned int i;

    for( i = 0; i < len; i++ )
    {
        UART1_Put( *p++ );
    }

    return ( len );
}
