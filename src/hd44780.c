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
#include <port.h>

/*
 * Works using busy wait method
 * 
 * TODO: add busy poll method
 * TODO: supports HD44780 controller only, add a way to determine controller
 */

/*****************************************************************************/

/*****************************************************************************/
#if defined AVR_LCD_RUNTIME_HARDWARE_REPR || \
  ( \
    (defined AVR_LCD_USE_PORT_ADDR || defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS) && \
    defined AVR_LCD_USE_SEPARATE_PORTS \
  ) || \
  defined AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS

#ifndef AVR_LCD_4BIT

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

#define hd44780_set_data_port_lower_nibble(x)

#endif /* AVR_LCD_4BIT */

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
void hd44780_set_data_pins_lower_nibble(
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!p_hw) {
    return;
  }

  if (byte & 0x01) {
    SET_PIN_NUMBER(p_hw->data.pins.d0);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d0);
  }
  if (byte & 0x02) {
    SET_PIN_NUMBER(p_hw->data.pins.d1);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d1);
  }
  if (byte & 0x04) {
    SET_PIN_NUMBER(p_hw->data.pins.d2);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d2);
  }
  if (byte & 0x08) {
    SET_PIN_NUMBER(p_hw->data.pins.d3);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d3);
  }
}

#endif /* AVR_LCD_4BIT */

static
void hd44780_set_data_pins_upper_nibble(
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!p_hw) {
    return;
  }

  if (byte & 0x01) {
    SET_PIN_NUMBER(p_hw->data.pins.d4);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d4);
  }
  if (byte & 0x02) {
    SET_PIN_NUMBER(p_hw->data.pins.d5);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d5);
  }
  if (byte & 0x04) {
    SET_PIN_NUMBER(p_hw->data.pins.d6);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d6);
  }
  if (byte & 0x08) {
    SET_PIN_NUMBER(p_hw->data.pins.d7);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d7);
  }
}

#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR || AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS */

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
    SET_PIN_NUMBER(lcd.hw.ctl.rs);
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

  SET_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  SET_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  SET_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rs);

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
    CLR_PIN_NUMBER(lcd.hw.ctl.rs);
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

  CLR_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  CLR_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  CLR_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.rs);

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
    SET_PIN_NUMBER(lcd.hw.ctl.en);
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

  SET_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  SET_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  SET_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.en);

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
    CLR_PIN_NUMBER(lcd.hw.ctl.en);
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

  CLR_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  CLR_PIN(AVR_LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  CLR_PIN(AVR_LCD_CTL_PORT, lcd.hw.ctl.en);

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

  //*data_port_addr |= ((nibble & 0x0F) << lcd.hw.data.d4);
  uint8_t data_port_byte = *data_port_addr;
  CLR_MASK(data_port_byte, 0xF << lcd.hw.data.d4);
  SET_MASK(data_port_byte, (nibble & 0xF) << lcd.hw.data.d4);
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

  uint8_t data_port_byte = READ_PORT(AVR_LCD_DATA_PORT);
  CLR_MASK(data_port_byte, 0xF << lcd.hw.data.d4);
  SET_MASK(data_port_byte, (nibble & 0xF) << lcd.hw.data.d4);
  WRITE_PORT(AVR_LCD_DATA_PORT, data_port_byte);

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

  WRITE_PORT(AVR_LCD_DATA_PORT, byte);

#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#endif


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  hd44780_toggle_en();
}

#else /* AVR_LCD_4BIT */

#define hd44780_write_byte(x)

#endif /* !AVR_LCD_4BIT */


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

    //hd44780_write_nibble(0x02);
  }

#else /* AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_4BIT
  hd44780_write_nibble(0x03);
  _delay_ms(5);

  hd44780_write_nibble(0x03);
  _delay_ms(1);

  hd44780_write_nibble(0x03);
  _delay_ms(1);

  //hd44780_write_nibble(0x02);
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

//#ifdef LCD_DISPLAY_CONTROL
//  hd44780_cmd(
//      LCD_CMD_DISPLAY_CONTROL
//#ifdef AVR_LCD_DISPLAY_OFF
//      | LCD_CMD_DISPLAY_OFF
//#else
//      | LCD_CMD_DISPLAY_ON
//#endif
//#ifdef AVR_LCD_CURSOR_ON
//      | LCD_CMD_CURSOR_ON
//#else
//      | LCD_CMD_CURSOR_OFF
//#endif
//#ifdef AVR_LCD_BLINK_ON
//      | LCD_CMD_BLINK_ON
//#else
//      | LCD_CMD_BLINK_OFF
//#endif
//    );
//#endif /* LCD_DISPLAY_CONTROL */

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
  if ((row < AVR_LCD_ROWS) && (col < AVR_LCD_COLS)) {
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

/*****************************************************************************/

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
      if (p->ctl.rw != PIN_NC) {
        *(p->ctl_port.addrs.rw_port_addr - 1) |= (1 << p->ctl.rw);
      }
      if (p->ctl.bl != PIN_NC) {
        *(p->ctl_port.addrs.bl_port_addr - 1) |= (1 << p->ctl.bl);
        *(p->ctl_port.addrs.bl_port_addr) |= (1 << p->ctl.bl);
      }
    } else {
      /* DDR is located one byte leading PORT */
      *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.rs);
      *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.en);
      if (p->ctl.rw != PIN_NC) {
        *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.rw);
      }
      if (p->ctl.bl != PIN_NC) {
        *(p->ctl_port.ctl_port_addr - 1) |= (1 << p->ctl.bl);
        *(p->ctl_port.ctl_port_addr) |= (1 << p->ctl.bl);
      }
    }

  } else if (lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      if (!(lcd.hw.mode & BUS_4BIT)) {
        SET_OUTPUT_PINS_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.pins.d0,
            p->data.pins.d1,
            p->data.pins.d2,
            p->data.pins.d3
          );
      }

      SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->data_port.data_port_num,
          p->data.pins.d4,
          p->data.pins.d5,
          p->data.pins.d6,
          p->data.pins.d7
        );
    } else {
      if (lcd.hw.mode & BUS_4BIT) {
        SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(
            p->data_port.data_port_num,
            p->data.d4
          );
      } else {
        SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port.data_port_num);
      }
    }

    if (lcd.hw.mode & SERPARATE_PORTS) {
      SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.rs_port_num, p->ctl.rs);
      SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.en_port_num, p->ctl.en);
      if (p->ctl.rw != PIN_NC) {
        SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.rw_port_num, p->ctl.rw);
      }
      if (p->ctl.bl != PIN_NC) {
        SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.nums.bl_port_num, p->ctl.bl);
        SET_PIN_FROM_PORT_NUM(p->ctl_port.nums.bl_port_num, p->ctl.bl);
      }
    } else {
      SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->ctl_port.ctl_port_num,
          p->ctl.rs,
          p->ctl.en
        );
      if (p->ctl.rw != PIN_NC) {
        SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.rw);
      }
      if (p->ctl.bl != PIN_NC) {
        SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.bl);
        SET_PIN_FROM_PORT_NUM(p->ctl_port.ctl_port_num, p->ctl.bl);
      }
    }
  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    if (!(lcd.hw.mode & BUS_4BIT)) {
      OUTPUT_PIN_NUMBER(p->data.pins.d0);
      OUTPUT_PIN_NUMBER(p->data.pins.d1);
      OUTPUT_PIN_NUMBER(p->data.pins.d2);
      OUTPUT_PIN_NUMBER(p->data.pins.d3);
    }

    OUTPUT_PIN_NUMBER(p->data.pins.d4);
    OUTPUT_PIN_NUMBER(p->data.pins.d5);
    OUTPUT_PIN_NUMBER(p->data.pins.d6);
    OUTPUT_PIN_NUMBER(p->data.pins.d7);

    OUTPUT_PIN_NUMBER(p->ctl.rs);
    OUTPUT_PIN_NUMBER(p->ctl.en);
    if (p->ctl.rw != PIN_NC) {
      OUTPUT_PIN_NUMBER(p->ctl.rw);
    }
    if (p->ctl.bl != PIN_NC) {
      OUTPUT_PIN_NUMBER(p->ctl.bl);
      SET_PIN_NUMBER(p->ctl.bl);
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
  if (p->ctl.rw != PIN_NC) {
    *(p->rw_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    *(p->bl_port_addr - 1) |= (1 << p->ctl.bl);
    *(p->bl_port_addr) |= (1 << p->ctl.bl);
  }

#else /* AVR_LCD_USE_SEPARATE_PORTS */

  /* DDR is located one byte leading PORT */
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.rs);
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.bl);
    *(p->ctl_port_addr) |= (1 << p->ctl.bl);
  }

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
    );

#endif /* !AVR_LCD_4BIT */

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7,
    );

#else /* !AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef AVR_LCD_4BIT

  SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(p->data_port_num, p->data.d4);

#else /* !AVR_LCD_4BIT */

  SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port_num);

#endif /* AVR_LCD_4BIT */


#endif /* AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS */

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  SET_OUTPUT_PIN_FROM_PORT_NUM(p->rs_port_num, p->ctl.rs);
  SET_OUTPUT_PIN_FROM_PORT_NUM(p->en_port_num, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->rw_port_num, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->bl_port_num, p->ctl.bl);
    SET_PIN_FROM_PORT_NUM(p->bl_port_num, p->ctl.bl);
  }

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->ctl_port_num, 
      p->ctl.rs,
      p->ctl.en
    );
  if (p->ctl.rw != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.bl);
    SET_PIN_FROM_PORT_NUM(p->ctl_port_num, p->ctl.bl);
  }

#endif /* AVR_LCD_USE_SEPARATE_PORTS */


#elif defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS


#ifndef AVR_LCD_4BIT

  OUTPUT_PIN_NUMBER(p->data.pins.d0);
  OUTPUT_PIN_NUMBER(p->data.pins.d1);
  OUTPUT_PIN_NUMBER(p->data.pins.d2);
  OUTPUT_PIN_NUMBER(p->data.pins.d3);

#endif /* !AVR_LCD_4BIT */

  OUTPUT_PIN_NUMBER(p->data.pins.d4);
  OUTPUT_PIN_NUMBER(p->data.pins.d5);
  OUTPUT_PIN_NUMBER(p->data.pins.d6);
  OUTPUT_PIN_NUMBER(p->data.pins.d7);

  OUTPUT_PIN_NUMBER(p->ctl.rs);
  OUTPUT_PIN_NUMBER(p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    OUTPUT_PIN_NUMBER(p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    OUTPUT_PIN_NUMBER(p->ctl.bl);
    SET_PIN_NUMBER(p->ctl.bl);
  }

#else /* 
       * !AVR_LCD_USE_PORT_ADDR &&
       * !AVR_LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#ifdef AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef AVR_LCD_4BIT

  SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
  );

#endif /* AVR_LCD_4BIT */

  SET_PORT_OUTPUT_PINS(
      AVR_LCD_DATA_PORT,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7
  );


#else


#ifdef AVR_LCD_4BIT

  SET_DDR(AVR_LCD_DATA_PORT, (0xF << p->data.d4));

#else /* !AVR_LCD_4BIT */

  OUTPUT_PORT(AVR_LCD_DATA_PORT);

#endif /* AVR_LCD_4BIT */


#endif


#ifdef AVR_LCD_USE_SEPARATE_PORTS

  OUTPUT_PIN(AVR_LCD_RS_PORT, p->ctl.rs);
  OUTPUT_PIN(AVR_LCD_EN_PORT, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    OUTPUT_PIN(AVR_LCD_RW_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    OUTPUT_PIN(AVR_LCD_BL_PORT, p->ctl.bl);
    /* Temporary */
    SET_PIN(AVR_LCD_BL_PORT, p->ctl.bl);
  }

#else

  OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.rs);
  OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    OUTPUT_PIN(AVR_LCD_CTL_PORT, p->ctl.bl);
    /* Temporary */
    SET_PIN(AVR_LCD_CTL_PORT, p->ctl.bl);
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
