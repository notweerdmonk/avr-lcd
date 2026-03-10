/**
 * @file common.h
 * @author notweerdmonk
 * @brief common declarations and header files used in all modules
 */

#ifndef _AVR_ASCII_H_
#define _AVR_ASCII_H_

/*****************************************************************************/
/* ASCII                                                                     */
/*****************************************************************************/
#undef ascii
#define ascii(c) (48 + c)
/**< @brief Convert digit (0-9) to ASCII character */

/**
 * @brief ASCII carriage return character (0x0D)
 */
#undef c_RETURN
#define c_RETURN   0x0D

/**
 * @brief ASCII newline character (0x0A)
 */
#undef c_NEWLINE
#define c_NEWLINE  0x0A

/**
 * @brief ASCII tab character (0x09)
 */
#undef c_TAB
#define c_TAB      0x09

/**
 * @brief ASCII backspace character (0x08)
 */
#undef c_BKSPACE
#define c_BKSPACE  0x08

/**
 * @brief ASCII escape character (0x1B)
 */
#undef c_ESCAPE
#define c_ESCAPE   0x1B

/**
 * @brief ASCII delete character (0x7F)
 */
#undef c_DEL
#define c_DEL      0x7F

/* ASCII escape sequences */
/**
 * @brief Escape sequence to clear terminal screen
 */
#undef c_CLEARSCREEN_STRING
#define c_CLEARSCREEN_STRING "\x1b\x5b\x48\x1b\x5b\x32\x4a"

/* CRLF */
/**
 * @brief Carriage return + newline string
 */
#undef c_CRLF
#define c_CRLF "\x0d\x0a"

#endif /* _AVR_ASCII_H_ */
