/**
 * Hardware Initialization
 *
 *
 * Copyright (c) 2019 Control Solutions LLC. All Rights Reserved.
 *
 * @private
 *
 */

#include "driver/gfx/drv_gfx_display.h"
#include "nvm_w25.h"
#include <stdlib.h>
#include <system.h>
#include <xc.h>

// CONFIG4

// CONFIG3
#pragma config WPFP = WPFP255     // Write Protection Flash Page Segment Boundary (Highest Page (same as page 170))
#pragma config SOSCSEL = SOSC     // Secondary Oscillator Power Mode Select
#pragma config WUTSEL = LEG       // Voltage Regulator Wake-up Time Select (Default regulator start-up time is used)
#pragma config ALTPMP = ALTPMPEN  // Alternate PMP Pin Mapping (EPMP pins are in alternate location mode)
#pragma config WPDIS = WPDIS      // Segment Write Protection Disable (Segmented code protection is disabled)
#pragma config WPCFG = WPCFGDIS   // Write Protect Configuration Page Select (Last page (at the top of program memory)
                                  // and Flash Configuration Words are not write-protected)
#pragma config WPEND = WPENDMEM   // Segment Write Protection End Page Select (Protected code segment upper boundary is
// at the last page of program memory; the lower boundary is the code page specified by
// WPFP)

// CONFIG2
#pragma config POSCMOD = NONE  // Primary Oscillator Select (Primary oscillator is disabled)
#pragma config IOL1WAY = OFF   // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared as needed, provided
                               // the unlock sequence has been completed)
#pragma config OSCIOFNC = ON   // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD  // Clock Switching and Fail-Safe Clock Monitor
#pragma config FNOSC = FRCPLL  // Initial Oscillator Select (Fast RC Oscillator with Postscaler and PLL module (FRCPLL))
#pragma config PLL96MHZ = ON   // 96MHz PLL Startup Select (96 MHz PLL is enabled automatically on start-up)
#pragma config PLLDIV = DIV2   // 96 MHz PLL Prescaler Select (Oscillator input is divided by 2 (8 MHz input))
#pragma config IESO = OFF      // Internal External Switchover (IESO mode (Two-Speed Start-up) is disabled)

// CONFIG1
#pragma config WDTPS = PS32768  // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128    // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF     // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF     // Watchdog Timer (Watchdog Timer is disabled)
#pragma config ICS = PGx3       // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC3/PGED3)
#pragma config GWRP = OFF       // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF        // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF     // JTAG Port Enable (JTAG port is disabled)

//------------------------------//----------------------------------------------
// Definitions

//------------------------------//----------------------------------------------
// Global variables

volatile TIMER_TICK _timerTick;

//------------------------------//----------------------------------------------
// Local variables

static bool _bBacklightOn = false;

//------------------------------//----------------------------------------------
// Local functions

void SYSTEM_TickInit( void );

//------------------------------//----------------------------------------------
// Public functions

void SYSTEM_Backlight( unsigned int percent )
{
    if( percent )
    {
        _LATB2 = 1;
        _bBacklightOn = true;
    }
    else
    {
        _LATB2 = 0;
        _bBacklightOn = false;
    }
}

// *****************************************************************************
/**
 *  Powerup Initialization
 *
 *  Call once at startup to initialize the chip and peripherals
 *
 */

void SYSTEM_Initialize( void )
{
    // reset all the reasons-for-reset flags.  Want this for
    // debugging so when there is a reset we can tell why
    RCON = 0;

    //------------------------------//----------------------------------------------
    // Clock

    // Clock already running as needed from CONFIG words (FRCDIV with PLL on)
    // FOSC=32MHZ=(96MHZ/1/3),FCY=16MHZ,FRC=8MHZ,DOZE=OFF(4MHZ)
    OSCCON = 0b000100010000000;
    CLKDIV = 0b000000000010000;  // fastest clock, enable pll

    //------------------------------//----------------------------------------------
    // Interrupts

    // I2C slave interrupt needs to be high priority due to timing requirements
    IPC21bits.SI2C3P = 6;

    // Timer 3 interrupt priority
    IPC2bits.T3IP = 3;

    // graphics interrupt priority
    IPC25bits.GFX1IP = 5;

    // uart interrupt priority
    IPC2bits.U1RXIP = 1;
    IPC3bits.U1TXIP = 1;

    // ---------------------------------------------------------
    // Make sure the display DO NOT flicker at start up
    // ---------------------------------------------------------

    DisplayBacklightConfig();

    ANSCbits.ANSC14 = 0;    // set pin as digital pin //PIC24FJ256DA206
    ANSCbits.ANSC13 = 0;    // set pin as digital pin //PIC24FJ256DA206
    TRISCbits.TRISC14 = 1;  // set as input //PIC24FJ256DA206
    TRISCbits.TRISC13 = 1;  // set as input //PIC24FJ256DA206
    TRISDbits.TRISD0 = 1;   // set as input //PIC24FJ256DA206

    // ---------------------------------------------------------
    // Timing Controller (TCON) pins
    // ---------------------------------------------------------

    // TCON chip select pin
    TCON_CS_DIG();     // set as digital pin
    TCON_CS_TRIS = 0;  // set as output
    TCON_CS_LAT = 1;   // set initial value as high

    // TCON data/command pin
    TCON_DC_DIG();     // set as digital pin
    TCON_DC_TRIS = 0;  // set as output
    TCON_DC_LAT = 1;   // set initial value as high (data)

    // TCON spi clock pin
    TCON_SCL_DIG();     // set as digital pin
    TCON_SCL_TRIS = 0;  // set as output
    TCON_SCL_LAT = 0;   // set initial value as high

    // TCON spi data out
    TCON_SDO_DIG();     // set as digital pin
    TCON_SDO_TRIS = 0;  // set as output
    TCON_SDO_LAT = 1;   // set initial value as high

    // initialize the Timing Controller (TCON) driver
    // chip select signal
    TCON_CS_TRIS = 0;  // set as output
    TCON_CS_LAT = 1;   // initialize to 1 (not selected)

    // ---------------------------------------------------------
    // Initialize the Display Driver
    // ---------------------------------------------------------
    // note the original DRV_GFX_Initialize sets the _GDBEN = 0xFFFF which is not
    // what we want with this hardware because the I2C3 pins overlap the GD pins.
    // The driver was changed to _GDBEN = 0xCFFF;
    DRV_GFX_Initialize();

    //------------------------------//----------------------------------------------
    // IO Pins

    // ---------------------------------------------------------
    // Digital outputs

    _LATB2 = 0;
    _LATF3 = 0;
    _LATF7 = 0;
    _LATG2 = 0;
    _LATG3 = 0;

    _TRISF3 = 0;  // HORN_OUT
    _TRISF7 = 0;  // GPS ENABLE
    _TRISG2 = 0;  // LED 1
    _TRISG3 = 0;  // LED 2

    _TRISB2 = 0;  // CTRL4: backlight
    
    // ---------------------------------------------------------

    TRISDbits.TRISD5 = 1;  //
    TRISDbits.TRISD6 = 1;  //
    TRISDbits.TRISD7 = 1;  //

    G1CON3bits.DPPWROE = 0;  // set D5 pin as normal IO pin
    ANSDbits.ANSD6 = 0;      // set pin as digital pin
    ANSDbits.ANSD7 = 0;      // set pin as digital pin

    TRISCbits.TRISC14 = 1;  //
    TRISCbits.TRISC13 = 1;  //
    TRISDbits.TRISD0 = 1;   //

    ANSCbits.ANSC14 = 0;  // set pin as digital pin
    ANSCbits.ANSC13 = 0;  // set pin as digital pin
    
    

    // ---------------------------------------------------------
    // PIC24FJ256DA210 Development Board - SPI-Flash Device pins
    // ---------------------------------------------------------
    // chip select pin
    //    TRISAbits.TRISA14 = 0;  // set as output//PIC24FJ256DA210
    //    LATAbits.LATA14   = 1;  // initialize to 1 (not selected)//PIC24FJ256DA210

    TRISBbits.TRISB3 = 0;  // set as output//PIC24FJ256DA206
    LATBbits.LATB3 = 1;    // initialize to 1 (not selected)//PIC24FJ256DA206

    TRISBbits.TRISB8 = 0;  // set as output//PIC24FJ256DA206
    LATBbits.LATB8 = 1;    // initialize to 1 (not selected)//PIC24FJ256DA206
    // spi-clock pin
    //    TRISDbits.TRISD8  = 0;//PIC24FJ256DA210
    TRISBbits.TRISB6 = 0;  // PIC24FJ256DA206

    // ---------------------------------------------------------
    // J3 button inputs
    // Digital inputs pulled high.

    ANSBbits.ANSB7 = 0;   // set pin as digital pin
    ANSBbits.ANSB8 = 0;     //Keyswitch read pin is digital only
    ANSBbits.ANSB9 = 0;   // set pin as digital pin
    ANSBbits.ANSB10 = 0;  // set pin as digital pin

    TRISBbits.TRISB7 = 1;   // HORN_IN
    TRISBbits.TRISB9 = 1;   // BUTTON_1
    TRISBbits.TRISB10 = 1;  // BUTTON_2
    TRISBbits.TRISB8 = 1;   // KEYSWITCH INPUT

    CNPU2bits.CN25PUE = 1;  // horn_in pull-up
    CNPU2bits.CN27PUE = 1;  // button_1 pull-up
    CNPU2bits.CN28PUE = 1;  // BUTTON_2 pull-up
    //Keyswitch is pulled up in Hardware

    // ---------------------------------------------------------

    // spi-output pin
    TRISGbits.TRISG7 = 0;  // PIC24FJ256DA206
    // spi-intput pin
    TRISGbits.TRISG6 = 1;  // PIC24FJ256DA206

    ANSBbits.ANSB6 = 0;  // set pin as digital pin

    ANSGbits.ANSG6 = 0;  // set pin as digital pin
    ANSGbits.ANSG7 = 0;  // set pin as digital pin

    // ---------------------------------------------------------
    // UART

    ANSBbits.ANSB0 = 0;
    ANSBbits.ANSB1 = 0;

    TRISBbits.TRISB0 = 1;  // U1RX
    TRISBbits.TRISB1 = 0;  // U1TX

    // ---------------------------------------------------------
    // Remappable peripherals
    //
    // set the PPS pins
    __builtin_write_OSCCONL( OSCCON & 0xbf );  // unlock PPS

    RPOR3bits.RP6R = 8;      // RP6(17) re-mapped to SPI1 CLK
    RPINR20bits.SDI1R = 21;  // RP21(4) re_mapped to SPI1 MISO
    RPOR13bits.RP26R = 7;    // RP26(5) re-mapped to SPI1 MOSI

    RPOR0bits.RP1R = 3;     // RP1->UART1:U1TX;
    RPINR18bits.U1RXR = 0;  // RP0->UART1:U1RX;

    RPINR0bits.INT1R = 37;  // PIC24FJ256DA206
    RPINR1bits.INT2R = 20;  // PIC24FJ256DA206
    RPINR1bits.INT3R = 11;  // PIC24FJ256DA206

    __builtin_write_OSCCONL( OSCCON | 0x40 );  // lock   PPS

    // Setting UTRDIS bit to use RG2 and RG3 as GPIO
    U1CNFG2bits.UTRDIS = 1;

    // Timer
    SYSTEM_TickInit();
}

// *****************************************************************************
/*  Function:
    __T3_ISR _T3Interrupt(void)

    Summary:
        This function initializes the interrupt used for the tick timer
        of the demo.

    Description:
        This function initializes the interrupt used for the tick timer
        of the demo. This is also used to sample the resistive touch
        screen module.

*/
// *****************************************************************************
#define __T3_ISR __attribute__( ( interrupt, shadow, auto_psv ) )
void __T3_ISR _T3Interrupt( void )
{
    _timerTick++;

    TMR3 = 0;
    // Clear flag
    IFS0bits.T3IF = 0;

    // toggle backlight to reduce power consumption
 /*   if( _bBacklightOn )
    {
        _LATB2 = !_LATB2;
    }
 */
}

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD 1000  // us
#define TICK_PERIOD ( SYS_CLK_FrequencyPeripheralGet() / SAMPLE_PERIOD )

// *****************************************************************************
/*  Function:
    void SYSTEM_TickInit(void)

    Summary:
        Initializes the tick timer of the demo.

    Description:
        This function initializes the tick timer of the demo.

*/
// *****************************************************************************
void SYSTEM_TickInit( void )
{
    // Initialize Timer3
    TMR3 = 0;
    PR3 = TICK_PERIOD;
    IFS0bits.T3IF = 0;  // Clear flag
    IEC0bits.T3IE = 1;  // Enable interrupt
    T3CONbits.TON = 1;  // Run timer
}

// *****************************************************************************
/*  Function:
    GFX_STATUS GFX_ExternalResourceCallback(
                                GFX_RESOURCE_HDR *pResource,
                                uint32_t offset,
                                uint16_t nCount,
                                void     *pBuffer)

    Summary:
        This function performs data fetch from external memory.

    Description:
        This function must be implemented in the application.
        The library will call this function each time when
        the external memory data will be required. The application
        must copy requested byte quantity into the buffer provided.
        Data start address in external memory is a sum of the address
        in GFX_RESOURCE_HDR structure and offset.

    Precondition:
        None.

    Parameters:
        pResource - Pointer to the external memory resource information.
        offset - offset of the data from the location of the resource
                 in external memory.
        nCount - Number of bytes to be transferred into the buffer.
        buffer - Pointer to the buffer that will hold the retrieved data.

    Returns:
        GFX_STATUS_SUCCESS when all the data was succesfully retrieved.
        GFX_STATUS_FAILURE when partial or no data was retrieved.

    Example:
        None.

*/
// *****************************************************************************
// If there are several memories in the system they can be selected by IDs.
// In this demo ID for memory chip installed on Graphics PICTail board is assumed to be 0.
#define SST39_MEMORY 0
/* */

GFX_STATUS GFX_ExternalResourceCallback( GFX_RESOURCE_HDR *pResource, uint32_t offset, uint16_t nCount, void *pBuffer )
{
    uint32_t addr;

    // get the proper address
    switch( pResource->type )
    {
        case GFX_RESOURCE_FONT_EXTERNAL_NONE:
            addr = pResource->resource.font.location.extAddress;
            break;
        case GFX_RESOURCE_MCHP_MBITMAP_EXTERNAL_RLE:
        case GFX_RESOURCE_MCHP_MBITMAP_EXTERNAL_NONE:
            addr = pResource->resource.image.location.extAddress;
            break;
        default:
            // type is incorrect
            return ( GFX_STATUS_FAILURE );
    }
    addr += offset;

    if( pResource->ID == SST39_MEMORY )
    {
        // Read data requested into buffer provided
        EXTFLASH_BeginTransaction( 1 );
        EXTFLASH_Read( addr, (uint8_t *)pBuffer, nCount );
        EXTFLASH_EndTransaction();
    }

    return ( GFX_STATUS_SUCCESS );
}

void __attribute__( ( interrupt, no_auto_psv ) ) _INT1Interrupt( void )
{
}
void __attribute__( ( interrupt, no_auto_psv ) ) _INT2Interrupt( void )
{
}
void __attribute__( ( interrupt, no_auto_psv ) ) _INT3Interrupt( void )
{
}
void __attribute__( ( interrupt, auto_psv ) ) _T2Interrupt( void )
{
}
void __attribute__( ( interrupt, auto_psv ) ) _T4Interrupt( void )
{
}
void __attribute__( ( interrupt, auto_psv ) ) _T5Interrupt( void )
{
}
