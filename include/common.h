/**
 * @file common.h
 * @author notweerdmonk
 * @brief common declarations and header files used in all modules
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define UNUSED_FUNCTION __attribute__ (( unused ))

#define UNUSED_VARIABLE __attribute__ (( unused ))

#define DEPRECATED_FUNCTION(msg) __attribute__ (( deprecated(msg) ))

#define DEPRECATED_VARIABLE(msg) __attribute__ (( deprecated(msg) ))

typedef uint8_t boolean;

#define TRUE  (boolean)1
#define FALSE 0

#define INVALID_ID 255

/* ASCII character codes */
#define c_RETURN   0x0D
#define c_NEWLINE  0x0A
#define c_TAB      0x09
#define c_BKSPACE  0x08
#define c_ESCAPE   0x1B
#define c_DEL      0x7F

#define ASCII(c) (48 + c)

#define CLEARSCREEN_STRING "\x1b\x5b\x48\x1b\x5b\x32\x4a"
#define NEWLINE_STRING   "\x0d\x0a"

/* pin that is not connected */
#define PIN_NC -1

#endif /* _COMMON_H_ */
