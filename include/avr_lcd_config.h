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
 * @file avr_lcd_config.h
 * @brief Configuration for AVR LCD library
 * @author notweerdmonk
 * 
 * This header provides compile-time configuration options for the AVR HD44780
 * LCD driver library. Feature flags are prefixed with AVR_ to avoid
 * clobbering user code.
 * 
 * @section config_options Configuration Options
 * 
 * - LCD Type: Select controller type
 * - Screen Dimensions: Rows and columns
 * - Data Bus: 4-bit or 8-bit mode
 * - Buffering: Enable/disable buffered mode
 * - Hardware Representation: Pin mapping modes
 * - Display Settings: Initial display state
 * - Backlight: PWM configuration
 */

#ifndef _AVR_LCD_CONFIG_H_
#define _AVR_LCD_CONFIG_H_

#define _PREFIX(p, x) p ## _ ## x
#define PREFIX(...) _PREFIX(__VA_ARGS__)
#define LCD_PREFIX(x) PREFIX(AVR_LCD_TYPE, x)

#define LCD_HEADER <AVR_LCD_TYPE.h>
#define LCD_SOURCE <AVR_LCD_TYPE.c>

/******************************************************************************/
/* LCD Type Configuration                                                     */
/******************************************************************************/

/**
 * @defgroup lcd_type LCD Controller Type
 * @{
 */

/**
 * @def AVR_LCD_TYPE
 * @brief LCD controller type
 * Currently only hd44780 is supported
 */
#define AVR_LCD_TYPE hd44780

/*@}*/

/******************************************************************************/
/* LCD Screen Dimensions                                                      */
/******************************************************************************/

/**
 * @defgroup lcd_dimensions Screen Dimensions
 * @{
 */

/**
 * @def AVR_LCD_ROWS
 * @brief Number of display rows
 */
#ifndef AVR_LCD_ROWS
#define AVR_LCD_ROWS 2
#endif

/**
 * @def AVR_LCD_COLS
 * @brief Number of display columns
 */
#ifndef AVR_LCD_COLS
#define AVR_LCD_COLS 16
#endif

/*@}*/

/******************************************************************************/
/* LCD Data Bus Configuration                                                 */
/******************************************************************************/

/**
 * @defgroup lcd_bus Data Bus Width
 * @{
 */

/**
 * @def AVR_LCD_4BIT
 * @brief Use 4-bit data bus mode
 * 
 * When defined, uses 4-bit mode (D4-D7). Saves 4 I/O pins.
 * When commented out, uses 8-bit mode.
 */
#define AVR_LCD_4BIT

/*@}*/

/******************************************************************************/
/* Buffered Mode                                                              */
/******************************************************************************/

/**
 * @defgroup lcd_buffer Buffered Operation
 * @{
 */

/**
 * @def AVR_LCD_BUFFERED
 * @brief Enable buffered write mode
 * 
 * When enabled:
 * - Writes go to RAM buffer instead of directly to LCD
 * - Non-blocking character writes
 * - Automatic dirty character tracking
 * - Call avr_lcd_display() to flush buffer to LCD
 * 
 * Disable for simpler code or when direct LCD control is needed.
 */
#define AVR_LCD_BUFFERED

/*@}*/

/******************************************************************************/
/* Hardware Representation Modes                                              */
/******************************************************************************/

/**
 * @defgroup lcd_hw_repr Hardware Representation
 * @{
 * 
 * Choose ONE mode that matches your hardware wiring:
 * - AVR_LCD_RUNTIME_HARDWARE_REPR
 * - AVR_LCD_USE_PORT_ADDR
 * - AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 * - AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 * 
 * Optional modifiers:
 * - AVR_LCD_USE_SEPARATE_PORTS
 * - AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 */

/**
 * @def AVR_LCD_RUNTIME_HARDWARE_REPR
 * @brief Enable runtime hardware configuration
 * 
 * Allows dynamic pin configuration at runtime via avr_lcd_set_pins() function.
 * Overrides any other hardware representation mode when enabled.
 */

/**
 * @def AVR_LCD_USE_PORT_ADDR
 * @brief Use direct port addresses
 * 
 * Use direct I/O register addresses for data and control pins.
 */

/**
 * @def AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 * @brief Use relative port numbers
 * 
 * Use port numbers: PORTA=0, PORTB=1, PORTC=2, PORTD=3, etc.
 */

/**
 * @def AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 * @brief Use absolute Arduino-style pin numbers
 * 
 * Use digital pin numbers (0-21 on ATmega328P).
 */

/**
 * @def AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 * @brief Allow non-contiguous data pins
 * 
 * Data pins can be spread across different ports. Can be combined with:
 * - AVR_LCD_USE_PORT_ADDR
 * - AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 * - AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 */

/**
 * @def AVR_LCD_USE_SEPARATE_PORTS
 * @brief Use separate ports for control signals
 * 
 * Control signals (RS, EN, RW, BL) can use different ports.
 * Should be combined with:
 * - AVR_LCD_USE_PORT_ADDR
 * - AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 */

/* To enable runtime configuration, uncomment: */
// #define AVR_LCD_RUNTIME_HARDWARE_REPR

/* To use port addresses, uncomment: */
// #define AVR_LCD_USE_PORT_ADDR

/* To use relative pin numbers, uncomment: */
// #define AVR_LCD_USE_RELATIVE_PIN_NUMBERS

/* To use absolute pin numbers, uncomment: */
// #define AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

/* To use separate ports for control signals, uncomment: */
// #define AVR_LCD_USE_SEPARATE_PORTS

/* To use non-contiguous data pins, uncomment: */
// #define AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

/*@}*/

/******************************************************************************/
/* Runtime Configuration Override                                             */
/******************************************************************************/

/**
 * @defgroup lcd_runtime_override Runtime Configuration
 * @{
 */

/**
 * @def AVR_LCD_RUNTIME_CONFIG
 * @brief Override hardware representation at runtime
 * 
 * When defined, automatically enables AVR_LCD_RUNTIME_HARDWARE_REPR
 */
#ifdef AVR_LCD_RUNTIME_CONFIG
#define AVR_LCD_RUNTIME_HARDWARE_REPR
#endif

/*@}*/

/******************************************************************************/
/* LCD Hardware Wiring (Macro-based mode)                                     */
/******************************************************************************/

/**
 * @defgroup lcd_wiring Hardware Wiring
 * @{
 * 
 * These define the default pin connections when not using runtime configuration.
 */

/** @brief Control signals port (B, C, D, etc.) */
#ifndef AVR_LCD_CTL_PORT
#define AVR_LCD_CTL_PORT B
#endif

/** @brief Data port */
#ifndef AVR_LCD_DATA_PORT
#ifdef AVR_LCD_SIMTEST
#define AVR_LCD_DATA_PORT B
#else
#define AVR_LCD_DATA_PORT D
#endif
#endif

/*@}*/

/******************************************************************************/
/* LCD Display Lines                                                          */
/******************************************************************************/

/**
 * @defgroup lcd_lines Display Line Mode
 * @{
 */

/**
 * @def AVR_LCD_1LINE
 * @brief Use 1-line display mode
 * 
 * Default is 2-line mode for displays with 2+ rows.
 */

/* To enable 1-line mode, uncomment: */
// #define AVR_LCD_1LINE

/*@}*/

/******************************************************************************/
/* LCD Font Size                                                              */
/******************************************************************************/

/**
 * @defgroup lcd_font Font Configuration
 * @{
 */

/**
 * @def AVR_LCD_5x10DOTS
 * @brief Use 5x10 dot font
 * 
 * Only valid for 1-line display mode.
 * Default is 5x8 dots.
 */

/* To enable 5x10 dot font, uncomment: */
// #define AVR_LCD_5x10DOTS

/*@}*/

/******************************************************************************/
/* LCD Display Configuration                                                  */
/******************************************************************************/

/**
 * @defgroup lcd_display Display Settings
 * @{
 * 
 * These control the initial display state.
 */

/**
 * @def AVR_LCD_DISPLAY_OFF
 * @brief Start with display off
 */

/* To start with display off, uncomment: */
// #define AVR_LCD_DISPLAY_OFF

/**
 * @def AVR_LCD_CURSOR_ON
 * @brief Enable cursor display
 */

/* To enable cursor, uncomment: */
// #define AVR_LCD_CURSOR_ON

/**
 * @def AVR_LCD_BLINK_ON
 * @brief Enable cursor blinking
 */

/* To enable blink, uncomment: */
// #define AVR_LCD_BLINK_ON

/**
 * @def AVR_LCD_ENTRY_DECR
 * @brief Decrement cursor position
 * 
 * Default is increment (left to right).
 */

/* To decrement cursor position, uncomment: */
// #define AVR_LCD_ENTRY_DECR

/**
 * @def AVR_LCD_ENTRY_SHIFT
 * @brief Shift display with cursor
 */

/* To shift display with cursor, uncomment: */
// #define AVR_LCD_ENTRY_SHIFT

/**
 * @def AVR_LCD_CURSOR_MOVE_LEFT
 * @brief Make cursor move left on character write
 */

/* To make cursor move left, uncomment: */
// #define AVR_LCD_CURSOR_MOVE_LEFT

/**
 * @def AVR_LCD_DISPLAY_MOVE_LEFT
 * @brief Make display shift left on character write
 */

/* To make display shift left, uncomment: */
// #define AVR_LCD_DISPLAY_MOVE_LEFT

/*@}*/

/******************************************************************************/
/* Example Configurations                                                     */
/******************************************************************************/

/**
 * @defgroup lcd_examples Example Configurations
 * @{
 * 
 * @subsection ex1 Example 1: Typical 16x2 LCD, 4-bit mode (default)
 * @code
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_4BIT
 * #define AVR_LCD_BUFFERED
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 * @endcode
 * 
 * @subsection ex2 Example 2: 20x4 LCD, absolute pin numbers
 * @code
 * #define AVR_LCD_ROWS 4
 * #define AVR_LCD_COLS 20
 * #define AVR_LCD_4BIT
 * #define AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 * #define AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT B
 * @endcode
 * 
 * @subsection ex3 Example 3: Runtime configuration
 * @code
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_RUNTIME_HARDWARE_REPR
 * // Then call avr_lcd_set_pins() in main() before avr_lcd_reset()
 * @endcode
 * 
 * @subsection ex4 Example 4: Minimal unbuffered, no cursor
 * @code
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #undef AVR_LCD_BUFFERED  // Disable buffered mode
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 * @endcode
 * 
 * @subsection ex5 Example 5: 1-line display with 5x10 font
 * @code
 * #define AVR_LCD_ROWS 1
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_1LINE
 * #define AVR_LCD_5x10DOTS
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 * @endcode
 */

/*@}*/

#endif /* _AVR_LCD_CONFIG_H_ */
