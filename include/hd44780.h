#ifndef _AVR_LCD_HD44780_H_
#define _AVR_LCD_HD44780_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct hardware_repr {

#define LCD_DATA_PIN_SIZE 3

#ifdef LCD_RUNTIME_HW_REPR_SEL

#undef LCD_DATA_PIN_SIZE
#define LCD_DATA_PIN_SIZE 6

  uint8_t mode;

  union {
    volatile uint8_t *data_port_addr;
    uint8_t data_port_num;
  } data_port;

  union {
    volatile uint8_t *ctl_port_addr;
    struct {
      volatile uint8_t *rs_port_addr;
      volatile uint8_t *en_port_addr;
      volatile uint8_t *rw_port_addr;
      volatile uint8_t *bl_port_addr;
    } addrs;
    uint8_t ctl_port_num;
    struct {
      uint8_t rs_port_num;
      uint8_t en_port_num;
      uint8_t rw_port_num;
      uint8_t bl_port_num;
    } nums;
  } ctl_port;

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#ifdef LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr;

#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr;
  volatile uint8_t *en_port_addr;
  volatile uint8_t *rw_port_addr;
  volatile uint8_t *bl_port_addr;

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *ctl_port_addr;

#endif /* LCD_USE_SEPARATE_PORTS */

#elif defined LCD_USE_RELATIVE_PIN_NUMBERS

  /*
   * PORTA 0
   * PORTB 1
   * PORTC 2
   * PORTD 3
   * PORTE 4
   * PORTF 5
   */

  uint8_t data_port_num : 3;

#ifdef LCD_USE_SEPARATE_PORTS

  uint8_t rs_port_num : 3;
  uint8_t en_port_num : 3;
  uint8_t rw_port_num : 3;
  uint8_t bl_port_num : 3;

#else /* !LCD_USE_SEPARATE_PORTS */

  uint8_t ctl_port_num : 3;

#endif /* LCD_USE_SEPARATE_PORTS */


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

#undef LCD_DATA_PIN_SIZE
#define LCD_DATA_PIN_SIZE 6

#else /* 
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#endif /* LCD_USE_PORT_ADDR */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

#ifdef LCD_RUNTIME_HW_REPR_SEL

  //uint8_t bus_bitmask[8];
  union {
    struct __attribute__((packed)) {
      uint8_t d0 : LCD_DATA_PIN_SIZE;
      uint8_t d1 : LCD_DATA_PIN_SIZE;
      uint8_t d2 : LCD_DATA_PIN_SIZE;
      uint8_t d3 : LCD_DATA_PIN_SIZE;
      uint8_t d4 : LCD_DATA_PIN_SIZE;
      uint8_t d5 : LCD_DATA_PIN_SIZE;
      uint8_t d6 : LCD_DATA_PIN_SIZE;
      uint8_t d7 : LCD_DATA_PIN_SIZE;
    } pins;

    uint8_t d4;

  } data;

#else
/*
#elif defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS || \
  defined LCD_USE_ABSOLUTE_PIN_NUMBERS
*/

#if defined LCD_USE_NONCONTIGUOUS_DATA_PINS || \
  defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  union {
    struct __attribute__((packed)) {
      uint8_t d0 : LCD_DATA_PIN_SIZE;
      uint8_t d1 : LCD_DATA_PIN_SIZE;
      uint8_t d2 : LCD_DATA_PIN_SIZE;
      uint8_t d3 : LCD_DATA_PIN_SIZE;
      uint8_t d4 : LCD_DATA_PIN_SIZE;
      uint8_t d5 : LCD_DATA_PIN_SIZE;
      uint8_t d6 : LCD_DATA_PIN_SIZE;
      uint8_t d7 : LCD_DATA_PIN_SIZE;
    } pins;
  } data;

#else /*
       * !LCD_USE_NONCONTIGUOUS_DATA_PINS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_4BIT

  union {
    uint8_t d4;
  } data;

#endif /* LCD_4BIT */


#endif/*
       * LCD_USE_NONCONTIGUOUS_DATA_PINS ||
       * LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

  struct __attribute__((packed)) {
    uint8_t rs : 3;
    uint8_t en : 3;
    uint8_t rw : 3;
    uint8_t bl : 3;
  } ctl;

} hardware_repr_t;

void hd44780_set_pins(struct hardware_repr*);

void hd44780_set_brightness(uint8_t value);

void hd44780_reset();

#ifdef LCD_RUNTIME_HW_REPR_SEL
void hd44780_setup(
    uint8_t entry_mode,
    uint8_t display,
    uint8_t cursor_shift,
    uint8_t function
  );
#else
void hd44780_setup();
#endif

void hd44780_set_cursor(uint8_t row, uint8_t col);

void hd44780_put_char(char c);

#endif /* _AVR_LCD_HD44780_H_ */
