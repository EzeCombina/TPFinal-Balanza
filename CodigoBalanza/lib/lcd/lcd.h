#ifndef _LCD_
#define _LCD_


void lcd_init();

void lcd_data(char data);

void lcd_cmd(char cmd);

void lcd_create_char(int pos, char* arr);

void lcd_print(char* str);

#endif
