/*
 * @file lcd_config.h
 * @author notweerdmonk
 * @brief configuration for lcd module
 */

#ifndef _LCD_CONFIG_H_
#define _LCD_CONFIG_H_

#define LCD_ROWS 2

#define LCD_COLS 16

#ifndef LCD_PWM_CHANNEL
#define LCD_PWM_CHANNEL PWM_CHANNEL_A
#endif

#ifndef LCD_BL_VALUE
#define LCD_BL_VALUE 150
#endif

#endif /* _LCD_CONFIG_H_ */
