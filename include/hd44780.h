/**
 * @file hd44780.h
 * @brief HD44780 LCD controller hardware representation
 * @author notweerdmonk
 * 
 * This file defines the hardware_repr structure used to represent
 * the physical connections between the AVR microcontroller and the
 * HD44780-compatible LCD display.
 * 
 * @note This header is automatically included by avr_lcd.h
 */

#ifndef _AVR_LCD_HD44780_H_
#define _AVR_LCD_HD44780_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Hardware representation structure for LCD connections
 * 
 * This structure defines how the LCD is physically connected to the AVR.
 * The structure adapts based on compilation flags:
 * - AVR_LCD_RUNTIME_HARDWARE_REPR: All fields available for runtime configuration
 * - AVR_LCD_USE_PORT_ADDR: Use direct port addresses
 * - AVR_LCD_USE_RELATIVE_PIN_NUMBERS: Use port numbers (PORTA=0, PORTB=1, etc.)
 * - AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS: Use Arduino-style pin numbers
 */
typedef struct hardware_repr {

/** @brief Size of pin bitfield (3 for standard, 6 for extended) */
#define __AVR_LCD_PIN_SIZE 3

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR

#undef __AVR_LCD_PIN_SIZE
#define __AVR_LCD_PIN_SIZE 6

  /** @brief Hardware representation mode flags */
  uint8_t mode;

  /** @brief Data port configuration (union for different modes) */
  union {
    volatile uint8_t *data_port_addr;   ///< Direct port address 
    uint8_t data_port_num;              ///< Relative port number 
  } data_port;

  /** @brief Control port configuration (union for different modes) */
  union {
    volatile uint8_t *ctl_port_addr;    ///< Direct control port address 
    struct {
      volatile uint8_t *rs_port_addr;   ///< RS pin port address 
      volatile uint8_t *en_port_addr;   ///< EN pin port address 
      volatile uint8_t *rw_port_addr;   ///< RW pin port address 
      volatile uint8_t *bl_port_addr;   ///< Backlight pin port address 
    } addrs;
    uint8_t ctl_port_num;               ///< Relative control port number 
    struct {
      uint8_t rs_port_num;              ///< RS pin port number 
      uint8_t en_port_num;              ///< EN pin port number 
      uint8_t rw_port_num;              ///< RW pin port number 
      uint8_t bl_port_num;              ///< Backlight pin port number 
    } nums;
  } ctl_port;

#else /* !AVR_LCD_RUNTIME_HARDWARE_REPR */


#ifdef AVR_LCD_USE_PORT_ADDR

  /** @brief Data port address (direct I/O register) */
  volatile uint8_t *data_port_addr;

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  /** @brief Register Select pin port address */
  volatile uint8_t *rs_port_addr;
  /** @brief Enable pin port address */
  volatile uint8_t *en_port_addr;
  /** @brief Read/Write pin port address */
  volatile uint8_t *rw_port_addr;
  /** @brief Backlight pin port address */
  volatile uint8_t *bl_port_addr;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  /** @brief Control signals port address */
  volatile uint8_t *ctl_port_addr;

#endif /* AVR_LCD_USE_SEPARATE_PORTS */

#elif defined AVR_LCD_USE_RELATIVE_PIN_NUMBERS

  /**
   * @brief Data port number
   * 
   * Port numbering:
   * - PORTA = 0
   * - PORTB = 1
   * - PORTC = 2
   * - PORTD = 3
   * - PORTE = 4
   * - PORTF = 5
   */
  uint8_t data_port_num : 3;

#ifdef AVR_LCD_USE_SEPARATE_PORTS

  /** @brief Register Select pin port number */
  uint8_t rs_port_num : 3;
  /** @brief Enable pin port number */
  uint8_t en_port_num : 3;
  /** @brief Read/Write pin port number */
  uint8_t rw_port_num : 3;
  /** @brief Backlight pin port number */
  uint8_t bl_port_num : 3;

#else /* !AVR_LCD_USE_SEPARATE_PORTS */

  /** @brief Control port number */
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

  /** @brief Data pin mappings (bitfield for each data line) */
  union {
    struct __attribute__((packed)) {
      uint8_t d0 : __AVR_LCD_PIN_SIZE;  ///< D0 pin 
      uint8_t d1 : __AVR_LCD_PIN_SIZE;  ///< D1 pin 
      uint8_t d2 : __AVR_LCD_PIN_SIZE;  ///< D2 pin 
      uint8_t d3 : __AVR_LCD_PIN_SIZE;  ///< D3 pin 
      uint8_t d4 : __AVR_LCD_PIN_SIZE;  ///< D4 pin 
      uint8_t d5 : __AVR_LCD_PIN_SIZE;  ///< D5 pin 
      uint8_t d6 : __AVR_LCD_PIN_SIZE;  ///< D6 pin 
      uint8_t d7 : __AVR_LCD_PIN_SIZE;  ///< D7 pin 
    } pins;

    uint8_t d4;  /**< Lower nibble (for 4-bit mode) */

  } data;

#else

#if defined AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS || \
  defined AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS

  /** @brief Data pin mappings for non-contiguous pins */
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

  /** @brief Data pins (4-bit mode uses only D4-D7) */
  union {
    uint8_t d4;
  } data;

#endif /* AVR_LCD_4BIT */


#endif/*
       * AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS ||
       * AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS
       */


#endif /* AVR_LCD_RUNTIME_HARDWARE_REPR */

  /** @brief Control pin definitions */
  struct __attribute__((packed)) {
    uint8_t rs : __AVR_LCD_PIN_SIZE;  ///< Register Select pin
    uint8_t en : __AVR_LCD_PIN_SIZE;  ///< Enable pin
    uint8_t rw : __AVR_LCD_PIN_SIZE;  ///< Read/Write pin (255 if not used)
    uint8_t bl : __AVR_LCD_PIN_SIZE;  ///< Backlight pin (255 if not used)
  } ctl;

} hardware_repr_t;

/**
 * @brief Set LCD pin configuration
 * @param p Hardware representation pointer
 */
void hd44780_set_pins(struct hardware_repr *p);

/**
 * @brief Set backlight brightness
 * @param value Brightness value (0-255)
 */
void hd44780_set_brightness(uint8_t value);

/**
 * @brief Reset LCD controller
 */
void hd44780_reset();

#ifdef AVR_LCD_RUNTIME_HARDWARE_REPR
/**
 * @brief Initialize LCD with runtime configuration
 * @param entry_mode Entry mode flags
 * @param display Display control flags
 * @param cursor_shift Cursor/shift flags
 * @param function Function set flags
 */
void hd44780_setup(
    uint8_t entry_mode,
    uint8_t display,
    uint8_t cursor_shift,
    uint8_t function
  );
#else
/**
 * @brief Initialize LCD with compile-time configuration
 */
void hd44780_setup();
#endif

/**
 * @brief Set cursor position
 * @param row Row number (0-indexed)
 * @param col Column number (0-indexed)
 */
void hd44780_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Write character to LCD
 * @param c Character to write
 */
void hd44780_put_char(char c);

#endif /* _AVR_LCD_HD44780_H_ */
