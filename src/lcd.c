/*
 * @file lcd.c
 * @author notweerdmonk
 * #brief HD44780 LCD module driver
 */

#include <common.h>
#include <utility.h>
//#include <coroutine.h>
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
  struct char_buffer buffer[LCD_ROWS][LCD_COLS];
} screen_buffer_t;

typedef struct lcd {
  struct screen_buffer screen;
  struct cursor cursor;

  //event_id_t rdy_ev;

  struct hardware_repr hw; /* Hardware representation */

  bool clr : 1;
  bool upt : 1;
  bool rdy : 1;

} lcd_t;

/*****************************************************************************/

static struct lcd lcd;

/*****************************************************************************/

#define SCREEN(x, y) screen.buffer[y][x]

#define CURX cursor.col
#define CURY cursor.row

#if defined LCD_RUNTIME_HW_REPR_SEL || \
  ( \
    (defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS) && \
    defined LCD_USE_NONCONTIGUOUS_DATA_PINS \
  )

#ifndef LCD_4BIT

static
void set_data_port_lower_nibble(
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

#endif /* !LCD_4BIT */

static
void set_data_port_upper_nibble(
    volatile uint8_t *port_addr,
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!port_addr || !p_hw) {
    return;
  }

  if (byte & 0x10) {
    *port_addr |= (1 << p_hw->data.pins.d4);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d4);
  }
  if (byte & 0x20) {
    *port_addr |= (1 << p_hw->data.pins.d5);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d5);
  }
  if (byte & 0x40) {
    *port_addr |= (1 << p_hw->data.pins.d6);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d6);
  }
  if (byte & 0x80) {
    *port_addr |= (1 << p_hw->data.pins.d7);
  } else {
    *port_addr &= ~(1 << p_hw->data.pins.d7);
  }
}

#endif /*
        * LCD_USE_RELATIVE_PIN_NUMBERS ||
        * LCD_USE_NONCONTIGUOUS_DATA_PINS
        */

#if defined LCD_RUNTIME_HW_REPR_SEL || defined LCD_USE_ABSOLUTE_PIN_NUMBERS

#ifndef LCD_4BIT

static
void set_data_pins_lower_nibble(
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

#endif /* !LCD_4BIT */

static
void set_data_pins_upper_nibble(
    struct hardware_repr *p_hw,
    uint8_t byte
  ) {

  if (!p_hw) {
    return;
  }

  if (byte & 0x10) {
    SET_PIN_NUMBER(p_hw->data.pins.d4);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d4);
  }
  if (byte & 0x20) {
    SET_PIN_NUMBER(p_hw->data.pins.d5);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d5);
  }
  if (byte & 0x40) {
    SET_PIN_NUMBER(p_hw->data.pins.d6);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d6);
  }
  if (byte & 0x80) {
    SET_PIN_NUMBER(p_hw->data.pins.d7);
  } else {
    CLR_PIN_NUMBER(p_hw->data.pins.d7);
  }
}

#endif /* LCD_RUNTIME_HW_REPR_SEL || LCD_USE_ABSOLUTE_PIN_NUMBERS */

static
void lcd_set_rs() {

#ifdef LCD_RUNTIME_HW_REPR_SEL

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

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = lcd.hw.rs_port_addr;

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* LCD_USE_RELATIVE_PIN_NUMBERS */ 


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.rs_port_num);

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif

  *rs_port_addr |= (1 << lcd.hw.ctl.rs);


#endif /* LCD_USE_PORT_ADDR */


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  SET_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_USE_SEPARATE_PORTS

  SET_PORT(LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !LCD_USE_SEPARATE_PORTS */

  SET_PORT(LCD_CTL_PORT, lcd.hw.ctl.rs);

#endif /* LCD_USE_SEPARATE_PORTS */


#endif /*
        * LCD_USE_PORT_ADDR || 
        * LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

}

static
void lcd_clear_rs() {

#ifdef LCD_RUNTIME_HW_REPR_SEL

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

#else /* !LCD_RUNTIME_HW_REPR_SEL */
 

#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = lcd.hw.rs_port_addr;

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *rs_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* LCD_USE_RELATIVE_PIN_NUMBERS */


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.rs_port_num);

#else

  volatile uint8_t *rs_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif

  *rs_port_addr &= ~(1 << lcd.hw.ctl.rs);


#endif


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  CLR_PIN_NUMBER(lcd.hw.ctl.rs);

#else /* 
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_USE_SEPARATE_PORTS

  CLR_PORT(LCD_RS_PORT, lcd.hw.ctl.rs);

#else /* !LCD_USE_SEPARATE_PORTS */

  CLR_PORT(LCD_CTL_PORT, lcd.hw.ctl.rs);

#endif /* LCD_USE_SEPARATE_PORTS */


#endif /*
        * LCD_USE_PORT_ADDR || 
        * LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

}

static
void lcd_set_en() {

#ifdef LCD_RUNTIME_HW_REPR_SEL

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

#else /* !LCD_RUNTIME_HW_REPR_SEL */
 

#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = lcd.hw.en_port_addr;

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *en_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* LCD_USE_RELATIVE_PIN_NUMBERS */


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.en_port_num);

#else

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.ctl_port_num);

#endif

  *en_port_addr |= (1 << lcd.hw.ctl.en);


#endif


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  SET_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_USE_SEPARATE_PORTS

  SET_PORT(LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !LCD_USE_SEPARATE_PORTS */

  SET_PORT(LCD_CTL_PORT, lcd.hw.ctl.en);

#endif /* LCD_USE_SEPARATE_PORTS */


#endif /*
        * LCD_USE_PORT_ADDR || 
        * LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

}

static
void lcd_clear_en() {

#ifdef LCD_RUNTIME_HW_REPR_SEL

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

#else /* !LCD_RUNTIME_HW_REPR_SEL */
 

#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = lcd.hw.en_port_addr;

#else /* !LCD_USE_SEPARATE_PORTS */

  volatile uint8_t *en_port_addr = lcd.hw.ctl_port_addr;

#endif


#else /* LCD_USE_RELATIVE_PIN_NUMBERS */


#ifdef LCD_USE_SEPARATE_PORTS

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.en_port_num);

#else

  volatile uint8_t *en_port_addr = port_num_to_addr(lcd.hw.en_port_num);

#endif

  *en_port_addr &= ~(1 << lcd.hw.ctl.en);


#endif


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  CLR_PIN_NUMBER(lcd.hw.ctl.en);

#else /* 
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_USE_SEPARATE_PORTS

  CLR_PORT(LCD_RS_PORT, lcd.hw.ctl.en);

#else /* !LCD_USE_SEPARATE_PORTS */

  CLR_PORT(LCD_CTL_PORT, lcd.hw.ctl.en);

#endif /* LCD_USE_SEPARATE_PORTS */


#endif /*
        * LCD_USE_PORT_ADDR || 
        * LCD_USE_RELATIVE_PIN_NUMBERS
        */


#endif /* LCD_RUNTIME_HW_REPR_SEL */
 

}

static
void lcd_toggle_en() {

  lcd_clear_en();
  DELAY_US(1);

  lcd_set_en();
  DELAY_US(2000);

  lcd_clear_en();
  DELAY_US(100);

}

#if defined LCD_RUNTIME_HW_REPR_SEL || defined LCD_4BIT

static
void lcd_write_nibble(uint8_t nibble) {

#ifdef LCD_RUNTIME_HW_REPR_SEL

  if (lcd.hw.mode & PORT_ADDR ||  lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *data_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      data_port_addr = lcd.hw.data_port.data_port_addr;
    } else {
      data_port_addr = port_num_to_addr(lcd.hw.data_port.data_port_num);
    }

    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      set_data_port_upper_nibble(data_port_addr, &lcd.hw, nibble);
    } else {
      *data_port_addr |= ((nibble & 0x0F) << lcd.hw.data.d4);
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    set_data_pins_upper_nibble(&lcd.hw, nibble);
  }

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr = lcd.hw.data_port_addr;

#else /* LCD_USE_RELATIVE_PIN_NUMBERS */
  
  volatile uint8_t *data_port_addr = port_num_to_addr(lcd.hw.data_port_num);

#endif /* LCD_USE_PORT_ADDR */


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS

  set_data_port_upper_nibble(data_port_addr, &lcd.hw, nibble);

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */

  *data_port_addr |= ((nibble & 0x0F) << lcd.hw.data.d4);

#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  set_data_pins_upper_nibble(&lcd.hw, nibble);

#else /*
        * !LCD_USE_PORT_ADDR &&
        * !LCD_USE_RELATIVE_PIN_NUMBERS &&
        * !LCD_USE_ABSOLUTE_PIN_NUMBERS
        */


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS

  set_data_port_upper_nibble(&CONCAT(PORT, LCD_DATA_PORT), &lcd.hw, nibble);

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */

  SET_PORT(LCD_DATA_PORT, ((nibble & 0x0F) << lcd.hw.data.d4));

#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */


#endif


#endif /* LCD_RUNTIME_HW_REPR_SEL */

  lcd_toggle_en();
}

#endif /* LCD_RUNTIME_HW_REPR_SEL || LCD_4BIT */

#if defined LCD_RUNTIME_HW_REPR_SEL || !defined LCD_4BIT

static
void lcd_write_byte(uint8_t byte) {

#ifdef LCD_RUNTIME_HW_REPR_SEL

  if (lcd.hw.mode & PORT_ADDR ||  lcd.hw.mode & RELATIVE_PIN_NUMBERS) {
    volatile uint8_t *data_port_addr = NULL;

    if (lcd.hw.mode & PORT_ADDR) {
      data_port_addr = lcd.hw.data_port.data_port_addr;
    } else {
      data_port_addr = port_num_to_addr(lcd.hw.data_port.data_port_num);
    }

    if (lcd.hw.mode & NONCONTIGUOUS_DATA_PINS) {
      set_data_port_lower_nibble(data_port_addr, &lcd.hw, byte);
      set_data_port_upper_nibble(data_port_addr, &lcd.hw, byte);
    } else {
      *data_port_addr = byte;
    }

  } else if (lcd.hw.mode & ABSOLUTE_PIN_NUMBERS) {
    set_data_pins_lower_nibble(&lcd.hw, byte);
    set_data_pins_upper_nibble(&lcd.hw, byte);
  }

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#if defined LCD_USE_PORT_ADDR || defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_PORT_ADDR

  volatile uint8_t *data_port_addr = lcd.hw.data_port_addr;

#else /* LCD_USE_RELATIVE_PIN_NUMBERS */

  volatile uint8_t *data_port_addr = port_num_to_addr(lcd.hw.data_port_num);

#endif /* LCD_USE_PORT_ADDR */


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS

  set_data_port_lower_nibble(data_port_addr, &lcd.hw, byte);
  set_data_port_upper_nibble(data_port_addr, &lcd.hw, byte);

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */

  *data_port_addr = byte;

#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS

  set_data_pins_lower_nibble(&lcd.hw, byte);
  set_data_pins_upper_nibble(&lcd.hw, byte);

#else /*
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS

  set_data_port_lower_nibble(&CONCAT(PORT, LCD_DATA_PORT), &lcd.hw, byte);
  set_data_port_upper_nibble(&CONCAT(PORT, LCD_DATA_PORT), &lcd.hw, byte);

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */

  SET_PORT(LCD_DATA_PORT, ((byte & 0x0F) << lcd.hw.data.d4));

#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */


#endif


#endif /* LCD_RUNTIME_HW_REPR_SEL */

  lcd_toggle_en();
}

#endif /* LCD_RUNTIME_HW_REPR_SEL || !LCD_4BIT */


static
void lcd_write(unsigned char byte) {
#ifdef LCD_RUNTIME_HW_REPR_SEL

  if (lcd.hw.mode & BUS_4BIT) {
    /* write upper nibble */
    lcd_write_nibble(byte & 0xF0);
    /* write lower nibble */
    lcd_write_nibble(byte << 4);
  } else {
  /* write byte */
  lcd_write_byte(byte);
  }

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#ifdef LCD_4BIT
  /* write upper nibble */
  lcd_write_nibble(byte & 0xF0);
  /* write lower nibble */
  lcd_write_nibble(byte << 4);

#else /* !LCD_4BIT */
  /* write byte */
  lcd_write_byte(byte);
#endif /* LCD_4BIT */


#endif /* LCD_RUNTIME_HW_REPR_SEL */

}

static
void lcd_data(unsigned char data) {
  lcd_set_rs();
  lcd_write(data);
}

static
void lcd_cmd(unsigned char cmd) {
  lcd_clear_rs();
  lcd_write(cmd);
}

/* TODO: 8-bit mode reset routine */
void lcd_reset() {

  lcd_clear_rs();
  lcd_clear_en();

  sleep(20);

#ifdef LCD_RUNTIME_HW_REPR_SEL
  if (lcd.hw.mode & BUS_4BIT) {
    lcd_write_nibble(0x03 << 4);
    sleep(5);

    lcd_write_nibble(0x03 << 4);
    sleep(5);

    lcd_write_nibble(0x03 << 4);
    sleep(1);

    lcd_write_nibble(0x02 << 4);
  }
#else /* LCD_RUNTIME_HW_REPR_SEL */


#ifdef LCD_4BIT
  lcd_write_nibble(0x03 << 4);
  sleep(5);

  lcd_write_nibble(0x03 << 4);
  sleep(5);

  lcd_write_nibble(0x03 << 4);
  sleep(1);

  lcd_write_nibble(0x02 << 4);
#endif /* LCD_4BIT */


#endif
}

/* TODO: 8-bit mode setup routine */

#ifdef LCD_RUNTIME_HW_REPR_SEL
void lcd_setup(
      uint8_t entry_mode,
      uint8_t display,
      uint8_t cursor_shift,
      uint8_t function
    ) {

  /* Set entry mode */
  if (entry_mode & LCD_CMD_ENTRY_MODE_SET) {
    lcd_cmd(entry_mode);
  }
  /* Set display characteristics */
  if (display & LCD_CMD_DISPLAY_CONTROL) {
    lcd_cmd(display);
  } else  {
    /* Display on, cusror on, blink on */
    lcd_cmd(
        LCD_CMD_DISPLAY_CONTROL |
        LCD_CMD_DISPLAY_ON |
        LCD_CMD_CURSOR_OFF |
        LCD_CMD_BLINK_OFF
      );
  }
  /* Set cursor characteristics */
  if (cursor_shift & LCD_CMD_CURSOR_SHIFT) {
    lcd_cmd(cursor_shift);
  }
  /* Set LCD function */
  if (function & LCD_CMD_FUNCTION_SET) {
    lcd_cmd(function);
  } else {
    /* 4-bit mode - 2 line - 5x8 font */
    lcd_cmd(
        LCD_CMD_FUNCTION_SET |
        ((lcd.hw.mode & BUS_4BIT) ? LCD_CMD_4BIT_MODE : LCD_CMD_8BIT_MODE) |
        LCD_CMD_2LINE |
        LCD_CMD_5x8DOTS
      );
  }

}

#else /* !LCD_RUNTIME_HW_REPR_SEL */

void lcd_setup() {

#ifdef LCD_ENTRY_MODE_SET
  lcd_cmd(
      LCD_CMD_ENTRY_MODE_SET |
#ifdef LCD_ENTRY_INCR
      LCD_CMD_ENTRY_INCR |
#else
      LCD_CMD_ENTRY_DECR |
#endif
#ifdef LCD_ENTRY_SHIFT_ON
      LCD_CMD_ENTRY_SHIFT_ON |
#else
      LCD_CMD_ENTRY_SHIFT_OFF |
#endif
    );
#endif /* LCD_ENTRY_MODE_SET */

#ifdef LCD_DISPLAY_CONTROL
  lcd_cmd(
      LCD_CMD_DISPLAY_CONTROL |
#ifdef LCD_DISPLAY_ON
      LCD_CMD_DISPLAY_ON |
#else
      LCD_CMD_DISPLAY_OFF |
#endif
#ifdef LCD_CURSOR_ON
      LCD_CMD_CURSOR_ON |
#else
      LCD_CMD_CURSOR_OFF |
#endif
#ifdef LCD_BLINK_ON
      LCD_CMD_BLINK_ON |
#else
      LCD_CMD_BLINK_OFF
#endif
    );
#endif /* LCD_DISPLAY_CONTROL */

#ifdef LCD_CURSOR_SHIFT
  lcd_cmd(
      LCD_CMD_CURSOR_SHIFT |
#ifdef LCD_DISPLAY_MOVE
      LCD_CMD_DISPLAY_MOVE |
#else
      LCD_CMD_CURSOR_MOVE |
#endif
#ifndef LCD_MOVE_LEFT
      LCD_CMD_MOVE_RIGHT |
#else
      LCD_CMD_MOVE_LEFT
#endif
    );
#endif /* LCD_CURSOR_SHIFT */

  lcd_cmd(
      LCD_CMD_FUNCTION_SET | 
#ifndef LCD_4BIT
      LCD_CMD_8BIT_MODE | 
#else
      LCD_CMD_4BIT_MODE | 
#endif
#ifndef LCD_1LINE
      LCD_CMD_2LINE | 
#else
      LCD_CMD_1LINE |
#endif
#ifdef LCD_5x10DOTS
      LCD_CMD_5x10DOTS
#else
      LCD_CMD_5x8DOTS
#endif
    );

  lcd_cmd(
      LCD_CMD_DISPLAY_CONTROL |
#ifndef LCD_DISPLAY_OFF
      LCD_CMD_DISPLAY_ON |
#else
      LCD_CMD_DISPLAY_OFF |
#endif
#ifdef LCD_CURSOR_ON
      LCD_CMD_CURSOR_ON |
#else
      LCD_CMD_CURSOR_OFF |
#endif
#ifdef LCD_BLINK_ON
      LCD_CMD_BLINK_ON |
#else
      LCD_CMD_BLINK_OFF
#endif
    );
}

#endif /* LCD_RUNTIME_HW_REPR_SEL */

UNUSED_FUNCTION
static
void lcd_set_ddram(uint8_t row, uint8_t col) {
  if ((row < LCD_ROWS) && (col < LCD_COLS)) {
    switch (row) {
      case 0:
        lcd_cmd(LCD_CMD_SET_DDRAMADDR | LCD_CMD_DDRAMADDR_LINE1 | col);
        break;

      case 1:
        lcd_cmd(LCD_CMD_SET_DDRAMADDR | LCD_CMD_DDRAMADDR_LINE2 | col);
        break;

      default:
        ;
    }
  }
}

void lcd_display() {
  uint8_t col, row, ddram_addr = LCD_CMD_SET_DDRAMADDR;
  bool set_ddram = FALSE;

  if (lcd.clr) {
    lcd_cmd(LCD_CMD_CLEAR_DISPLAY);
  }

  if (lcd.upt) {
    lcd_cmd(ddram_addr);

    for (row = 0; row < LCD_ROWS; row++) {
      if (row > 0) {
        ddram_addr |= LCD_CMD_DDRAMADDR_LINE2;
        lcd_cmd(ddram_addr);
      }

      //alive();

      for (col = 0; col < LCD_COLS; col++) {
        if (lcd.SCREEN(col, row).is_dirty) {
          if (set_ddram) {
            lcd_cmd(ddram_addr | col);
            set_ddram = FALSE;
          }

          lcd_data(lcd.SCREEN(col, row).c);
          lcd.SCREEN(col, row).is_dirty = FALSE;
        }
        else {
          if (lcd.clr) {
            lcd.SCREEN(col, row).c = 0;
          }

          set_ddram = TRUE;
        }
      }
    }

    lcd.clr = lcd.upt = FALSE;
  }
}

/*****************************************************************************/

void lcd_set_pins(struct hardware_repr *p) {
  if (!p) return;

  lcd.hw = *p;

#if 0

#ifdef LCD_RUNTIME_HW_REPR_SEL

  lcd.hw.data_port = p->data_port;

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#ifdef LCD_USE_PORT_ADDR

  lcd.hw.data_port_addr = p->data_port_addr;

#ifdef LCD_USE_SEPARATE_PORTS

  lcd.hw.rs_port_addr = p->rs_port_addr;
  lcd.hw.en_port_addr = p->en_port_addr;
  lcd.hw.rw_port_addr = p->rw_port_addr;
  lcd.hw.bl_port_addr = p->bl_port_addr;

#else /* LCD_USE_SEPARATE_PORTS */

  lcd.hw.ctl_port_addr = p->ctl_port_addr;

#endif /* LCD_USE_SEPARATE_PORTS */

#elif defined LCD_USE_RELATIVE_PIN_NUMBERS

  lcd.hw.data_port_num = p->data_port_num;

#ifdef LCD_USE_SEPARATE_PORTS

  lcd.hw.rs_port_num = p->rs_port_num;
  lcd.hw.en_port_num = p->en_port_num;
  lcd.hw.rw_port_num = p->rw_port_num;
  lcd.hw.bl_port_num = p->bl_port_num;

#else /* LCD_USE_SEPARATE_PORTS */

  lcd.hw.ctl_port_num = p->ctl_port_num;

#endif /* LCD_USE_SEPARATE_PORTS */

#endif


#endif /* LCD_RUNTIME_HW_REPR_SEL */


#endif

#ifdef LCD_RUNTIME_HW_REPR_SEL

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
      }
    } else {
      SET_OUTPUT_PINS_FROM_PORT_NUM(
          p->ctl_port.ctl_port_num,
          p->ctl.rs,
          p->ctl.en,
          p->ctl.rw,
          p->ctl.bl
        );
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

#else /* !LCD_RUNTIME_HW_REPR_SEL */


#ifdef LCD_USE_PORT_ADDR


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d0);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d1);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d2);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d3);

#endif /* !LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d4);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d5);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d6);
  *(p->data_port_addr - 1) |= (1 << p->data.pins.d7);

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef LCD_4BIT

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) |= (0xF << p->data.d4);

#else /* !LCD_4BIT */

  /* DDR is located one byte leading PORT */
  *(p->data_port_addr - 1) = 0xFF;

#endif /* LCD_4BIT */


#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef LCD_USE_SEPARATE_PORTS

  /* DDR is located one byte leading PORT */
  *(p->rs_port_addr - 1) |= (1 << p->ctl.rs);
  *(p->en_port_addr - 1) |= (1 << p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    *(p->rw_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    *(p->bl_port_addr - 1) |= (1 << p->ctl.bl);
  }

#else /* LCD_USE_SEPARATE_PORTS */

  /* DDR is located one byte leading PORT */
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.rs);
  *(p->ctl_port_addr - 1) |= (1 << p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    *(p->ctl_port_addr - 1) |= (1 << p->ctl.bl);
  }

#endif /* LCD_USE_SEPARATE_PORTS */


#elif defined LCD_USE_RELATIVE_PIN_NUMBERS


#ifdef LCD_USE_NONCONTIGUOUS_DATA_PINS


#ifndef LCD_4BIT

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d0,
      p->data.pins.d1,
      p->data.pins.d2,
      p->data.pins.d3,
    );

#endif /* !LCD_4BIT */

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->data_port_num,
      p->data.pins.d4,
      p->data.pins.d5,
      p->data.pins.d6,
      p->data.pins.d7,
    );

#else /* !LCD_USE_NONCONTIGUOUS_DATA_PINS */


#ifdef LCD_4BIT

  SET_OUTPUT_PORT_NIBBLE_FROM_PORT_NUM(p->data_port_num, p->data.d4);

#else /* !LCD_4BIT */

  SET_OUTPUT_PORT_BYTE_FROM_PORT_NUM(p->data_port_num);

#endif /* LCD_4BIT */

#endif /* LCD_USE_NONCONTIGUOUS_DATA_PINS */

#ifdef LCD_USE_SEPARATE_PORTS

  SET_OUTPUT_PIN_FROM_PORT_NUM(p->rs_port_num, p->ctl.rs);
  SET_OUTPUT_PIN_FROM_PORT_NUM(p->en_port_num, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->rw_port_num, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    SET_OUTPUT_PIN_FROM_PORT_NUM(p->bl_port_num, p->ctl.bl);
  }

#else /* !LCD_USE_SEPARATE_PORTS */

  SET_OUTPUT_PINS_FROM_PORT_NUM(
      p->ctl_port_num, 
      p->ctl.rs,
      p->ctl.en,
      p->ctl.rw,
      p->ctl.bl
    );

#endif /* LCD_USE_SEPARATE_PORTS */


#elif defined LCD_USE_ABSOLUTE_PIN_NUMBERS


#ifndef LCD_4BIT

  OUTPUT_PIN_NUMBER(p->data.pins.d0);
  OUTPUT_PIN_NUMBER(p->data.pins.d1);
  OUTPUT_PIN_NUMBER(p->data.pins.d2);
  OUTPUT_PIN_NUMBER(p->data.pins.d3);

#endif /* !LCD_4BIT */

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
       * !LCD_USE_PORT_ADDR &&
       * !LCD_USE_RELATIVE_PIN_NUMBERS &&
       * !LCD_USE_ABSOLUTE_PIN_NUMBERS
       */

  /* Data and control ports are defined as macros */

#ifdef LCD_8BIT

  OUTPUT_PORT(LCD_DATA_PORT);

#else /* LCD_4BIT */

  SET_DDR(LCD_DATA_PORT, (1 << p->data.d4));

#endif

#ifdef LCD_USE_SEPARATE_PORTS

  OUTPUT_PIN(LCD_RS_PORT, p->ctl.rs);
  OUTPUT_PIN(LCD_EN_PORT, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    OUTPUT_PIN(LCD_RW_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    OUTPUT_PIN(LCD_BL_PORT, p->ctl.bl);
  }

#else

  OUTPUT_PIN(LCD_CTL_PORT, p->ctl.rs);
  OUTPUT_PIN(LCD_CTL_PORT, p->ctl.en);
  if (p->ctl.rw != PIN_NC) {
    OUTPUT_PIN(LCD_CTL_PORT, p->ctl.rw);
  }
  if (p->ctl.bl != PIN_NC) {
    OUTPUT_PIN(LCD_CTL_PORT, p->ctl.bl);
  }

#endif

#endif


#endif /* LCD_RUNTIME_HW_REPR_SEL */

}

/**
 * Calling lcd_set_brightness before registering lcd_task will result in
 * overriding of the brightness value to LCD_BL_VALUE.
 */
void lcd_set_brightness(uint8_t value) {
  //set_software_pwm_value(LCD_PWM_CHANNEL, value);
}

bool is_lcd_ready() {
  return (bool)lcd.rdy;
}

//event_id_t get_lcd_ready_event() {
//  return lcd.rdy_ev;
//}

cursor_t lcd_get_cursor() {
  return lcd.cursor;
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  if ((row < LCD_ROWS) && (col < LCD_COLS)) {
    lcd.CURY = row;
    lcd.CURX = col;
  }
}

void lcd_clear() {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  lcd.CURX = lcd.CURY = 0;
  lcd.clr = TRUE;

  for (uint8_t row = 0; row < LCD_ROWS; row++) {
    for (uint8_t col = 0; col < LCD_COLS; col++) {
      lcd.SCREEN(col, row).is_dirty = FALSE;
    }
  }
}

void lcd_clear_till(uint8_t n) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  cursor_t prev_cursor = lcd.cursor;

  while (n--) lcd_put_char(' ');
  lcd_set_cursor(prev_cursor.row, prev_cursor.col);
}

void lcd_force_clear() {
  lcd_cmd(LCD_CMD_CLEAR_DISPLAY);
}

void lcd_put_char(char c) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  if ((lcd.CURY == LCD_ROWS) || (lcd.CURX == LCD_COLS)) {
    return;
  }

  char_buffer_t *p_pixel = &(lcd.SCREEN(lcd.CURX, lcd.CURY));
  if ((p_pixel->c != c) || lcd.clr){
    p_pixel->c = c;
    p_pixel->is_dirty = TRUE;
  }

  ++lcd.CURX;
  if (lcd.CURX == LCD_COLS) {
    lcd.CURY = (lcd.CURY + 1) % 2;
    lcd.CURX = 0;
  }

  lcd.upt = TRUE;
}

void lcd_put_string(char *str) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  while(*str) lcd_put_char(*str++);
}

void lcd_put_pgm_string(PGM_P s) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  for (char c = pgm_read_byte(s); c != 0; c = pgm_read_byte(++s)) {
    lcd_put_char(c);
  }
}

void lcd_put_uint(unsigned int u) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

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

  while (idx > 0) {
    lcd_put_char(ASCII(digits[--idx]));
  }
}

void lcd_put_int(int n) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  if(n < 0) {
    lcd_put_char('-');
    n = -n;
  }

  lcd_put_uint(n);
}

/* TODO: sprintf with -lprintf_flt or dtostrf, but bigger size */
void lcd_put_float(float f, uint8_t m) {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

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

void lcd_force_display() {
  //CHECK_RESOURCE_RETURN(SYS_RES_LCD);

  lcd_display();
}

//void* lcd_task(UNUSED_VARIABLE task_data_t data) {
//  COROUTINE_BEGIN();
//
//  lcd.rdy = FALSE;
//  lcd.rdy_ev = register_event();
//
//  COROUTINE_DELEGATE(lcd_reset);
//
//  lcd_setup();
//
//  REGISTER_SOFTWARE_PWM(LCD_PWM_CHANNEL, lcd.bl, LCD_BL_VALUE);
//
//  lcd.rdy = TRUE;
//  trigger_event(lcd.rdy_ev, EV_NOW);
//
//  COROUTINE_YIELD();
//
//  lcd_display();
//
//  COROUTINE_END();
//}
