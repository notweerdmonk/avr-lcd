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
/* Project Configuration Flags
 *****************************************************************************/

/* Enable runtime UART configuration (call uart_setup with config struct) */
// #define AVR_LCD_RUNTIME_CONFIG

/* Enable use of UART in I/O streams (stdin/stdout/stderr) */
// #define AVR_LCD_UART_IOSTREAM

/* Enable UART input pattern match */
// #define AVR_LCD_UART_MATCH

/* Use strncmp for pattern matching */
// #define AVR_LCD_STRNCMP_MATCH

/* Emit a trigger signal that can be used by logic analyser to start capture */
// #define AVR_LCD_EMIT_TRIGGER

/* SIM denotes that source code will compiled for simulation */
// #define AVR_LCD_SIMULATION

/* Denotes that source code will be compiled for off-target testing */
// #define AVR_LCD_SIMTEST

/* Demo mode with serial communication program */
// #define AVR_LCD_DEMO

/* Enable debug build with debugging information and symbols */
// #define AVR_LCD_DEBUG

/* Preserve compilation intermediaries */
// #define AVR_LCD_SAVE_TEMPS

/* Set the compiler optimization level (-O0, -O1, -O2, -O3, -Os, etc.) */
// #define AVR_LCD_OPTIM -O2

#endif /* CONFIG_H_ */
