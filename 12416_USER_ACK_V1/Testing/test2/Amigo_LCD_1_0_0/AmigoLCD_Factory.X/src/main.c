/**
 * Entry point for Factory Test application
 *
 * Copyright (c) 2019 Control Solutions LLC. All Rights Reserved.
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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//------------------------------//----------------------------------------------
// Definitions

#define DEBUG_MODE

#define APP_WaitUntilFinished( x ) while( !x )

#define APP_SCREEN_DELAY_MS ( 1000 )

// Number of pixels around the text area of a box
// So the text doesn't go right to the edge of the box
#define PAD_BOX 3

// Define the look of the title box
#define POS_TITLE_TOP 1
#define POS_TITLE_LEFT 1
#define POS_TITLE_RIGHT POS_TITLE_LEFT + GFX_MaxXGet() - 2
#define POS_TITLE_HEIGHT ( GFX_MaxYGet() / 4 )
#define COLOR_TITLE_BOX CS_BLUE
#define COLOR_TITLE_TEXT WHITE
#define TITLE_FONT (GFX_RESOURCE_HDR *)&FiraMono_Regular_36

// Define the look of the status box
#define POS_STATUS_TOP GFX_MaxYGet() / 4
#define POS_STATUS_LEFT 1
#define POS_STATUS_RIGHT POS_STATUS_LEFT + GFX_MaxXGet() - 2
#define POS_STATUS_HEIGHT GFX_MaxYGet() / 4
#define COLOR_STATUS_BOX WHITE
#define COLOR_STATUS_TEXT CS_BLUE
#define STATUS_FONT (GFX_RESOURCE_HDR *)&FiraMono_Regular_36

// Define the look of the info box
#define POS_INFO_TOP GFX_MaxYGet() / 2
#define POS_INFO_LEFT 1
#define POS_INFO_RIGHT POS_INFO_LEFT + GFX_MaxXGet() - 2
#define POS_INFO_HEIGHT GFX_MaxYGet() / 2
#define COLOR_INFO_BOX CS_BLUE
#define COLOR_INFO_TEXT WHITE
#define INFO_FONT (GFX_RESOURCE_HDR *)&Arial_Bold_14

#define DLUX_BASE_ADDR 0x4E
#define ULTA_BASE_ADDR 0x40
#define HHP_ADDR 0x80

//#define BLACK GFX_RGBConvert(0, 0, 0)//INDEX_0
//#define WHITE GFX_RGBConvert(255, 255, 255)//INDEX_255
//#define RED GFX_RGBConvert(128, 0, 0)//INDEX_249

//#define CS_BLUE BLUE
//------------------------------//----------------------------------------------
// Global variables

//------------------------------//----------------------------------------------
// Local variables

static uint8_t _lastHornIn = 0;
static uint8_t _lastButton1In = 0;
static uint8_t _lastButton2In = 0;
static uint8_t _cntrl_runtime = 23;
static uint8_t _chg_NoFloats = 45;
static uint8_t _Low_batt = 4;
static _ulta_wa _lastUltaStatus;
static uint8_t batt_num = 100;



//Different states of the User Interface screen
typedef enum 
{       create_normal, display_normal, 
        create_fault, display_fault, 
        create_charging, display_charging
}UI_STATE;
UI_STATE ui_state = create_normal;
//Character constants
GFX_XCHAR disclaimer_main_18point[] = {'F','O','R',' ','Y','O','U','R',' ','S','A','F','E','T','Y',':',0xa,0x0a,
                                'T','h','i','s',' ','c','a','r','t',' ','i','s',' ','f','o','r',' ','i','n','d','o','o','r',' ','u','s','e',' ','o','n','l','y', '.', 0x0a,                     
                                'S','t','a','y',' ','a','t',' ','l','e','a','s','t',' ','3',' ','f','e','e','t',' ','a','w','a','y',' ','f','r','o','m',0x0a, 
                                'p','e','o','p','l','e',' ','a','n','d',' ','t','u','r','n',' ','c','a','r','t',' ','o','f','f',' ','w','h','e','n',0x0a,
                                's','t','o','p','p','e','d',' ','i','n',' ','c','h','e','c','k','o','u','t',' ','l','a','n','e','.','B','e','f','o','r','e',0x0a,
                                'b','a','c','k','i','n','g',' ','u','p',',','p','l','e','a','s','e',' ','t','u','r','n',' ','a','r','o','u','n','d',' ','t','o',0x0a,
                                's' ,'e','e',' ','i','f',' ','a','n','y','o','n','e',' ','i','s',' ','b','e','h','i','n','d',' ','y','o','u','.',0x0a,
                                'Y','o','u',' ','a','r','e',' ','r','e','s','p','o','n','s','i','b','l','e',0x0a,'f','o','r',' ','y','o','u','r',' ','d','r','i','v','i','n','g','!',0 
                                };
GFX_XCHAR amigo_address[] = {'A','m','i','g','o',' ','M','o','b','i','l','i','t','y',' ','I','n','t','e','r','n','a','t','i','o','n','a','l',',',' ','I','n','c','.',0x0a,
                                '6','6','9','3',' ','D','i','x','i','e',' ','H','i','g','h','w','a','y',0x0a,
                                'B','r','i','d','g','e','p','o','r','t',',',' ','M','I',' ','4','8','7','2','2', 0
                                };

GFX_XCHAR battlevel_num[] = {'9','0',0};//{'9', '0',0}; 
GFX_XCHAR battlevel_time[] = {'~',' ','6','h',' ', '3', '0', 'm',' ','l','e','f','t',0};
GFX_XCHAR perc_sign[] = {'%',0};
GFX_XCHAR emergency_text[] = {'E','M','E','R','G','E','N','C','Y',0};
GFX_XCHAR faultcode_throttle[] = {'5','3',':','T','h','r','o','t','t','l','e',' ','F','a','u','l','t',0};
GFX_XCHAR faultcode_brakeopen[] = {'2','1',':','B','R','A','K','E',' ','O','P','E','N',0};
GFX_XCHAR charging_msg[] = {'C','H','A','R','G','I','N','G',0};
GFX_XCHAR status_reverse_mode[] = {'R','E','V','E','R','S','E',' ','O','N','L','Y',' ','M','O','D','E',' ','A','C','T','I','V','E',0};
GFX_XCHAR batt_life_text[] = {'B','a','t','t','e','r','y',' ','l','i','f','e',0};


//------------------------------//----------------------------------------------
// Local functions

/**
 * Draw border lines to show the limits of the screen view area
 *
 */
/*
static void _DrawScreen( void )
{
    GFX_ColorSet( WHITE );
    APP_WaitUntilFinished( GFX_LineDraw( 0, 0, GFX_MaxXGet(), 0 ) );
    APP_WaitUntilFinished( GFX_LineDraw( GFX_MaxXGet(), 0, GFX_MaxXGet(), GFX_MaxYGet() ) );
    APP_WaitUntilFinished( GFX_LineDraw( 0, GFX_MaxYGet(), GFX_MaxXGet(), GFX_MaxYGet() ) );
    APP_WaitUntilFinished( GFX_LineDraw( 0, 0, 0, GFX_MaxYGet() ) );
}

static void _SetTitle( const char *p )
{
    // Draw the title box
    GFX_ColorSet( COLOR_TITLE_BOX );
    APP_WaitUntilFinished(
        GFX_RectangleFillDraw( POS_TITLE_LEFT, POS_TITLE_TOP, POS_TITLE_RIGHT, POS_TITLE_TOP + POS_TITLE_HEIGHT ) );

    GFX_ColorSet( COLOR_TITLE_TEXT );
    //GFX_FontSet( TITLE_FONT );

    APP_WaitUntilFinished( GFX_TextStringBoxDraw(
        POS_TITLE_LEFT + PAD_BOX, POS_TITLE_TOP + PAD_BOX, POS_TITLE_RIGHT - PAD_BOX,
        POS_TITLE_TOP + POS_TITLE_HEIGHT - PAD_BOX - PAD_BOX, (GFX_XCHAR *)p, 0, GFX_ALIGN_CENTER ) );
}

static void _SetStatus( const char *p )
{
    printf( "Status: %s\n", p );

    GFX_ColorSet( COLOR_STATUS_BOX );
    APP_WaitUntilFinished( GFX_RectangleFillDraw( POS_STATUS_LEFT, POS_STATUS_TOP, POS_STATUS_RIGHT,
                                                  POS_STATUS_TOP + POS_STATUS_HEIGHT ) );

    GFX_ColorSet( COLOR_STATUS_TEXT );
    //GFX_FontSet( STATUS_FONT );

    APP_WaitUntilFinished( GFX_TextStringBoxDraw(
        POS_STATUS_LEFT + PAD_BOX, POS_STATUS_TOP + PAD_BOX, POS_STATUS_RIGHT - PAD_BOX,
        POS_STATUS_TOP + POS_STATUS_HEIGHT - PAD_BOX - PAD_BOX, (GFX_XCHAR *)p, 0, GFX_ALIGN_TOP ) );
}

static void _SetInfo( const char *p, GFX_COLOR color )
{
    GFX_ColorSet( color );
    APP_WaitUntilFinished(
        GFX_RectangleFillDraw( POS_INFO_LEFT, POS_INFO_TOP, POS_INFO_RIGHT, POS_INFO_TOP + POS_INFO_HEIGHT ) );

    GFX_ColorSet( COLOR_INFO_TEXT );
    GFX_FontSet( INFO_FONT );

    APP_WaitUntilFinished( GFX_TextStringBoxDraw(
        POS_INFO_LEFT + PAD_BOX, POS_INFO_TOP + PAD_BOX, POS_INFO_RIGHT - PAD_BOX,
        POS_INFO_TOP + POS_INFO_HEIGHT - PAD_BOX - PAD_BOX, (GFX_XCHAR *)p, 0, GFX_ALIGN_BOTTOM ) );
}

 * */
/**
 * @brief      Called if any of the tests failrxq
 *
 * @param[in]  p     string description of failure
 */
/*
static void _TestFailed( const char *p )
{
    _SetInfo( p, RED );

    while( 1 )
        ;
}

static bool _TestNvm( void )
{
    bool result = true;

    _SetTitle( "External Flash" );
    _SetStatus( "Checking" );

    // Check presence of chip 1
    EXTFLASH_BeginTransaction( 1 );
    uint32_t jedec = EXTFLASH_JEDEC();
    EXTFLASH_EndTransaction();

    if( jedec != SPI_C_ID_W25M512JV )
        result = false;

    printf( "Flash1: %08lX %s\n", jedec, result ? "OK" : "Not Detected" );

    if( !result )
    {
        _TestFailed( "External Flash not found" );
    }

    return true;
}
*/
/**
 * @brief      Turn the backlight on and off so user can observe
 *
 * We don't have any way to detect a failure so this always passes
 *
 * @return     true
 */
/*
static bool _TestBacklight( void )
{
    _SetTitle( "Backlight" );
    _SetStatus( "On" );

    __delay_ms( 500 );

    SYSTEM_Backlight( 0 );
    _SetStatus( "Off" );

    __delay_ms( 500 );

    SYSTEM_Backlight( 100 );

    return true;
}
*/
/**
 * @brief      Turn the horn on briefly
 *
 * We don't have any way to detect a failure so this always passes
 *
 * @return     true
 */
/*
static bool _TestHorn( void )
{
    _SetTitle( "Horn" );
    _SetStatus( "On" );

    //Commented out to stop beeping during testing.
    //OPIN_HORN_OUT = 1;

    __delay_ms( 500 );

    _SetStatus( "Off" );

    OPIN_HORN_OUT = 0;

    __delay_ms( 500 );

    return true;
}
*/
/**
 * @brief      Outputs the result of the test
 *
 * @param[in]  result  boolean true if all tests passed
 *
 * @return     { description_of_the_return_value }
 */
/*
static void _TestResult( bool result )
{
    _SetTitle( "Test" );
    if( result )
        _SetStatus( "Passed" );
    else
        _SetStatus( "Failed" );
}
*/
/*
static void _UpdateInfoBox( void )
{
    char s[ 250 ];

    // can fit about 6 lines on screen
    sprintf( s,
             "H: %d  B1: %d  B2:%d  LED: %d\nIndoor: %d\nLow Volts: %d\nBrake: %d  QSTOP: %d\nCharge: %d\nReverse: %d",
             _lastHornIn, _lastButton1In, _lastButton2In, _lastUltaStatus.misc_data.bits.temp_led,
             _lastUltaStatus.misc_data.bits.indoor, _lastUltaStatus.misc_data.bits.low_volts,
             _lastUltaStatus.misc_data.bits.brk_rel, _lastUltaStatus.misc_data.bits.q_stopn,
             _lastUltaStatus.misc_data.bits.chg_mode, CS1108_ultaStatus.spare1//_lastUltaStatus.misc_data.bits.reverse

    );

    _SetInfo( s, COLOR_INFO_BOX );
}
*/
/**
 * @brief      Create the second UI screen. 
 *
 * Create the screen, print the disclaimer text.
 *
 * @return     none
 */
void create_disclaimer(void)
{
    GFX_ColorSet(DISCLAIMER_BKGCLR);
    GFX_ScreenClear();
    GFX_ColorSet(DISCLAIMER_TXTCLR);
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
    GFX_TextStringBoxDraw(5,5,310,230,disclaimer_main_18point, 0, GFX_ALIGN_CENTER);
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

/**
 * @brief      Display debug battery info on Logo splash screen 
 *
 * If the user presses the horn button
 *
 * @return     true if pushed, false else
 */
void display_battery_info()
{
    //If the user has pushed down on the horn button while powering up the unit
    //the splashscreen will have info related to battery on the bottom right corner.
    //This is mainly for technicians to warranty battery?
    //Note: Need to replace the arguments for text location with #define ones.
     char s[ 50 ];

    // can fit about 6 lines on screen
    sprintf( s,
             "1: %d\n2: %d\n3: %d",
             _cntrl_runtime, _chg_NoFloats, _Low_batt);
    
    GFX_FontSet(&Arial_10);
    GFX_ColorSet(BLACK);
    APP_WaitUntilFinished(GFX_TextStringBoxDraw(280,190,50,13*3,s, 0, GFX_ALIGN_LEFT));
}

/**
 * @brief      Create the footer on UI
 *
 * This will be called inside the UI FSM. Value can be faultcode or charging status
 *
 * @return     None
 */
void update_footer(char DRIVE_STATE)
{
    
    switch(DRIVE_STATE)
    {
        case create_normal:
            GFX_ColorSet(DRIVE_BKGCLR);
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            break;
        
        case create_fault:
            GFX_ColorSet(FOOTER_CLR_RED);//(253,238,57));   //yellow
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            GFX_ColorSet(BLACK);
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
            GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,faultcode_brakeopen,0, GFX_ALIGN_CENTER);
            break;
        
        /*case create_warning:
            GFX_ColorSet(FOOTER_CLR_YELLOW);//(253,238,57));   //yellow
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            GFX_ColorSet(BLACK);
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
            GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,status_reverse_mode,0, GFX_ALIGN_CENTER);
            break;
        */
        case create_charging:
            GFX_ColorSet(FOOTER_CLR_GREEN);//(253,238,57));   //yellow
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            GFX_ColorSet(BLACK);
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
            GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,charging_msg,0, GFX_ALIGN_CENTER);
            break;
    }
    //Switch case to decide color for the message bar goes here.
    
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
    //Draw emergency stop 
    GFX_ImageDraw(235,6, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_STOPSIGN);
    //and arrow icon 
    GFX_ImageDraw(295,25, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_ARROWSIGN);
    //Amigo corporate logo topleft
    GFX_ImageDraw(15,13, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_AMIGOBW);
    //Battery Icon
    //Draw text for emergency stop
    GFX_ColorSet(BLACK);
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
    GFX_TextStringBoxDraw(130,18,strlen(emergency_text)*12,17,emergency_text,0, GFX_ALIGN_CENTER);
     //Battery life description
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_14);
    //Sizeof returns bytes. GFX_CHAR seems to use 2 bytes per char. so divide by 2 to find # of char.
    GFX_TextStringBoxDraw(120,160,strlen(batt_life_text)*12,20,batt_life_text,0, GFX_ALIGN_CENTER);   
}


/**
 * @brief      Draw and update the battery icon on the UI
 *
 * In normal operation draw once. In charging state, use timer module to update charge level.
 *
 * @return     None
 */
void update_batt_icon(UI_STATE state)
{
    typedef enum{
        LOW, 
        MEDIUM, 
        FULL}charge;
        
        static charge batt_level = LOW;
        static TIMER_TICK prevTick = 0;
        
    switch(state)
    {
        case create_normal:
        case display_normal:
        case create_fault:
        case display_fault:
        case create_charging:
            prevTick = TIMER_Now();
            //Battery Icon
            GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATTICON_FULL);
            break;
            
        case display_charging:
            if(TIMER_Since(prevTick)>1000)
            {   
                if(batt_level<=FULL)
                    batt_level++;
                else
                    batt_level = LOW;
                
                prevTick = TIMER_Now();
             GFX_ColorSet(DRIVE_BKGCLR);  
             GFX_RectangleFillDraw(62,105,85,147);
            }
                
            switch(batt_level)
            {
                case FULL:
                    //Draw 3 rect
                    GFX_ColorSet(GREEN);
                    GFX_RectangleFillDraw(62,105,85,117);
                    //while(1);
                    //break;
                case MEDIUM:
                    //Draw 2 rect
                    //GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
                    GFX_ColorSet(ORANGE);
                    GFX_RectangleFillDraw(62,120,85,132);
                    //break;
                case LOW:
                    //Draw 1 rect
                    //GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
                    GFX_ColorSet(RED);
                    GFX_RectangleFillDraw(62,135,85,147);
                    break;
            }
            //Battery Icon
            //GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATTICON);
            //GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATTICON_FULL);
                
            //GFX_ImageDraw(batt_icon_pos_x, batt_icon_pos_y, (GFX_RESOURCE_HDR*)MENU_INTERNAL_FLASH_BATTICON);
            break;
        
    }
    
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
     //Battery level numeric 
    GFX_ColorSet(BLACK);
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Black_75);
    //GFX_TextStringBoxDraw(body_midpoint_x-(batt_num_size_x/2)*((sizeof(battlevel_num))-1),batt_lvl_y,batt_num_size_x*(sizeof(battlevel_num)-1),batt_num_size_y,battlevel_num,0, GFX_ALIGN_CENTER);
    sprintf(batt, "%d",battery_level);
    GFX_TextStringBoxDraw(batt_num_pos_x,batt_num_pos_y,batt_font_size_x*(strlen(batt)),batt_font_size_y, batt, 0, GFX_ALIGN_LEFT);//batt_font_size_x*(sizeof(batt))
    //Print smaller percentage
    GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
    //GFX_TextStringBoxDraw(batt_num_pos_x+batt_font_size_x*4,batt_num_pos_y+50,batt_num_pos_x+batt_font_size_x*4+18, 18*1,batt_font_size_y, "%", 0, GFX_ALIGN_LEFT);
    //GFX_TextStringBoxDraw(perc_pos_x,perc_pos_y,perc_pos_x+perc_font_size_x, perc_pos_y+perc_font_size_y, "%", 0, GFX_ALIGN_LEFT);
    GFX_TextStringBoxDraw(250,130,perc_font_size_x, perc_font_size_y, "%", 0, GFX_ALIGN_LEFT);
}

/**
 * @brief      State machine for UI screen
 *
 * This will be called inside the Main periodically
 *
 * @return     None
 */
void UI_state()
{
    //static enum UI_STATE ui_state = create_normal;
    static TIMER_TICK prevTick; 
    
    switch(ui_state)
    {
        
        case create_normal:
            // Draw the main user interface screen. 
            GFX_ColorSet(DRIVE_BKGCLR);
            GFX_ScreenClear();           
            update_battlevel_num(100);
            update_footer(create_normal);
           // create_dash();
            create_static_symbols();
            ui_state = display_normal;
            prevTick = TIMER_Now();
            break;
        
        case display_normal:
            update_batt_icon(ui_state);
            if(TIMER_Since(prevTick)>3000)
                ui_state = create_fault;
            break;
            
        case create_fault:
            //GFX_ColorSet(DRIVE_BKGCLR);//remove later?
            //GFX_ScreenClear();//remove later?
            GFX_ColorSet(FOOTER_CLR_RED);//(253,238,57));   //yellow
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            GFX_ColorSet(BLACK);
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
            GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,faultcode_brakeopen,0, GFX_ALIGN_CENTER);
            ui_state = display_fault;
            prevTick = TIMER_Now();
            break;
            
        case display_fault:  
             update_batt_icon(ui_state);
            if(TIMER_Since(prevTick)>3000)
                ui_state = create_charging;
           break;
            
        case create_charging:
            //GFX_ColorSet(DRIVE_BKGCLR);//remove later?
            //GFX_ScreenClear();//remove later?
            GFX_ColorSet(FOOTER_CLR_GREEN);//(253,238,57));   //yellow
            GFX_RectangleFillDraw(footer_topleft_x,footer_topleft_y,footer_bottomright_x,footer_bottomright_y);
            GFX_ColorSet(BLACK);
            GFX_FontSet((GFX_RESOURCE_HDR*)&Arial_Bold_18);
            GFX_TextStringBoxDraw(footer_text_top_x,footer_text_top_y,319,footer_text_height,charging_msg,0, GFX_ALIGN_CENTER);
            ui_state = display_charging;
            prevTick = TIMER_Now();
            break;
            
        case display_charging:
            update_batt_icon(ui_state);
             if(TIMER_Since(prevTick)>3000)
                //ui_state = create_normal;
            break;
            
        default: break;    
            
    }
    
}

void create_splashscreen()
{
    // Splash image
    APP_WaitUntilFinished( GFX_ImageDraw( 0, 0, APP_INTERNAL_FLASH_SPLASH_IMAGE) );
    //print address text
    GFX_ColorSet(BLACK);
    GFX_FontSet(&Arial_Narrow_12);
    APP_WaitUntilFinished(GFX_TextStringBoxDraw(20,180,280,17*3,amigo_address, 0, GFX_ALIGN_CENTER));
    //Display debug info if user had pressed horn at powerup
    if(check_horn())
        display_battery_info();
}


//------------------------------//----------------------------------------------
// Public functions

int main( void )
{
    uint8_t _temp = 0, _prevtemp = 0;
    bool print_flag = true;
    //memset( &RX_data, sizeof(CS_read_data), 0 );// clear full struct
    
    //RX_data.fault.addr_bytes = {0x01, 0x38};
    
    // initialize hardware components and peripherals
    SYSTEM_Initialize();

    // Init graphics engine
    GFX_Initialize();

    // Serial port
    UART1_Initialize();
    
    // Motor controller interface
    CS1108_Initialize(DLUX_BASE_ADDR);
    
    //while(!DISPLAY_ACK_DONE){};
    
    //__delay_ms(100);
    
    
    //DRV_GFX_PaletteDisable();
    GFX_PaletteSet( (GFX_RESOURCE_HDR *)&CrayolaByHue, 0, 255 );

    // we are optimists
    bool result = true;

    
    //GFX_ColorSet( BLACK );
    GFX_ScreenClear();
    SYSTEM_Backlight( 100 );

    create_splashscreen();
    
    
    __delay_ms( APP_SCREEN_DELAY_MS * 1 );
    
    FAULT_CODE_READ = false;
    
    create_disclaimer();
    //Read fault code
    //CS1108_Initialize(HHP_ADDR);
    
    //__delay_ms(100);
    //while(!FAULT_CODE_READ){};
            // Motor controller interface
    //CS1108_Initialize(DLUX_BASE_ADDR);
    
    __delay_ms( APP_SCREEN_DELAY_MS * 1 );
    
    
    //_DrawScreen();

    //_SetInfo( "Control Solutions LLC Test App", COLOR_INFO_BOX );

    //result &= _TestNvm();

    //result &= _TestBacklight();

    //result &= _TestHorn();

    //_TestResult( result );

    TIMER_TICK _timer = TIMER_Now();
    

    memset( &_lastUltaStatus, 0, sizeof( _ulta_wa ) );

    //GFX_ColorSet( CS_BLUE );  // 1
    //GFX_ScreenClear();

    while( 1 )
    {
        
        // check for input states
      /*  if( IPIN_HORN_IN != _lastHornIn )
        {
            _lastHornIn = IPIN_HORN_IN;
            _UpdateInfoBox();
            printf( "Horn: %d\n", _lastHornIn );
        }

        if( IPIN_BUTTON_1 != _lastButton1In )
        {
            _lastButton1In = IPIN_BUTTON_1;
            _UpdateInfoBox();
            printf( "Button1: %d\n", _lastButton1In );
        }

        if( IPIN_BUTTON_2 != _lastButton2In )
        {
            _lastButton2In = IPIN_BUTTON_2;
            _UpdateInfoBox();
            printf( "Button2: %d\n", _lastButton2In );
        }

        
        // ulta_status
        if( CS1108_ultaStatus.aux_data != _lastUltaStatus.aux_data )
        {
            _lastUltaStatus.aux_data = CS1108_ultaStatus.aux_data;
            _UpdateInfoBox();
            //printf( "AUX: %02X\n", _lastUltaStatus.aux_data );
        }

        if( CS1108_ultaStatus.led_data != _lastUltaStatus.led_data )
        {
            _lastUltaStatus.led_data = CS1108_ultaStatus.led_data;
            _UpdateInfoBox();
            //printf( "LED: %02X\n", _lastUltaStatus.led_data );
        }

        if( CS1108_ultaStatus.mode_data != _lastUltaStatus.mode_data )
        {
            _lastUltaStatus.mode_data = CS1108_ultaStatus.mode_data;
            _UpdateInfoBox();
            //printf( "MODE: %02X\n", _lastUltaStatus.mode_data );
        }

        if( ( CS1108_ultaStatus.misc_data.byte & 0xFF ) != ( _lastUltaStatus.misc_data.byte & 0xFF ) )
        {
            _lastUltaStatus.misc_data.byte = CS1108_ultaStatus.misc_data.byte;
            _UpdateInfoBox();
            //printf( "MISC: %02X\n", _lastUltaStatus.misc_data.byte );
        }
        
  */      _temp = RX_data.fault.rx_data;
    
         if( CBUF_Len( I2C_rxQ ) > 2 )
         {
             uint8_t flag = CBUF_Pop( I2C_rxQ );
             uint8_t rx = CBUF_Pop( I2C_rxQ );
             uint8_t tx = CBUF_Pop( I2C_rxQ );
             printf( ": %c%c %02X %02X\n", ( flag & 0x20 ) ? 'D' : 'A', ( flag & 0x04 ) ? 'R' : 'W', rx, tx );
         }
        
         //printf( "MODE: %02X\n", _lastUltaStatus.mode_data );   
        // printf( "MISC: %02X\n", _lastUltaStatus.misc_data.byte);
         //printf( "LED: %02X\n", _lastUltaStatus.led_data );
        // printf( "AUX: %02X\n", _lastUltaStatus.aux_data );
        
        if(FAULT_CODE_READ & print_flag) 
        {
            printf("FAULT: %02X\n", _temp);
            print_flag = false;
            //FAULT_CODE_READ_EXT = false;
        }
        
            //_prevtemp = _temp;
        
        
        if( TIMER_Since( _timer ) >= 1000 )
        {
            //OPIN_LED_1 = !OPIN_LED_1;
            OPIN_LED_2 = !OPIN_LED_2;
            OPIN_GPS_ENABLE = !OPIN_GPS_ENABLE;

            _timer = TIMER_Now();
        }
        //_UpdateInfoBox();
        //UI_state();    
       // CS1108_Tasks();
    }
}

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
