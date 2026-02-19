/*
 * @file config.h
 * @author notweerdmonk 
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

/*****************************************************************************/
/* LCD Type
 *
 * - hd44780
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_TYPE hd44780
/*****************************************************************************/

/*****************************************************************************/
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
/*****************************************************************************/
/* Define your selection here */

/*****************************************************************************/

/*****************************************************************************/
/*
 * LCD data bus width selection
 *  - LCD_4BIT : Use 4 bit bus or else use 8 bit bus.
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_4BIT
/*****************************************************************************/

/*****************************************************************************/
/* Buffered mode
 *  - LCD_BUFFERED : Writes to RAM buffer and updates the LCD in separate cycle
 */
/*****************************************************************************/
/* Define your selection here */
#define LCD_BUFFERED
/*****************************************************************************/

/*****************************************************************************/
/*
 * Instructions
 *  - LCD_DISPLAY_OFF
 *  - LCD_CURSOR_ON
 *  - LCD_BLINK_ON
 *  - LCD_ENTRY_DECR
 *  - LCD_ENTRY_SHIFT
 *  - LCD_CURSOR_MOVE_LEFT
 *  - LCD_DISPLAY_MOVE_LEFT
 */
/*****************************************************************************/
/* Define your selection here */

/*****************************************************************************/

/*****************************************************************************/
/*
 * Define macros in this file to configure features and compilation behaviour.
 * Uncomment the line(s) which contain definition of desired macro(s).
 */
/*****************************************************************************/
/* Enable runtime UART configuration (call uart_setup with config struct) */
//#define AVR_UART_RUNTIME_CONFIG

/* Enable use of UART in I/O streams (stdin/stdout/stderr) */
//#define AVR_UART_UART_IOSTREAM

/* Enable UART input pattern match */
//#define AVR_UART_UART_MATCH

/* Use strncmp for pattern matching */
//#define AVR_UART_STRNCMP_MATCH

/* Emit a trigger signal that can be used by logic analyser to start capture */
//#define AVR_UART_EMIT_TRIGGER

/* SIM denotes that source code will compiled for simulation */
//#define AVR_UART_SIMULATION

/* Denotes that source code will be compiled for off-target testing */
//#define AVR_UART_SIMTEST

/* Demo mode with serial communication program */
//#define AVR_UART_DEMO

/* Enable debug build with debugging information and symbols */
//#define AVR_UART_DEBUG

/* Preserve compilation intermediaries */
//#define AVR_UART_SAVE_TEMPS

/* Set the compiler optimization level (-O0, -O1, -O2, -O3, -Os, etc.) */
//#define AVR_UART_OPTIM -O2
/*****************************************************************************/

#endif /* CONFIG_H_ */
