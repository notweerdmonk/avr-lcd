/**
 * @file avr_lcd.h
 * @author notweerdmonk
 * @brief header file for lcd module
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

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

typedef enum avr_lcd_hw_repr_mode {
  /* Mutually exclusive bits */
  MACRO_DEF                 = 0x00,
  PORT_ADDR                 = 0x01,
  RELATIVE_PIN_NUMBERS      = 0x02,
  ABSOLUTE_PIN_NUMBERS      = 0x04,
  BUS_4BIT                  = 0x08,
  SERPARATE_PORTS           = 0x10,
  NONCONTIGUOUS_DATA_PINS   = 0x20
} avr_lcd_hw_repr_mode_t;

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

#include LCD_HEADER

typedef struct hardware_repr avr_lcd_data_t;

typedef struct cursor {
  uint8_t row;
  uint8_t col;
} cursor_t;

typedef void (*avr_lcd_pwm_set_value_hook)(uint16_t value);

inline
void avr_lcd_set_pins(struct hardware_repr *p) {
  LCD_PREFIX(set_pins)(p);
}

inline
void avr_lcd_reset() {
  LCD_PREFIX(reset)();
}

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
void avr_lcd_setup() {
  LCD_PREFIX(setup)();
#endif
}

bool avr_lcd_ready();

struct cursor avr_lcd_get_cursor();

void avr_lcd_set_cursor(uint8_t row, uint8_t col);

void avr_lcd_clear();

void avr_lcd_clear_till(uint8_t n);

#define AVR_LCD_CLEAR_LINE() avr_lcd_clear_till(AVR_LCD_COLS - avr_lcd_get_cursor().row)

void avr_lcd_put_char(char c);

void avr_lcd_put_string(char *str);

void avr_lcd_put_pgm_string(PGM_P str);

void avr_lcd_put_uint(unsigned int u);

void avr_lcd_put_int(int n);

void avr_lcd_put_float(float f, uint8_t m);

#ifdef AVR_LCD_BUFFERED

void avr_lcd_display();

#endif /* AVR_LCD_BUFFERED */

avr_lcd_pwm_set_value_hook
avr_lcd_set_pwm_set_value_hook(avr_lcd_pwm_set_value_hook hook);

void avr_lcd_set_brightness(uint8_t value);

#endif /* _AVR_LCD_H_ */
