/*
 * @file lcd.c
 * @author notweerdmonk
 * #brief HD44780 LCD module driver
 */

#include <avr/io.h>
#include <util/delay.h>

#include <lcd.h>
#include <port.h>

/*
 * Works using busy wait method
 * 
 * TODO: add busy poll method
 * TODO: supports HD44780 controller only, add a way to determine controller
 */

/*****************************************************************************/

typedef struct char_buffer {
  char c;
  bool is_dirty;
} char_buffer_t;

typedef struct screen_buffer {
  struct char_buffer buffer[AVR_LCD_ROWS][AVR_LCD_COLS];
} screen_buffer_t;

typedef struct lcd {
  struct screen_buffer screen;
  struct cursor cursor;

  //event_id_t rdy_ev;

  struct hardware_repr hw; /* Hardware representation */

  bool clr : 1;
  bool upt : 1;
  bool rdy : 1;
  bool f0  : 1;
  bool f1  : 1;
  bool f2  : 1;
  bool f3  : 1;
  bool f4  : 1;

} lcd_t;

static struct lcd lcd;

/*****************************************************************************/

/* 
 * NOTE:
 * Always include hardware specific source after definition of struct lcd
 */
#include <hd44780.c>

/*****************************************************************************/

#ifdef AVR_LCD_BUFFERED
#define SCREEN(x, y) screen.buffer[y][x]
#endif

#define CURX cursor.col
#define CURY cursor.row

void lcd_put_char(char c) {
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

void lcd_put_string(char *str) {
  while(*str) lcd_put_char(*str++);
}

void lcd_put_pgm_string(PGM_P s) {
  for (char c = pgm_read_byte(s); c != 0; c = pgm_read_byte(++s)) {
    lcd_put_char(c);
  }
}

void lcd_put_uint(unsigned int u) {
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
    lcd_put_char(ascii(digits[--idx]));
  }
}

void lcd_put_int(int n) {
  if(n < 0) {
    lcd_put_char('-');
    n = -n;
  }

  lcd_put_uint(n);
}

/* TODO: sprintf with -lprintf_flt or dtostrf, but bigger size */
void lcd_put_float(float f, uint8_t m) {
  /* fractional part greater than 4 does not work */
  if (m > 4) m = 4;

  lcd_put_int(f);
  lcd_put_char('.');

  f = f - (int)f;
  for (uint8_t i = 0; i < m; i++) {
    f = f * 10;
  }

  lcd_put_int(f);
}

//event_id_t get_lcd_ready_event() {
//  return lcd.rdy_ev;
//}

bool lcd_ready() {
  return (bool)lcd.rdy;
}

cursor_t lcd_get_cursor() {
  return lcd.cursor;
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
  if ((row < AVR_LCD_ROWS) && (col < AVR_LCD_COLS)) {
    lcd.CURY = row;
    lcd.CURX = col;
  }

#ifndef AVR_LCD_BUFFERED
  LCD_PREFIX(set_cursor)(row, col);
#endif
}

void lcd_force_clear() {
  LCD_PREFIX(clear)();
}

void lcd_clear() {
  lcd.CURX = lcd.CURY = 0;

#ifdef AVR_LCD_BUFFERED

  lcd.clr = true;

  for (uint8_t row = 0; row < AVR_LCD_ROWS; row++) {
    for (uint8_t col = 0; col < AVR_LCD_COLS; col++) {
      lcd.SCREEN(col, row).is_dirty = false;
    }
  }

#else

  lcd_force_clear();
  LCD_PREFIX(cmd)(LCD_CMD_RETURN_HOME);

#endif
}

void lcd_clear_till(uint8_t n) {
  cursor_t prev_cursor = lcd.cursor;

  while (n--) lcd_put_char(' ');
  lcd_set_cursor(prev_cursor.row, prev_cursor.col);
}

#ifdef AVR_LCD_BUFFERED

void lcd_display() {
  uint8_t col, row;//, ddram_addr = LCD_CMD_SET_DDRAMADDR;
  bool set_ddram = true;

  if (lcd.clr) {
    LCD_PREFIX(clear)();
  }

  if (lcd.upt) {
    //hd44780_cmd(ddram_addr);

    for (row = 0; row < AVR_LCD_ROWS; row++) {
      if (row > 0) {
        //ddram_addr |= LCD_CMD_DDRAMADDR_LINE2;
        //hd44780_cmd(ddram_addr);
      }

      for (col = 0; col < AVR_LCD_COLS; col++) {
        if (lcd.SCREEN(col, row).is_dirty) {
          if (set_ddram) {
            LCD_PREFIX(set_cursor)(row, col);
            //hd44780_cmd(ddram_addr | col);
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
}

void lcd_force_display() {
  lcd_display();
}

#endif /* AVR_LCD_BUFFERED */

//void* lcd_task(UNUSED_VARIABLE task_data_t data) {
//  COROUTINE_BEGIN();
//
//  lcd.rdy = false;
//  lcd.rdy_ev = register_event();
//
//  COROUTINE_DELEGATE(lcd_reset);
//
//  lcd_setup();
//
//  REGISTER_SOFTWARE_PWM(AVR_LCD_PWM_CHANNEL, lcd.bl, AVR_LCD_BL_VALUE);
//
//  lcd.rdy = true;
//  trigger_event(lcd.rdy_ev, EV_NOW);
//
//  COROUTINE_YIELD();
//
//  lcd_display();
//
//  COROUTINE_END();
//}
