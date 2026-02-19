/**
 * @file lcd.h
 * @author notweerdmonk
 * @brief header file for lcd module
 */

#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <config.h>
#include <io.h>
#include <utility.h>
#include <ascii.h>
#include <lcd_config.h>
#include <hd44780_cmds.h>

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

typedef enum lcd_hw_repr {
  /* Mutually exclusive bits */
  MACRO_DEF                 = 0x00,
  PORT_ADDR                 = 0x01,
  RELATIVE_PIN_NUMBERS      = 0x02,
  ABSOLUTE_PIN_NUMBERS      = 0x04,
  BUS_4BIT                  = 0x08,
  SERPARATE_PORTS           = 0x10,
  NONCONTIGUOUS_DATA_PINS   = 0x20
} lcd_hw_repr_t;

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

#define _PREFIX(p, x) p ## _ ## x
#define PREFIX(...) _PREFIX(__VA_ARGS__)
#define LCD_PREFIX(x) PREFIX(AVR_LCD_TYPE, x)

#define LCD_HEADER <AVR_LCD_TYPE.h>

#include LCD_HEADER

typedef struct hardware_repr lcd_data_t;

typedef struct cursor {
  uint8_t row;
  uint8_t col;
} cursor_t;

inline
void lcd_set_pins(struct hardware_repr *p) {
  LCD_PREFIX(set_pins)(p);
}

inline
void lcd_set_brightness(uint8_t value) {
  LCD_PREFIX(set_brightness)(value);
}

inline
void lcd_reset() {
  LCD_PREFIX(reset)();
}

inline
#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
void lcd_setup(
    uint8_t entry_mode,
    uint8_t display,
    uint8_t cursor_shift,
    uint8_t function
  ) {
  LCD_PREFIX(setup)(entry_mode, display, cursor_shift, function);
#else
void lcd_setup() {
  LCD_PREFIX(setup)();
#endif
}

//event_id_t get_lcd_ready_event();

bool lcd_ready();

struct cursor lcd_get_cursor();

void lcd_set_cursor(uint8_t row, uint8_t col);

void lcd_clear();

void lcd_clear_till(uint8_t n);

#define LCD_CLEAR_LINE() lcd_clear_till(AVR_LCD_COLS - lcd_get_cursor().row)

void lcd_put_char(char c);

void lcd_put_string(char *str);

void lcd_put_pgm_string(PGM_P str);

void lcd_put_uint(unsigned int u);

void lcd_put_int(int n);

void lcd_put_float(float f, uint8_t m);

#ifdef AVR_LCD_BUFFERED

void lcd_display();

void lcd_force_display();

#endif /* AVR_LCD_BUFFERED */

//void* lcd_task(task_data_t data);

#endif /* _LCD_H_ */
