/*
 * avr-lcd - LCD library for AVR microcontrollers.
 * Copyright (C) 2026 notweerdmonk
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
*/

/**
 * @file avr_utility.h
 * @brief Utility and helper macros for AVR development
 * @author notweerdmonk
 * 
 * This header provides various utility macros including:
 * - Compiler attributes
 * - Preprocessor utilities
 * - Bitmask manipulation
 * - Atomic operations
 * - EEPROM access
 * - Data format conversion
 */

#ifndef _AVR_USER_UTILITY_H_
#define _AVR_USER_UTILITY_H_

/** @brief Convert milliseconds to timer value */
#define TIMER_VAL_FROM_SEC(x)   (x)*1000

/******************************************************************************/
/* Compiler Attributes                                                        */
/******************************************************************************/

/**
 * @name Function/Variable Attributes
 * GCC attribute macros for special function/variable behavior
 */
/*@{*/

/** @brief Mark function as intentionally unused */
#define UNUSED_FUNCTION __attribute__ (( unused ))

/** @brief Mark variable as intentionally unused */
#define UNUSED_VARIABLE __attribute__ (( unused ))

/**
 * @brief Mark function as deprecated
 * @param msg Deprecation message
 */
#define DEPRECATED_FUNCTION(msg) __attribute__ (( deprecated(msg) ))

/**
 * @brief Mark variable as deprecated
 * @param msg Deprecation message
 */
#define DEPRECATED_VARIABLE(msg) __attribute__ (( deprecated(msg) ))

/*@}*/

/******************************************************************************/
/* Preprocessor Utilities                                                     */
/******************************************************************************/

/**
 * @name String Concatenation
 * Macros for concatenating tokens at preprocessing time
 */
/*@{*/

/**
 * @brief Concatenate tokens (expands arguments)
 * @param A First token
 * @param B Second token
 * @return Concatenated token
 */
#define CONCAT_EXPAND(A, B) A ## B

/**
 * @brief Concatenate tokens
 * @param A First token
 * @param B Second token
 * @return Concatenated token
 */
#define CONCAT(A, B) CONCAT_EXPAND(A, B)

/*@}*/

/**
 * @name Stringification
 * Macros for converting tokens to strings
 */
/*@{*/

/**
 * @brief Stringify helper (internal)
 * @param s Token to stringify
 * @return String literal
 */
#define STRINGIFY_HELPER(s) #s

/**
 * @brief Stringify token
 * @param s Token to stringify
 * @return String literal
 */
#define STRINGIFY(s) STRINGIFY_HELPER(s)

/*@}*/

/******************************************************************************/
/* Bitmask Manipulation                                                       */
/******************************************************************************/

/**
 * @name Bit Operations
 * Macros for bit manipulation operations
 */
/*@{*/

/**
 * @brief Generate 8-bit mask from bit position
 * @param byte Bit position (0-7)
 * @return 8-bit mask with only that bit set
 * 
 * @example
 * @code
 * uint8_t mask = BITMASK(3);  // mask = 0x08
 * @endcode
 */
#define BITMASK(byte) \
  ({                       \
    register uint8_t mask; \
    __asm__ __volatile__(  \
        "ldi %0, 1"           "\n\t" \
        "mov __tmp_reg__, %1" "\n\t" \
        "rjmp .+2"            "\n\t" \
        "lsl %0"              "\n\t" \
        "dec __tmp_reg__"     "\n\t" \
        "brpl .-6"            "\n\t" \
        : "=&r" (mask)               \
        : "r"   (byte)               \
        : "r0" );                    \
    mask;                            \
  })

/**
 * @brief Set bits in a variable
 * @param x Variable to modify
 * @param y Bitmask to set
 */
#define SET_MASK(x, y)      (x) |= (y)

/**
 * @brief Clear bits in a variable
 * @param x Variable to modify
 * @param y Bitmask to clear
 */
#define CLR_MASK(x, y)      (x) &= ~(y)

/**
 * @brief Clear all bits in a variable
 * @param x Variable to clear
 */
#define ZERO_MASK(x)        (x) = 0

/**
 * @brief Set a single bit
 * @param x Variable to modify
 * @param y Bit position (0-7)
 */
#define SET_BIT(x, y)       (x) |= BITMASK(y)

/**
 * @brief Clear a single bit
 * @param x Variable to modify
 * @param y Bit position (0-7)
 */
#define CLR_BIT(x, y)       (x) &= ~BITMASK(y)

/**
 * @brief Get a single bit value
 * @param x Variable to read
 * @param y Bit position (0-7)
 * @return Non-zero if bit is set, zero otherwise
 */
#define GET_BIT(x, y)       ((x) & BITMASK(y))

/*@}*/

/******************************************************************************/
/* Atomic Operations                                                          */
/******************************************************************************/

/**
 * @name Critical Sections
 * Macros for atomic operations with interrupts disabled
 */
/*@{*/

/**
 * @brief Atomically copy a byte with interrupts disabled
 * @param dest Destination variable
 * @param src Source variable
 */
#define ATOMIC_COPY_BYTE(dest, src) \
  do { cli(); dest = src; sei(); } while (0)

/**
 * @brief Atomically copy a word with interrupts disabled
 * @param dest Destination variable
 * @param src Source variable
 */
#define ATOMIC_COPY_WORD(dest, src) \
  do { cli(); dest = src; sei(); } while (0)

/*@}*/

/******************************************************************************/
/* EEPROM Memory Access                                                       */
/******************************************************************************/

/**
 * @name Program Memory Strings
 * Macros for accessing strings stored in flash memory
 */
/*@{*/

/**
 * @brief Copy PROGMEM string to RAM buffer
 * @param pgm_ptr Pointer to string in program memory
 * @return Pointer to local buffer containing the string
 */
#define GET_PROGMEM_STR(pgm_ptr) \
  ({                                                   \
     char __buf[128];                                  \
     strcpy_P(__buf, (PGM_P)pgm_read_word((pgm_ptr))); \
     __buf;                                            \
   })

/*@}*/

/******************************************************************************/
/* Data Format Conversion                                                     */
/******************************************************************************/

/**
 * @name Hex Conversion
 * Macros for converting between hex strings and byte values
 */
/*@{*/

/**
 * @brief Convert hex string (2 chars) to byte
 * @param hex_ptr Pointer to 2-character hex string
 * @return Converted byte value
 */
#define HEX_TO_BYTE(hex_ptr) \
  (uint8_t)( (hex_to_nibble(hex_ptr) << 4) | \
             (hex_to_nibble(hex_ptr + 1)) )

/**
 * @brief Convert hex string (4 chars) to word
 * @param hex_ptr Pointer to 4-character hex string
 * @return Converted 16-bit value
 */
#define HEX_TO_WORD(hex_ptr) \
  (uint16_t)( (hex_to_nibble(hex_ptr) << 12) |    \
               (hex_to_nibble(hex_ptr + 1) << 8) | \
               (hex_to_nibble(hex_ptr + 2) << 4) | \
               (hex_to_nibble(hex_ptr + 3)) )

/*@}*/

/**
 * @brief Convert byte to hex string
 * @param byte Value to convert
 * @param str Output buffer (minimum 3 bytes)
 */
void byte_to_hex(uint8_t byte, char *str);

/**
 * @brief Convert word to hex string
 * @param word Value to convert
 * @param str Output buffer (minimum 5 bytes)
 */
void word_to_hex(uint16_t word, char *str);

/**
 * @brief Convert single hex digit to nibble
 * @param hex Hex character (0-9, A-F, a-f)
 * @return Nibble value (0-15)
 */
uint8_t hex_to_nibble(const char *hex);

#ifdef AVR_UTILITY_IMPL

static const char PROGMEM hex_lut[] = \
  {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void byte_to_hex(uint8_t byte, char *str) {
  *str++ = pgm_read_byte(&hex_lut[(byte >> 4) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[byte & 0xf]);
  *str = 0;
}

void word_to_hex(uint16_t word, char *str) {
  *str++ = pgm_read_byte(&hex_lut[(word >> 12) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[(word >> 8) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[(word >> 4) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[word & 0xf]);
  *str = 0;
}

uint8_t hex_to_nibble(const char *hex) {
  uint8_t byte = *hex;
  if (byte >= 'a') byte = (byte - 'a') + 10;
  else if (byte >= 'A') byte = (byte - 'A') + 10;
  else byte -= '0';
  return byte;
}

#endif /* AVR_UTILITY_IMPL */

#endif /* _AVR_USER_UTILITY_H_ */
