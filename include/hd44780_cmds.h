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
 * @file hd44780_cmds.h
 * @brief HD44780 LCD controller command definitions
 * @author notweerdmonk
 * 
 * This file provides command constants for configuring and controlling
 * the HD44780-compatible LCD display.
 */

#ifndef _AVR_LCD_HD44780_COMMANDS_H_
#define _AVR_LCD_HD44780_COMMANDS_H_

/**
 * @name LCD Commands
 * Basic command flags for HD44780 controller
 */
/*@{*/

/**
 * @brief Clear display command
 * 
 * Clears all display data and returns cursor to home position (0,0).
 * Requires 1.52ms to execute.
 */
#define LCD_CMD_CLEAR_DISPLAY      0x01

/**
 * @brief Return home command
 * 
 * Returns cursor to home position (0,0). Display data is unchanged.
 * Requires 1.52ms to execute.
 */
#define LCD_CMD_RETURN_HOME        0x02

/**
 * @brief Entry mode set command
 * 
 * Sets cursor move direction and display shift mode.
 * Combine with LCD_CMD_ENTRY_INCR or LCD_CMD_ENTRY_DECR and
 * LCD_CMD_ENTRY_SHIFT_ON or LCD_CMD_ENTRY_SHIFT_OFF.
 */
#define LCD_CMD_ENTRY_MODE_SET     0x04

/**
 * @brief Display on/off control command
 * 
 * Controls display, cursor, and cursor blink.
 * Combine with LCD_CMD_DISPLAY_ON/OFF, LCD_CMD_CURSOR_ON/OFF,
 * LCD_CMD_BLINK_ON/OFF.
 */
#define LCD_CMD_DISPLAY_CONTROL    0x08

/**
 * @brief Cursor or display shift command
 * 
 * Moves cursor or shifts display without changing DDRAM content.
 * Combine with LCD_CMD_CURSOR_MOVE or LCD_CMD_DISPLAY_MOVE and
 * LCD_CMD_MOVE_RIGHT or LCD_CMD_MOVE_LEFT.
 */
#define LCD_CMD_CURSOR_SHIFT       0x10

/**
 * @brief Function set command
 * 
 * Sets interface data length, number of display lines, and font.
 * Combine with LCD_CMD_8BIT_MODE or LCD_CMD_4BIT_MODE,
 * LCD_CMD_2LINE or LCD_CMD_1LINE, LCD_CMD_5x10DOTS or LCD_CMD_5x8DOTS.
 */
#define LCD_CMD_FUNCTION_SET       0x20

/**
 * @brief Set CGRAM address command
 * 
 * Sets CGRAM (Character Generator RAM) address.
 * Follow with data to write to character generator RAM.
 */
#define LCD_CMD_SET_CGRAMADDR      0x40

/**
 * @brief Set DDRAM address command
 * 
 * Sets DDRAM (Display Data RAM) address.
 * Follow with data to write to display.
 */
#define LCD_CMD_SET_DDRAMADDR      0x80

/*@}*/

/**
 * @name Function Set Flags
 * Flags for display function configuration
 */
/*@{*/

/**
 * @brief 8-bit data interface mode
 */
#define LCD_CMD_8BIT_MODE          0x10

/**
 * @brief 4-bit data interface mode
 * 
 * @note Uses only D4-D7, saving 4 I/O pins
 */
#define LCD_CMD_4BIT_MODE          0x00

/**
 * @brief 2-line display mode
 */
#define LCD_CMD_2LINE              0x08

/**
 * @brief 1-line display mode
 */
#define LCD_CMD_1LINE              0x00

/**
 * @brief 5x10 dot character font
 * 
 * @note Only available in 1-line mode
 */
#define LCD_CMD_5x10DOTS           0x04

/**
 * @brief 5x8 dot character font (default)
 */
#define LCD_CMD_5x8DOTS            0x00

/*@}*/

/**
 * @name Display Control Flags
 * Flags for display, cursor, and blink control
 */
/*@{*/

/**
 * @brief Turn display on
 */
#define LCD_CMD_DISPLAY_ON         0x04

/**
 * @brief Turn display off
 */
#define LCD_CMD_DISPLAY_OFF        0x00

/**
 * @brief Show cursor
 */
#define LCD_CMD_CURSOR_ON          0x02

/**
 * @brief Hide cursor
 */
#define LCD_CMD_CURSOR_OFF         0x00

/**
 * @brief Enable cursor blinking
 */
#define LCD_CMD_BLINK_ON           0x01

/**
 * @brief Disable cursor blinking
 */
#define LCD_CMD_BLINK_OFF          0x00

/*@}*/

/**
 * @name Cursor/Display Shift Flags
 * Flags for cursor movement and display shift
 */
/*@{*/

/**
 * @brief Shift display (instead of cursor)
 */
#define LCD_CMD_DISPLAY_MOVE       0x08

/**
 * @brief Move cursor (instead of display)
 */
#define LCD_CMD_CURSOR_MOVE        0x00

/**
 * @brief Shift right
 */
#define LCD_CMD_MOVE_RIGHT         0x04

/**
 * @brief Shift left
 */
#define LCD_CMD_MOVE_LEFT          0x00

/*@}*/

/**
 * @name Entry Mode Flags
 * Flags for text entry direction
 */
/*@{*/

/**
 * @brief Increment cursor after character write
 * 
 * Cursor moves right after each character.
 */
#define LCD_CMD_ENTRY_INCR         0x02

/**
 * @brief Decrement cursor after character write
 * 
 * Cursor moves left after each character.
 */
#define LCD_CMD_ENTRY_DECR         0x00

/**
 * @brief Enable display shift
 * 
 * Display shifts with each character write.
 */
#define LCD_CMD_ENTRY_SHIFT_ON     0x01

/**
 * @brief Disable display shift
 * 
 * Display does not shift (cursor only moves).
 */
#define LCD_CMD_ENTRY_SHIFT_OFF    0x00

/*@}*/

/**
 * @name DDRAM Address Constants
 * Starting addresses for each display line
 */
/*@{*/

/**
 * @brief DDRAM address for line 1 (first line)
 */
#define LCD_CMD_DDRAMADDR_LINE1    0x00

/**
 * @brief DDRAM address for line 2 (second line)
 */
#define LCD_CMD_DDRAMADDR_LINE2    0x40

/*@}*/

#endif /* _AVR_LCD_HD44780_COMMANDS_H_ */
