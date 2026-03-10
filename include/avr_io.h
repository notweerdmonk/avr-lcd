/**
 * @file io.h
 * @author notweerdmonk
 * @brief Header file with utility and helper macros for GPIO
 */

#ifndef _AVR_USER_IO_H_
#define _AVR_USER_IO_H_

#include <stddef.h>

/* Pin that is not connected */
#define PIN_NC -1

/* Available PWM channels per timer corresponding two channels per timer */
typedef enum _pwm_channel_id {
  PWM_CHANNEL_A,
  PWM_CHANNEL_B,
} pwm_channel_id_t;

/*****************************************************************************/
/* Register manipulation */
/*****************************************************************************/
#define WRITE_DDR(x, y)     (CONCAT(DDR,x) = (y))
#define WRITE_PORT(x, y)    (CONCAT(PORT,x) = (y))

#define READ_DDR(x)         (CONCAT(DDR,(x)))
#define READ_PORT(x)        (CONCAT(PORT,(x)))

#define SET_DDR(x, y)       (CONCAT(DDR,x) |= (y))
#define CLR_DDR(x, y)       (CONCAT(DDR,x) &= ~(y))

#define SET_PORT(x, y)      (CONCAT(PORT,x) |= (y))
#define CLR_PORT(x, y)      (CONCAT(PORT,x) &= ~(y))
#define ZERO_PORT(x)        (CONCAT(PORT,x) = 0)

#define OUTPUT_PORT(x)      (CONCAT(DDR,x) = 0xFF)
#define INPUT_PORT(x)       (CONCAT(DDR,x) = 0x00)

#define OUTPUT_PIN(x, y)    (CONCAT(DDR,x) |= (BITMASK((y))))
#define INPUT_PIN(x, y)     (CONCAT(DDR,x) &=	~(BITMASK((y))))

#define READ_PIN(x, y)      (CONCAT(PIN,x) & (BITMASK((y))))
#define SET_PIN(x, y)       (CONCAT(PORT,x) |= (BITMASK((y))))
#define CLR_PIN(x, y)       (CONCAT(PORT,x) &= ~(BITMASK((y))))

/* Takes number literals as argument for bit */
#define OUTPUT_PIN_IMM(x, y) \
  asm volatile(                               \
      "sbi %[ddr], %[bit]\n"                  \
      :: [ddr] "I" _SFR_IO_ADDR(CONCAT(DDR, x)), \
      [bit] "I" (y))

#define INPUT_PIN_IMM(x, y) \
  asm volatile(                               \
      "cbi %[ddr], %[bit]\n"                  \
      :: [ddr] "I" _SFR_IO_ADDR(CONCAT(DDR, x)), \
      [bit] "I" (y))

/* Takes number literals as argument for bit */
#define SET_PIN_IMM(x, y) \
  asm volatile(                                 \
      "sbi %[port], %[bit]\n"                   \
      :: [port] "I" _SFR_IO_ADDR(CONCAT(PORT, x)), \
      [bit] "I" (y))

#define CLR_PIN_IMM(x, y) \
  asm volatile(                                 \
      "cbi %[port], %[bit]\n"                   \
      :: [port] "I" _SFR_IO_ADDR(CONCAT(PORT, x)), \
      [bit] "I" (y))

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || \
    defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || \
    defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || \
    defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || \
    defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#define TOGGLE_PIN(x, y)   (CONCAT(PIN, x) |= (BITMASK(y)))

/* Takes number literals as argument for bit */
#define TOGGLE_PIN_IMM(x, y) \
  asm volatile(                               \
      "sbi %[pin], %[bit]\n"                  \
      :: [pin] "I" _SFR_IO_ADDR(CONCAT(PIN, x)), \
      [bit] "I" (y))

#else

#define TOGGLE_PIN(x, y)   (CONCAT(PORT,x) ^= (BITMASK(y)))

#endif

#define POLL_PIN_LEVEL(port, pin, state) \
  ({                                                      \
    static uint8_t _debounce = 0;                         \
    if ( (CONCAT(PIN,port) & (BITMASK(pin))) == state ) \
      {if (++_debounce == -1) _debounce = 3;}             \
    else _debounce = 0;                                   \
    (_debounce >= 3) ? TRUE : FALSE;                      \
  })

#endif /* _AVR_USER_IO_H_ */
