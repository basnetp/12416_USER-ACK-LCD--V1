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
#include "cs1108.h"
#include "main.h"
//// CONFIG3
//#pragma config WPFP = WPFP255    //Write Protection Flash Page Segment Boundary->Highest Page (same as page 170)
//#pragma config SOSCSEL = EC    //Secondary Oscillator Power Mode Select->External clock (SCLKI) or Digital I/O mode(
//#pragma config WUTSEL = LEG    //Voltage Regulator Wake-up Time Select->Default regulator start-up time is used
//#pragma config ALTPMP = ALPMPDIS    //Alternate PMP Pin Mapping->EPMP pins are in default location mode
//#pragma config WPDIS = WPDIS    //Segment Write Protection Disable->Segmented code protection is disabled
//#pragma config WPCFG = WPCFGDIS    //Write Protect Configuration Page Select->Last page (at the top of program memory) and Flash Configuration Words are not write-protected
//#pragma config WPEND = WPENDMEM    //Segment Write Protection End Page Select->Protected code segment upper boundary is at the last page of program memory; the lower boundary is the code page specified by WPFP
//
//// CONFIG2
//#pragma config POSCMOD = NONE    //Primary Oscillator Select->Primary oscillator is disabled
//#pragma config IOL1WAY = ON    //IOLOCK One-Way Set Enable->The IOLOCK bit (OSCCON<6>) can be set once, provided the unlock sequence has been completed. Once set, the Peripheral Pin Select registers cannot be written to a second time.
//#pragma config OSCIOFNC = ON    //OSCO Pin Configuration->OSCO/CLKO/RC15 functions as port I/O (RC15)
//#pragma config FCKSM = CSDCMD    //Clock Switching and Fail-Safe Clock Monitor->Clock switching and Fail-Safe Clock Monitor are disabled
//#pragma config FNOSC = FRCDIV    //Initial Oscillator Select->Fast RC Oscillator with Postscaler (FRCDIV)
//#pragma config PLL96MHZ = ON    //96MHz PLL Startup Select->96 MHz PLL is enabled automatically on start-up
//#pragma config PLLDIV = DIV12    //96 MHz PLL Prescaler Select->Oscillator input is divided by 12 (48 MHz input)
//#pragma config IESO = OFF    //Internal External Switchover->IESO mode (Two-Speed Start-up) is disabled
//
//// CONFIG1
//#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler->1:32768
//#pragma config FWPSA = PR128    //WDT Prescaler->Prescaler ratio of 1:128
//#pragma config ALTVREF = ALTVREDIS    //Alternate VREF location Enable->VREF is on a default pin (VREF+ on RA9 and VREF- on RA10)
//#pragma config WINDIS = OFF    //Windowed WDT->Standard Watchdog Timer enabled,(Windowed-mode is disabled)
//#pragma config FWDTEN = OFF    //Watchdog Timer->Watchdog Timer is disabled
//#pragma config ICS = PGx2    //Emulator Pin Placement Select bits->Emulator functions are shared with PGEC2/PGED2
//#pragma config GWRP = OFF    //General Segment Write Protect->Writes to program memory are allowed
//#pragma config GCP = OFF    //General Segment Code Protect->Code protection is disabled
//#pragma config JTAGEN = OFF    //JTAG Port Enable->JTAG port is disabled
//// CONFIG4
//
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
#pragma config WDTPS = PS4096  // Watchdog Timer Postscaler (1:32,768)//Changed from PS32768
#pragma config FWPSA = PR32    // WDT Prescaler (Prescaler ratio of 1:128)//Changed from  PR128 
#pragma config WINDIS = OFF     // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON     // Watchdog Timer (Watchdog Timer is disabled)//Changed from OFF
#pragma config ICS = PGx3       // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC3/PGED3)
#pragma config GWRP = OFF       // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF        // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF     // JTAG Port Enable (JTAG port is disabled)

//------------------------------//----------------------------------------------
// Definitions

//------------------------------//----------------------------------------------
// Global variables

volatile TIMER_TICK _timerTick;
volatile bool GONE_TO_SLEEP = false;
volatile bool FLAG_CHARGING = false;


volatile TIMER_TICK lowbatt_timer_nonvolatile __attribute__((persistent)); 

volatile POWER_STATE curr_pwr_state = DISP_ON;//May be declare as persistent variables?
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
      _LATB11 = 1 ;  //_LATB2 = 1; //CTRL4   100 pin RB11 and 64 pin RB2
         _bBacklightOn = true;
    }
    else
    {
         _LATB11 = 0;    //_LATB2 = 0;
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


void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;
    LATE = 0x0000;
    LATF = 0x0000;
    LATG = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0000;
    TRISB = 0x04C1;
    TRISC = 0x0000;
    TRISD = 0x0461;
    TRISE = 0x00C0;
    TRISF = 0x0000;
    TRISG = 0x0040;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPD1 = 0x0000;
    CNPD2 = 0x0000;
    CNPD3 = 0x0000;
    CNPD4 = 0x0000;
    CNPD5 = 0x0000;
    CNPD6 = 0x0000;
    CNPU1 = 0x0000;
    CNPU2 = 0x0000;
    CNPU3 = 0x0000;
    CNPU4 = 0x0000;
    CNPU5 = 0x0000;
    CNPU6 = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0010;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x0000;
    ANSB = 0x0000;
    ANSC = 0x0000;
    ANSD = 0x0000;
    ANSE = 0x0000;
    ANSF = 0x0000;
    ANSG = 0x0000;
    
    //Setting UTRDIS bit to use RG2 and RG3 as GPIO 
    U1CNFG2bits.UTRDIS = 1;
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR20bits.SDI1R = 0x0015;    //RG6->SPI1:SDI1
    RPOR7bits.RP14R = 0x000A;    //RB14->SPI2:SDO2
    RPOR14bits.RP29R = 0x000B;    //RB15->SPI2:SCK2OUT
    RPOR13bits.RP27R = 0x0009;    //RG9->SPI1:SS1OUT
    RPOR13bits.RP26R = 0x0007;    //RG7->SPI1:SDO1
    RPOR0bits.RP1R = 0x0003;    //RB1->UART1:U1TX
    RPINR18bits.U1RXR = 0x0000;    //RB0->UART1:U1RX
    RPOR9bits.RP19R = 0x0008;    //RG8->SPI1:SCK1OUT

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
}

void SPI1_Initialize (void)
{
    // MSTEN Master; DISSDO disabled; PPRE 64:1; SPRE 8:1; MODE16 disabled; SMP Middle; DISSCK disabled; CKP Idle:Low, Active:High; CKE Idle to Active; SSEN disabled; 
    SPI1CON1 = 0x20;
    // SPIFSD disabled; SPIBEN enabled; SPIFPOL disabled; SPIFE disabled; FRMEN disabled; 
    SPI1CON2 = 0x01;
    // SISEL SPI_INT_SPIRBF; SPIROV disabled; SPIEN enabled; SPISIDL disabled; 
    SPI1STAT = 0x800C;
}
void SPI2_Initialize (void)
{
    // MSTEN Master; DISSDO disabled; PPRE 64:1; SPRE 8:1; MODE16 disabled; SMP Middle; DISSCK disabled; CKP Idle:Low, Active:High; CKE Idle to Active; SSEN disabled; 
    SPI2CON1 = 0x20;
    // SPIFSD disabled; SPIBEN enabled; SPIFPOL disabled; SPIFE disabled; FRMEN disabled; 
    SPI2CON2 = 0x01;
    // SISEL SPI_INT_SPIRBF; SPIROV disabled; SPIEN enabled; SPISIDL disabled; SPIBEC disabled; 
    SPI2STAT = 0x800C;
}
void INTERRUPT_Initialize (void)
{
    //    MICI: MI2C3 - I2C3 Master Events
    //    Priority: 1
        IPC21bits.MI2C3P = 1;
    //    SICI: SI2C3 - I2C3 Slave Events
    //    Priority: 1
        IPC21bits.SI2C3P = 1;

}


void CLOCK_Initialize(void)
{
    // CPDIV 1:1; RCDIV FRC/2; DOZE 1:8; G1CLKSEL disabled; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3100;
    // GCLKDIV 1; 
    CLKDIV2 = 0x00;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ADC1MD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; SPI2MD enabled; SPI1MD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // OC5MD enabled; OC6MD enabled; OC7MD enabled; OC8MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; IC6MD enabled; IC7MD enabled; IC5MD enabled; IC8MD enabled; IC4MD enabled; IC3MD enabled; 
    PMD2 = 0x00;
    // I2C3MD enabled; PMPMD enabled; U3MD enabled; RTCCMD enabled; CMPMD enabled; CRCMD enabled; I2C2MD enabled; 
    PMD3 = 0x00;
    // U4MD enabled; UPWMMD enabled; USB1MD enabled; CTMUMD enabled; REFOMD enabled; LVDMD enabled; 
    PMD4 = 0x00;
    // IC9MD enabled; OC9MD enabled; 
    PMD5 = 0x00;
    // GFX1MD enabled; SPI3MD enabled; 
    PMD6 = 0x00;
    // CF no clock failure; NOSC FRCDIV; SOSCEN disabled; POSCEN disabled; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x07));
    __builtin_write_OSCCONL((uint8_t) (0x00));
}

void I2C3_Initialize(void)
{
    
//    i2c3_object.pTrHead = i2c3_tr_queue;
//    i2c3_object.pTrTail = i2c3_tr_queue;
//    i2c3_object.trStatus.s.empty = true;
//    i2c3_object.trStatus.s.full = false;
//
//    i2c3_object.i2cErrors = 0;
    
    // initialize the hardware
    // Baud Rate Generator Value: I2CBRG 19;   
    I2C3BRG = 0x13;
    // ACKEN disabled; STREN disabled; GCEN disabled; SMEN disabled; DISSLW enabled; I2CSIDL disabled; ACKDT Sends ACK; SCLREL Holds; RSEN disabled; IPMIEN disabled; A10M 7 Bit; PEN disabled; RCEN disabled; SEN disabled; I2CEN enabled; 
    I2C3CON = 0x8000;
    // BCL disabled; P disabled; S disabled; I2COV disabled; IWCOL disabled; 
    I2C3STAT = 0x00;

    /* MI2C3 - I2C3 Master Events */
    // clear the master interrupt flag
    IFS5bits.MI2C3IF = 0;
    // enable the master interrupt
    IEC5bits.MI2C3IE = 1;

}
//void UART1_InitializeD(void)
//{
///**    
//     Set the UART1 module to the options selected in the user interface.
//     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
//*/
//    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; RXINV disabled; UEN TX_RX; 
//    // Data Bits = 8; Parity = None; Stop Bits = 1;
//    U1MODE = (0x8008 & ~(1<<15));  // disabling UARTEN bit
//    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
//    U1STA = 0x00;
//    // BaudRate = 9600; Frequency = 2000000 Hz; BRG 51; 
//    U1BRG = 0x33;
//    
//    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
//    U1STAbits.UTXEN = 1;
//}
void SYSTEM_Initialize( void )
{
//    // reset all the reasons-for-reset flags.  Want this for
//    // debugging so when there is a reset we can tell why
//    RCON = 0;
//
//    //------------------------------//----------------------------------------------
//    // Clock
//


    
   // PIN_MANAGER_Initialize ();
    //CLOCK_Initialize();
    //INTERRUPT_Initialize ();
    //SPI1_Initialize ();
    //SPI2_Initialize ();
   // I2C3_Initialize();
   // UART1_InitializeD();
    
    //    // Clock already running as needed from CONFIG words (FRCDIV with PLL on)
      // debugging so when there is a reset we can tell why
    RCON = 0;
 //    // FOSC=32MHZ=(96MHZ/1/3),FCY=16MHZ,FRC=8MHZ,DOZE=OFF(4MHZ)
   OSCCON = 0b000100010000000;
  CLKDIV = 0b000000000010000;  // fastest clock, enable pll
    //------------------------------//----------------------------------------------
    // Interrupts

    // I2C slave interrupt needs to be high priority due to timing requirements
    IPC21bits.SI2C3P = 6;
    
    //Added
   //IPC21bits.MI2C3P = 1;
    // Timer 3 interrupt priority
    IPC2bits.T3IP = 3;

    // graphics interrupt priority
    IPC25bits.GFX1IP = 5;
    
    //Keyswitch change notification priority
    IPC4bits.CNIP = 7;   
    //Give lower priority to interrupt
    
    
    // uart interrupt priority
    IPC2bits.U1RXIP = 4;
    IPC3bits.U1TXIP = 4;

    // ---------------------------------------------------------
    // Make sure the display DO NOT flicker at start up
    // ---------------------------------------------------------
   // displaybackligh();
    
    DisplayBacklightConfig();
    
    ANSCbits.ANSC13 = 0; //                            ANSCbits.ANSC14 = 0;    // set pin as digital pin //PIC24FJ256DA206//J8- pin 1 -RC13 for 100 and RC14 for 64
    //nothing for 100 pin mcu                 //  ANSCbits.ANSC13 = 0;    // set pin as digital pin //PIC24FJ256DA206
    TRISCbits.TRISC13 = 1;                //TRISCbits.TRISC14 = 1;  // set as input //PIC24FJ256DA206
     //noting                          TRISCbits.TRISC13 = 1;  // set as input //PIC24FJ256DA206
    ANSCbits.ANSC14 = 0; //  set as digital pin//J8-3 pin RC14 for 100 and RD0 for 64 pin
    TRISCbits.TRISC14 = 1;   // TRISDbits.TRISD0 = 1;//                              TRISDbits.TRISD0 = 1;   // set as input //PIC24FJ256DA206
        
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
    TCON_SCL_LAT = 0;   // set initial value as low
    
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
    //  
    // ---------------------------------------------------------
    // Digital outputs
    //
            _LATB11= 0;     //    _LATB2 = 0;    //CTRL4
            _LATD7 = 0;     //            //    _LATF3 = 0;   //HORN_OUT       
           // _LATD11 = 0;     //    _LATF7 = 0;  //V BUS
            _LATG2 = 0;     //    _LATG2 = 0;   led uC1
            _LATG3 = 0;     //    _LATG3 = 0;   //led uC2
//           
//            
//
         
           _TRISD7 = 0;    //    _TRISF3 = 0;  // HORN_OUT
          // _TRISD11 = 0;    //    _TRISF7 = 0;  // GPS ENABLE//DO not make it low for the first 
           _TRISG2 = 0;    //    _TRISG2 = 0;  // LED 1
           _TRISG3 = 0;    //    _TRISG3 = 0;  // LED 2
           _TRISB11= 0;    //    _TRISB2 = 0;  // CTRL4: backlight

          // Basnetp--LCD PINS 
//    
//    // ---------------------------------------------------------
//
           
           //G1CGON3bits.DPPWROE = 0;
           
           
     TRISBbits.TRISB9 = 1;       //    TRISDbits.TRISD5 = 1;  //LCD_13
     TRISBbits.TRISB8 = 1;//    TRISDbits.TRISD6 = 1;  ////LCD_12
      //nothing                  //    TRISDbits.TRISD7 = 1;  // used as horn out in 100 pins for 64 pins it was just TP
//
     ANSBbits.ANSB9 = 0;             //  nothing using B9 for lcd_13 in 100 pin  //    G1CON3bits.DPPWROE = 0;  // set D5 pin as normal IO pin
//   
     ANSBbits.ANSB8 = 0;        //ANSDbits.ANSD6 = 0;      // set pin as digital pin
   // nothing//    ANSDbits.ANSD7 = 0;      // set pin as digital pin
//
    TRISCbits.TRISC13 = 1;        //    TRISCbits.TRISC14 = 1;  //J8-1 pin
     //unused RC13 in 64 pin                   //    TRISCbits.TRISC13 = 1;  //Test Point
    TRISCbits.TRISC14 = 1;        //    TRISDbits.TRISD0 = 1;   //  J8-3 pin
   // UPDATED UPTO HERE_BASNETP 6/18/2021
   //
    
    
           ANSCbits.ANSC14 = 0;         //    ANSCbits.ANSC14 = 0;  // set pin as digital pin
           ANSCbits.ANSC13 = 0;  // set pin as digital pin
           //It was just test point  on 64 pin MCU   //    ANSCbits.ANSC13 = 0;  // set pin as digital pin
           //    
           //
//    // ---------------------------------------------------------
//    // PIC24FJ256DA210 Development Board - SPI-Flash Device pins
//    // ---------------------------------------------------------
//    // chip select pin
//    //    TRISAbits.TRISA14 = 0;  // set as output//PIC24FJ256DA210
//    //    LATAbits.LATA14   = 1;  // initialize to 1 (not selected)//PIC24FJ256DA210
//
           // ANSGbits.ANSG9 = 0;       //100 pin- set pin as digital- RG9 in 64 it was analog too pin RB3
            TRISGbits.TRISG9 = 0;//               //    TRISBbits.TRISB3 = 0;  // set as output//PIC24FJ256DA206
            LATGbits.LATG9 = 1;                //    LATBbits.LATB3 = 1;    // initialize to 1 (not selected)//PIC24FJ256DA206
//
//    //TRISBbits.TRISB8 = 0;  // set as output//PIC24FJ256DA206
//    //LATBbits.LATB8 = 1;    // initialize to 1 (not selected)//PIC24FJ256DA206
//    // spi-clock pin
//    //    TRISDbits.TRISD8  = 0;//PIC24FJ256DA210
           //ANSGbits.ANSG8 = 0;    //RG8 is analog pin//Flash CLK defined below
           TRISGbits.TRISG8 = 0;  //RG8//64 had RB6 both analog pin //Flash CLK   //    TRISBbits.TRISB6 = 0;  // PIC24FJ256DA206-RB6/CN6//AN pin Flash CLK

//    // ---------------------------------------------------------
//    // J3 button inputs
//    // Digital inputs pulled high.
//
           ANSDbits.ANSD6=0;//this is important!!for RD6 to activate!!
            //ANSDbits.ANSD5=0;
          // ANSDbits.ANSD5=0;
         ODCDbits.ODD10=0;//RD10 is not analog pin                //    ANSBbits.ANSB7 = 0;   // set pin as digital pin //HORN IN
         ODCDbits.ODD5=0; //RD5 is not analog pin //ODC pin       //    ANSBbits.ANSB9 = 0;   // set pin as digital pin//BUTTON 1
         ODCDbits.ODD6=0; //RD6 is not analog pin                 //    ANSBbits.ANSB10 = 0;  // set pin as digital pin//BUTTON_2
         ANSBbits.ANSB10 = 0;//RB10 is analog pin                 //    ANSBbits.ANSB8 = 0;   // set pin as digital pin//Key Switch
    
         TRISDbits.TRISD11 = 1;                                    // TRISFbits.TRISF7 = 1;   //GPS_ENABLE IN 
         TRISDbits.TRISD10 = 1;                                    //    TRISBbits.TRISB7 = 1;   // HORN_IN
         TRISDbits.TRISD5 = 1;                                     //    TRISBbits.TRISB9 = 1;   // BUTTON_1
         TRISDbits.TRISD6 = 1;                                     //    TRISBbits.TRISB10 = 1;  // BUTTON_2
         TRISBbits.TRISB10 = 1;                                    //    TRISBbits.TRISB8 = 1;   //Key switch In
//
         
         
//    
//    //******Change Notification interrupt. For Keyswitch*********
        CNEN2bits.CN28IE = 1;   //CN28//RB10     for 100 pin         //    CNEN2bits.CN26IE = 1;   //Enable CN on RB8 
        IEC1bits.CNIE = 1;   //IN Interrupt                          //    IEC1bits.CNIE = 1;      //Enable CN interrupts for Key switch
        IFS1bits.CNIF = 0;                                           //    IFS1bits.CNIF = 0;      //Clear interrupt flag for CN
//    
        CNPD1bits.CN14PDE = 1;
        CNPD1bits.CN15PDE = 1;
        CNPD4bits.CN56PDE = 1;
        
        CNPU4bits.CN55PUE = 1;   //   100 pin RD10//CN55            //    CNPU2bits.CN25PUE = 1;  // horn_in pull-up 64 pin-RB7
//      CNPU1bits.CN14PUE = 1;   //100 pin RD5//CN14-               //    //CNPU2bits.CN27PUE = 1;  // button_1 pull-up... Disabled for smart-sensor
//      CNPU1bits.CN15PUE = 1;   //100 pin RD6//CN15                 //    //CNPU2bits.CN28PUE = 1;  // BUTTON_2 pull-up--Disable for smart sensor//Basnetp
//    
        
        
//     CNPU2bits.CN28PUE = 1; //100 pin RB10//CN28-Externally Pulled up  //CNPU2bits.CN26PUE = 1;    //Key switch input. Not needed. Pulled up externally
    
//BASNETP-2021-July-16  DONE!!   
//    // ---------------------------------------------------------
//
//    // spi-output pin//DI
                 
        ANSGbits.ANSG7 = 0;//RB18 is analog pin 
        TRISGbits.TRISG7 = 0;  //100 pin AN18-RG7;      //    TRISGbits.TRISG7 = 0;  // PIC24FJ256DA206//RP26//RG7 not analog pin
//    // spi-input pin//D0
        ANSGbits.ANSG6 = 0; 
        TRISGbits.TRISG6 = 1; //RG6                            //    TRISGbits.TRISG6 = 1;  // PIC24FJ256DA206//RP21/RG6  not analog pin for 64 pin
        ANSGbits.ANSG8 = 0;    //RG8 is analog pin//Flash CLK//Did above with  TRISGbits.TRISG8 = 0; //G8   //    ANSBbits.ANSB6 = 0;  // set pin as digital pin
//
//     //Did 2 step above                                        //  ANSGbits.ANSG6 = 0;  // set pin as digital pin
//    //Set keyswitch an digital pin
//
//    // ---------------------------------------------------------
//    // UART-FROM_GPS and TO_GPS pins

      ANSBbits.ANSB0 = 0; // RB0 is analog pin   // ANSBbits.ANSB0 = 0; //FROM_GPS-RB0
      ANSBbits.ANSB1 = 0; //  RB1 is analog pin   // ANSBbits.ANSB1 = 0;//TO_GPS  -RB1

      TRISBbits.TRISB0 = 1;  // U1RX            // TRISBbits.TRISB0 = 1;  // U1RX -  FROM_GPS
      TRISBbits.TRISB1 = 0;  // U1TX             //TRISBbits.TRISB1 = 0;  // U1TX -  TO_GPS
//DONE_BASNETP_2:24 PM
      
      
      
      ///
      //
//      __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
//
//    RPINR20bits.SDI1R = 0x0015;    //RG6->SPI1:SDI1
//    RPOR7bits.RP14R = 0x000A;    //RB14->SPI2:SDO2
//    RPOR14bits.RP29R = 0x000B;    //RB15->SPI2:SCK2OUT
//    RPOR13bits.RP27R = 0x0009;    //RG9->SPI1:SS1OUT
//    RPOR13bits.RP26R = 0x0007;    //RG7->SPI1:SDO1
//    RPOR0bits.RP1R = 0x0003;    //RB1->UART1:U1TX
//    RPINR18bits.U1RXR = 0x0000;    //RB0->UART1:U1RX
//    RPOR9bits.RP19R = 0x0008;    //RG8->SPI1:SCK1OUT
//
//    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
      ////
      ///
      
//    // ---------------------------------------------------------
//   // Re-mappable peripherals
//    //
//    // set the PPS pins
   __builtin_write_OSCCONL( OSCCON & 0xbf );  // unlock PPS    //    __builtin_write_OSCCONL( OSCCON & 0xbf );  // unlock PPS
//
        RPOR9bits.RP19R = 8; //RP19 /RG8                       //    RPOR3bits.RP6R = 8;      // RP6(17) RB6 re-mapped to SPI1 CLK
        RPINR20bits.SDI1R = 21;  //D0-RP21-RG6                 //    RPINR20bits.SDI1R = 21;  // RP21(4) re_mapped to SPI1 MISO
        RPOR13bits.RP26R = 7;   //D1-RP26-RG7                  //    RPOR13bits.RP26R = 7;    // RP26(5) re-mapped to SPI1 MOSI

     RPOR0bits.RP1R = 3;  // RP1-TO GPS //output               //   RPOR0bits.RP1R = 3;     // RP1->UART1:U1TX;
     RPINR18bits.U1RXR = 0;// RP0 GROM GPS //input             // RPINR18bits.U1RXR = 0;  // RP0->UART1:U1RX;
//
      RPOR7bits.RP14R = 0x000A;    //RB14->SPI2:SDO2
      RPOR14bits.RP29R = 0x000B;    //RB15->SPI2:SCK2OUT
//    //Need to add UART2 init code below for Debugging UART2 TO WORK
//    //RPINR19BITS.U2RXR = x;
//    //RP11R
//    
    //??? no  // RPINR0bits.INT1R = 37; //RC13-CN1 no RP register??                    //    RPINR0bits.INT1R = 37;  // PIC24FJ256DA206   RC13-CN1???
      RPINR1bits.INT2R = 9; //RP9                    //    RPINR1bits.INT2R = 20;  // PIC24FJ256DA206//RPI20-RD5
      RPINR1bits.INT3R = 37;  //RPI37-RC14,CN0                   //    RPINR1bits.INT3R = 11;  // PIC24FJ256DA206//RP11/RD0

  __builtin_write_OSCCONL( OSCCON | 0x40 );  // lock   PPS      //    __builtin_write_OSCCONL( OSCCON | 0x40 );  // lock   PPS
//
//    // Setting UTRDIS bit to use RG2 and RG3 as GPIO
 U1CNFG2bits.UTRDIS = 1; // To use CN83 and CN84, which are on the
                         //D+ and D- pins, the UTRDIS bit (U1CNFG2<0>) should be set.    // //    U1CNFG2bits.UTRDIS = 1;  
                                

   
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


///////
///
//////
void __attribute__( ( interrupt, shadow, auto_psv ) ) _CNInterrupt( void )
{
    static bool prev_state = ON;  //At startup this should be ON.
    static bool ONOFF_state = OFF;
    bool debounced_input = 0;
//    bool raw_input = 0;
    
    //raw_input = KSWITCH_IN;
   //__delay_ms(30);
    debounced_input = KSWITCH_IN;
    
//    if(debounced_input == raw_input)
//    {
//        horn_state ^= 1;
//        OPIN_HORN_OUT = horn_state;
//    }
    //Keep GPS Enabled. Active Low
    OPIN_GPS_ENABLE = 0;
    //beep_horn(200);
    
    if(debounced_input) //Turn off display
    {   
        //Only if unit is not charging, respond to switch state change
        if(!FLAG_CHARGING)
        {
            LED_BACKLIGHT_OUT = 0;
            //Currently OFF, so update state.
            ONOFF_state = OFF;
            //Disable interrupts
            IEC0bits.T3IE = 0; //Timer
            //Disable i2c interrupts
            //IEC5bits.SI2C3IE = 0;
            //Disable GFX controller Interrupt
            IEC6bits.GFX1IE = 0;
            //Disable GFX Module
            G1CON1bits.G1EN = 0;
            //Turn OFF horn
            OPIN_HORN_OUT = 0;
            //Turn off front facing LEDs
            OPIN_LED_1 = 0;
            OPIN_LED_2 = 0;
            
            //lowbatt_timer_nonvolatile = lowbatt_timer;
            
            //Save state before sleep
            prev_state = ONOFF_state;
            
            curr_pwr_state = DISP_OFF;
            
            ui_state = sleeping;
            
            //GONE_TO_SLEEP = true;
            
            //Go to sleep
            asm("PWRSAV #0");

        }//Since pin is pulled High, an high on the pin means key is in OFF position
       
     
    }
    else
    {   
        
        //Not currently charging
        if(!FLAG_CHARGING)
        {
            curr_pwr_state = DISP_ON;
        //GONE_TO_SLEEP = false;
        //Only if unit is not charging, respond to switch state change
        //if((!(CS1108_ultaStatus.misc_data.bits.chg_mode)&&(prev_state == OFF)) || (!I2C_ACTIVE ))
        //if((GONE_TO_SLEEP) || (!I2C_ACTIVE ))
        //{
            asm("RESET");
        //}
        //else    
        //{
        //    LED_BACKLIGHT_OUT = 1;
        //    ONOFF_state = OFF;
        }
        
    } 
    
    //prev_state = ONOFF_state;
        
    
    IFS1bits.CNIF = 0;
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
