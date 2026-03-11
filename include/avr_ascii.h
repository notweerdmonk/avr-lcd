/**
 * @file avr_ascii.h
 * @brief ASCII character constants and escape sequences
 * @author notweerdmonk
 * 
 * This header provides ASCII character constants and escape sequences
 * for use in serial communication and terminal output.
 */

#ifndef _AVR_ASCII_H_
#define _AVR_ASCII_H_

/******************************************************************************/
/* ASCII Character Constants                                                  */
/******************************************************************************/

/**
 * @name Control Characters
 * ASCII control character definitions
 */
/*@{*/

/**
 * @brief Convert digit (0-9) to ASCII character
 * @param c Digit value (0-9)
 * @return ASCII character code
 */
#undef ascii
#define ascii(c) (48 + c)

/**
 * @brief ASCII carriage return (CR)
 * Value: 0x0D
 */
#undef c_RETURN
#define c_RETURN   0x0D

/**
 * @brief ASCII line feed / newline (LF)
 * Value: 0x0A
 */
#undef c_NEWLINE
#define c_NEWLINE  0x0A

/**
 * @brief ASCII horizontal tab (HT)
 * Value: 0x09
 */
#undef c_TAB
#define c_TAB      0x09

/**
 * @brief ASCII backspace (BS)
 * Value: 0x08
 */
#undef c_BKSPACE
#define c_BKSPACE  0x08

/**
 * @brief ASCII escape (ESC)
 * Value: 0x1B
 */
#undef c_ESCAPE
#define c_ESCAPE   0x1B

/**
 * @brief ASCII delete (DEL)
 * Value: 0x7F
 */
#undef c_DEL
#define c_DEL      0x7F

/*@}*/

/**
 * @name Escape Sequences
 * Terminal escape sequence definitions
 */
/*@{*/

/**
 * @brief Escape sequence to clear terminal screen
 * Moves cursor to home and clears screen
 */
#undef c_CLEARSCREEN_STRING
#define c_CLEARSCREEN_STRING "\x1b\x5b\x48\x1b\x5b\x32\x4a"

/**
 * @brief Carriage return + line feed (CRLF)
 * Used for newlines in Windows-style line endings
 */
#undef c_CRLF
#define c_CRLF "\x0d\x0a"

/*@}*/

#endif /* _AVR_ASCII_H_ */
