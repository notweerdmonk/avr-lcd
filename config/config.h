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
 * @file config.h
 * @author notweerdmonk
 * @brief avr-lcd configuration header
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* Versioning */
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define REVISION      0

/* F_CPU */
/* !!! IMPORTANT !!! always define F_CPU before including any AVR headers */
#ifndef F_CPU
#error *** F_CPU not defined! ***
#endif

/******************************************************************************/
/* Project Configuration Flags */
/******************************************************************************/

/**
 * @defgroup Project configuration
 * @{
 */

/**
 * @def AVR_LCD_RUNTIME_CONFIG
 * @brief
 * Enable runtime hardware representation selection (call lcd_set_pins with
 * config). When enabled, overrides any other hardware representation mode.
 */
// #define AVR_LCD_RUNTIME_CONFIG

/**
 * @def AVR_LCD_UART_IOSTREAM
 * @brief
 * Enable use of UART in I/O streams (stdin/stdout/stderr)
 */
// #define AVR_LCD_UART_IOSTREAM

/**
 * @def AVR_LCD_UART_MATCH
 * @brief
 * Enable UART input pattern match
 */
// #define AVR_LCD_UART_MATCH

/**
 * @def AVR_LCD_STRNCMP_MATCH
 * @brief
 * Use strncmp for pattern matching
 */
// #define AVR_LCD_STRNCMP_MATCH

/**
 * @def AVR_LCD_EMIT_TRIGGER
 * @brief
 * Emit a trigger signal that can be used by logic analyser to start capture
 */
// #define AVR_LCD_EMIT_TRIGGER

/**
 * @def AVR_LCD_SIMULATION
 * @brief
 * SIM denotes that source code will compiled for simulation
 */
// #define AVR_LCD_SIMULATION

/**
 * @def AVR_LCD_SIMTEST
 * @brief
 * Denotes that source code will be compiled for off-target testing
 */
// #define AVR_LCD_SIMTEST

/**
 * @def AVR_LCD_DEMO
 * @brief
 * Demo mode with serial communication program
 */
// #define AVR_LCD_DEMO

/**
 * @def AVR_LCD_DEBUG
 * @brief
 * Enable debug build with debugging information and symbols
 */
// #define AVR_LCD_DEBUG

/**
 * @def AVR_LCD_SAVE_TEMPS
 * @brief
 * Preserve compilation intermediaries
 */
// #define AVR_LCD_SAVE_TEMPS

/**
 * @def AVR_LCD_OPTIM
 * @brief
 * Set the compiler optimization level (-O0, -O1, -O2, -O3, -Os, etc.)
 */
// #define AVR_LCD_OPTIM -O2

/*@}*/

#endif /* CONFIG_H_ */
