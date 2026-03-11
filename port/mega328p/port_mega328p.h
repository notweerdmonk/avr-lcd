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
 * @file port_mega328p.h
 * @brief ATmega328P port abstraction layer
 * @author notweerdmonk
 * 
 * This file provides GPIO and peripheral macros for the ATmega328P microcontroller.
 * It includes pin manipulation, timer configurations, USART, ADC, and other
 * peripheral definitions.
 * 
 * @note This header is automatically included by port.h when compiling for ATmega328P
 */

#ifndef PORT_MEGA328P_H
#define PORT_MEGA328P_H

#include <avr_io.h>

#ifdef __AVR_ATmega328P__

/* TODO: Use BITMASK */

/******************************************************************************/
/* PORT and DDR register manipulation                                         */
/******************************************************************************/

/**
 * @name Port Numbering
 * 
 * Port numbering for relative pin mode:
 * - 1 : DDRB/PORTB/PINB
 * - 2 : DDRC/PORTC/PINC
 * - 3 : DDRD/PORTD/PIND
 */

/**
 * @name Pin Numbering
 * 
 * Absolute pin numbering for ATmega328P:
 * - 8..15   : PB0..PB7
 * - 16..21  : PC0..PC6
 * - 22, 23  : undefined
 * - 24..31  : PD0..PD7
 */

/******************************************************************************/
/* Relative port number macros                                                */
/******************************************************************************/

/**
 * @brief Set DDR register by port number
 * @param ddr Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param mask Bitmask to set
 */
#define SET_DDR_NUMBER(ddr, mask) \
  do {                  \
    if (ddr == 1)       \
      SET_DDR(B, mask); \
    else if (ddr == 2)  \
      SET_DDR(C, mask); \
    else if (ddr == 3)  \
      SET_DDR(D, mask); \
  } while (0)

/**
 * @brief Clear DDR register bits by port number
 * @param ddr Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param mask Bitmask to clear
 */
#define CLR_DDR_NUMBER(ddr, mask) \
  do {                  \
    if (ddr == 1)       \
      CLR_DDR(B, mask); \
    else if (ddr == 2)  \
      CLR_DDR(C, mask); \
    else if (ddr == 3)  \
      CLR_DDR(D, mask); \
  } while (0)

/**
 * @brief Set PORT register bits by port number
 * @param port Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param mask Bitmask to set
 */
#define SET_PORT_NUMBER(port, mask) \
  do {                   \
    if (port == 1)       \
      SET_PORT(B, mask); \
    else if (port == 2)  \
      SET_PORT(C, mask); \
    else if (port == 3)  \
      SET_PORT(D, mask); \
  } while (0)

/**
 * @brief Clear PORT register bits by port number
 * @param port Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param mask Bitmask to clear
 */
#define CLR_PORT_NUMBER(port, mask) \
  do {                   \
    if (port == 1)       \
      CLR_PORT(B, mask); \
    else if (port == 2)  \
      CLR_PORT(C, mask); \
    else if (port == 3)  \
      CLR_PORT(D, mask); \
  } while (0)

/******************************************************************************/
/* Absolute Pin Number Macros                                                 */
/******************************************************************************/

/**
 * @name Absolute Pin Numbers
 * Convert absolute pin numbers (8-31) to port-relative pin numbers
 */
/*@{*/

/**
 * @brief Convert absolute pin number to port-relative pin
 * @param p Absolute pin number (8-31)
 * @return Pin number relative to port (0-7)
 */
#define CONVERT_PIN_NUMBER(p) \
  ({                             \
    uint8_t pin = p;             \
    if (p <= 7)                  \
      pin = p;                   \
    else if (p >= 8 && p <= 15)  \
      pin = p - 8;               \
    else if (p >= 16 && p <= 21) \
      pin = p - 16;              \
    else if (p >= 24 && p <= 31) \
      pin = p - 24;              \
    pin;                         \
  })

/**
 * @brief Get PORT register address from absolute pin number
 * @param p Absolute pin number (8-31)
 * @return Pointer to PORT register
 */
#define PIN_NUMBER_TO_PORT(p) \
  ({                                 \
    volatile uint8_t *port = &PORTB; \
    if (p >= 8 && p <= 15)           \
      port = &PORTB;                 \
    else if (16 <= p && p <= 21)     \
      port = &PORTC;                 \
    else if (p >= 24 && p <= 31)     \
      port = &PORTD;                 \
    port;                            \
  })

/**
 * @brief Get DDR register address from absolute pin number
 * @param p Absolute pin number (8-31)
 * @return Pointer to DDR register
 */
#define PIN_NUMBER_TO_DDR(p) \
  ({                               \
    volatile uint8_t *ddr = &DDRB; \
    if (p >= 8 && p <= 15)         \
      ddr = &DDRB;                 \
    else if (p >= 16 && p <= 21)   \
      ddr = &DDRC;                 \
    else if (p >= 24 && p <= 31)   \
      ddr = &DDRD;                 \
    ddr;                           \
  })

/**
 * @brief Set pin as output by absolute pin number
 * @param p Absolute pin number (8-31)
 */
#define OUTPUT_PIN_NUMBER(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      OUTPUT_PIN(B, p - 8);      \
    else if (p >= 16 && p <= 21) \
      OUTPUT_PIN(C, p - 16);     \
    else if (p >= 24 && p <= 31) \
      OUTPUT_PIN(D, p - 24);     \
  } while (0)

/**
 * @brief Set pin as output by absolute pin number (immediate)
 * @param p Absolute pin number (8-31)
 */
#define OUTPUT_PIN_NUMBER_IMM(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      OUTPUT_PIN_IMM(B, p - 8);  \
    else if (p >= 16 && p <= 21) \
      OUTPUT_PIN_IMM(C, p - 16); \
    else if (p >= 24 && p <= 31) \
      OUTPUT_PIN_IMM(D, p - 24); \
  } while (0)

/**
 * @brief Set pin as input by absolute pin number
 * @param p Absolute pin number (8-31)
 */
#define INPUT_PIN_NUMBER(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      INPUT_PIN(B, p - 8);       \
    else if (p >= 16 && p <= 21) \
      INPUT_PIN(C, p - 16);      \
    else if (p >= 24 && p <= 31) \
      INPUT_PIN(D, p - 24);      \
  } while (0)

/**
 * @brief Set pin as input by absolute pin number (immediate)
 * @param p Absolute pin number (8-31)
 */
#define INPUT_PIN_NUMBER_IMM(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      INPUT_PIN_IMM(B, p - 8);   \
    else if (p >= 16 && p <= 21) \
      INPUT_PIN_IMM(C, p - 16);  \
    else if (p >= 24 && p <= 31) \
      INPUT_PIN_IMM(D, p - 24);  \
  } while (0)

/**
 * @brief Set pin high by absolute pin number
 * @param p Absolute pin number (8-31)
 */
#define SET_PIN_NUMBER(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      SET_PIN(B, p - 8);         \
    else if (p >= 16 && p <= 21) \
      SET_PIN(C, p - 16);        \
    else if (p >= 24 && p <= 31) \
      SET_PIN(D, p - 24);        \
  } while (0)

/**
 * @brief Set pin high by absolute pin number (immediate)
 * @param p Absolute pin number (8-31)
 */
#define SET_PIN_NUMBER_IMM(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      SET_PIN_IMM(B, p - 8);     \
    else if (p >= 16 && p <= 21) \
      SET_PIN_IMM(C, p - 16);    \
    else if (p >= 24 && p <= 31) \
      SET_PIN_IMM(D, p - 24);    \
  } while (0)

/**
 * @brief Clear pin by absolute pin number
 * @param p Absolute pin number (8-31)
 */
#define CLR_PIN_NUMBER(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      CLR_PIN(B, p - 8);         \
    else if (p >= 16 && p <= 21) \
      CLR_PIN(C, p - 16);        \
    else if (p >= 24 && p <= 31) \
      CLR_PIN(D, p - 24);        \
  } while (0)

/**
 * @brief Clear pin by absolute pin number (immediate)
 * @param p Absolute pin number (8-31)
 */
#define CLR_PIN_NUMBER_IMM(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      CLR_PIN_IMM(B, p - 8);     \
    else if (p >= 16 && p <= 21) \
      CLR_PIN_IMM(C, p - 16);    \
    else if (p >= 24 && p <= 31) \
      CLR_PIN_IMM(D, p - 24);    \
  } while (0)

/**
 * @brief Toggle pin by absolute pin number
 * @param p Absolute pin number (8-31)
 */
#define TOGGLE_PIN_NUMBER(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      TOGGLE_PIN(B, p - 8);      \
    else if (p >= 16 && p <= 21) \
      TOGGLE_PIN(C, p - 16);     \
    else if (p >= 24 && p <= 31) \
      TOGGLE_PIN(D, p - 24);     \
  } while (0)

/**
 * @brief Toggle pin by absolute pin number (immediate)
 * @param p Absolute pin number (8-31)
 */
#define TOGGLE_PIN_NUMBER_IMM(p) \
  do {                           \
    if (p >= 8 && p <= 15)       \
      TOGGLE_PIN_IMM(B, p - 8);  \
    else if (p >= 16 && p <= 21) \
      TOGGLE_PIN_IMM(C, p - 16); \
    else if (p >= 24 && p <= 31) \
      TOGGLE_PIN_IMM(D, p - 24); \
  } while (0)

/**
 * @brief Read pin state by absolute pin number
 * @param p Absolute pin number (8-31)
 * @return Pin state
 */
#define GET_PIN_NUMBER(p) \
  ({                                \
    register uint8_t pin_val = 0;   \
    if (p >= 8 && p <= 15)          \
      pin_val = GET_PIN(B, p - 8);  \
    else if (p >= 16 && p <= 21)    \
      pin_val = GET_PIN(C, p - 16); \
    else if (p >= 24 && p <= 31)    \
      pin_val = GET_PIN(D, p - 24); \
    pin_val;                        \
  })

/*@}*/

/******************************************************************************/
/* Port Address Functions                                                     */
/******************************************************************************/

/**
 * @brief Convert port number to PORT register address
 * 
 * Converts a relative port number (1-3) to the corresponding PORT register
 * address for the ATmega328P.
 * 
 * @param portnum Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @return Pointer to PORT register, or NULL if invalid
 * 
 * @note Only available when AVR_LCD_RUNTIME_HARDWARE_REPR or 
 *       AVR_LCD_USE_RELATIVE_PIN_NUMBERS is defined
 */
#if defined AVR_LCD_RUNTIME_HARDWARE_REPR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS
static
volatile uint8_t* port_num_to_addr(uint8_t portnum) {
  volatile uint8_t *addr = NULL;

 switch (portnum) {
    case 1: {
      addr = &PORTB;
      break;
    }
    case 2: {
      addr = &PORTC;
      break;
    }
    case 3: {
      addr = &PORTD;
      break;
    }
    default:
      ;
  };

  return addr;
}
#endif

/******************************************************************************/
/* Variable Argument Pin Macros                                               */
/******************************************************************************/

/**
 * @name Variable Pin Macros
 * Mac variableros for handling number of pins
 */
/*@{*/

/** @brief Helper macro for token concatenation */
#define __CAT(x, y, ...) x##y

/** @brief Token concatenation */
#define CAT(x, ...) __CAT(x, __VA_ARGS__)

/** @brief Sequence counter for variadic macros */
#define _SEQ(_1, _2, _3, _4, _5, _6, _7, _8, n, ...) n

/** @brief Count number of arguments */
#define _COUNT(...) _SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

/*@}*/

/**
 * @name Port Address Pin Macros
 * Set multiple pins from port address
 */
/*@{*/

/**
 * @brief Set single output pin from port address
 * @param addr Port address
 * @param pin Pin number
 */
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_1(addr, pin) \
  *addr |= (1 << pin) 
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_2(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_1(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_3(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_2(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_4(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_3(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_5(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_4(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_6(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_5(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_7(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_6(addr, __VA_ARGS__)
#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_8(addr, pin, ...) \
  *addr |= (1 << pin); _SET_OUTPUT_PINS_FROM_PORT_ADDR_7(addr, __VA_ARGS__)

#define _SET_OUTPUT_PINS_FROM_PORT_ADDR_EXPAND(n, addr, ...) \
  CAT(_SET_OUTPUT_PINS_FROM_PORT_ADDR_, n)(addr, __VA_ARGS__)

/**
 * @brief Set multiple output pins from port address
 * @param addr Port register address
 * @param ... Pin numbers to set as output
 */
#define SET_OUTPUT_PINS_FROM_PORT_ADDR(addr, ...) \
  _SET_OUTPUT_PINS_FROM_PORT_ADDR_EXPAND(COUNT(__VA_ARGS__), addr, __VA_ARGS__)

/**
 * @brief Set port nibble as output by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param startbit Starting bit position (0-4)
 */
#define SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(num, startbit) \
  switch (num) {                        \
    case 1: {                           \
      SET_DDR(B, (0x0F << startbit));   \
      break;                            \
    }                                   \
    case 2: {                           \
      SET_DDR(C, (0x0F << startbit));   \
      break;                            \
    }                                   \
    case 3: {                           \
      SET_DDR(D, (0x0F << startbit));   \
      break;                            \
    }                                   \
    default:                            \
      return;                           \
  }

/**
 * @brief Set entire port as output by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 */
#define SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(num) \
  switch (num) {        \
    case 1: {           \
      OUTPUT_PORT(B);   \
      break;            \
    }                   \
    case 2: {           \
      OUTPUT_PORT(C);   \
      break;            \
    }                   \
    case 3: {           \
      OUTPUT_PORT(D);   \
      break;            \
    }                   \
    default:            \
      return;           \
  }

/**
 * @brief Set single pin as output by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param pin Pin number (0-7)
 */
#define SET_OUTPUT_PIN_FROM_PORT_NUM(num, pin) \
  switch (num) {            \
    case 1: {               \
      OUTPUT_PIN(B, pin);   \
      break;                \
    }                       \
    case 2: {               \
      OUTPUT_PIN(C, pin);   \
      break;                \
    }                       \
    case 3: {               \
      OUTPUT_PIN(D, pin);   \
      break;                \
    }                       \
    default:                \
      return;               \
  } 

/*@}*/

/**
 * @name Set Multiple Output Pins
 * Helper macros for setting multiple output pins
 */
/*@{*/

/**
 * @brief Set single output pin (internal)
 * @param port Port letter
 * @param pin Pin number
 */
#define _SET_PORT_OUTPUT_PINS_1(port, pin) \
  do {                        \
    if (pin != -1) {          \
      OUTPUT_PIN(port, pin);  \
    }                         \
  } while (0)

/** @brief Set two output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_2(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_1(port, __VA_ARGS__)

/** @brief Set three output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_3(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_2(port, __VA_ARGS__)

/** @brief Set four output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_4(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_3(port, __VA_ARGS__)

/** @brief Set five output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_5(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_4(port, __VA_ARGS__)

/** @brief Set six output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_6(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_5(port, __VA_ARGS__)

/** @brief Set seven output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_7(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_6(port, __VA_ARGS__)

/** @brief Set eight output pins (internal) */
#define _SET_PORT_OUTPUT_PINS_8(port, pin, ...) \
  OUTPUT_PIN(port, pin); _SET_PORT_OUTPUT_PINS_7(port, __VA_ARGS__)

#define _SET_PORT_OUTPUT_PINS_EXPAND(n, port, ...) \
  CAT(_SET_PORT_OUTPUT_PINS_, n)(port, __VA_ARGS__)

/**
 * @brief Set multiple pins as output on a port
 * @param port Port letter (A, B, C, D)
 * @param ... Pin numbers to set as output
 */
#define SET_PORT_OUTPUT_PINS(port, ...) \
  _SET_PORT_OUTPUT_PINS_EXPAND(_COUNT(__VA_ARGS__), port, __VA_ARGS__)

/**
 * @brief Set multiple pins as output by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param ... Pin numbers to set as output
 */
#define SET_OUTPUT_PINS_FROM_PORT_NUM(num, ...) \
  switch (num) {                            \
    case 1: {                               \
      SET_PORT_OUTPUT_PINS(B, __VA_ARGS__); \
      break;                                \
    }                                       \
    case 2: {                               \
      SET_PORT_OUTPUT_PINS(C, __VA_ARGS__); \
      break;                                \
    }                                       \
    case 3: {                               \
      SET_PORT_OUTPUT_PINS(D, __VA_ARGS__); \
      break;                                \
    }                                       \
  }

/**
 * @brief Set pin high by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param pin Pin number (0-7)
 */
#define SET_PIN_FROM_PORT_NUM(num, pin) \
  switch (num) {        \
    case 1: {           \
      SET_PIN(B, pin);  \
      break;            \
    }                   \
    case 2: {           \
      SET_PIN(C, pin);  \
      break;            \
    }                   \
    case 3: {           \
      SET_PIN(D, pin);  \
      break;            \
    }                   \
    default:            \
      return;           \
  } 

/*@}*/

/**
 * @name Set Multiple Pins High
 * Helper macros for setting multiple pins high
 */
/*@{*/

/** @brief Set single pin high (internal) */
#define _SET_PORT_PINS_1(port, pin) \
  do {                      \
    if (pin != -1) {        \
      SET_PIN(port, pin);   \
    }                       \
  } while (0)

/** @brief Set two pins high (internal) */
#define _SET_PORT_PINS_2(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_1(port, __VA_ARGS__)

/** @brief Set three pins high (internal) */
#define _SET_PORT_PINS_3(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_2(port, __VA_ARGS__)

/** @brief Set four pins high (internal) */
#define _SET_PORT_PINS_4(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_3(port, __VA_ARGS__)

/** @brief Set five pins high (internal) */
#define _SET_PORT_PINS_5(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_4(port, __VA_ARGS__)

/** @brief Set six pins high (internal) */
#define _SET_PORT_PINS_6(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_5(port, __VA_ARGS__)

/** @brief Set seven pins high (internal) */
#define _SET_PORT_PINS_7(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_6(port, __VA_ARGS__)

/** @brief Set eight pins high (internal) */
#define _SET_PORT_PINS_8(port, pin, ...) \
  SET_PIN(port, pin); _SET_PORT_PINS_7(port, __VA_ARGS__)

#define _SET_PORT_PINS_EXPAND(n, port, ...) \
  CAT(_SET_PORT_PINS_, n)(port, __VA_ARGS__)

/**
 * @brief Set multiple pins high on a port
 * @param port Port letter (A, B, C, D)
 * @param ... Pin numbers to set high
 */
#define SET_PORT_PINS(port, ...) \
  _SET_PORT_PINS_EXPAND(_COUNT(__VA_ARGS__), port, __VA_ARGS__)

/**
 * @brief Set multiple pins high by port number
 * @param num Port number (1=PORTB, 2=PORTC, 3=PORTD)
 * @param ... Pin numbers to set high
 */
#define SET_PINS_FROM_PORT_NUM(num, ...) \
  switch (num) {                      \
    case 1: {                         \
      SET_PORT_PINS(B, __VA_ARGS__);  \
      break;                          \
    }                                 \
    case 2: {                         \
      SET_PORT_PINS(C, __VA_ARGS__);  \
      break;                          \
    }                                 \
    case 3: {                         \
      SET_PORT_PINS(D, __VA_ARGS__);  \
      break;                          \
    }                                 \
  }

/*@}*/

/******************************************************************************/
/* Timer/Counter Modules                                                      */
/******************************************************************************/

/**
 * @name Timer/Counter 0
 * 8-bit timer with two compare channels
 */
/*@{*/

/** @brief Enable Timer/Counter 0 */
#define TIMER0

/**
 * @name Timer/Counter 0 Registers
 * Timer 0 register definitions
 */
/*@{*/

/** @brief Timer 0 counter register */
#define PORT_TIMER0_COUNTER_REGISTER \
  TCNT0

/** @brief Timer 0 control register A */
#define PORT_TIMER0_CONTROL_REGISTER_A \
  TCCR0A

/** @brief Timer 0 control register B */
#define PORT_TIMER0_CONTROL_REGISTER_B \
  TCCR0B

/*@}*/

/**
 * @name Timer/Counter 0 Waveform Generation Mode
 */
 /*@{*/

/** @brief waveform generation mode */
enum _port_timer0_wgm {
  PORT_TIMER0_WGM_NORMAL = 0,          /**< Normal mode */
  PORT_TIMER0_WGM_PWM_PHASE_CORRECT_1, /**< PWM, Phase Correct, 8-bit */
  PORT_TIMER0_WGM_CTC,                 /**< Clear Timer on Compare Match */
  PORT_TIMER0_WGM_FAST_PWM_1,          /**< Fast PWM, 8-bit */
  PORT_TIMER0_WGM_PWM_PHASE_CORRECT_2, /**< PWM, Phase Correct, 9-bit */
  PORT_TIMER0_WGM_FAST_PWM_2           /**< Fast PWM, 10-bit */
};

/**
 * @brief Set Timer 0 to Phase Correct PWM mode (8-bit)
 */
#define PORT_TIMER0_PWM_PHASE_CORRECT_1() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<WGM00)

/**
 * @brief Set Timer 0 to CTC mode
 */
#define PORT_TIMER0_CTC() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<WGM01)

/**
 * @brief Set Timer 0 to Fast PWM mode (8-bit)
 */
#define PORT_TIMER0_FAST_PWM_1() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<WGM00) | (1<<WGM01)

/**
 * @brief Set Timer 0 to Phase Correct PWM mode (10-bit)
 */
#define PORT_TIMER0_PWM_PHASE_CORRECT_2() \
  do { \
    PORT_TIMER0_CONTROL_REGISTER_A |= (1<<WGM00); \
    PORT_TIMER0_CONTROL_REGISTER_B |= (1<<WGM02); \
  } while (0)

/**
 * @brief Set Timer 0 to Fast PWM mode (10-bit)
 */
#define PORT_TIMER0_FAST_PWM_2() \
  do { \
    PORT_TIMER0_CONTROL_REGISTER_A |= (1<<WGM00) | (1<<WGM01); \
    PORT_TIMER0_CONTROL_REGISTER_B |= (1<<WGM02); \
  } while (0)

/*@}*/

/**
 * @name Timer/Counter 0 Compare Output Mode (Channel A)
 */
 /*@{*/

/** @brief compare output mode */
enum _port_timer0_com_a {
  PORT_TIMER0_COMA_NORMAL = 0,   /**< Normal port operation */
  PORT_TIMER0_COMA_TOGGLE_OC0A,  /**< Toggle OC0A on compare match */
  PORT_TIMER0_COMA_CLEAR_OC0A,   /**< Clear OC0A on compare match */
  PORT_TIMER0_COMA_SET_OC0A      /**< Set OC0A on compare match */
};

/**
 * @brief Toggle OC0A on compare match
 */
#define PORT_TIMER0_TOGGLE_OC0A() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0A0)

/**
 * @brief Clear OC0A on compare match
 */
#define PORT_TIMER0_CLEAR_OC0A() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0A1)

/**
 * @brief Set OC0A on compare match
 */
#define PORT_TIMER0_SET_OC0A() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0A0) | (1<<COM0A1)

/*@}*/

/**
 * @name Timer/Counter 0 Compare Output Mode (Channel B)
 */
 /*@{*/

/** @brief compare output mode */
enum _port_timer0_com_b {
  PORT_TIMER0_COMB_NORMAL = 0,   /**< Normal port operation */
  PORT_TIMER0_COMB_TOGGLE_OC0B,  /**< Toggle OC0B on compare match */
  PORT_TIMER0_COMB_CLEAR_OC0B,   /**< Clear OC0B on compare match */
  PORT_TIMER0_COMB_SET_OC0B      /**< Set OC0B on compare match */
};

/**
 * @brief Toggle OC0B on compare match
 */
#define PORT_TIMER0_TOGGLE_OC0B() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0B0)

/**
 * @brief Clear OC0B on compare match
 */
#define PORT_TIMER0_CLEAR_OC0B() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0B1)

/**
 * @brief Set OC0B on compare match
 */
#define PORT_TIMER0_SET_OC0B() \
  PORT_TIMER0_CONTROL_REGISTER_A |= (1<<COM0B0) | (1<<COM0B1)

/*@}*/

/**
 * @name Timer/Counter 0 Clock Select
 */
/*@{*/

/** @brief Clock select: No clock source */
#define PORT_TIMER0_CS_1_MASK \
  (1<<CS00)

/** @brief Clock select: IO clock / 1 */
#define PORT_TIMER0_CS_8_MASK \
  (1<<CS01)

/** @brief Clock select: IO clock / 8 */
#define PORT_TIMER0_CS_64_MASK \
  (1<<CS00) | (1<<CS01)

/** @brief Clock select: IO clock / 64 */
#define PORT_TIMER0_CS_256_MASK \
  (1<<CS02)

/** @brief Clock select: IO clock / 256 */
#define PORT_TIMER0_CS_1024_MASK \
  (1<<CS00) | (1<<CS02)

/** @brief Clock select: IO clock / 1024 */
#define PORT_TIMER0_CS_EXT_FALLING_MASK \
  (1<<CS01) | (1<<CS02)

/** @brief Clock select: External clock on T0 pin, falling edge */
#define PORT_TIMER0_CS_EXT_RISING_MASK \
  (1<<CS00) | (1<<CS01) | (1<<CS02)

/** @brief Clock select: External clock on T0 pin, rising edge */

/**
 * @brief Set Timer 0 clock source
 * @param cs Clock select (1, 8, 64, 256, 1024, EXT_FALLING, EXT_RISING)
 */
#define PORT_TIMER0_SET_CS(cs) \
  PORT_TIMER0_CONTROL_REGISTER_B |= \
    PORT_TIMER0_CS_##cs##_MASK

/*@}*/

/**
 * @name Timer/Counter 0 Counter/Compare
 */
/*@{*/

/**
 * @brief Set Timer 0 counter value
 * @param value Counter value (0-255)
 */
#define PORT_TIMER0_SET_COUNT(value) \
  PORT_TIMER0_COUNTER_REGISTER = (value)

/**
 * @brief Set Timer 0 output compare register
 * @param channel Compare channel (A or B)
 * @param value Compare value (0-255)
 */
#define PORT_TIMER0_SET_OCR(channel, value) \
  OCR0##channel = (value)

/*@}*/

/**
 * @name Timer/Counter 0 Force Output Compare
 */
/*@{*/

/** @brief Force Output Compare Channel A */
#define PORT_TIMER0_FORCE_OCRA_MASK \
  (1<<FOC0A)

/** @brief Force Output Compare Channel B */
#define PORT_TIMER0_FORCE_OCRB_MASK \
  (1<<FOC0B)

/**
 * @brief Force output compare on channel
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER0_FORCE_OUTPUT_COMPARE(channel) \
  PORT_TIMER0_CONTROL_REGISTER_B |= \
    PORT_TIMER0_FORCE_OCR##channel##_MASK

/*@}*/

/**
 * @name Timer/Counter 0 Interrupt Vectors
 */
/*@{*/

/** @brief Timer 0 Compare Match A interrupt vector */
#define PORT_TIMER0_COMPA_VECT \
  TIMER0_COMPA_vect

/** @brief Timer 0 Compare Match B interrupt vector */
#define PORT_TIMER0_COMPB_VECT \
  TIMER0_COMPB_vect

/** @brief Timer 0 Overflow interrupt vector */
#define PORT_TIMER0_OVF_VECT \
  TIMER0_OVF_vect

/*@}*/

/**
 * @name Timer/Counter 0 Interrupts
 */
/*@{*/

/** @brief Timer 0 interrupt mask register */
#define PORT_TIMER0_INTERRUPT_MASK_REGISTER \
  TIMSK0

/** @brief Timer 0 Compare Match A interrupt enable mask */
#define PORT_TIMER0_ENABLE_OCRA_INTERRUPT_MASK \
  (1<<OCIE0A)

/** @brief Timer 0 Compare Match B interrupt enable mask */
#define PORT_TIMER0_ENABLE_OCRB_INTERRUPT_MASK \
  (1<<OCIE0B)

/**
 * @brief Enable Timer 0 compare match interrupt
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER0_ENABLE_OCR_INTERRUPT(channel) \
  PORT_TIMER0_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER0_ENABLE_OCR##channel##_INTERRUPT_MASK

/** @brief Timer 0 overflow interrupt enable mask */
#define PORT_TIMER0_ENABLE_OVERFLOW_INTERRUPT_MASK \
  (1<<TOIE0)

/**
 * @brief Enable Timer 0 overflow interrupt
 */
#define PORT_TIMER0_ENABLE_OVERFLOW_INTERRUPT() \
  PORT_TIMER0_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER0_ENABLE_OVERFLOW_INTERRUPT_MASK

/*@}*/

/**
 * @name Timer/Counter 0 Interrupt Flags
 */
/*@{*/

/** @brief Timer 0 interrupt flag register */
#define PORT_TIMER0_INTERRUPT_FLAG_REGISTER \
  TIFR0

/** @brief Timer 0 Compare Match A interrupt flag */
#define PORT_TIMER0_0CRA_INTERRUPT_MASK \
  (1<<OCF0A)

/** @brief Timer 0 Compare Match B interrupt flag */
#define PORT_TIMER0_0CRB_INTERRUPT_MASK \
  (1<<OCF0B)

#define PORT_TIMER0_TOV_INTERRUPT_MASK \
  (1<<TOV0)

/*@}*/

/**
 * @name Timer/Counter 1
 * 16-bit timer with two compare channels and input capture
 */
/*@{*/

/** @brief Enable Timer/Counter 1 */
#define TIMER1

/**
 * @name Timer/Counter 1 Registers
 */
/*@{*/

/** @brief Timer 1 counter register (16-bit) */
#define PORT_TIMER1_COUNTER_REGISTER \
  TCNT1

/** @brief Timer 1 control register A */
#define PORT_TIMER1_CONTROL_REGISTER_A \
  TCCR1A

/** @brief Timer 1 control register B */
#define PORT_TIMER1_CONTROL_REGISTER_B \
  TCCR1B

/** @brief Timer 1 control register C */
#define PORT_TIMER1_CONTROL_REGISTER_C \
  TCCR1C

/*@}*/

/**
 * @name Timer/Counter 1 Waveform Generation Mode
 */
 /*@{*/

/** @brief waveform generation mode */
enum _port_timer1_wgm {
  PORT_TIMER1_WGM_NORMAL = 0,                 /**< Normal mode */
  PORT_TIMER1_WGM_PWM_PHASE_CORRECT_8BIT,     /**< PWM, Phase Correct, 8-bit */
  PORT_TIMER1_WGM_PWM_PHASE_CORRECT_9BIT,     /**< PWM, Phase Correct, 9-bit */
  PORT_TIMER1_WGM_PWM_PHASE_CORRECT_10BIT,    /**< PWM, Phase Correct, 10-bit */
  PORT_TIMER1_WGM_CTC,                        /**< Clear Timer on Compare Match */
  PORT_TIMER1_WGM_FAST_PWM_8BIT,              /**< Fast PWM, 8-bit */
  PORT_TIMER1_WGM_FAST_PWM_9BIT,              /**< Fast PWM, 9-bit */
  PORT_TIMER1_WGM_FAST_PWM_10BIT,             /**< Fast PWM, 10-bit */
  PORT_TIMER1_WGM_PWM_PHASE_FREQ_CORRECT_ICP, /**< PWM, Phase/Freq Correct, ICP */
  PORT_TIMER1_WGM_PWM_PHASE_FREQ_CORRECT,     /**< PWM, Phase/Freq Correct */
  PORT_TIMER1_WGM_PWM_PHASE_CORRECT_ICP,      /**< PWM, Phase Correct, ICP */
  PORT_TIMER1_WGM_PWM_PHASE_CORRECT,          /**< PWM, Phase Correct */
  PORT_TIMER1_WGM_CTC_ICP,                    /**< CTC with ICP */
  PORT_TIMER1_WGM_FAST_PWM_ICP,               /**< Fast PWM with ICP */
  PORT_TIMER1_WGM_FAST_PWM                    /**< Fast PWM */
};

/**
 * @brief Set Timer 1 to Phase Correct PWM (8-bit)
 */
#define PORT_TIMER1_PWM_PHASE_CORRECT_8BIT() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10)

/**
 * @brief Set Timer 1 to Phase Correct PWM (9-bit)
 */
#define PORT_TIMER1_PWM_PHASE_CORRECT_9BIT() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM11)

/**
 * @brief Set Timer 1 to Phase Correct PWM (10-bit)
 */
#define PORT_TIMER1_PWM_PHASE_CORRECT_10BIT() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10) | (1<<WGM11)

/**
 * @brief Set Timer 1 to CTC mode
 */
#define PORT_TIMER1_CTC() \
  PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12)

/**
 * @brief Set Timer 1 to Fast PWM (8-bit)
 */
#define PORT_TIMER1_FAST_PWM_8BIT() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12); \
  } while(0)

/**
 * @brief Set Timer 1 to Fast PWM (9-bit)
 */
#define PORT_TIMER1_FAST_PWM_9BIT() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12); \
  } while(0)

/**
 * @brief Set Timer 1 to Fast PWM (10-bit)
 */
#define PORT_TIMER1_FAST_PWM_10BIT() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10) | (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12); \
  } while(0)

/**
 * @brief Set Timer 1 to Phase/Freq Correct PWM with ICP
 */
#define PORT_TIMER1_PWM_PHASE_FREQ_CORRECT_ICP() \
  PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM13)

/**
 * @brief Set Timer 1 to Phase/Freq Correct PWM
 */
#define PORT_TIMER1_PWM_PHASE_FREQ_CORRECT() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM13); \
  } while(0)

/**
 * @brief Set Timer 1 to Phase Correct PWM with ICP
 */
#define PORT_TIMER1_PWM_PHASE_CORRECT_ICP() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM13); \
  } while(0)

/**
 * @brief Set Timer 1 to Phase Correct PWM
 */
#define PORT_TIMER1_PWM_PHASE_CORRECT() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10) | (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM13); \
  } while(0)

/**
 * @brief Set Timer 1 to CTC with ICP
 */
#define PORT_TIMER1_CTC_ICP() \
  PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12) | (1<<WGM13)

/**
 * @brief Set Timer 1 to Fast PWM with ICP
 */
#define PORT_TIMER1_FAST_PWM_ICP() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12) | (1<<WGM13); \
  } while(0)

/**
 * @brief Set Timer 1 to Fast PWM
 */
#define PORT_TIMER1_FAST_PWM() \
  do { \
    PORT_TIMER1_CONTROL_REGISTER_A |= (1<<WGM10) | (1<<WGM11); \
    PORT_TIMER1_CONTROL_REGISTER_B |= (1<<WGM12) | (1<<WGM13); \
  } while(0)

/*@}*/

/**
 * @name Timer/Counter 1 Compare Output Mode (Channel A)
 */
 /*@{*/

/** @brief compare output mode */
enum _port_timer1_com_a {
  PORT_TIMER1_COMA_NORMAL = 0,    /**< Normal port operation */
  PORT_TIMER1_COMA_TOGGLE_OC1A,   /**< Toggle OC1A on compare match */
  PORT_TIMER1_COMA_CLEAR_OC1A,    /**< Clear OC1A on compare match */
  PORT_TIMER1_COMA_SET_OC1A       /**< Set OC1A on compare match */
};

/**
 * @brief Toggle OC1A on compare match
 */
#define PORT_TIMER1_TOGGLE_OC1A() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1A0)

/**
 * @brief Clear OC1A on compare match
 */
#define PORT_TIMER1_CLEAR_OC1A() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1A1)

/**
 * @brief Set OC1A on compare match
 */
#define PORT_TIMER1_SET_OC1A() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1A0) | (1<<COM1A1)

/*@}*/

/**
 * @name Timer/Counter 1 Compare Output Mode (Channel B)
 */
/*@{*/

/** @brief compare output mode */
enum _port_timer1_com_b {
  PORT_TIMER1_COMB_NORMAL = 0,    /**< Normal port operation */
  PORT_TIMER1_COMB_TOGGLE_OC1B,   /**< Toggle OC1B on compare match */
  PORT_TIMER1_COMB_CLEAR_OC1B,    /**< Clear OC1B on compare match */
  PORT_TIMER1_COMB_SET_OC1B       /**< Set OC1B on compare match */
};

/**
 * @brief Toggle OC1B on compare match
 */
#define PORT_TIMER1_TOGGLE_OC1B() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1B0)

/**
 * @brief Clear OC1B on compare match
 */
#define PORT_TIMER1_CLEAR_OC1B() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1B1)

/**
 * @brief Set OC1B on compare match
 */
#define PORT_TIMER1_SET_OC1B() \
  PORT_TIMER1_CONTROL_REGISTER_A |= (1<<COM1B0) | (1<<COM1B1)

/*@}*/

/**
 * @name Timer/Counter 1 Clock Select
 */
/*@{*/

/** @brief Clock select: No clock source */
#define PORT_TIMER1_CS_1_MASK \
  (1<<CS10)

/** @brief Clock select: IO clock / 1 */
#define PORT_TIMER1_CS_8_MASK \
  (1<<CS11)

/** @brief Clock select: IO clock / 8 */
#define PORT_TIMER1_CS_64_MASK \
  (1<<CS10) | (1<<CS11)

/** @brief Clock select: IO clock / 64 */
#define PORT_TIMER1_CS_256_MASK \
  (1<<CS12)

/** @brief Clock select: IO clock / 256 */
#define PORT_TIMER1_CS_1024_MASK \
  (1<<CS10) | (1<<CS12)

/** @brief Clock select: External clock on T1 pin, falling edge */
#define PORT_TIMER1_CS_EXT_FALLING_MASK \
  (1<<CS11) | (1<<CS12)

/** @brief Clock select: External clock on T1 pin, rising edge */
#define PORT_TIMER1_CS_EXT_RISING_MASK \
  (1<<CS10) | (1<<CS11) | (1<<CS12)

/**
 * @brief Set Timer 1 clock source
 * @param cs Clock select (1, 8, 64, 256, 1024, EXT_FALLING, EXT_RISING)
 */
#define PORT_TIMER1_SET_CS(cs) \
  PORT_TIMER1_CONTROL_REGISTER_B |= \
    PORT_TIMER1_CS_##cs##_MASK 

/**
 * @brief Set Timer 1 counter value
 * @param value Counter value (0-65535)
 */
#define PORT_TIMER1_SET_COUNT(value) \
  PORT_TIMER1_COUNTER_REGISTER = (value)

/**
 * @brief Set Timer 1 output compare register
 * @param channel Compare channel (A or B)
 * @param value Compare value (0-65535)
 */
#define PORT_TIMER1_SET_OCR(channel, value) \
  OCR1##channel = (value)

/** @brief Force Output Compare Channel A */
#define PORT_TIMER1_FORCE_OCRA_MASK \
  (1<<FOC1A)

/** @brief Force Output Compare Channel B */
#define PORT_TIMER1_FORCE_OCRB_MASK \
  (1<<FOC1B)

/**
 * @brief Force output compare on channel
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER1_FORCE_OUTPUT_COMPARE(channel) \
  PORT_TIMER1_CONTROL_REGISTER_C |= \
    PORT_TIMER1_FORCE_OCR##channel##_MASK

/** @brief Timer 1 Input Capture interrupt vector */
#define PORT_TIMER1_CAPT_VECT \
  TIMER1_CAPT_vect

/** @brief Timer 1 Compare Match A interrupt vector */
#define PORT_TIMER1_COMPA_VECT \
  TIMER1_COMPA_vect

/** @brief Timer 1 Compare Match B interrupt vector */
#define PORT_TIMER1_COMPB_VECT \
  TIMER1_COMPB_vect

/** @brief Timer 1 Overflow interrupt vector */
#define PORT_TIMER1_OVF_VECT \
  TIMER1_OVF_vect

/*@}*/

/**
 * @name Timer/Counter 1 Interrupts
 */
/*@{*/

/** @brief Timer 1 interrupt mask register */
#define PORT_TIMER1_INTERRUPT_MASK_REGISTER \
  TIMSK1

/** @brief Timer 1 Input Capture interrupt enable mask */
#define PORT_TIMER1_ENABLE_ICP_INTERRUPT_MASK \
  (1<<ICIE1)

/**
 * @brief Enable Timer 1 Input Capture interrupt
 */
#define PORT_TIMER1_ENABLE_ICP_INTERRUPT() \
  PORT_TIMER1_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER1_ENABLE_ICP_INTERRUPT_MASK

/** @brief Timer 1 Compare Match A interrupt enable mask */
#define PORT_TIMER1_ENABLE_OCRA_INTERRUPT_MASK \
  (1<<OCIE1A);

/** @brief Timer 1 Compare Match B interrupt enable mask */
#define PORT_TIMER1_ENABLE_OCRB_INTERRUPT_MASK \
  (1<<OCIE1B);

/**
 * @brief Enable Timer 1 compare match interrupt
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER1_ENABLE_OCR_INTERRUPT(channel) \
  PORT_TIMER1_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER1_ENABLE_OCR##channel##_INTERRUPT_MASK

/** @brief Timer 1 overflow interrupt enable mask */
#define PORT_TIMER1_ENABLE_OVERFLOW_INTERRUPT_MASK \
  (1<<TOIE1)

/**
 * @brief Enable Timer 1 overflow interrupt
 */
#define PORT_TIMER1_ENABLE_OVERFLOW_INTERRUPT() \
  PORT_TIMER1_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER1_ENABLE_OVERFLOW_INTERRUPT_MASK

/*@}*/

/**
 * @name Timer/Counter 1 Interrupt Flags
 */
/*@{*/

/** @brief Timer 1 interrupt flag register */
#define PORT_TIMER1_INTERRUPT_FLAG_REGISTER \
  TIFR1

/** @brief Timer 1 Input Capture interrupt flag */
#define PORT_TIMER1_ICP_INTERRUPT_MASK \
  (1<<ICF1)

/** @brief Timer 1 Compare Match A interrupt flag */
#define PORT_TIMER1_0CRA_INTERRUPT_MASK \
  (1<<OCF1A)

/** @brief Timer 1 Compare Match B interrupt flag */
#define PORT_TIMER1_0CRB_INTERRUPT_MASK \
  (1<<OCF1B)

/** @brief Timer 1 Overflow interrupt flag */
#define PORT_TIMER1_TOV_INTERRUPT_MASK \
  (1<<TOV1)

/*@}*/

/**
 * @name Timer/Counter 2
 * 8-bit async timer with two compare channels
 */
/*@{*/

/** @brief Enable Timer/Counter 2 */
#define TIMER2

/**
 * @name Timer/Counter 2 Registers
 */
/*@{*/

/** @brief Timer 2 counter register */
#define PORT_TIMER2_COUNTER_REGISTER \
  TCNT2

/** @brief Timer 2 control register A */
#define PORT_TIMER2_CONTROL_REGISTER_A \
  TCCR2A

/** @brief Timer 2 control register B */
#define PORT_TIMER2_CONTROL_REGISTER_B \
  TCCR2B

/*@}*/

/**
 * @name Timer/Counter 2 Waveform Generation Mode
 */
/*@{*/

/** @brief waveform generation mode */
enum _port_timer2_wgm {
  PORT_TIMER2_WGM_NORMAL = 0,           /**< Normal mode */
  PORT_TIMER2_WGM_PWM_PHASE_CORRECT_1,  /**< PWM, Phase Correct */
  PORT_TIMER2_WGM_CTC,                  /**< Clear Timer on Compare Match */
  PORT_TIMER2_WGM_FAST_PWM_1,           /**< Fast PWM */
  PORT_TIMER2_WGM_PWM_PHASE_CORRECT_2,  /**< PWM, Phase Correct */
  PORT_TIMER2_WGM_FAST_PWM_2            /**< Fast PWM */
};

/**
 * @brief Set Timer 2 to Phase Correct PWM mode
 */
#define PORT_TIMER2_PWM_PHASE_CORRECT_1() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<WGM20)

/**
 * @brief Set Timer 2 to CTC mode
 */
#define PORT_TIMER2_CTC() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<WGM21)

/**
 * @brief Set Timer 2 to Fast PWM mode
 */
#define PORT_TIMER2_FAST_PWM_1() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<WGM20) | (1<<WGM21)

/**
 * @brief Set Timer 2 to Phase Correct PWM mode (advanced)
 */
#define PORT_TIMER2_PWM_PHASE_CORRECT_2() \
  do { \
    PORT_TIMER2_CONTROL_REGISTER_A |= (1<<WGM20); \
    PORT_TIMER2_CONTROL_REGISTER_B |= (1<<WGM22); \
  } while (0)

/**
 * @brief Set Timer 2 to Fast PWM mode (advanced)
 */
#define PORT_TIMER2_FAST_PWM_2() \
  do { \
    PORT_TIMER2_CONTROL_REGISTER_A |= (1<<WGM20) | (1<<WGM21); \
    PORT_TIMER2_CONTROL_REGISTER_B |= (1<<WGM22); \
  } while (0)

/*@}*/

/**
 * @name Timer/Counter 2 Compare Output Mode (Channel A)
 */
/*@{*/

/** @brief compare output mode */
enum _port_timer2_com_a {
  PORT_TIMER2_COMA_NORMAL = 0,    /**< Normal port operation */
  PORT_TIMER2_COMA_TOGGLE_OC2A,   /**< Toggle OC2A on compare match */
  PORT_TIMER2_COMA_CLEAR_OC2A,    /**< Clear OC2A on compare match */
  PORT_TIMER2_COMA_SET_OC2A       /**< Set OC2A on compare match */
};

/**
 * @brief Toggle OC2A on compare match
 */
#define PORT_TIMER2_TOGGLE_OC2A() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2A0)

/**
 * @brief Clear OC2A on compare match
 */
#define PORT_TIMER2_CLEAR_OC2A() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2A1)

/**
 * @brief Set OC2A on compare match
 */
#define PORT_TIMER2_SET_OC2A() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2A0) | (1<<COM2A1)

/*@}*/

/**
 * @name Timer/Counter 2 Compare Output Mode (Channel B)
 */
/*@{*/

/** @brief compare output mode */
enum _port_timer2_com_b {
  PORT_TIMER2_COMB_NORMAL = 0,    /**< Normal port operation */
  PORT_TIMER2_COMB_TOGGLE_OC2B,   /**< Toggle OC2B on compare match */
  PORT_TIMER2_COMB_CLEAR_OC2B,    /**< Clear OC2B on compare match */
  PORT_TIMER2_COMB_SET_OC2B       /**< Set OC2B on compare match */
};

/**
 * @brief Toggle OC2B on compare match
 */
#define PORT_TIMER2_TOGGLE_OC2B() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2B0)

/**
 * @brief Clear OC2B on compare match
 */
#define PORT_TIMER2_CLEAR_OC2B() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2B1)

/**
 * @brief Set OC2B on compare match
 */
#define PORT_TIMER2_SET_OC2B() \
  PORT_TIMER2_CONTROL_REGISTER_A |= (1<<COM2B0) | (1<<COM2B1)

/*@}*/

/**
 * @name Timer/Counter 2 Clock Select
 */
/*@{*/

/** @brief Clock select: No clock source */
#define PORT_TIMER2_CS_1_MASK \
  (1<<CS20)

/** @brief Clock select: IO clock / 1 */
#define PORT_TIMER2_CS_8_MASK \
  (1<<CS21)

/** @brief Clock select: IO clock / 8 */
#define PORT_TIMER2_CS_32_MASK \
  (1<<CS20) | (1<<CS21)

/** @brief Clock select: IO clock / 32 */
#define PORT_TIMER2_CS_64_MASK \
  (1<<CS22)

/** @brief Clock select: IO clock / 64 */
#define PORT_TIMER2_CS_128_MASK \
  (1<<CS20) | (1<<CS22)

/** @brief Clock select: IO clock / 128 */
#define PORT_TIMER2_CS_256_MASK \
  (1<<CS21) | (1<<CS22)

/** @brief Clock select: IO clock / 256 */
#define PORT_TIMER2_CS_1024_MASK \
  (1<<CS20) | (1<<CS21) | (1<<CS22)

/**
 * @brief Set Timer 2 clock source
 * @param cs Clock select (1, 8, 32, 64, 128, 256, 1024)
 */
#define PORT_TIMER2_SET_CS(cs) \
  PORT_TIMER2_CONTROL_REGISTER_B |= \
    PORT_TIMER2_CS_##cs##_MASK

#define PORT_TIMER2_SET_COUNT(value) \
  PORT_TIMER2_COUNTER_REGISTER = (value)

#define PORT_TIMER2_SET_OCR(channel, value) \
  OCR2##channel = (value)

/** @brief Force Output Compare Channel A */
#define PORT_TIMER2_FORCE_OCRA_MASK \
  (1<<FOC2A)

/** @brief Force Output Compare Channel B */
#define PORT_TIMER2_FORCE_OCRB_MASK \
  (1<<FOC2B)

/**
 * @brief Force output compare on channel
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER2_FORCE_OUTPUT_COMPARE(channel) \
  PORT_TIMER2_CONTROL_REGISTER_B |= \
    PORT_TIMER2_FORCE_OCR##channel##_MASK

/** @brief Timer 2 Compare Match A interrupt vector */
#define PORT_TIMER2_COMPA_VECT \
  TIMER2_COMPA_vect

/** @brief Timer 2 Compare Match B interrupt vector */
#define PORT_TIMER2_COMPB_VECT \
  TIMER2_COMPB_vect

/** @brief Timer 2 Overflow interrupt vector */
#define PORT_TIMER2_OVF_VECT \
  TIMER2_OVF_vect

/** @brief Timer 2 interrupt mask register */
#define PORT_TIMER2_INTERRUPT_MASK_REGISTER \
  TIMSK2

/** @brief Timer 2 Compare Match A interrupt enable mask */
#define PORT_TIMER2_ENABLE_OCRA_INTERRUPT_MASK \
  (1<<OCIE2A);

/** @brief Timer 2 Compare Match B interrupt enable mask */
#define PORT_TIMER2_ENABLE_OCRB_INTERRUPT_MASK \
  (1<<OCIE2B);

/**
 * @brief Enable Timer 2 compare match interrupt
 * @param channel Compare channel (A or B)
 */
#define PORT_TIMER2_ENABLE_OCR_INTERRUPT(channel) \
  PORT_TIMER2_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER2_ENABLE_OCR##channel##_INTERRUPT_MASK

/** @brief Timer 2 overflow interrupt enable mask */
#define PORT_TIMER2_ENABLE_OVERFLOW_INTERRUPT_MASK \
  (1<<TOIE2)

/**
 * @brief Enable Timer 2 overflow interrupt
 */
#define PORT_TIMER2_ENABLE_OVERFLOW_INTERRUPT() \
  PORT_TIMER2_INTERRUPT_MASK_REGISTER |= \
    PORT_TIMER2_ENABLE_OVERFLOW_INTERRUPT_MASK

/** @brief Timer 2 interrupt flag register */
#define PORT_TIMER2_INTERRUPT_FLAG_REGISTER \
  TIFR2

/** @brief Timer 2 Compare Match A interrupt flag */
#define PORT_TIMER2_0CRA_INTERRUPT_MASK \
  (1<<OCF2A)

/** @brief Timer 2 Compare Match B interrupt flag */
#define PORT_TIMER2_0CRB_INTERRUPT_MASK \
  (1<<OCF2B)

/** @brief Timer 2 Overflow interrupt flag */
#define PORT_TIMER2_TOV_INTERRUPT_MASK \
  (1<<TOV2)

/** @brief Timer 2 Asynchronous Status Register */
#define PORT_TIMER2_ASYNC_STATUS_REGISTER \
  ASSR

/**
 * @brief Enable external clock input for Timer 2
 */
#define PORT_TIMER2_ENABLE_EXTERNAL_CLK_INPUT() \
  PORT_TIMER2_ASYNC_STATUS_REGISTER |= (1<<EXCLK)

/**
 * @brief Enable asynchronous operation for Timer 2
 */
#define PORT_TIMER2_ASYNC_OPERATION() \
  PORT_TIMER2_ASYNC_STATUS_REGISTER |= (1<<AS2)

/** @brief Check if TCNT2 update is busy (async mode) */
#define PORT_TIMER2_TCNT_UPDATE_BUSY() \
  (PORT_TIMER2_ASYNC_STATUS_REGISTER & (1<<TCN2UB))

/** @brief Check if OCR2A update is busy (async mode) */
#define PORT_TIMER2_OCRA_UPDATE_BUSY() \
  (PORT_TIMER2_ASYNC_STATUS_REGISTER & (1<<OCR2AUB))

/** @brief Check if OCR2B update is busy (async mode) */
#define PORT_TIMER2_OCRB_UPDATE_BUSY() \
  (PORT_TIMER2_ASYNC_STATUS_REGISTER & (1<<OCR2BUB))

/** @brief Check if TCCR2A update is busy (async mode) */
#define PORT_TIMER2_TCCRA_UPDATE_BUSY() \
  (PORT_TIMER2_ASYNC_STATUS_REGISTER & (1<<TCR2AUB))

/** @brief Check if TCCR2B update is busy (async mode) */
#define PORT_TIMER2_TCCRB_UPDATE_BUSY() \
  (PORT_TIMER2_ASYNC_STATUS_REGISTER & (1<<TCR2BUB))

/*@}*/

/**
 * @name General Timer Control
 */
/*@{*/

/** @brief General Timer Control Register */
#define PORT_GENERAL_TIMER_CONTROL_REGISTER \
  GTCCR

/**
 * @brief Start timer synchronization mode
 */
#define PORT_TIMER_SYNCHRONIZATION_MODE_START() \
  PORT_GENERAL_TIMER_CONTROL_REGISTER |= (1<<TSM)

/**
 * @brief End timer synchronization mode
 */
#define PORT_TIMER_SYNCHRONIZATION_MODE_END() \
  PORT_GENERAL_TIMER_CONTROL_REGISTER &= ~(1<<TSM)

/**
 * @brief Reset prescaler for synchronous timers
 */
#define PORT_SYNC_TIMER_PRESCALER_RESET() \
  PORT_GENERAL_TIMER_CONTROL_REGISTER |= (1<<PSRSYNC)

/**
 * @brief Reset prescaler for asynchronous timers
 */
#define PORT_ASYNC_TIMER_PRESCALER_RESET() \
  PORT_GENERAL_TIMER_CONTROL_REGISTER |= (1<<PSRASY)

/*@}*/

/******************************************************************************/
/* USART Module                                                               */
/******************************************************************************/

/**
 * @name USART
 * Universal Synchronous/Asynchronous Receiver/Transmitter
 */
/*@{*/

/** @brief USART Data Register Empty interrupt vector */
#define PORT_UDRE_VECT \
  USART_UDRE_vect

/** @brief USART Receive Complete interrupt vector */
#define PORT_RXC_VECT \
  USART_RX_vect

/** @brief USART I/O Data Register */
#define PORT_UDR \
  UDR0

/**
 * @brief Calculate USART baud rate register value
 * @param baud_rate Desired baud rate
 * @return UBRR value
 */
#define PORT_UART_BAUDREG_CALC(baud_rate) \
  (uint16_t)((F_CPU / (16L * baud_rate)) - 1)

/**
 * @brief Set USART baud rate
 * @param baud_rate Desired baud rate
 */
#define PORT_UART_SET_BAUD_RATE(baud_rate) \
  UBRR0 = PORT_UART_BAUDREG_CALC(baud_rate)

/**
 * @brief Set USART character size
 * @param n Character size (5-9 bits)
 */
#define PORT_UART_SET_CHAR_SIZE(n) \
  if (n > 5) { \
    if (n == 9) UCSR0C |= (3<<UCSZ00); \
    else UCSR0C |= ((n-5)<<UCSZ00); \
  }

/**
 * @brief Set USART stop bits
 * @param n Number of stop bits (1 or 2)
 */
#define PORT_UART_SET_STOP_BITS(n) \
  if (n == 2) { \
    UCSR0C |= (1<<USBS0); \
  }

/**
 * @brief Set USART parity mode
 * @param parity Parity mode (UART_PARITY_EVEN or UART_PARITY_ODD)
 */
#define PORT_UART_SET_PARITY(parity) \
  if (parity == UART_PARITY_EVEN) { \
    UCSR0C |= (1<<UPM01); \
  } else if (parity == UART_PARITY_ODD) { \
    UCSR0C |= (1<<UPM00) | (1<<UPM01); \
  }

/**
 * @brief Initialize USART
 * Enables receiver, transmitter, and RX complete interrupt
 */
#define PORT_UART_INIT() \
  do { \
    UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0); \
  } while (0)

/** @brief Enable USART Data Register Empty interrupt */
#define PORT_ENABLE_UDRE_INTERRUPT() \
  UCSR0B |= (1<<UDRIE0)

/** @brief Disable USART Data Register Empty interrupt */
#define PORT_DISABLE_UDRE_INTERRUPT() \
  UCSR0B &= ~(1<<UDRIE0)

/** @brief Enable USART Receive Complete interrupt */
#define PORT_ENABLE_RXC_INTERRUPT() \
  UCSR0B |= (1<<RXCIE0)

/** @brief Disable USART Receive Complete interrupt */
#define PORT_DISABLE_RXC_INTERRUPT() \
  UCSR0B &= ~(1<<RXCIE0)

/*@}*/

/******************************************************************************/
/* ADC Module                                                                 */
/******************************************************************************/

/**
 * @name ADC
 * 10-bit Analog-to-Digital Converter
 */
/*@{*/

/** @brief ADC Control/Status Register A */
#define PORT_ADC_CONTROL_STATUS_REGISTER_A \
  ADCSRA

/** @brief ADC Control/Status Register B */
#define PORT_ADC_CONTROL_STATUS_REGISTER_B \
  ADCSRB

/** @brief ADC Multiplexer Control Register */
#define PORT_ADC_MUX_CONTROL_REGISTER \
  ADMUX

/** @brief ADC Data Register (10-bit result) */
#define PORT_ADC_DATA_REGISTER \
  ADC

/** @brief Enable ADC */
#define PORT_ADC_ENABLE() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= (1<<ADEN)

/** @brief Disable ADC */
#define PORT_ADC_DISABLE() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A &= ~(1<<ADEN)

/** @brief Check if ADC is enabled */
#define PORT_ADC_IS_ENABLED() \
  (PORT_ADC_CONTROL_STATUS_REGISTER_A & (1<<ADEN))

/*@}*/

/**
 * @name ADC Prescaler
 * Clock prescaler for ADC conversion
 */
/*@{*/

/** @brief ADC prescaler values */
enum _port_adc_prescaler {
  PORT_ADC_PS_2 = 1,
  PORT_ADC_PS_4,
  PORT_ADC_PS_8,
  PORT_ADC_PS_16,
  PORT_ADC_PS_32,
  PORT_ADC_PS_64,
  PORT_ADC_PS_128 
};

/** @brief Prescaler divide by 2 */
#define PORT_ADC_PS_2_MASK \
  (1<<ADPS0)

/** @brief Prescaler divide by 4 */
#define PORT_ADC_PS_4_MASK \
  (1<<ADPS1)

/** @brief Prescaler divide by 8 */
#define PORT_ADC_PS_8_MASK \
  (1<<ADPS0) | (1<<ADPS1)

/** @brief Prescaler divide by 16 */
#define PORT_ADC_PS_16_MASK \
  (1<<ADPS2)

/** @brief Prescaler divide by 32 */
#define PORT_ADC_PS_32_MASK \
  (1<<ADPS0) | (1<<ADPS2)

/** @brief Prescaler divide by 64 */
#define PORT_ADC_PS_64_MASK \
  (1<<ADPS1) | (1<<ADPS2)

/** @brief Prescaler divide by 128 */
#define PORT_ADC_PS_128_MASK \
  (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2)

/**
 * @brief Set ADC prescaler
 * @param ps Prescaler value (2, 4, 8, 16, 32, 64, 128)
 */
#define PORT_ADC_SET_PS(ps) \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= PORT_ADC_PS_##ps##_MASK

/** @brief ADC voltage reference selection */
enum _port_adc_reference {
  PORT_ADC_REF_AREF = 0,    /**< External AREF */
  PORT_ADC_REF_AVCC = 1,    /**< AVCC with external cap */
  PORT_ADC_REF_INTERNAL = 3 /**< Internal 1.1V */
};

/** @brief Set reference to AREF (external) */
#define PORT_ADC_SET_REF_AREF() \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<REFS0) | (1<<REFS1))

/** @brief Set reference to AVCC */
#define PORT_ADC_SET_REF_AVCC() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<REFS0), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~(1<<REFS1)

/** @brief Set reference to internal 1.1V */
#define PORT_ADC_SET_REF_INTERNAL() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<REFS0) | (1<<REFS1)

/**
 * @brief Set ADC voltage reference
 * @param ref Reference (AREF, AVCC, INTERNAL)
 */
#define PORT_ADC_SET_REF(ref) \
  PORT_ADC_SET_REF_##ref()

/** @brief Select ADC channel 0 */
#define PORT_ADC_SET_CHAN_0() \
  PORT_ADC_MUX_CONTROL_REGISTER &= 0Xf0

/** @brief Select ADC channel 1 */
#define PORT_ADC_SET_CHAN_1() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX0), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX1) | (1<<MUX2) | (1<<MUX3)) 

/** @brief Select ADC channel 2 */
#define PORT_ADC_SET_CHAN_2() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX1), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX0) | (1<<MUX2) | (1<<MUX3)) 

/** @brief Select ADC channel 3 */
#define PORT_ADC_SET_CHAN_3() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX0) | (1<<MUX1), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX2) | (1<<MUX3)) 

/** @brief Select ADC channel 4 */
#define PORT_ADC_SET_CHAN_4() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX2), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX3)) 

/** @brief Select ADC channel 5 */
#define PORT_ADC_SET_CHAN_5() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX0) | (1<<MUX2), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX1) | (1<<MUX3)) 

/** @brief Select ADC channel 6 */
#define PORT_ADC_SET_CHAN_6() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX1) | (1<<MUX2), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX0) | (1<<MUX3)) 

/** @brief Select ADC channel 7 */
#define PORT_ADC_SET_CHAN_7() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX0) | (1<<MUX1) | (1<<MUX2), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~(1<<MUX3) 

/** @brief Select ADC channel 8 (temperature sensor) */
#define PORT_ADC_SET_CHAN_8() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX3), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2)) 

/** @brief Select internal reference (1.1V) channel */
#define PORT_ADC_SET_CHAN_INTERNAL() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<MUX1) | (1<<MUX2) | (1<<MUX3), \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~(1<<MUX0) 

/** @brief Select GND channel (for offset calibration) */
#define PORT_ADC_SET_CHAN_GND() \
  PORT_ADC_MUX_CONTROL_REGISTER |= 0X0f

/**
 * @brief Select ADC channel
 * @param ch Channel number (0-7, 14=temp, 15=1.1V)
 */
#define PORT_ADC_SET_CHAN(ch) \
  if ((ch < 9) || (ch == 14) || (ch == 15)) { \
    PORT_ADC_MUX_CONTROL_REGISTER = \
    (PORT_ADC_MUX_CONTROL_REGISTER & 0xf0) | ch; \
  }

/** @brief Left-adjust ADC result (8-bit in ADCH) */
#define PORT_ADC_SET_LAR() \
  PORT_ADC_MUX_CONTROL_REGISTER |= (1<<ADLAR)

/** @brief Right-adjust ADC result (10-bit) */
#define PORT_ADC_SET_NO_LAR() \
  PORT_ADC_MUX_CONTROL_REGISTER &= ~(1<<ADLAR)

/** @brief Start ADC conversion */
#define PORT_ADC_START_CONVERSION() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= (1<<ADSC)

/** @brief Auto-trigger enable bit mask */
#define PORT_ADC_AUTO_TRIGGER_ENABLE_MASK \
  (1<<ADATE)

/** @brief Enable ADC auto-triggering */
#define PORT_ADC_ENABLE_AUTO_TRIGGER() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= PORT_ADC_AUTO_TRIGGER_ENABLE_MASK

/** @brief Disable ADC auto-triggering */
#define PORT_ADC_DISABLE_AUTO_TRIGGER() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A &= ~PORT_ADC_AUTO_TRIGGER_ENABLE_MASK

/** @brief Check if auto-trigger is enabled */
#define PORT_ADC_IS_AUTO_TRIGGER_ENABLED() \
  (PORT_ADC_CONTROL_STATUS_REGISTER_A & PORT_ADC_AUTO_TRIGGER_ENABLE_MASK)

/** @brief Set free-running mode trigger */
#define PORT_ADC_SET_FREE_RUNNING() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B &= 0xf8

/** @brief Set analog comparator as trigger */
#define PORT_ADC_SET_AT_ANALOG_COMPARATOR() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS0)

/** @brief Set external interrupt 0 as trigger */
#define PORT_ADC_SET_AT_EXTERNAL_IRQ0() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS1)

/** @brief Set Timer0 Compare Match A as trigger */
#define PORT_ADC_SET_AT_TIMER0_COMPARE_MATCH_A() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS0) | (1<<ADTS1)

/** @brief Set Timer0 overflow as trigger */
#define PORT_ADC_SET_AT_TIMER0_OVERFLOW() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS2)

/** @brief Set Timer1 Compare Match B as trigger */
#define PORT_ADC_SET_AT_TIMER1_COMPARE_MATCH_B() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS0) | (1<<ADTS2)

/** @brief Set Timer1 overflow as trigger */
#define PORT_ADC_SET_AT_TIMER1_OVERFLOW() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS1) | (1<<ADTS2)

/** @brief Set Timer1 capture event as trigger */
#define PORT_ADC_SET_AT_TIMER1_CAPTURE_EVENT() \
  PORT_ADC_CONTROL_STATUS_REGISTER_B |= (1<<ADTS0) | (1<<ADTS1) | (1<<ADTS2)

/** @brief ADC conversion complete interrupt vector */
#define PORT_ADC_VECT \
  ADC_vect

/** @brief ADC interrupt enable bit mask */
#define PORT_ADC_INTERRUPT_ENABLE_MASK \
  (1<<ADIE)

/** @brief Enable ADC interrupt */
#define PORT_ADC_ENABLE_INTERRUPT() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= PORT_ADC_INTERRUPT_ENABLE_MASK

/** @brief Disable ADC interrupt */
#define PORT_ADC_DISABLE_INTERRUPT() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A &= ~PORT_ADC_INTERRUPT_ENABLE_MASK

/** @brief Check if ADC interrupt is enabled */
#define PORT_ADC_IS_INTERRUPT_ENABLED() \
  (PORT_ADC_CONTROL_STATUS_REGISTER_A & PORT_ADC_INTERRUPT_ENABLE_MASK)

/** @brief ADC interrupt flag bit mask */
#define PORT_ADC_INTERRUPT_MASK \
  (1<<ADIF)

/** @brief Check if ADC interrupt flag is set */
#define PORT_ADC_IS_INTERRUPT_FLAG_SET() \
  (PORT_ADC_CONTROL_STATUS_REGISTER_A & PORT_ADC_INTERRUPT_MASK)

/** @brief Clear ADC interrupt flag */
#define PORT_ADC_CLEAR_INTERRUPT_FLAG() \
  PORT_ADC_CONTROL_STATUS_REGISTER_A |= PORT_ADC_INTERRUPT_MASK

/** @brief Digital Input Disable Register */
#define PORT_DIGITAL_INPUT_DISABLE_REGISTER \
  DIDR0

/** @brief Disable digital input on ADC0 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_0_MASK \
  (1<<ADC0D)

/** @brief Disable digital input on ADC1 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_1_MASK \
  (1<<ADC1D)

/** @brief Disable digital input on ADC2 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_2_MASK \
  (1<<ADC2D)

/** @brief Disable digital input on ADC3 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_3_MASK \
  (1<<ADC3D)

/** @brief Disable digital input on ADC4 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_4_MASK \
  (1<<ADC4D)

/** @brief Disable digital input on ADC5 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN_5_MASK \
  (1<<ADC5D)

/**
 * @brief Disable digital input on ADC pin
 * @param pin Pin number (0-5)
 */
#define PORT_DIGITAL_INPUT_DISABLE_PIN(pin) \
  PORT_DIGITAL_INPUT_DISABLE_REGISTER |= \
    PORT_DIGITAL_INPUT_DISABLE_PIN_##pin##_MASK

/*@}*/

/******************************************************************************/
/* Watchdog Timer                                                             */
/******************************************************************************/

/**
 * @name Watchdog Timer
 */
/*@{*/

/** @brief Watchdog Timer Control Register */
#define PORT_WDT_CONTROL_REGISTER \
  WDTCSR

/** @brief Watchdog Change Enable bit mask */
#define PORT_WDT_CHANGE_ENABLE_MASK \
  (1<<WDCE)

/** @brief Watchdog System Reset Enable bit mask */
#define PORT_WDT_ENABLE_SYSTEM_RESET_MASK \
  (1<<WDE)

/** @brief Watchdog Interrupt Enable bit mask */
#define PORT_WDT_ENABLE_INTERRUPT_MASK \
  (1<<WDIE)

/** @brief Watchdog timeout: 15 ms */
#define PORT_WDT_TIMEOUT_15MS   WDTO_15MS

/** @brief Watchdog timeout: 30 ms */
#define PORT_WDT_TIMEOUT_30MS   WDTO_30MS

/** @brief Watchdog timeout: 60 ms */
#define PORT_WDT_TIMEOUT_60MS   WDTO_60MS

/** @brief Watchdog timeout: 120 ms */
#define PORT_WDT_TIMEOUT_120MS  WDTO_120MS

/** @brief Watchdog timeout: 250 ms */
#define PORT_WDT_TIMEOUT_250MS  WDTO_250MS

/** @brief Watchdog timeout: 500 ms */
#define PORT_WDT_TIMEOUT_500MS  WDTO_500MS

/** @brief Watchdog timeout: 1 second */
#define PORT_WDT_TIMEOUT_1S     WDTO_1S

/** @brief Watchdog timeout: 2 seconds */
#define PORT_WDT_TIMEOUT_2S     WDTO_2S

/** @brief Watchdog timeout: 4 seconds */
#define PORT_WDT_TIMEOUT_4S     WDTO_4S

/** @brief Watchdog timeout: 8 seconds */
#define PORT_WDT_TIMEOUT_8S     WDTO_8S

/** @brief Watchdog timer interrupt vector */
#define PORT_WDT_VECT \
  WDT_vect

/**
 * @brief Enable watchdog timer with interrupt
 * @param timeout Watchdog timeout (e.g., WDTO_1S)
 */
#define PORT_WDT_ENABLE_INTERRUPT(timeout) \
  asm volatile (                                               \
      "in __tmp_reg__, __SREG__"  "\n\t"                       \
      "cli"                       "\n\t"                       \
      "wdr"                       "\n\t"                       \
      "sts %0, %1"                "\n\t"                       \
      "out __SREG__, __tmp_reg__" "\n\t"                       \
      "sts %0, %2"                "\n\t"                       \
      : /* no outputs */                                       \
      : "n" (_SFR_MEM_ADDR(PORT_WDT_CONTROL_REGISTER)),        \
      "r" ((uint8_t)(PORT_WDT_CHANGE_ENABLE_MASK |             \
          PORT_WDT_ENABLE_SYSTEM_RESET_MASK)),                 \
      "r" ((uint8_t) ((timeout & 0x08 ? (1<<WDP3) : 0x00) |    \
          PORT_WDT_ENABLE_INTERRUPT_MASK | (timeout & 0x07)) ) \
      : "r0"                                                   \
  );

/**
 * @brief Get task timeout in milliseconds from watchdog timeout
 * @param wdt_timeout Watchdog timeout enum value
 * @return Timeout in milliseconds
 */
#define PORT_GET_TASK_TIMEOUT(wdt_timeout) \
  ({                           \
    uint16_t timeout = 0;      \
    switch(wdt_timeout) {      \
      case WDTO_15MS:          \
        timeout = 15; break;   \
      case WDTO_30MS:          \
        timeout = 30; break;   \
      case WDTO_60MS:          \
        timeout = 60; break;   \
      case WDTO_120MS:         \
        timeout = 120; break;  \
      case WDTO_250MS:         \
        timeout = 250; break;  \
      case WDTO_500MS:         \
        timeout = 500; break;  \
      case WDTO_1S:            \
        timeout = 1000; break; \
      case WDTO_2S:            \
        timeout = 2000; break; \
    }; timeout;                \
  })

/*@}*/

/******************************************************************************/
/* Sleep Modes                                                                */
/******************************************************************************/

/**
 * @name Sleep Modes
 */
/*@{*/

/** @brief Sleep Mode Control Register */
#define PORT_SLEEP_MODE_CONTROL_REGISTER \
  SMCR

/** @brief Check if Idle sleep mode is active */
#define PORT_SLEEP_MODE_IS_IDLE() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0)

/** @brief Check if ADC Noise Reduction sleep mode is active */
#define PORT_SLEEP_MODE_IS_ADC_NOISE_RED() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0x1)

/** @brief Check if Power-down sleep mode is active */
#define PORT_SLEEP_MODE_IS_POWER_DOWN() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0x2)

/** @brief Check if Power-save sleep mode is active */
#define PORT_SLEEP_MODE_IS_POWER_SAVE() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0x3)

/** @brief Check if Standby sleep mode is active */
#define PORT_SLEEP_MODE_IS_STANDBY() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0x6)

/** @brief Check if Extended Standby sleep mode is active */
#define PORT_SLEEP_MODE_IS_EXTERNAL_STANDBY() \
  ((PORT_SLEEP_MODE_CONTROL_REGISTER & 0xe) == 0x7)

/*@}*/

/******************************************************************************/
/* Power Reduction                                                            */
/******************************************************************************/

/**
 * @name Power Reduction
 * Control peripheral power to reduce power consumption
 */
/*@{*/

/** @brief Power reduction for TWI */
#define PORT_SET_PWR_REDUCTION_TWI() \
  SET_BIT(PRR, PRTWI)

/** @brief Power reduction for Timer/Counter 0 */
#define PORT_SET_PWR_REDUCTION_TIMER0() \
  SET_BIT(PRR, PRTIM0)

/** @brief Power reduction for Timer/Counter 1 */
#define PORT_SET_PWR_REDUCTION_TIMER1() \
  SET_BIT(PRR, PRTIM1)

/** @brief Power reduction for Timer/Counter 2 */
#define PORT_SET_PWR_REDUCTION_TIMER2() \
  SET_BIT(PRR, PRTIM2)

/** @brief Power reduction for SPI */
#define PORT_SET_PWR_REDUCTION_SPI() \
  SET_BIT(PRR, PRSPI)

/** @brief Power reduction for USART0 */
#define PORT_SET_PWR_REDUCTION_USART0() \
  SET_BIT(PRR, PRUSART0)

/** @brief Power reduction for ADC */
#define PORT_SET_PWR_REDUCTION_ADC() \
  SET_BIT(PRR, PRADC)

/*@}*/

/******************************************************************************/
/* Trap / Error Handling                                                      */
/******************************************************************************/

/** @brief External Interrupt Mask Register for trap */
#define PORT_TRAP_INTERRUPT_MASK_REGISTER \
  EIMSK

/** @brief Trap interrupt mask (INT1) */
#define PORT_TRAP_INTERRUPT_MASK \
  (1<<INT1)

/** @brief Trap interrupt vector */
#define PORT_TRAP_VECT \
  INT1_vect

/** @brief Trap port (D) */
#define PORT_TRAP_PORT \
  D

/** @brief Trap pin (PD3/INT1) */
#define PORT_TRAP_PIN \
  3

/******************************************************************************/
/* EEPROM Error Storage                                                       */
/******************************************************************************/

/** @brief EEPROM address for error flag */
#define PORT_EEPROM_ERROR_FLAG_ADDR 0x3ff

/** @brief EEPROM address for error value */
#define PORT_EEPROM_ERROR_VALUE_ADDR 0x3fe

/** @brief EEPROM address for program counter high byte */
#define PORT_EEPROM_PC_VALUE_HIGH_ADDR 0x3fd

/** @brief EEPROM address for program counter low byte */
#define PORT_EEPROM_PC_VALUE_LOW_ADDR 0x3fc

#endif /* __AVR_ATmega328P__ */

#endif /* PORT_MEGA328P_H */
