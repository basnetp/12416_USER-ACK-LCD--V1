/**
 * Implements miscellaneous interrupt/trap handlers
 *
 * Copyright (c) 2019 Control Solutions LLC. All Rights Reserved.
 *
 * @private
 */
#include "system.h"
#include <xc.h>

//------------------------------//----------------------------------------------
// Definitions

//------------------------------//----------------------------------------------
// Global variables

//------------------------------//----------------------------------------------
// Local variables

//------------------------------//----------------------------------------------
// Local functions

//------------------------------//----------------------------------------------
// Public functions

/* DEFAULT TRAP HANDLERS */
void __attribute__( ( interrupt( no_auto_psv ) ) ) _DefaultInterrupt( void )
{
    // If you enable an interrupt, and forget to define the handler for it,
    // you end up here.
    // This would be a good place to put a breakpoint, but for production
    // we will just reset ourselves and hope for the best.

    // while(1);

    asm( "RESET" );
};

//Add ISR below for IO change notification. Can be used for smartsensor.
//void __attribute__( ( interrupt( no_auto_psv ) ) ) _CNInterrupt( void )

void __attribute__( ( interrupt( no_auto_psv ) ) ) _OscillatorFail( void )
{
    INTCON1bits.OSCFAIL = 0;
};

void __attribute__( ( interrupt( no_auto_psv ) ) ) _AddressError( void )
{
    INTCON1bits.ADDRERR = 0;
};

void __attribute__( ( interrupt( no_auto_psv ) ) ) _StackError( void )
{
    INTCON1bits.STKERR = 0;
};

void __attribute__( ( interrupt( no_auto_psv ) ) ) _MathError( void )
{
    INTCON1bits.MATHERR = 0;
};
