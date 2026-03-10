#ifndef _AVR_LCD_HD44780_H_
#define _AVR_LCD_HD44780_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct hardware_repr {

#define __AVR_LCD_PIN_SIZE 3

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

#undef __AVR_LCD_PIN_SIZE
#define __AVR_LCD_PIN_SIZE 6

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

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr;

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr;
  volatile uint8_t *en_port_addr;
  volatile uint8_t *rw_port_addr;
  volatile uint8_t *bl_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *ctl_port_addr;

#endif /* AVR_LCD_USE_SEPARATE_PORTS */

#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS

  /*
   * PORTA 0
   * PORTB 1
   * PORTC 2
   * PORTD 3
   * PORTE 4
   * PORTF 5
   */

  uint8_t data_port_num : 3;

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  uint8_t rs_port_num : 3;
  uint8_t en_port_num : 3;
  uint8_t rw_port_num : 3;
  uint8_t bl_port_num : 3;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  uint8_t ctl_port_num : 3;

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

#undef __AVR_LCD_PIN_SIZE
#define __AVR_LCD_PIN_SIZE 6

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */
#ifndef AVR_LCD_DATA_PORT
#error AVR_LCD_DATA_PORT not defined
#endif

#ifndef AVR_LCD_CTL_PORT
#error AVR_LCD_CTL_PORT not defined
#endif

#endif /* AVR_LCD_USE_PORT_ADDR */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  //uint8_t bus_bitmask[8];
  union {
    struct __attribute__((packed)) {
      uint8_t d0 : __AVR_LCD_PIN_SIZE;
      uint8_t d1 : __AVR_LCD_PIN_SIZE;
      uint8_t d2 : __AVR_LCD_PIN_SIZE;
      uint8_t d3 : __AVR_LCD_PIN_SIZE;
      uint8_t d4 : __AVR_LCD_PIN_SIZE;
      uint8_t d5 : __AVR_LCD_PIN_SIZE;
      uint8_t d6 : __AVR_LCD_PIN_SIZE;
      uint8_t d7 : __AVR_LCD_PIN_SIZE;
    } pins;

    uint8_t d4;

  } data;

#else

/*
#elif defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS || \
  defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
*/

#if defined AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS || \
  defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  union {
    struct __attribute__((packed)) {
      uint8_t d0 : __AVR_LCD_PIN_SIZE;
      uint8_t d1 : __AVR_LCD_PIN_SIZE;
      uint8_t d2 : __AVR_LCD_PIN_SIZE;
      uint8_t d3 : __AVR_LCD_PIN_SIZE;
      uint8_t d4 : __AVR_LCD_PIN_SIZE;
      uint8_t d5 : __AVR_LCD_PIN_SIZE;
      uint8_t d6 : __AVR_LCD_PIN_SIZE;
      uint8_t d7 : __AVR_LCD_PIN_SIZE;
    } pins;
  } data;

#else /*
       * !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_4BIT

  union {
    uint8_t d4;
  } data;

#endif /* AVR_LCD_4BIT */


#endif/*
       * AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS ||
       * AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  struct __attribute__((packed)) {
    uint8_t rs : __AVR_LCD_PIN_SIZE;
    uint8_t en : __AVR_LCD_PIN_SIZE;
    uint8_t rw : __AVR_LCD_PIN_SIZE;
    uint8_t bl : __AVR_LCD_PIN_SIZE;
  } ctl;

} hardware_repr_t;

void hd44780_set_pins(struct hardware_repr*);

void hd44780_set_brightness(uint8_t value);

void hd44780_reset();

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
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
