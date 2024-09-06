/**
 * Application global definitions
 *
 * You probably want this file included in all .c files
 */

#ifndef _APP_H_
#define _APP_H_

#ifndef __XC16__
// This is here because my linter (CLANG) does not like the __prog__ type
// qualifier
// so it is defined to empty if not compiling for the XC compiler
#define __prog__
#endif

// Define this to turn off assert() code generation for production
//#define NDEBUG

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>//it was shadowed resulting error
#include <xc.h>

#ifndef NDEBUG
#define assert( ignore ) ( (void)0 )
#else
#undef assert
#undef __myassert
#define assert( expression ) ( (void)( ( expression ) ? 0 : ( __myassert( #expression, __FILE__, __LINE__ ), 0 ) ) )

#define __myassert( expression, file, line )                                                                           \
    printf( "Failed assertion `%s' at line %d of `%s'.", expression, line, file )
#endif

// how many ms we wait for a response from the MODBUS slave
#define COMMAND_TIMEOUT 1000//500//100

// define this to enable debug outputs like the TRACE macro.
// You can undefine it globally, or at the top of a specific
// module (after the includes) to disable debug output for that module.
#define ENABLE_DEBUG

#if defined( ENABLE_DEBUG )
// a debug print macro that can be #defined-out to remove the
// debug messages from the code
#define TRACE( x ) printf x;

#else
#define TRACE( x )
#endif

//------------------------------//----------------------------------------------
// Versioning

#define VERSION_PRODUCT 1

// Semantic Software version (see https://semver.org)
#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1

//------------------------------//----------------------------------------------
// UART Setup
// See uart.h

#define ENABLE_UART1
#define UART1_BAUD 19200L
//230400

// Size of the receive and transmit queues.  Note there are restrictions
// on the name of these macros, and on the size that is allowed.  See uart.h
#define UART1_rxQ_SIZE 256
#define UART1_txQ_SIZE 256

#define ENABLE_UART2
#define UART2_BAUD 115200

// Size of the receive and transmit queues.  Note there are restrictions
// on the name of these macros, and on the size that is allowed.  See uart.h
#define UART2_rxQ_SIZE 32
#define UART2_txQ_SIZE 256

#endif
