/*
 * @file lcd_config.h
 * @author notweerdmonk
 * @brief Configuration for LCD module
 *
 * Feature flags are prefixed with AVR_ to avoid clobbering user code.
 * Uncomment or define flags as needed for your hardware configuration.
 */

#ifndef _AVR_LCD_CONFIG_H_
#define _AVR_LCD_CONFIG_H_

/******************************************************************************/
/*
 * LCD Type
 *
 * Specifies the LCD controller type. Currently only hd44780 is supported.
 */
/******************************************************************************/
#define AVR_LCD_TYPE hd44780

/******************************************************************************/
/*
 * LCD Screen Dimensions
 *
 * - AVR_LCD_ROWS : Number of rows (default: 2)
 * - AVR_LCD_COLS : Number of columns (default: 16)
 */
/******************************************************************************/
#ifndef AVR_LCD_ROWS
#define AVR_LCD_ROWS 2
#endif

#ifndef AVR_LCD_COLS
#define AVR_LCD_COLS 16
#endif

/******************************************************************************/
/*
 * LCD Data Bus Width
 *
 * - AVR_LCD_4BIT : Use 4-bit data bus (saves 4 pins)
 *
 * Note: Define AVR_LCD_4BIT to enable 4-bit mode. Otherwise uses 8-bit.
 */
/******************************************************************************/
#define AVR_LCD_4BIT

/******************************************************************************/
/*
 * Buffered Mode
 *
 * - AVR_LCD_BUFFERED : Writes to RAM buffer and updates LCD in separate
 *                     cycle
 *
 * Benefits:
 *   - Faster character writes (no blocking LCD wait)
 *   - Automatic dirty character tracking
 *   - Call lcd_display() to flush buffer to LCD
 *
 * Disable for simpler code or if direct LCD control is needed.
 */
/******************************************************************************/
#define AVR_LCD_BUFFERED

/******************************************************************************/
/*
 * LCD Hardware Representation Mode
 *
 * Choose ONE mode that matches your wiring:
 *   - AVR_LCD_RUNTIME_HARDWARE_REPR
 *   - AVR_LCD_USE_PORT_ADDR
 *     - AVR_LCD_USE_SEPARATE_PORTS
 *   - AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 *     - AVR_LCD_USE_SEPARATE_PORTS
 *   - AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 *   - AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 *
 * - AVR_LCD_RUNTIME_HARDWARE_REPR
 *   Runtime hardware representation selection. Allows dynamic pin
 *   configuration at runtime via lcd_set_pins() function.
 *
 * - AVR_LCD_USE_PORT_ADDR
 *   Use direct port addresses for data/control pins.
 *
 * - AVR_LCD_USE_RELATIVE_PIN_NUMBERS
 *   Use relative port numbers (PORTA=0, PORTB=1, PORTC=2, etc.)
 *
 * - AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 *   Use absolute Arduino-style pin numbers (digital pins 0-21 on
 *   ATmega328P)
 *
 * - AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 *   Data pins can be non-contiguous (sparse). Useful for spread-out
 *   wiring.
 *
 * - AVR_LCD_USE_SEPARATE_PORTS
 *   Control signals (RS, EN, RW, BL) can use separate ports.
 *   Should be combined with AVR_LCD_USE_PORT_ADDR or
 *   AVR_LCD_USE_RELATIVE_PIN_NUMBERS. Has no effect with other options.
 */
/******************************************************************************/

/* To enable runtime configuration, uncomment the following line: */
// #define AVR_LCD_RUNTIME_HARDWARE_REPR

/* To use port addresses, uncomment the following line: */
// #define AVR_LCD_USE_PORT_ADDR

/* To use relative pin numbers, uncomment the following line: */
// #define AVR_LCD_USE_RELATIVE_PIN_NUMBERS

/* To use absolute pin numbers, uncomment the following line: */
// #define AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

/* To use non-contiguous data pins, uncomment the following line: */
// #define AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

/* To use separate ports for control signals, uncomment the following line: */
// #define AVR_LCD_USE_SEPARATE_PORTS

/******************************************************************************/
/*
 * AVR_LCD_RUNTIME_CONFIG can override any hardware representation choice
 * by setting AVR_LCD_RUNTIME_HARDWARE_REPR
 */
/******************************************************************************/
#ifdef AVR_LCD_RUNTIME_CONFIG
#define AVR_LCD_RUNTIME_HARDWARE_REPR
#endif

/******************************************************************************/
/*
 * LCD Hardware Wiring (for macro-based mode)
 *
 * These are used when not using runtime configuration.
 */
/******************************************************************************/
#ifndef AVR_LCD_CTL_PORT
#define AVR_LCD_CTL_PORT B
#endif

#ifndef AVR_LCD_DATA_PORT
#ifdef AVR_LCD_SIMTEST
#define AVR_LCD_DATA_PORT B
#else
#define AVR_LCD_DATA_PORT D
#endif
#endif

/******************************************************************************/
/*
 * LCD Display Lines
 *
 * - AVR_LCD_1LINE : Use 1-line display mode
 * - AVR_LCD_2LINE : Use 2-line display mode (default for 2+ rows)
 *
 * Note: Only needed for special displays.
 *
 */
/******************************************************************************/
/* To enable 1-line mode, uncomment the following line: */
// #define AVR_LCD_1LINE

/******************************************************************************/
/*
 * LCD Font Size
 *
 * - AVR_LCD_5x10DOTS : Use 5x10 dot font (only valid for 1-line mode)
 * - AVR_LCD_5x8DOTS  : Use 5x8 dot font (default)
 *
 * Note: 5x10 dots only available in 1-line mode.
 */
/******************************************************************************/
/* To enable 5x10 dot font, uncomment the following line: */
// #define AVR_LCD_5x10DOTS

/******************************************************************************/
/*
 * LCD Display Configuration
 *
 * These flags control the initial display state.
 */
/******************************************************************************/
/* To start with display off, uncomment the following line: */
// #define AVR_LCD_DISPLAY_OFF

/* To enable cursor, uncomment the following line: */
// #define AVR_LCD_CURSOR_ON

/* To enable blink, uncomment the following line: */
// #define AVR_LCD_BLINK_ON

/* To decrement cursor position, uncomment the following line: */
// #define AVR_LCD_ENTRY_DECR

/* To shift display with cursor, uncomment the following line: */
// #define AVR_LCD_ENTRY_SHIFT

/* To make cursor move left, uncomment the following line: */
// #define AVR_LCD_CURSOR_MOVE_LEFT

/* To make display shift left, uncomment the following line: */
// #define AVR_LCD_DISPLAY_MOVE_LEFT

/******************************************************************************/
/*
 * LCD Backlight PWM
 *
 * - AVR_LCD_PWM_CHANNEL : PWM channel for backlight control
 * - AVR_LCD_BL_VALUE    : Backlight brightness (0-255)
 */
/******************************************************************************/
#ifndef AVR_LCD_PWM_CHANNEL
#define AVR_LCD_PWM_CHANNEL PWM_CHANNEL_A
#endif

#ifndef AVR_LCD_BL_VALUE
#define AVR_LCD_BL_VALUE 150
#endif

/******************************************************************************/
/*
 * Example Configurations
 *
 * ========================================================================
 * Example 1: Typical 16x2 LCD, 4-bit mode, macros (default)
 * ========================================================================
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_4BIT
 * #define AVR_LCD_BUFFERED
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 *
 * ========================================================================
 * Example 2: 20x4 LCD, 4-bit mode, absolute pin numbers
 * ========================================================================
 * #define AVR_LCD_ROWS 4
 * #define AVR_LCD_COLS 20
 * #define AVR_LCD_4BIT
 * #define AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
 * #define AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT B
 *
 * ========================================================================
 * Example 3: Runtime configuration (set pins at runtime)
 * ========================================================================
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_RUNTIME_HARDWARE_REPR
 * // Then call lcd_set_pins() in main() before lcd_reset()
 *
 * ========================================================================
 * Example 4: Minimal unbuffered, 4-bit, no cursor
 * ========================================================================
 * #define AVR_LCD_ROWS 2
 * #define AVR_LCD_COLS 16
 * #undef AVR_LCD_BUFFERED  // Disable buffered mode
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 *
 * ========================================================================
 * Example 5: 1-line display with 5x10 font
 * ========================================================================
 * #define AVR_LCD_ROWS 1
 * #define AVR_LCD_COLS 16
 * #define AVR_LCD_1LINE
 * #define AVR_LCD_5x10DOTS
 * #define AVR_LCD_CTL_PORT B
 * #define AVR_LCD_DATA_PORT D
 *
 */
/******************************************************************************/

#endif /* _AVR_LCD_CONFIG_H_ */
