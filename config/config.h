/*
 * config.h
 *
 * Created: 09-07-2017 12:50:22
 *  Author: weerdmonk 
 */


#ifndef CONFIG_H_
#define CONFIG_H_

/* Versioning */
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define REVISION      0

/* F_CPU */
/* !!! IMPORTANT !!! always define F_CPU before including any AVR headers */
#ifndef F_CPU
#error *** F_CPU not defined! ***
#endif

/*
 * LCD hardware representation modes selection
 *  - LCD_RUNTIME_HW_REPR_SEL
 *
 *  - LCD_USE_PORT_ADDR
 *  - LCD_USE_RELATIVE_PIN_NUMBERS
 *    - LCD_USE_SEPARATE_PORTS
 *  - LCD_USE_ABSOLUTE_PIN_NUMBERS
 *
 *  - LCD_USE_NONCONTIGUOUS_DATA_PINS
 */

/* Define your selection here */
//#define LCD_USE_ABSOLUTE_PIN_NUMBERS
//#define LCD_USE_NONCONTIGUOUS_DATA_PINS
//#define LCD_RUNTIME_HW_REPR_SEL

/*
 * LCD data bus width selection
 *  - LCD_4BIT : Use 4 bit bus or else use 8 bit bus.
 */

/* Define your selection here */
#define LCD_4BIT

#endif /* CONFIG_H_ */
