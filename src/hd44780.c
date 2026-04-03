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
 * @file hd44780.c
 * @author notweerdmonk
 * #brief HD44780 LCD module driver
 */

#include <avr/io.h>
#include <util/delay.h>

#include <config.h>
#include <avr_lcd_config.h>
#include <avr_portable.h>
#include <avr_utility.h>

/*****************************************************************************/

/*
 * Works using busy wait method
 * 
 * TODO: add busy poll method
 */

/*****************************************************************************/

/* Forward declarations */

void hd44780_input_pins(struct hardware_repr *p);
void hd44780_output_pins(struct hardware_repr *p);

/* Forward declarations */

#if defined AVR_LCD_RUNTIME_HARDWARE_REPR || \
  ( \
    (defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS) && \
    defined AVR_LCD_USE_SEPARATE_PORTS \
  ) || \
  defined AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

#ifndef AVR_LCD_4BIT

static
uint8_t hd44780_get_data_port_lower_nibble(
    volatile uint8_t *port_addr,
    struct hardware_repr *p_hw
) {
  if (!port_addr || !p_hw) {
    return;
  }

  uint8_t byte = 0;

  if (*port_addr & (1 << p_hw->data.pins.d0)) {
    byte |= 0x01;
  }
  if (*port_addr & (1 << p_hw->data.pins.d1)) {
    byte |= 0x02;
  }
  if (*port_addr & (1 << p_hw->data.pins.d2)) {
    byte |= 0x04;
  }
  if (*port_addr & (1 << p_hw->data.pins.d3)) {
    byte |= 0x08;
  }

  return byte;
}

static
void hd44780_set_data_port_lower_nibble(
    volatile uint8_t *port_addr,
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!port_addr || !p_hw) {
    return;
  }

  if (byte & 0x01) {
    *port_addr |= (1 << p_hw->data.pins.d0);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d0);
  }
  if (byte & 0x02) {
    *port_addr |= (1 << p_hw->data.pins.d1);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d1);
  }
  if (byte & 0x04) {
    *port_addr |= (1 << p_hw->data.pins.d2);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d2);
  }
  if (byte & 0x08) {
    *port_addr |= (1 << p_hw->data.pins.d3);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d3);
  }
}

#else /* !AVR_LCD_4BIT */

#define hd44780_get_data_port_lower_nibble(...)
#define hd44780_set_data_port_lower_nibble(...)

#endif /* AVR_LCD_4BIT */

static
uint8_t hd44780_get_data_port_upper_nibble(
    volatile uint8_t *port_addr,
    struct hardware_repr *p_hw
) {
  if (!port_addr || !p_hw) {
    return;
  }

  uint8_t byte = 0;

  if (*port_addr & (1 << p_hw->data.pins.d4)) {
    byte |= 0x01;
  }
  if (*port_addr & (1 << p_hw->data.pins.d5)) {
    byte |= 0x02;
  }
  if (*port_addr & (1 << p_hw->data.pins.d6)) {
    byte |= 0x04;
  }
  if (*port_addr & (1 << p_hw->data.pins.d7)) {
    byte |= 0x08;
  }

  return byte;
}

static
void hd44780_set_data_port_upper_nibble(
    volatile uint8_t *port_addr,
    struct hardware_repr *p_hw,
    uint8_t byte
) {

  if (!port_addr || !p_hw) {
    return;
  }

  if (byte & 0x01) {
    *port_addr |= (1 << p_hw->data.pins.d4);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d4);
  }
  if (byte & 0x02) {
    *port_addr |= (1 << p_hw->data.pins.d5);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d5);
  }
  if (byte & 0x04) {
    *port_addr |= (1 << p_hw->data.pins.d6);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d6);
  }
  if (byte & 0x08) {
    *port_addr |= (1 << p_hw->data.pins.d7);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d7);
  }
}

#endif

#if defined AVR_LCD_RUNTIME_HARDWARE_REPR || defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

#ifndef AVR_LCD_4BIT

static
uint8_t hd44780_get_data_pins_lower_nibble(
    struct hardware_repr *p_hw
) {

  if (!p_hw) {
    return;
  }

  uint8_t byte = 0;

  if (GET_PIN_NUMBER(p_hw->data.pins.d0)) {
    byte |= 0x01;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d1)) {
    byte |= 0x02;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d2)) {
    byte |= 0x04;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d4)) {
    byte |= 0x08;
  }

  return byte;
}

static
void hd44780_set_data_pins_lower_nibble(
    struct hardware_repr *p_hw,
    uint8_t byte
) {

  if (!p_hw) {
    return;
  }

  if (byte & 0x01) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d0);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d0);
  }
  if (byte & 0x02) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d1);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d1);
  }
  if (byte & 0x04) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d2);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d2);
  }
  if (byte & 0x08) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d3);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d3);
  }
}

#endif /* AVR_LCD_4BIT */

static
uint8_t hd44780_get_data_pins_lower_nibble(
    struct hardware_repr *p_hw
) {

  if (!p_hw) {
    return;
  }

  uint8_t byte = 0;

  if (GET_PIN_NUMBER(p_hw->data.pins.d4)) {
    byte |= 0x01;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d5)) {
    byte |= 0x02;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d6)) {
    byte |= 0x04;
  }
  if (GET_PIN_NUMBER(p_hw->data.pins.d7)) {
    byte |= 0x08;
  }

  return byte;
}

static
void hd44780_set_data_pins_upper_nibble(
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!p_hw) {
    return;
  }

  if (byte & 0x01) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d4);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d4);
  }
  if (byte & 0x02) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d5);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d5);
  }
  if (byte & 0x04) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d6);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d6);
  }
  if (byte & 0x08) {
    PORT_SET_PIN_NUMBER(p_hw->data.pins.d7);
  } else {
    PORT_CLR_PIN_NUMBER(p_hw->data.pins.d7);
  }
}

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR || AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS */

static
void hd44780_set_rw() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERW) {
    volatile uint8_t *rw_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rw_port_addr = lcd.hw.ctl_port.addrw.rw_port_addr;
      } else {
        rw_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rw_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.rw_port_num);
      } else {
        rw_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *rw_port_addr |= (1 << lcd.hw.ctl.rw);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERW) {
    PORT_SET_PIN_NUMBER(lcd.hw.ctl.rw);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERW


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rw_port_addr = lcd.hw.rw_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rw_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */ 


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rw_port_addr = port_num_to_addr(lcd.hw.rw_port_num);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rw_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif


#endif /* AVR_LCD_USE_PORT_ADDR */

  *rw_port_addr |= (1 << lcd.hw.ctl.rw);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERW

  PORT_SET_PIN_NUMBER(lcd.hw.ctl.rw);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERW &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERW
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_SET_PIN(AVR_LCD_RW_PORT, lcd.hw.ctl.rw);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_SET_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rw);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERW
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

static
void hd44780_clear_rw() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERW) {
    volatile uint8_t *rw_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rw_port_addr = lcd.hw.ctl_port.addrw.rw_port_addr;
      } else {
        rw_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rw_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.rw_port_num);
      } else {
        rw_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *rw_port_addr &= ~(1 << lcd.hw.ctl.rw);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERW) {
    PORT_CLR_PIN_NUMBER(lcd.hw.ctl.rw);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */
 

#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERW


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rw_port_addr = lcd.hw.rw_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rw_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rw_port_addr = port_num_to_addr(lcd.hw.rw_port_num);

#else

  volatile uint8_t *rw_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif

#endif

  *rw_port_addr &= ~(1 << lcd.hw.ctl.rw);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERW

  PORT_CLR_PIN_NUMBER(lcd.hw.ctl.rw);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERW &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERW
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_CLR_PIN(AVR_LCD_RW_PORT, lcd.hw.ctl.rw);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_CLR_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rw);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERW
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

static
void hd44780_set_rs() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *rs_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rs_port_addr = lcd.hw.ctl_port.addrs.rs_port_addr;
      } else {
        rs_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rs_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.rs_port_num);
      } else {
        rs_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *rs_port_addr |= (1 << lcd.hw.ctl.rs);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    PORT_SET_PIN_NUMBER(lcd.hw.ctl.rs);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = lcd.hw.rs_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */ 


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.rs_port_num);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif


#endif /* AVR_LCD_USE_PORT_ADDR */

  *rs_port_addr |= (1 << lcd.hw.ctl.rs);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  PORT_SET_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_SET_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_SET_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rs);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

static
void hd44780_clear_rs() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *rs_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rs_port_addr = lcd.hw.ctl_port.addrs.rs_port_addr;
      } else {
        rs_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        rs_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.rs_port_num);
      } else {
        rs_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *rs_port_addr &= ~(1 << lcd.hw.ctl.rs);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    PORT_CLR_PIN_NUMBER(lcd.hw.ctl.rs);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */
 

#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = lcd.hw.rs_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.rs_port_num);

#else

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif

#endif

  *rs_port_addr &= ~(1 << lcd.hw.ctl.rs);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  PORT_CLR_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_CLR_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_CLR_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rs);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

static
void hd44780_set_en() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *en_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        en_port_addr = lcd.hw.ctl_port.addrs.en_port_addr;
      } else {
        en_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        en_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.en_port_num);
      } else {
        en_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *en_port_addr |= (1 << lcd.hw.ctl.en);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    PORT_SET_PIN_NUMBER(lcd.hw.ctl.en);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */
 

#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = lcd.hw.en_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *en_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.en_port_num);

#else

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /* AVR_LCD_USE_PORT_ADDR */

  *en_port_addr |= (1 << lcd.hw.ctl.en);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  PORT_SET_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_SET_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_SET_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.en);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

static
void hd44780_clear_en() {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR || lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *en_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        en_port_addr = lcd.hw.ctl_port.addrs.en_port_addr;
      } else {
        en_port_addr = lcd.hw.ctl_port.ctl_port_addr;
      }
    } else {
      if (lcd.hw.mode & SERPARATE_PORTS) {
        en_port_addr = port_num_to_addr(lcd.hw.ctl_port.nums.en_port_num);
      } else {
        en_port_addr = port_num_to_addr(lcd.hw.ctl_port.ctl_port_num);
      }
    }

    *en_port_addr &= ~(1 << lcd.hw.ctl.en);

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    PORT_CLR_PIN_NUMBER(lcd.hw.ctl.en);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */
 

#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = lcd.hw.en_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *en_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* !AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.en_port_num);

#else

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif


#endif

  *en_port_addr &= ~(1 << lcd.hw.ctl.en);

#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  PORT_CLR_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_CLR_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  AVR_UTIL_CLR_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.en);

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#endif /*
        * AVR_LCD_USE_PORT_ADDR || 
        * AVR_LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */
 

}

static
void hd44780_toggle_en() {

  hd44780_clear_en();
  _delay_us(1);

  hd44780_set_en();
  _delay_us(2000);

  hd44780_clear_en();
  _delay_us(100);

}

#if defined AVR_LCD_RUNTIME_HARDWARE_REPR || defined AVR_LCD_4BIT

static
uint8_t hd44780_read_nibble() {

  uint8_t nibble = 0;

  hd44780_toggle_en();

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR ||  lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *data_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      data_port_addr = lcd.hw.data_port.data_port_addr;
    } else {
      data_port_addr = port_num_to_addr(lcd.hw.data_port.data_port_num);
    }

    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      nibble =
        hd44780_get_data_port_upper_nibble(data_port_addr, &lcd.hw);
    } else {
      *data_port_addr |= ((nibble & 0x0F) << lcd.hw.data.d4);
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    nibble = hd44780_get_data_pins_upper_nibble(&lcd.hw);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr = lcd.hw.data_port_addr;

#else /* !AVR_LCD_USE_PORT_ADDR */
  
  volatile uint8_t *data_port_addr = port_num_to_addr(lcd.hw.data_port_num);

#endif /* AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  nibble =
    hd44780_get_data_port_upper_nibble(data_port_addr, &lcd.hw);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  nibble = *data_port_addr & (0xF << lcd.hw.data.d4);


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  nibble = hd44780_set_data_pins_upper_nibble(&lcd.hw);

#else /*
        * !AVR_LCD_USE_PORT_ADDR &&
        * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
        * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
        */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  nibble =
    hd44780_get_data_port_upper_nibble(
        &AVR_UTIL_CONCAT(PORT, AVR_LCD_DATA_PORT),
        &lcd.hw
    );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  nibble = AVR_UTIL_READ_PORT(AVR_LCD_DATA_PORT) & (0xF << lcd.hw.data.d4);

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

#endif

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  return nibble;
}

static
void hd44780_write_nibble(uint8_t nibble) {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR ||  lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *data_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      data_port_addr = lcd.hw.data_port.data_port_addr;
    } else {
      data_port_addr = port_num_to_addr(lcd.hw.data_port.data_port_num);
    }

    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      hd44780_set_data_port_upper_nibble(data_port_addr, &lcd.hw, nibble);
    } else {
      *data_port_addr |= ((nibble & 0x0F) << lcd.hw.data.d4);
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    hd44780_set_data_pins_upper_nibble(&lcd.hw, nibble);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr = lcd.hw.data_port_addr;

#else /* !AVR_LCD_USE_PORT_ADDR */
  
  volatile uint8_t *data_port_addr = port_num_to_addr(lcd.hw.data_port_num);

#endif /* AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  hd44780_set_data_port_upper_nibble(data_port_addr, &lcd.hw, nibble);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  uint8_t data_port_byte = *data_port_addr;
  AVR_UTIL_CLR_MASK(data_port_byte, 0xF << lcd.hw.data.d4);
  AVR_UTIL_SET_MASK(data_port_byte, (nibble & 0xF) << lcd.hw.data.d4);
  *data_port_addr = data_port_byte;

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  hd44780_set_data_pins_upper_nibble(&lcd.hw, nibble);

#else /*
        * !AVR_LCD_USE_PORT_ADDR &&
        * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
        * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
        */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  hd44780_set_data_port_upper_nibble(
      &CONCAT(PORT, AVR_LCD_DATA_PORT),
      &lcd.hw,
      nibble
  );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  uint8_t data_port_byte = AVR_UTIL_READ_PORT(AVR_LCD_DATA_PORT);
  AVR_UTIL_CLR_MASK(data_port_byte, 0xF << lcd.hw.data.d4);
  AVR_UTIL_SET_MASK(data_port_byte, (nibble & 0xF) << lcd.hw.data.d4);
  AVR_UTIL_WRITE_PORT(AVR_LCD_DATA_PORT, data_port_byte);

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

#endif

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  hd44780_toggle_en();
}

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR || AVR_LCD_4BIT */

#ifndef AVR_LCD_4BIT

static
void hd44780_write_byte(uint8_t byte) {

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR ||  lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *data_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      data_port_addr = lcd.hw.data_port.data_port_addr;
    } else {
      data_port_addr = port_num_to_addr(lcd.hw.data_port.data_port_num);
    }

    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      hd44780_set_data_port_lower_nibble(data_port_addr, &lcd.hw, byte);
      hd44780_set_data_port_upper_nibble(data_port_addr, &lcd.hw, byte);
    } else {
      *data_port_addr = byte;
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    hd44780_set_data_pins_lower_nibble(&lcd.hw, byte);
    hd44780_set_data_pins_upper_nibble(&lcd.hw, byte);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#if defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr = lcd.hw.data_port_addr;

#else /* !AVR_LCD_USE_PORT_ADDR */

  volatile uint8_t *data_port_addr = port_num_to_addr(lcd.hw.data_port_num);

#endif /* AVR_LCD_USE_PORT_ADDR */


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  hd44780_set_data_port_lower_nibble(data_port_addr, &lcd.hw, byte);
  hd44780_set_data_port_upper_nibble(data_port_addr, &lcd.hw, byte);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  *data_port_addr = byte;

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  hd44780_set_data_pins_lower_nibble(&lcd.hw, byte);
  hd44780_set_data_pins_upper_nibble(&lcd.hw, byte);

#else /*
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

  hd44780_set_data_port_lower_nibble(&CONCAT(PORT, AVR_LCD_DATA_PORT), &lcd.hw, byte);
  hd44780_set_data_port_upper_nibble(&CONCAT(PORT, AVR_LCD_DATA_PORT), &lcd.hw, byte);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

  AVR_UTIL_WRITE_PORT(AVR_LCD_DATA_PORT, byte);

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#endif


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  hd44780_toggle_en();
}

#else /* AVR_LCD_4BIT */

#define hd44780_write_byte(x)

#endif /* !AVR_LCD_4BIT */

static
unsigned char hd44780_read() {

  unsigned char byte = 0;

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & BUS_4BIT) {
    /* Read upper nibble */
    byte = hd44780_read_nibble() << 4;
    /* Read lower nibble */
    byte |= hd44780_read_nibble() & 0xF;
  } else {
    /* Read byte */
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_4BIT

  /* Read upper nibble */
  byte = hd44780_read_nibble() << 4;
  /* Read lower nibble */
  byte |= hd44780_read_nibble() & 0xF;

#else /* !AVR_LCD_4BIT */
  /* Write byte */
#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  return byte;
}


static
void hd44780_write(unsigned char byte) {
#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & BUS_4BIT) {
    /* write upper nibble */
    hd44780_write_nibble(byte >> 4);
    /* write lower nibble */
    hd44780_write_nibble(byte & 0xF);
  } else {
    /* write byte */
    hd44780_write_byte(byte);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_4BIT
  /* write upper nibble */
  hd44780_write_nibble(byte >> 4);
  /* write lower nibble */
  hd44780_write_nibble(byte & 0xF);

#else /* !AVR_LCD_4BIT */
  /* write byte */
  hd44780_write_byte(byte);
#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

}

unsigned char hd44780_read_data() {
  hd44780_set_rs();
  hd44780_set_rw();
  hd44780_input_pins(&lcd.hw);
  unsigned char data = hd44780_read();
  hd44780_output_pins(&lcd.hw);
  hd44780_clear_rw();
  return data;
}

void hd44780_data(unsigned char data) {
  hd44780_set_rs();
  hd44780_write(data);
}

void hd44780_cmd(unsigned char cmd) {
  hd44780_clear_rs();
  hd44780_write(cmd);
}

void hd44780_clear() {
  hd44780_cmd(LCD_CMD_CLEAR_DISPLAY);
  /*
   * Set decrement mode because the clear command shall set increment/decreent
   * mode to 1 (increment).
   */
  if (!lcd.f0) {
    hd44780_cmd(LCD_CMD_ENTRY_MODE_SET | LCD_CMD_ENTRY_DECR);
  }
}

/* TODO: 8-bit mode reset routine */
void hd44780_reset() {

  hd44780_clear_rs();
  hd44780_clear_en();

  _delay_ms(20);

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & BUS_4BIT) {
    hd44780_write_nibble(0x03);
    _delay_ms(5);

    hd44780_write_nibble(0x03);
    _delay_ms(1);

    hd44780_write_nibble(0x03);
    _delay_ms(1);
  }

#else /* AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_4BIT
  hd44780_write_nibble(0x03);
  _delay_ms(5);

  hd44780_write_nibble(0x03);
  _delay_ms(1);

  hd44780_write_nibble(0x03);
  _delay_ms(1);
#endif /* AVR_LCD_4BIT */

#endif
}

/* TODO: 8-bit mode setup routine */
#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
void hd44780_setup(
      uint8_t entry_mode,
      uint8_t display,
      uint8_t cursor_shift,
      uint8_t function
    ) {

  if (lcd.hw.mode & BUS_4BIT) {
    hd44780_write_nibble(0x02);
  }

  /* Set LCD function */
  if (function & LCD_CMD_FUNCTION_SET) {
    hd44780_cmd(function);
  } else {
    /* 4-bit mode - 2 line - 5x8 font */
    hd44780_cmd(
        LCD_CMD_FUNCTION_SET
        | ((lcd.hw.mode & BUS_4BIT) ? LCD_CMD_4BIT_MODE : LCD_CMD_8BIT_MODE)
        | LCD_CMD_2LINE
        | LCD_CMD_5x8DOTS
      );
  }


  /* Set display characteristics */
  if (display & LCD_CMD_DISPLAY_CONTROL) {
    hd44780_cmd(display);
  } else  {
    /* Display on, cusror on, blink on */
    hd44780_cmd(
        LCD_CMD_DISPLAY_CONTROL
        | LCD_CMD_DISPLAY_ON
        | LCD_CMD_CURSOR_OFF
        | LCD_CMD_BLINK_OFF
      );
  }

  hd44780_cmd(
      LCD_CMD_CLEAR_DISPLAY
    );

  /* Set entry mode */
  if (entry_mode & LCD_CMD_ENTRY_MODE_SET) {
    hd44780_cmd(entry_mode);
  }else {
    /* Entry mode increment */
    hd44780_cmd(
        LCD_CMD_ENTRY_MODE_SET
        | LCD_CMD_ENTRY_INCR
        | LCD_CMD_ENTRY_SHIFT_OFF
      );
  }

  /* Use flag 0 from struct lcd to store entry increment/decrement state */
  lcd.f0 = !(entry_mode & LCD_CMD_ENTRY_DECR);

  /* Set cursor characteristics */
  if (cursor_shift & LCD_CMD_CURSOR_SHIFT) {
    hd44780_cmd(cursor_shift);
  }
}

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */

void hd44780_setup() {

#ifdef AVR_LCD_4BIT
  hd44780_write_nibble(0x02);
#endif

  hd44780_cmd(
      LCD_CMD_FUNCTION_SET
#ifdef AVR_LCD_4BIT
      | LCD_CMD_4BIT_MODE
#else
      | LCD_CMD_8BIT_MODE
#endif
#ifndef AVR_LCD_1LINE
      | LCD_CMD_2LINE
#else
      | LCD_CMD_1LINE
#endif
#ifdef AVR_LCD_5x10DOTS
      | LCD_CMD_5x10DOTS
#else
      | LCD_CMD_5x8DOTS
#endif
    );

  /* This is as per the HD44780 datasheet */
  //hd44780_cmd(
  //    LCD_CMD_DISPLAY_CONTROL |
  //    LCD_CMD_DISPLAY_OFF
  //  );

  /* But it appears the display modes can be set at this point itself */
  hd44780_cmd(
      LCD_CMD_DISPLAY_CONTROL
#ifdef AVR_LCD_DISPLAY_OFF
      | LCD_CMD_DISPLAY_OFF
#else
      | LCD_CMD_DISPLAY_ON
#endif
#ifdef AVR_LCD_CURSOR_ON
      | LCD_CMD_CURSOR_ON
#else
      | LCD_CMD_CURSOR_OFF
#endif
#ifdef AVR_LCD_BLINK_ON
      | LCD_CMD_BLINK_ON
#else
      | LCD_CMD_BLINK_OFF
#endif
    );

  hd44780_cmd(
      LCD_CMD_CLEAR_DISPLAY
    );

  hd44780_cmd(
      LCD_CMD_ENTRY_MODE_SET
#ifdef AVR_LCD_ENTRY_DECR
      | LCD_CMD_ENTRY_DECR
#else
      | LCD_CMD_ENTRY_INCR
#endif
#ifdef AVR_LCD_ENTRY_SHIFT
      | LCD_CMD_ENTRY_SHIFT_ON
#else
      | LCD_CMD_ENTRY_SHIFT_OFF
#endif
    );

  /* Use flag 0 from struct lcd to store entry increment/decrement state */
#ifdef AVR_LCD_ENTRY_DECR
  lcd.f0 = 0;
#else
  lcd.f0 = 1;
#endif

#ifdef AVR_LCD_CURSOR_MOVE_LEFT
  hd44780_cmd(
      LCD_CMD_CURSOR_SHIFT | LCD_CMD_CURSOR_MOVE
#ifdef AVR_LCD_CURSOR_MOVE_LEFT
      | LCD_CMD_MOVE_LEFT
#else
      | LCD_CMD_MOVE_RIGHT
#endif
    );
#endif /* AVR_LCD_CURSOR_MOVE_LEFT */

#ifdef AVR_LCD_DISPLAY_MOVE_LEFT
  hd44780_cmd(
      LCD_CMD_CURSOR_SHIFT | LCD_CMD_DISPLAY_MOVE
#ifdef AVR_LCD_DISPLAY_MOVE_LEFT
      | LCD_CMD_MOVE_LEFT
#else
      | LCD_CMD_MOVE_RIGHT
#endif
    );
#endif /* AVR_LCD_DISPLAY_MOVE_LEFT */
}

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

UNUSED_FUNCTION
static
void hd44780_set_ddram(uint8_t row, uint8_t col) {
  if (row < AVR_LCD_ROWS && col < 40) {
    switch (row) {
      case 0:
        hd44780_cmd(LCD_CMD_SET_DDRAMADDR | LCD_CMD_DDRAMADDR_LINE1 | col);
        break;

      case 1:
        hd44780_cmd(LCD_CMD_SET_DDRAMADDR | LCD_CMD_DDRAMADDR_LINE2 | col);
        break;

      default:
        ;
    }
  }
}

void hd44780_input_pins(struct hardware_repr *p) {
  if (!p) return;

  lcd.hw = *p;

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d0);
        *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d1);
        *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d2);
        *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d3);
      }

      *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) &= ~(1 << p->data.pins.d4);

    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) &= ~(0xF << p->data.d4);
      } else {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) = 0;
      }
    }

  } else if (lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        SET_INPUT_PINS_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.pins.d0,
            p->data.pins.d1,
            p->data.pins.d2,
            p->data.pins.d3
          );
      }

      SET_INPUT_PINS_FROM_PORT_NUM(
          p->data_port.data_port_num,
          p->data.pins.d4,
          p->data.pins.d5,
          p->data.pins.d6,
          p->data.pins.d7
        );
    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        SET_INPUT_PORT_NIBBLE_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.d4
          );
      } else {
        SET_INPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port.data_port_num);
      }
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    if (!(lcd.hw.mode & BUS_4BIT)) {
      PORT_INPUT_PIN_NUMBER(p->data.pins.d0);
      PORT_INPUT_PIN_NUMBER(p->data.pins.d1);
      PORT_INPUT_PIN_NUMBER(p->data.pins.d2);
      PORT_INPUT_PIN_NUMBER(p->data.pins.d3);
    }

    PORT_INPUT_PIN_NUMBER(p->data.pins.d4);
    PORT_INPUT_PIN_NUMBER(p->data.pins.d5);
    PORT_INPUT_PIN_NUMBER(p->data.pins.d6);
    PORT_INPUT_PIN_NUMBER(p->data.pins.d7);

  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d0);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d1);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d2);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d4);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d5);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d6);
  *(p->data_port_addr - 1) &= ~(1 << p->data.pins.d7);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) &= ~(0xF << p->data.d4);

#else /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) = 0;

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_INPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
    );

#endif /* !AVR_LCD_4BIT */

  PORT_SET_INPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7,
    );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  PORT_SET_INPUT_PORT_NIBBLE_FROM_PORT_NUM(p->data_port_num, p->data.d4);

#else /* !AVR_LCD_4BIT */

  PORT_SET_INPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port_num);

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS


#ifndef AVR_LCD_4BIT

  PORT_INPUT_PIN_NUMBER(p->data.pins.d0);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d1);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d2);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  PORT_INPUT_PIN_NUMBER(p->data.pins.d4);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d5);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d6);
  PORT_INPUT_PIN_NUMBER(p->data.pins.d7);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_PORT_INPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
  );

#endif /* AVR_LCD_4BIT */

  PORT_SET_PORT_INPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7
  );


#else


#ifdef AVR_LCD_4BIT

  AVR_UTIL_CLR_DDR(AVR_LCD_DATA_PORT, (0xF << p->data.d4));

#else /* !AVR_LCD_4BIT */

  AVR_UTIL_INPUT_PORT(AVR_LCD_DATA_PORT);

#endif /* AVR_LCD_4BIT */


#endif


#endif


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */
}

void hd44780_output_pins(struct hardware_repr *p) {
  if (!p) return;

  lcd.hw = *p;

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d0);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d1);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d2);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d3);
      }

      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);

    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) |= (0xF << p->data.d4);
      } else {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) = 0xFF;
      }
    }

  } else if (lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.pins.d0,
            p->data.pins.d1,
            p->data.pins.d2,
            p->data.pins.d3
          );
      }

      PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->data_port.data_port_num,
          p->data.pins.d4,
          p->data.pins.d5,
          p->data.pins.d6,
          p->data.pins.d7
        );
    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        PORT_SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.d4
          );
      } else {
        PORT_SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port.data_port_num);
      }
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    if (!(lcd.hw.mode & BUS_4BIT)) {
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d0);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d1);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d2);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d3);
    }

    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d4);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d5);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d6);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d7);
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d0);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d1);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d2);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d4);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d5);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d6);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d7);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (0xF << p->data.d4);

#else /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) = 0xFF;

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
    );

#endif /* !AVR_LCD_4BIT */

  PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7,
    );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  PORT_SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(p->data_port_num, p->data.d4);

#else /* !AVR_LCD_4BIT */

  PORT_SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port_num);

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS


#ifndef AVR_LCD_4BIT

  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d0);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d1);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d2);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d4);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d5);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d6);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d7);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
  );

#endif /* AVR_LCD_4BIT */

  PORT_SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7
  );


#else


#ifdef AVR_LCD_4BIT

  AVR_UTIL_SET_DDR(AVR_LCD_DATA_PORT, (0xF << p->data.d4));

#else /* !AVR_LCD_4BIT */

  AVR_UTIL_OUTPUT_PORT(AVR_LCD_DATA_PORT);

#endif /* AVR_LCD_4BIT */


#endif


#endif


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */
}

void hd44780_set_pins(struct hardware_repr *p) {
  if (!p) return;

  lcd.hw = *p;

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

  if (lcd.hw.mode & PORT_ADDR) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d0);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d1);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d2);
        *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d3);
      }

      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);
      *(p->data_port.data_port_addr - 1) |= (1 << p->data.pins.d4);

    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) |= (0xF << p->data.d4);
      } else {
        /* DDR is located one byte leading PORT */
        *(p->data_port.data_port_addr - 1) = 0xFF;
      }
    }

    if (lcd.hw.mode & SERPARATE_PORTS) {
      /* DDR is located one byte leading PORT */
      *(p->ctl_port.addrs.rs_port_addr - 1) |= (1 << p->ctl.rs);
      *(p->ctl_port.addrs.en_port_addr - 1) |= (1 << p->ctl.en);
      if (p->ctl.rw != AVR_UTIL_PIN_NC) {
        *(p->ctl_port.addrs.rw_port_addr - 1) |= (1 << p->ctl.rw);
      }
      if (p->ctl.bl != AVR_UTIL_PIN_NC) {
        *(p->ctl_port.addrs.bl_port_addr - 1) |= (1 << p->ctl.bl);
        *(p->ctl_port.addrs.bl_port_addr) |= (1 << p->ctl.bl);
      }
    } else {
      /* DDR is located one byte leading PORT */
      *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.rs);
      *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.en);
      if (p->ctl.rw != AVR_UTIL_PIN_NC) {
        *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.rw);
      }
      if (p->ctl.bl != AVR_UTIL_PIN_NC) {
        *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.bl);
        *(p->ctl_port.ctl_port_addr) |= (1 << p->ctl.bl);
      }
    }

  } else if (lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.pins.d0,
            p->data.pins.d1,
            p->data.pins.d2,
            p->data.pins.d3
          );
      }

      PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->data_port.data_port_num,
          p->data.pins.d4,
          p->data.pins.d5,
          p->data.pins.d6,
          p->data.pins.d7
        );
    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        PORT_SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.d4
          );
      } else {
        PORT_SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port.data_port_num);
      }
    }

    if (lcd.hw.mode & SERPARATE_PORTS) {
      PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.rs_port_num, p->ctl.rs);
      PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.en_port_num, p->ctl.en);
      if (p->ctl.rw != AVR_UTIL_PIN_NC) {
        PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.rw_port_num, p->ctl.rw);
      }
      if (p->ctl.bl != AVR_UTIL_PIN_NC) {
        PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.bl_port_num, p->ctl.bl);
        PORT_SET_PIN_FROM_PORT_NUM(p->ctl_port.nums.bl_port_num, p->ctl.bl);
      }
    } else {
      PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->ctl_port.ctl_port_num,
          p->ctl.rs,
          p->ctl.en
        );
      if (p->ctl.rw != AVR_UTIL_PIN_NC) {
        PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.rw);
      }
      if (p->ctl.bl != AVR_UTIL_PIN_NC) {
        PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.bl);
        PORT_SET_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.bl);
      }
    }
  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    if (!(lcd.hw.mode & BUS_4BIT)) {
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d0);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d1);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d2);
      PORT_OUTPUT_PIN_NUMBER(p->data.pins.d3);
    }

    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d4);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d5);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d6);
    PORT_OUTPUT_PIN_NUMBER(p->data.pins.d7);

    PORT_OUTPUT_PIN_NUMBER(p->ctl.rs);
    PORT_OUTPUT_PIN_NUMBER(p->ctl.en);
    if (p->ctl.rw != AVR_UTIL_PIN_NC) {
      PORT_OUTPUT_PIN_NUMBER(p->ctl.rw);
    }
    if (p->ctl.bl != AVR_UTIL_PIN_NC) {
      PORT_OUTPUT_PIN_NUMBER(p->ctl.bl);
      PORT_SET_PIN_NUMBER(p->ctl.bl);
    }
  }

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_USE_PORT_ADDR


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d0);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d1);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d2);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d4);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d5);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d6);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d7);

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (0xF << p->data.d4);

#else /* !AVR_LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) = 0xFF;

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  /* DDR is located one byte leading PORT */
  *(p->rs_port_addr - 1) |= (1 << p->ctl.rs);
  *(p->en_port_addr - 1) |= (1 << p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    *(p->rw_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    *(p->bl_port_addr - 1) |= (1 << p->ctl.bl);
    *(p->bl_port_addr) |= (1 << p->ctl.bl);
  }

#else /* AVR_LCD_USE_SEPARATE_PORTS */

  /* DDR is located one byte leading PORT */
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.rs);
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.bl);
    *(p->ctl_port_addr) |= (1 << p->ctl.bl);
  }

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
    );

#endif /* !AVR_LCD_4BIT */

  PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7,
    );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  PORT_SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(p->data_port_num, p->data.d4);

#else /* !AVR_LCD_4BIT */

  PORT_SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port_num);

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->rs_port_num, p->ctl.rs);
  PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->en_port_num, p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->rw_port_num, p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->bl_port_num, p->ctl.bl);
    PORT_SET_PIN_FROM_PORT_NUM(p->bl_port_num, p->ctl.bl);
  }

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  PORT_SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->ctl_port_num, 
      p->ctl.rs,
      p->ctl.en
    );
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    PORT_SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.bl);
    PORT_SET_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.bl);
  }

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS


#ifndef AVR_LCD_4BIT

  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d0);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d1);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d2);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d4);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d5);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d6);
  PORT_OUTPUT_PIN_NUMBER(p->data.pins.d7);

  PORT_OUTPUT_PIN_NUMBER(p->ctl.rs);
  PORT_OUTPUT_PIN_NUMBER(p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    PORT_OUTPUT_PIN_NUMBER(p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    PORT_OUTPUT_PIN_NUMBER(p->ctl.bl);
    PORT_SET_PIN_NUMBER(p->ctl.bl);
  }

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  PORT_SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
  );

#endif /* AVR_LCD_4BIT */

  PORT_SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7
  );


#else


#ifdef AVR_LCD_4BIT

  AVR_UTIL_SET_DDR(AVR_LCD_DATA_PORT, (0xF << p->data.d4));

#else /* !AVR_LCD_4BIT */

  AVR_UTIL_OUTPUT_PORT(AVR_LCD_DATA_PORT);

#endif /* AVR_LCD_4BIT */


#endif


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  AVR_UTIL_OUTPUT_PIN(AVR_LCD_RS_PORT, p->ctl.rs);
  AVR_UTIL_OUTPUT_PIN(AVR_LCD_EN_PORT, p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    AVR_UTIL_OUTPUT_PIN(AVR_LCD_RW_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    AVR_UTIL_OUTPUT_PIN(AVR_LCD_BL_PORT, p->ctl.bl);
    /* Temporary */
    AVR_UTIL_SET_PIN(AVR_LCD_BL_PORT, p->ctl.bl);
  }

#else

  AVR_UTIL_OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.rs);
  AVR_UTIL_OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.en);
  if (p->ctl.rw != AVR_UTIL_PIN_NC) {
    AVR_UTIL_OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != AVR_UTIL_PIN_NC) {
    AVR_UTIL_OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.bl);
    /* Temporary */
    AVR_UTIL_SET_PIN(AVR_LCD_CTL_PORT, p->ctl.bl);
  }

#endif


#endif


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */
}

void hd44780_set_cursor(uint8_t row, uint8_t col) {
  hd44780_cmd(
      LCD_CMD_SET_DDRAMADDR |
      (!row ? LCD_CMD_DDRAMADDR_LINE1 : LCD_CMD_DDRAMADDR_LINE2) |
      col
    );
}

void hd44780_put_char(char c) {
  hd44780_data(c);
}

void hd44780_ddram_put_byte(uint8_t addr, unsigned char byte) {
  if (addr > 0x0f && addr < 0x28) {
    hd44780_set_ddram(0, addr);
  }
  if (addr > 0x4f && addr < 0x68) {
    hd44780_set_ddram(1, addr - 0x40);
  }

  hd44780_data(byte);
}

unsigned char hd44780_ddram_get_byte(uint8_t addr) {
  unsigned char byte = 0;

  if (addr > 0x0f && addr < 0x28) {
    hd44780_set_ddram(0, addr);
  }
  if (addr > 0x4f && addr < 0x68) {
    hd44780_set_ddram(1, addr - 0x40);
  }

  byte = hd44780_read_data();

  return byte;
}
