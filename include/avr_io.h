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
 * @file avr_io.h
 * @brief GPIO utility macros for AVR microcontrollers
 * @author notweerdmonk
 * 
 * This header provides low-level macros for manipulating GPIO pins on AVR
 * microcontrollers, including DDR, PORT, and PIN register operations.
 */

#ifndef _AVR_USER_IO_H_
#define _AVR_USER_IO_H_

#include <stddef.h>

/** @brief Pin not connected marker */
#define PIN_NC -1

/******************************************************************************/
/* Register Manipulation Macros                                               */
/******************************************************************************/

/**
 * @name Register Write Macros
 * Write values directly to DDR or PORT registers
 */
/*@{*/

/** @brief Write to DDR register */
#define WRITE_DDR(x, y)     (CONCAT(DDR,x) = (y))

/** @brief Write to PORT register */
#define WRITE_PORT(x, y)    (CONCAT(PORT,x) = (y))

/*@}*/

/**
 * @name Register Read Macros
 * Read values from DDR or PORT registers
 */
/*@{*/

/** @brief Read DDR register */
#define READ_DDR(x)         (CONCAT(DDR,x))

/** @brief Read PORT register */
#define READ_PORT(x)        (CONCAT(PORT,x))

/*@}*/

/**
 * @name Bit Manipulation Macros
 * Set or clear bits in DDR or PORT registers
 */
/*@{*/

/** @brief Set bits in DDR register */
#define SET_DDR(x, y)       (CONCAT(DDR,x) |= (y))

/** @brief Clear bits in DDR register */
#define CLR_DDR(x, y)       (CONCAT(DDR,x) &= ~(y))

/** @brief Set bits in PORT register */
#define SET_PORT(x, y)      (CONCAT(PORT,x) |= (y))

/** @brief Clear bits in PORT register */
#define CLR_PORT(x, y)      (CONCAT(PORT,x) &= ~(y))

/** @brief Clear all bits in PORT register */
#define ZERO_PORT(x)        (CONCAT(PORT,x) = 0)

/*@}*/

/**
 * @name Port Direction Macros
 * Set entire port as input or output
 */
/*@{*/

/** @brief Set entire port as output */
#define OUTPUT_PORT(x)      (CONCAT(DDR,x) = 0xFF)

/** @brief Set entire port as input */
#define INPUT_PORT(x)       (CONCAT(DDR,x) = 0x00)

/*@}*/

/**
 * @name Pin Direction Macros
 * Set individual pins as input or output
 */
/*@{*/

/**
 * @brief Set single pin as output
 * @param x Port letter (A, B, C, D, etc.)
 * @param y Pin number (0-7)
 */
#define OUTPUT_PIN(x, y)    (CONCAT(DDR,x) |= (BITMASK((y))))

/**
 * @brief Set single pin as input
 * @param x Port letter (A, B, C, D, etc.)
 * @param y Pin number (0-7)
 */
#define INPUT_PIN(x, y)     (CONCAT(DDR,x) &=	~(BITMASK((y))))

/*@}*/

/**
 * @name Pin State Read/Write Macros
 * Read or write individual pin states
 */
/*@{*/

/** @brief Read pin state */
#define READ_PIN(x, y)      (CONCAT(PIN,x) & (BITMASK((y))))

/** @brief Set pin high */
#define SET_PIN(x, y)       (CONCAT(PORT,x) |= (BITMASK((y))))

/** @brief Set pin low */
#define CLR_PIN(x, y)       (CONCAT(PORT,x) &= ~(BITMASK((y))))

/*@}*/

/**
 * @name Immediate Pin Macros
 * Use with literal values for smaller code size
 */
/*@{*/

/**
 * @brief Set pin as output using immediate addressing
 * @param x Port letter
 * @param y Pin number (literal)
 */
#define OUTPUT_PIN_IMM(x, y) \
  asm volatile(                                  \
      "sbi %[ddr], %[bit]\n"                     \
      :: [ddr] "I" _SFR_IO_ADDR(CONCAT(DDR, x)), \
      [bit] "I" (y))

/**
 * @brief Set pin as input using immediate addressing
 * @param x Port letter
 * @param y Pin number (literal)
 */
#define INPUT_PIN_IMM(x, y) \
  asm volatile(                                  \
      "cbi %[ddr], %[bit]\n"                     \
      :: [ddr] "I" _SFR_IO_ADDR(CONCAT(DDR, x)), \
      [bit] "I" (y))

/**
 * @brief Set pin high using immediate addressing
 * @param x Port letter
 * @param y Pin number (literal)
 */
#define SET_PIN_IMM(x, y) \
  asm volatile(                                    \
      "sbi %[port], %[bit]\n"                      \
      :: [port] "I" _SFR_IO_ADDR(CONCAT(PORT, x)), \
      [bit] "I" (y))

/**
 * @brief Clear pin using immediate addressing
 * @param x Port letter
 * @param y Pin number (literal)
 */
#define CLR_PIN_IMM(x, y) \
  asm volatile(                                    \
      "cbi %[port], %[bit]\n"                      \
      :: [port] "I" _SFR_IO_ADDR(CONCAT(PORT, x)), \
      [bit] "I" (y))

/*@}*/

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) ||     \
    defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) ||   \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) ||     \
    defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) ||   \
    defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) ||   \
    defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

/**
 * @brief Toggle pin state
 * @param x Port letter
 * @param y Pin number
 */
#define TOGGLE_PIN(x, y)   (CONCAT(PIN, x) |= (BITMASK(y)))

/**
 * @brief Toggle pin using immediate addressing
 * @param x Port letter
 * @param y Pin number (literal)
 */
#define TOGGLE_PIN_IMM(x, y) \
  asm volatile(                                  \
      "sbi %[pin], %[bit]\n"                     \
      :: [pin] "I" _SFR_IO_ADDR(CONCAT(PIN, x)), \
      [bit] "I" (y))

#else

#define TOGGLE_PIN(x, y)   (CONCAT(PORT,x) ^= (BITMASK(y)))

#endif

/**
 * @name Pin Debouncing
 * Macros for pin debouncing
 */
/*@{*/

/**
 * @brief Poll pin level with debouncing
 * @param port Port letter
 * @param pin Pin number
 * @param state Expected state (0 or non-zero)
 * @return TRUE after 3 consecutive reads matching state
 */
#define POLL_PIN_LEVEL(port, pin, state) \
  ({                                                      \
    static uint8_t _debounce = 0;                         \
    if ( (CONCAT(PIN,port) & (BITMASK(pin))) == state )   \
      {if (++_debounce == -1) _debounce = 3;}             \
    else _debounce = 0;                                   \
    (_debounce >= 3) ? TRUE : FALSE;                      \
  })

/*@}*/

#endif /* _AVR_USER_IO_H_ */
