/*
 * @file lcd_commands.h
 * @author notweerdmonk
 * @brief commands for HD44780 lcd driver
 */

#ifndef _LCD_COMMANDS_H_
#define _LCD_COMMANDS_H_

/* Flags for commands */
typedef enum lcd_command {
  LCD_CMD_CLEAR_DISPLAY      = 0x01,
  LCD_CMD_RETURN_HOME        = 0x02,
  LCD_CMD_ENTRY_MODE_SET     = 0x04,
  LCD_CMD_DISPLAY_CONTROL    = 0x08,
  LCD_CMD_CURSOR_SHIFT       = 0x10,
  LCD_CMD_FUNCTION_SET       = 0x20,
  LCD_CMD_SET_CGRAMADDR      = 0x40,
  LCD_CMD_SET_DDRAMADDR      = 0x80
} lcd_command_t;

/* Flags for addresses of display RAM */
typedef enum lcd_ddram_addr {
  LCD_CMD_DDRAMADDR_LINE1    = 0x00,
  LCD_CMD_DDRAMADDR_LINE2    = 0x40
} lcd_ddram_addr_t;

/* Flags for entry mode */
typedef enum lcd_entry {
  LCD_CMD_ENTRY_INCR         = 0x02,
  LCD_CMD_ENTRY_DECR         = 0x00,
  LCD_CMD_ENTRY_SHIFT_ON     = 0x01,
  LCD_CMD_ENTRY_SHIFT_OFF    = 0x00
} lcd_entry_t;

/* Flags for display and cursor control */
typedef enum lcd_display {
  LCD_CMD_DISPLAY_ON         = 0x04,
  LCD_CMD_DISPLAY_OFF        = 0x00,
  LCD_CMD_CURSOR_ON          = 0x02,
  LCD_CMD_CURSOR_OFF         = 0x00,
  LCD_CMD_BLINK_ON           = 0x01,
  LCD_CMD_BLINK_OFF          = 0x00
} lcd_display_t;

/* Flags for cursor movement and display shift */
typedef enum lcd_cursor_shift {
  LCD_CMD_DISPLAY_MOVE       = 0x08,
  LCD_CMD_CURSOR_MOVE        = 0x00,
  LCD_CMD_MOVE_RIGHT         = 0x04,
  LCD_CMD_MOVE_LEFT          = 0x00
} lcd_cursor_shift_t;

/* Flags for LCD function */
typedef enum lcd_function {
  LCD_CMD_8BIT_MODE          = 0x10,
  LCD_CMD_4BIT_MODE          = 0x00,
  LCD_CMD_2LINE              = 0x08,
  LCD_CMD_1LINE              = 0x00,
  LCD_CMD_5x10DOTS           = 0x04,
  LCD_CMD_5x8DOTS            = 0x00
} lcd_function_t;

#endif /* _LCD_COMMANDS_H_ */
