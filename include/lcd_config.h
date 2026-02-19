/*
 * @file lcd_config.h
 * @author notweerdmonk
 * @brief configuration for lcd module
 */

#ifndef _AVR_LCD_CONFIG_H_
#define _AVR_LCD_CONFIG_H_

/*****************************************************************************/
/* LCD Type
 *
 * - hd44780
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_TYPE hd44780
/*****************************************************************************/

/*****************************************************************************/
/*
 * LCD data bus width selection
 *  - LCD_4BIT : Use 4 bit bus or else use 8 bit bus.
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_4BIT
/*****************************************************************************/

/*****************************************************************************/
/* Buffered mode
 *  - LCD_BUFFERED : Writes to RAM buffer and updates the LCD in separate cycle
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_BUFFERED
/*****************************************************************************/

/*****************************************************************************/
/*
 * LCD screen dimensions
 *  - LCD_ROWS
 *  - LCD_COLS
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_ROWS 2
#define LCD_COLS 16
/*****************************************************************************/

/*****************************************************************************/
/*
 * LCD hardware representation modes selection
 *  - LCD_RUNTIME_HW_REPR_SEL
 *
 *  - LCD_USE_PORT_ADDR
 *  - LCD_USE_RELATIVE_PIN_NUMBERS
 *    - LCD_USE_SEPARATE_PORTS
 *  - LCD_USE_ABSOLUTE_PIN_NUMBERS
 *
 *  - LCD_USE_NONCONTIGUOUS_DATA_PINS
 */
/*****************************************************************************/
/* Define your selection here */

/*****************************************************************************/

/*****************************************************************************/
/*
 * LCD hardware wiring
 *  - LCD_CTL_PORT
 *  - LCD_DATA_PORT
 */
/*****************************************************************************/
#ifndef LCD_CTL_PORT
#define LCD_CTL_PORT B
#endif

#ifndef LCD_DATA_PORT
#ifdef AVR_UART_SIMTEST
#define LCD_DATA_PORT B
#else
#define LCD_DATA_PORT D
#endif
#endif
/*****************************************************************************/


/*****************************************************************************/
/*
 * LCD backlight PWN channel
 *  - LCD_PWM_CHANNEL
 *  - LCD_BL_VALUE
 */
/*****************************************************************************/
#ifndef LCD_PWM_CHANNEL
#define LCD_PWM_CHANNEL PWM_CHANNEL_A
#endif

#ifndef LCD_BL_VALUE
#define LCD_BL_VALUE 150
#endif
/*****************************************************************************/

/*****************************************************************************/
/*
 * Instructions
 *  - LCD_DISPLAY_OFF
 *  - LCD_CURSOR_ON
 *  - LCD_BLINK_ON
 *  - LCD_ENTRY_DECR
 *  - LCD_ENTRY_SHIFT
 *  - LCD_CURSOR_MOVE_LEFT
 *  - LCD_DISPLAY_MOVE_LEFT
 */
/*****************************************************************************/
/* Define your selection here */

/*****************************************************************************/

#endif /* _AVR_LCD_CONFIG_H_ */
