/**
 * Hardware interface
 *
 * These routines perform hardware-specific functions.
 *
 * Copyright(c) 2018 Control Solutions LLC.  All Rights Reserved
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "system_config.h"
#include <stdbool.h>
#include <stdlib.h>
#include <xc.h>

//------------------------------//----------------------------------------------
// Definitions
#define ON 1
#define OFF 0

// defines the range of intervals that can be timed
//typedef uint16_t TIMER_TICK;
typedef uint32_t TIMER_TICK;


//------------------------------//----------------------------------------------
// Global variables

// Count timer interrupts.  We just look at relative times
// (differences between current tick and a saved time mark) so
// we can measure time up to 65535 ticks.
extern volatile TIMER_TICK _timerTick;
extern volatile bool GONE_TO_SLEEP;
extern volatile bool FLAG_CHARGING;
//extern volatile TIMER_TICK lowbatt_timer_nonvolatile __attribute__((persistent)); 

//Enum for power state. 
typedef enum
{
    DISP_OFF = 0, DISP_ON = 1, DISP_SLEEP = 3
}POWER_STATE;

extern volatile POWER_STATE curr_pwr_state;
//------------------------------//----------------------------------------------
// Public functions

// the system clock frequency in Hertz.
#define SYS_CLK_FrequencySystemGet()   ( 32000000ul )//(4000000UL)

// the peripheral clock frequency in Hertz.
#define SYS_CLK_FrequencyPeripheralGet() ( SYS_CLK_FrequencySystemGet() / 2 )

// instruction clock frequency in Hertz.
#define SYS_CLK_FrequencyInstructionGet() ( SYS_CLK_FrequencySystemGet() / 2 )

/**********************************************************************
 * __delay_ms define - FCY must be defined before including
 * libpic30.h
 **********************************************************************/
#define FCY ( SYS_CLK_FrequencyInstructionGet() )

#include <libpic30.h>

#define SLEEP()  // asm( "PWRSAV #IDLE_MODE" );

/**
 * @brief      Powerup hardware initialization
 */
void SYSTEM_Initialize( void );

// returns the current system timer tick
#define TIMER_Now() ( _timerTick )

/**
 * Returns the number of milliseconds since the indicated start time
 */
#define TIMER_Since( start ) ( TIMER_Now() - start )

/**
 * @brief      Control the LCD backlight
 *
 * @param[in]  percent  Percent brightness
 */
void SYSTEM_Backlight( unsigned int percent );
void PIN_MANAGER_Initialize (void);
#endif



