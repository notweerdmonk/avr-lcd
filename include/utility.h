/**
 * @file utility.h
 * @author notweerdmonk
 * @brief header file with utility and helper macros
 */

#ifndef _AVR_USER_UTILITY_H_
#define _AVR_USER_UTILITY_H_

#define TIMER_VAL_FROM_SEC(x)   (x)*1000

/*****************************************************************************/
/* Attributes                                                                */
/*****************************************************************************/
#define UNUSED_FUNCTION __attribute__ (( unused ))

#define UNUSED_VARIABLE __attribute__ (( unused ))

#define DEPRECATED_FUNCTION(msg) __attribute__ (( deprecated(msg) ))

#define DEPRECATED_VARIABLE(msg) __attribute__ (( deprecated(msg) ))

/*****************************************************************************/
/* Preprocessor utilities                                                    */
/*****************************************************************************/
#define CONCAT_EXPAND(A, B) A ## B
#define CONCAT(A, B) CONCAT_EXPAND(A, B)

#define STRINGIFY_HELPER(s) #s
#define STRINGIFY(s) STRINGIFY_HELPER(s)

/*****************************************************************************/
/* Bitmask manipulation                                                      */
/*****************************************************************************/
/* Get 8-bit mask from 8-bit value */
#define BITMASK(byte) \
  ({                       \
    register uint8_t mask; \
    __asm__ __volatile__(  \
        "ldi %0, 1"           "\n\t" \
        "mov __tmp_reg__, %1" "\n\t" \
        "rjmp .+2"            "\n\t" \
        "lsl %0"              "\n\t" \
        "dec __tmp_reg__"     "\n\t" \
        "brpl .-6"            "\n\t" \
        : "=&r" (mask)               \
        : "r"   (byte)               \
        : "r0" );                    \
    mask;                            \
  })

#define SET_MASK(x, y)      (x) |= (y)
#define CLR_MASK(x, y)      (x) &= ~(y)
#define ZERO_MASK(x)        (x) = 0

#define SET_BIT(x, y)       (x) |= BITMASK(y)
#define CLR_BIT(x, y)       (x) &= ~BITMASK(y)
#define GET_BIT(x, y)       ((x) & BITMASK(y))

/*****************************************************************************/
/* Atomic operations                                                         */
/*****************************************************************************/
#define ATOMIC_COPY_BYTE(desc, src) \
  do { cli(); dest = src; sei(); } while (0)

#define ATOMIC_COPY_WORD(dest, src) \
  do { cli(); dest = src; sei(); } while (0)

/*****************************************************************************/
/* EEPROM memory access                                                      */
/*****************************************************************************/
#define GET_PROGMEM_STR(pgm_ptr) \
  ({                                                   \
     char __buf[128];                                  \
     strcpy_P(__buf, (PGM_P)pgm_read_word((pgm_ptr))); \
     __buf;                                            \
  })

/*****************************************************************************/
/* Data format conversion                                                    */
/*****************************************************************************/
#define HEX_TO_BYTE(hex_ptr) \
  (uint8_t)( (hex_to_nibble(hex_ptr) << 4) | \
             (hex_to_nibble(hex_ptr + 1)) )

#define HEX_TO_WORD(hex_ptr) \
  (uint16_t)( (hex_to_nibble(hex_ptr) << 12) |    \
              (hex_to_nibble(hex_ptr + 1) << 8) | \
              (hex_to_nibble(hex_ptr + 2) << 4) | \
              (hex_to_nibble(hex_ptr + 3)) )

void byte_to_hex(uint8_t byte, char *str);

void word_to_hex(uint16_t word, char *str);

uint8_t hex_to_nibble(const char *hex);

#ifdef AVR_UTILITY_IMPL

static const char PROGMEM hex_lut[] = \
  {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void byte_to_hex(uint8_t byte, char *str) {
  *str++ = pgm_read_byte(&hex_lut[(byte >> 4) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[byte & 0xf]);
  *str = 0;
}

void word_to_hex(uint16_t word, char *str) {
  *str++ = pgm_read_byte(&hex_lut[(word >> 12) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[(word >> 8) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[(word >> 4) & 0xf]);
  *str++ = pgm_read_byte(&hex_lut[word & 0xf]);
  *str = 0;
}

uint8_t hex_to_nibble(const char *hex) {
  uint8_t byte = *hex;
  if (byte >= 'a') byte = (byte - 'a') + 10;
  else if (byte >= 'A') byte = (byte - 'A') + 10;
  else byte -= '0';
  return byte;
}

#endif /* AVR_UTILITY_IMPL */

#endif /* _AVR_USER_UTILITY_H_ */
