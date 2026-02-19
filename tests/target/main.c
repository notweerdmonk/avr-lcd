#include <lcd.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "/home/weerdmonk/include/simavr/avr/avr_mcu_section.h"

AVR_MCU(F_CPU, "atmega328p");
AVR_MCU_VCD_FILE("lcd_trace.vcd", 1000);

const struct avr_mmcu_vcd_trace_t _lcd_trace[]  _MMCU_ = {
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

int main()
{

#ifdef AVR_LCD_RUNTIME_HW_REPR_SEL
  lcd_set_pins(&(struct hardware_repr) { 
                                         .mode = ABSOLUTE_PIN_NUMBERS | BUS_4BIT,
                                         .ctl.rs = 4,
                                         .ctl.en = 5,
                                         .ctl.rw = 6,
                                         .ctl.bl = PIN_NC,
                                         //.ctl.rs = 0,
                                         //.ctl.en = 1,
                                         //.ctl.rw = PIN_NC,
                                         //.ctl.bl = PIN_NC,
                                         .data.pins.d0 = PIN_NC,
                                         .data.pins.d1 = PIN_NC,
                                         .data.pins.d2 = PIN_NC,
                                         .data.pins.d3 = PIN_NC,
                                         .data.pins.d4 = 0,
                                         .data.pins.d5 = 1,
                                         .data.pins.d6 = 2,
                                         .data.pins.d7 = 3, 
                                         //.data.pins.d4 = 19,
                                         //.data.pins.d5 = 20,
                                         //.data.pins.d6 = 21,
                                         //.data.pins.d7 = 22, 
                                       });
#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
  lcd_set_pins(&(struct hardware_repr) { 
                                         .ctl.rs = 4, //0,
                                         .ctl.en = 5, //1,
                                         .ctl.rw = 6, //PIN_NC,
                                         .ctl.bl = PIN_NC, //2,
                                         .data.pins.d0 = PIN_NC,
                                         .data.pins.d1 = PIN_NC,
                                         .data.pins.d2 = PIN_NC,
                                         .data.pins.d3 = PIN_NC,
                                         .data.pins.d4 = 0, //19,
                                         .data.pins.d5 = 1, //20,
                                         .data.pins.d6 = 2, //21,
                                         .data.pins.d7 = 3, //22, 
                                       });
#else
  lcd_set_pins(&(struct hardware_repr) { 
#ifdef AVR_UART_SIMTEST
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
                                         //.data.pins.d0 = PIN_NC,
                                         //.data.pins.d1 = PIN_NC,
                                         //.data.pins.d2 = PIN_NC,
                                         //.data.pins.d3 = PIN_NC,
                                         //.data.pins.d4 = 0, //19,
                                         //.data.pins.d5 = 1, //20,
                                         //.data.pins.d6 = 2, //21,
                                         //.data.pins.d7 = 3, //22, 
#ifdef AVR_UART_SIMTEST
                                         .data.d4 = 0
#else
                                         .data.d4 = 4
#endif
                                       });
#endif

  lcd_reset();
#ifdef AVR_LCD_RUNTIME_HW_REPR_SEL
  lcd_setup(
      0,
      //0,
      LCD_CMD_DISPLAY_CONTROL |
      LCD_CMD_DISPLAY_ON |
      LCD_CMD_CURSOR_ON |
      LCD_CMD_BLINK_ON,
      0,
      0
    );
#else
  lcd_setup();
#endif
  lcd_put_string("hello not-devs");
  //lcd_put_string("hello");
  lcd_set_cursor(1, 0);
  lcd_put_string("^_^");
  lcd_set_cursor(1, 13);
  lcd_put_string("^_^");

#ifdef AVR_LCD_BUFFERED
  lcd_display();
#endif
  _delay_ms(1000);

  lcd_set_cursor(1, 0);
  lcd_clear_till(3);
  lcd_put_string(":)");

#ifdef AVR_LCD_BUFFERED
  lcd_display();
#endif

  cli();
  sleep_cpu();

  return 0;
}
