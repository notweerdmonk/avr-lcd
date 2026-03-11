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
 * @mainpage avr-lcd
 * 
 * @brief LCD library documentation for AVR microcontrollers
 * 
 * @section overview Overview
 * 
 * This is the API documentation for the avr-lcd library. For usage
 * information and detailed descriptions, see the README.md file.
 * 
 * @section features Features
 * 
 * - HD44780 LCD controller support
 * - 4-bit and 8-bit data bus modes
 * - Buffered and unbuffered operation
 * - Multiple hardware representation modes
 * - Runtime pin configuration option
 * - Minimal RAM footprint
 * - Simulation support via simavr
 * 
 * @section quick_start Quick Start
 * 
 * @subsection basic Basic Usage
 * 
 * @code
 * #include <avr_lcd.h>
 * 
 * int main(void) {
 *     avr_lcd_set_pins(NULL);
 *     avr_lcd_reset();
 *     avr_lcd_setup();
 *     
 *     avr_lcd_put_string("Hello, World!");
 *     
 * #ifdef AVR_LCD_BUFFERED
 *     avr_lcd_display();
 * #endif
 *     
 *     while(1);
 *     return 0;
 * }
 * @endcode
 * 
 * @subsection wiring Typical Wiring (4-bit mode)
 * 
 * @code
 * ATmega328P         HD44780 LCD
 * --------           ------------
 * PB0 (D4)    -->    D4
 * PB1 (D5)    -->    D5
 * PB2 (D6)    -->    D6
 * PB3 (D7)    -->    D7
 * PB4 (RS)    -->    RS
 * PB5 (EN)    -->    EN
 * @endcode
 * 
 * @section configuration Configuration
 * 
 * The library can be configured via compile-time flags in
 * - @ref avr_lcd_config.h "include/avr_lcd_config.h" - LCD feature flags
 * - @ref config.h "config/config.h" - Project compilation flags
 * 
 * @section build Building
 * 
 * @code
 * make             # Build library and tests
 * make build-lib   # Build library only
 * make sim         # Run with simavr
 * @endcode
 * 
 * @section api API Reference
 * 
 * See @ref avr_lcd.h for the complete API documentation.
 * 
 * @section files Source Files
 *
 * @section license License
 * 
 * MIT License - see LICENSE file for details.
 * 
 * @author notweerdmonk
 * @version 1.0.0
 * @date 2026
 */
