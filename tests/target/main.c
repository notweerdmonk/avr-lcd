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
 * @file main.c
 * @author notweerdmonk
 * #brief avr-lcd test program for target microcontroller 
 */

#include <avr_lcd.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <port.h>

#if defined AVR_LCD_SIMULATION || defined AVR_LCD_SIMTEST

#define MCU_NAME(m) STRINGIFY(m)
#define VCD_FILE(m) \
  STRINGIFY(PROJECT_ROOT)STRINGIFY(/simulation/)\
  STRINGIFY(m)STRINGIFY(_uart_trace.vcd)

#include <avr_mcu_section.h>
AVR_MCU(F_CPU, MCU_NAME(DEVICE_NAME));
AVR_MCU_VCD_FILE(VCD_FILE(DEVICE_NAME), 1000);

const struct avr_mmcu_vcd_trace_t _avr_lcd_trace[]  _MMCU_ = {
  {
    AVR_MCU_VCD_SYMBOL("PORTB"),
    .what = (void*)&PORTB,
  },
  {
    AVR_MCU_VCD_SYMBOL("D4-D7"),
    .mask = 0x0F,
    .what = (void*)&PORTB,
  },
  {
    AVR_MCU_VCD_SYMBOL("RS"),
    .mask = (1 << 4),
    .what = (void*)&PORTB,
  },
  {
    AVR_MCU_VCD_SYMBOL("EN"),
    .mask = (1 << 5),
    .what = (void*)&PORTB,
  },
{
    AVR_MCU_VCD_SYMBOL("RW"),
    .mask = (1 << 6),
    .what = (void*)&PORTB,
  },
};

#endif /* AVR_LCD_SIMULATION || defined AVR_LCD_SIMTEST */

#ifdef AVR_LCD_BUFFERED

ISR(TIMER2_OVF_vect) {
  avr_lcd_display();
}

#endif

int main() {

#ifdef AVR_LCD_BUFFERED

    PORT_TIMER2_SET_CS(32);
    PORT_TIMER2_ENABLE_OVERFLOW_INTERRUPT();

#endif

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  avr_lcd_set_pins(&(avr_lcd_pins_t) { 
    .mode = ABSOLUTE_PIN_NUMBERS | BUS_4BIT,
#ifdef AVR_LCD_SIMTEST
      .ctl.rs = 12,
      .ctl.en = 13,
      .ctl.rw = 14,
      .ctl.bl = PIN_NC,
#else
      .ctl.rs = 8,
      .ctl.en = 9,
      .ctl.rw = PIN_NC,
      .ctl.bl = 10,
#endif
      .data.pins.d0 = PIN_NC,
      .data.pins.d1 = PIN_NC,
      .data.pins.d2 = PIN_NC,
      .data.pins.d3 = PIN_NC,
#ifdef AVR_LCD_SIMTEST
      .data.pins.d4 = 8,
      .data.pins.d5 = 9,
      .data.pins.d6 = 10,
      .data.pins.d7 = 11, 
#else
      .data.pins.d4 = 28,
      .data.pins.d5 = 29,
      .data.pins.d6 = 30,
      .data.pins.d7 = 31, 
#endif
  });

#elif defined AVR_LCD_USE_PORT_ADDR

  avr_lcd_set_pins(&(avr_lcd_pins_t) {
#ifdef AVR_LCD_SIMTEST
      .data_port_addr = &PORTB,
      .ctl_port_addr = &PORTB,
      .ctl.rs = 4,
      .ctl.en = 5,
      .ctl.rw = 6,
      .ctl.bl = PIN_NC,
#else
      .data_port_addr = &PORTD,
      .ctl_port_addr = &PORTB,
      .ctl.rs = 0,
      .ctl.en = 1,
      .ctl.rw = PIN_NC,
      .ctl.bl = 2,
#endif
#ifdef AVR_LCD_SIMTEST
      .data.d4 = 0,
#else
      .data.d4 = 4,
#endif
  });

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  avr_lcd_set_pins(&(avr_lcd_pins_t) {
#ifdef AVR_LCD_SIMTEST
      .ctl.rs = 12,
      .ctl.en = 13,
      .ctl.rw = 14,
      .ctl.bl = PIN_NC,
#else
      .ctl.rs = 8,
      .ctl.en = 9,
      .ctl.rw = PIN_NC,
      .ctl.bl = 10,
#endif
      .data.pins.d0 = PIN_NC,
      .data.pins.d1 = PIN_NC,
      .data.pins.d2 = PIN_NC,
      .data.pins.d3 = PIN_NC,
#ifdef AVR_LCD_SIMTEST
      .data.pins.d4 = 8,
      .data.pins.d5 = 9,
      .data.pins.d6 = 10,
      .data.pins.d7 = 11, 
#else
      .data.pins.d4 = 28,
      .data.pins.d5 = 29,
      .data.pins.d6 = 30,
      .data.pins.d7 = 31, 
#endif
  });

#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS

  avr_lcd_set_pins(&(avr_lcd_pins_t) { 
#ifdef AVR_LCD_SIMTEST
      .data_port_num = 1,
      .ctl_port_num = 1,
      .ctl.rs = 4,
      .ctl.en = 5,
      .ctl.rw = 6,
      .ctl.bl = PIN_NC,
      .data.d4 = 0
#else
      .data_port_num = 3,
      .ctl_port_num = 1,
      .ctl.rs = 0,
      .ctl.en = 1,
      .ctl.rw = PIN_NC,
      .ctl.bl = 2,
      .data.d4 = 4
#endif
  });

#elif defined AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  avr_lcd_set_pins(&(avr_lcd_pins_t) { 
#ifdef AVR_LCD_SIMTEST
      .ctl.rs = 4,
      .ctl.en = 5,
      .ctl.rw = 6,
      .ctl.bl = PIN_NC,
#else
      .ctl.rs = 0,
      .ctl.en = 1,
      .ctl.rw = PIN_NC,
      .ctl.bl = 2,
#endif
#ifdef AVR_LCD_SIMTEST
      .data.pins.d4 = 0,
      .data.pins.d5 = 1,
      .data.pins.d6 = 2,
      .data.pins.d7 = 3, 
#else
      .data.pins.d4 = 4,
      .data.pins.d5 = 5,
      .data.pins.d6 = 6,
      .data.pins.d7 = 7, 
#endif
  });

#else

  avr_lcd_set_pins(&(avr_lcd_pins_t) { 
#ifdef AVR_LCD_SIMTEST
      .ctl.rs = 4,
      .ctl.en = 5,
      .ctl.rw = 6,
      .ctl.bl = PIN_NC,
#else
      .ctl.rs = 0,
      .ctl.en = 1,
      .ctl.rw = PIN_NC,
      .ctl.bl = 2,
#endif
#ifdef AVR_LCD_SIMTEST
      .data.d4 = 0,
#else
      .data.d4 = 4,
#endif
  });

#endif

  avr_lcd_reset();

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
  avr_lcd_setup(
      0,
      LCD_CMD_DISPLAY_CONTROL
      | LCD_CMD_DISPLAY_ON | LCD_CMD_CURSOR_ON | LCD_CMD_BLINK_ON,
      0,
      0
  );
#else
  avr_lcd_setup();
#endif

  avr_lcd_clear();

  sei();

  avr_lcd_put_string("hello not-devs");
  avr_lcd_set_cursor(1, 0);
  avr_lcd_put_string("^_^");
  avr_lcd_set_cursor(1, 13);
  avr_lcd_put_string("^_^");

  _delay_ms(1000);

  avr_lcd_set_cursor(1, 0);
  avr_lcd_clear_till(3);
  avr_lcd_put_string(":)");

  /* Vertical scroll */
#ifdef AVR_LCD_BUFFERED

  _delay_ms(1000);

  avr_lcd_set_vscroll(1);
  avr_lcd_newline();
  avr_lcd_put_string("newline");

  _delay_ms(1000);

  avr_lcd_newline();
  avr_lcd_put_string("again newline");

  _delay_ms(1000);

  avr_lcd_newline();
  avr_lcd_put_string("another newline");

  _delay_ms(1000);

  avr_lcd_newline();
  avr_lcd_put_string("some text");

  _delay_ms(1000);

#endif

  cli();
  sleep_cpu();

  return 0;
}
