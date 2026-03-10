/*
 * @file avr_lcd.c
 * @author notweerdmonk
 * #brief HD44780 LCD module driver
 */

#include <avr/io.h>
#include <util/delay.h>

#include <avr_lcd.h>
#include <port.h>

/*****************************************************************************/

/*
 * Works using busy wait method
 * 
 * TODO: add busy poll method
 */

/*****************************************************************************/

typedef struct char_buffer {
  char c;
  bool is_dirty;
} char_buffer_t;

typedef struct screen_buffer {
  struct char_buffer buffer[AVR_LCD_ROWS][AVR_LCD_COLS];
} screen_buffer_t;

typedef struct avr_lcd {
  struct screen_buffer screen;
  struct cursor cursor;

  avr_lcd_pwm_set_value_hook pwm_value_hook;

  struct hardware_repr hw; /* Hardware representation */

  bool clr : 1;
  bool upt : 1;
  bool rdy : 1;
  bool f0  : 1;
  bool f1  : 1;
  bool f2  : 1;
  bool f3  : 1;
  bool f4  : 1;

} avr_lcd_t;


static struct avr_lcd lcd;

/*****************************************************************************/

/* 
 * NOTE:
 * Always include hardware specific source after definition of struct lcd.
 */
#include LCD_SOURCE

/*****************************************************************************/

#ifdef AVR_LCD_BUFFERED
#define SCREEN(x, y) screen.buffer[y][x]
#endif

#define CURX cursor.col
#define CURY cursor.row

void avr_lcd_put_char(char c) {
  if ((lcd.CURY >= AVR_LCD_ROWS)) {
    return;
  }

#ifdef AVR_LCD_BUFFERED

  char_buffer_t *p_pixel = &(lcd.SCREEN(lcd.CURX, lcd.CURY));
  if ((p_pixel->c != c) || lcd.clr){
    p_pixel->c = c;
    p_pixel->is_dirty = true;
  }

  lcd.upt = true;

#else

 LCD_PREFIX(put_char)(c);

#endif

  ++lcd.CURX;
  if (lcd.CURX >= AVR_LCD_COLS) {
    lcd.CURY = (lcd.CURY + 1) % 2;
    lcd.CURX = 0;
  }
}

void avr_lcd_put_string(char *str) {
  while(*str) avr_lcd_put_char(*str++);
}

void avr_lcd_put_pgm_string(PGM_P s) {
  for (char c = pgm_read_byte(s); c != 0; c = pgm_read_byte(++s)) {
    avr_lcd_put_char(c);
  }
}

void avr_lcd_put_uint(unsigned int u) {
#if (__SIZEOF_INT__ == 1)
  uint8_t digits[4];
#elif (__SIZEOF_INT__ == 2)
  uint8_t digits[6];
#endif
  uint8_t idx = 0;

  if (u == 0) {
    digits[idx++] = 0;
  } else {
    while (u > 0) {
      digits[idx++] = u % 10;
      u /= 10;
    }
  }

  while (idx) {
    avr_lcd_put_char(ascii(digits[--idx]));
  }
}

void avr_lcd_put_int(int n) {
  if(n < 0) {
    avr_lcd_put_char('-');
    n = -n;
  }

  avr_lcd_put_uint(n);
}

/* TODO: sprintf with -lprintf_flt or dtostrf, but bigger size */
void avr_lcd_put_float(float f, uint8_t m) {
  /* Fractional part greater than 4 does not work */
  if (m > 4) m = 4;

  avr_lcd_put_int(f);
  avr_lcd_put_char('.');

  f = f - (int)f;
  for (uint8_t i = 0; i < m; i++) {
    f = f * 10;
  }

  avr_lcd_put_int(f);
}

bool avr_lcd_ready() {
  return (bool)lcd.rdy;
}

cursor_t avr_lcd_get_cursor() {
  return lcd.cursor;
}

void avr_lcd_set_cursor(uint8_t row, uint8_t col) {
  if ((row < AVR_LCD_ROWS) && (col < AVR_LCD_COLS)) {
    lcd.CURY = row;
    lcd.CURX = col;
  }

#ifndef AVR_LCD_BUFFERED
  LCD_PREFIX(set_cursor)(row, col);
#endif
}

void avr_lcd_force_clear() {
  LCD_PREFIX(clear)();
}

void avr_lcd_clear() {
  lcd.CURX = lcd.CURY = 0;

#ifdef AVR_LCD_BUFFERED

  lcd.clr = true;

  for (uint8_t row = 0; row < AVR_LCD_ROWS; row++) {
    for (uint8_t col = 0; col < AVR_LCD_COLS; col++) {
      lcd.SCREEN(col, row).is_dirty = false;
    }
  }

#else

  avr_lcd_force_clear();
  LCD_PREFIX(cmd)(LCD_CMD_RETURN_HOME);

#endif
}

void avr_lcd_clear_till(uint8_t n) {
  cursor_t prev_cursor = lcd.cursor;

  while (n--) avr_lcd_put_char(' ');
  avr_lcd_set_cursor(prev_cursor.row, prev_cursor.col);
}

#ifdef AVR_LCD_BUFFERED

void avr_lcd_display() {
  bool set_ddram = true;

  if (lcd.clr) {
    LCD_PREFIX(clear)();
  }

  if (!lcd.upt) {
    return;
  }

  for (uint8_t row = 0; row < AVR_LCD_ROWS; row++) {
    for (uint8_t col = 0; col < AVR_LCD_COLS; col++) {
      if (lcd.SCREEN(col, row).is_dirty) {
        if (set_ddram) {
          LCD_PREFIX(set_cursor)(row, col);
          set_ddram = false;
        }

        LCD_PREFIX(put_char)(lcd.SCREEN(col, row).c);
        lcd.SCREEN(col, row).is_dirty = false;
      }
      else {
        if (lcd.clr) {
          lcd.SCREEN(col, row).c = 0;
        }

        set_ddram = true;
      }
    }
  }

  lcd.clr = lcd.upt = false;
}

#endif /* AVR_LCD_BUFFERED */

avr_lcd_pwm_set_value_hook
avr_lcd_set_pwm_set_value_hook(avr_lcd_pwm_set_value_hook hook) {
  avr_lcd_pwm_set_value_hook prev_hook = lcd.pwm_value_hook;
  lcd.pwm_value_hook = hook;
  return prev_hook;
}

void avr_lcd_set_brightness(uint8_t value) {
  if (lcd.pwm_value_hook) {
    lcd.pwm_value_hook(value);
  }
}
