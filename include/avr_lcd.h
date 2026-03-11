/**
 * @file avr_lcd.h
 * @brief Main public API header for avr-lcd LCD library
 * @author notweerdmonk
 * 
 * A lightweight, configurable C library for driving HD44780-compatible
 * character LCD displays on AVR microcontrollers.
 * 
 * Usage:
 * @code
 * #include <avr_lcd.h>
 * 
 * int main(void) {
 *     avr_lcd_set_pins(NULL);    // Use macro-defined pins
 *     avr_lcd_reset();
 *     avr_lcd_setup();
 *     
 *     avr_lcd_put_string("Hello, World!");
 *     avr_lcd_set_cursor(1, 0);
 *     avr_lcd_put_string("AVR LCD v1.0");
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
 * Runtime configuration example:
 * @code
 * #define AVR_LCD_RUNTIME_HARDWARE_REPR
 * #include <avr_lcd.h>
 * 
 * int main(void) {
 *     struct hardware_repr hw = {
 *         .mode = ABSOLUTE_PIN_NUMBERS | BUS_4BIT,
 *         .ctl.rs = 8,
 *         .ctl.en = 9,
 *         .ctl.rw = 255,
 *         .data.pins.d4 = 4,
 *         .data.pins.d5 = 5,
 *         .data.pins.d6 = 6,
 *         .data.pins.d7 = 7,
 *     };
 *     
 *     avr_lcd_set_pins(&hw);
 *     avr_lcd_reset();
 *     avr_lcd_setup(0, LCD_CMD_DISPLAY_ON, 0, LCD_CMD_4BIT_MODE | LCD_CMD_2LINE);
 *     avr_lcd_put_string("Runtime config!");
 *     while(1);
 *     return 0;
 * }
 * @endcode
 */

#ifndef _AVR_LCD_H_
#define _AVR_LCD_H_

#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <config.h>
#include <avr_io.h>
#include <avr_utility.h>
#include <avr_ascii.h>
#include <avr_lcd_config.h>
#include <hd44780_cmds.h>

/**
 * @brief Hardware representation mode flags for runtime configuration
 */
#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

/** @brief Hardware representation mode enumeration */
typedef enum avr_lcd_hw_repr_mode {
  MACRO_DEF                 = 0x00,  /**< Use macro-defined pins (default) */
  PORT_ADDR                 = 0x01,  /**< Use direct port addresses */
  RELATIVE_PIN_NUMBERS      = 0x02,  /**< Use relative port numbers (PORTA=0, PORTB=1, etc.) */
  ABSOLUTE_PIN_NUMBERS      = 0x04,  /**< Use Arduino-style absolute pin numbers */
  BUS_4BIT                  = 0x08,  /**< Use 4-bit bus mode */
  SERPARATE_PORTS           = 0x10,  /**< Use separate ports for control signals */
  NONCONTIGUOUS_DATA_PINS   = 0x20   /**< Data pins can be non-contiguous */
} avr_lcd_hw_repr_mode_t;

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

#include LCD_HEADER

/** @brief Hardware representation structure */
typedef struct hardware_repr avr_lcd_data_t;

/**
 * @brief Cursor position structure
 */
typedef struct cursor {
  uint8_t row;  /**< Current row position (0-indexed) */
  uint8_t col;  /**< Current column position (0-indexed) */
} cursor_t;

/**
 * @brief PWM callback function type for backlight control
 * @param value PWM value (0-255)
 */
typedef void (*avr_lcd_pwm_set_value_hook)(uint8_t value);

/**
 * @brief Set LCD pin configuration
 * @param p Pointer to hardware_repr structure.
 */
inline
void avr_lcd_set_pins(struct hardware_repr *p) {
  LCD_PREFIX(set_pins)(p);
}

/**
 * @brief Reset the LCD controller
 */
inline
void avr_lcd_reset() {
  LCD_PREFIX(reset)();
}

/**
 * @brief Setup LCD with runtime configuration
 */
inline
#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
void avr_lcd_setup(
    uint8_t entry_mode,
    uint8_t display,
    uint8_t cursor_shift,
    uint8_t function
  ) {
  LCD_PREFIX(setup)(entry_mode, display, cursor_shift, function);
#else
/**
 * @brief Setup LCD with compile-time configuration
 */
void avr_lcd_setup() {
  LCD_PREFIX(setup)();
#endif
}

/**
 * @brief Check if LCD is ready for new data
 * @return true if LCD is ready, false otherwise
 */
bool avr_lcd_ready();

/**
 * @brief Get current cursor position
 * @return cursor_t structure containing row and column
 */
struct cursor avr_lcd_get_cursor();

/**
 * @brief Set cursor position
 * @param row Row number (0-indexed)
 * @param col Column number (0-indexed)
 */
void avr_lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Clear the entire display
 */
void avr_lcd_clear();

/**
 * @brief Clear display from cursor position
 * @param n Number of characters to clear
 */
void avr_lcd_clear_till(uint8_t n);

/**
 * @brief Clear current line
 */
#define AVR_LCD_CLEAR_LINE() avr_lcd_clear_till(AVR_LCD_COLS - avr_lcd_get_cursor().row)

/**
 * @brief Write a single character to LCD
 * @param c Character to write
 */
void avr_lcd_put_char(char c);

/**
 * @brief Write a string to LCD
 * @param str Pointer to null-terminated string (stored in RAM)
 */
void avr_lcd_put_string(char *str);

/**
 * @brief Write a PROGMEM string to LCD
 * @param str Pointer to PROGMEM string
 */
void avr_lcd_put_pgm_string(PGM_P str);

/**
 * @brief Write unsigned integer to LCD
 * @param u Unsigned integer to display
 */
void avr_lcd_put_uint(unsigned int u);

/**
 * @brief Write signed integer to LCD
 * @param n Signed integer to display (supports negative values)
 */
void avr_lcd_put_int(int n);

/**
 * @brief Write floating-point number to LCD
 * @param f Float to display
 * @param m Number of decimal places (0-2 for AVR)
 */
void avr_lcd_put_float(float f, uint8_t m);

#ifdef AVR_LCD_BUFFERED

/**
 * @brief Flush buffer to LCD display
 */
void avr_lcd_display();

#endif /* AVR_LCD_BUFFERED */

/**
 * @brief Set PWM callback for backlight control
 * @param hook Function pointer to PWM setter callback
 * @return Previous callback function pointer
 */
avr_lcd_pwm_set_value_hook
avr_lcd_set_pwm_set_value_hook(avr_lcd_pwm_set_value_hook hook);

/**
 * @brief Set backlight brightness
 * @param value Brightness value (0-255)
 */
void avr_lcd_set_brightness(uint8_t value);

#endif /* _AVR_LCD_H_ */
