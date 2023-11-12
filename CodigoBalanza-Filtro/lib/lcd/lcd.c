#include "lcd.h"

#include <util/delay.h>
#include <avr/io.h>

enum LCDMode
{
    Instruction,
    Char
};

void set_lcd_mode(enum LCDMode mode)
{
    DDRB = 0xFF;
    switch (mode)
    {
    case Instruction:
        PORTB = 0x00;
        break;
    case Char:
        PORTB = 0x01;
        break;
    }
}

void lcd_set_enable(int enable) {
    if(enable) 
        PORTB |= 0x04;
    else 
        PORTB = 0;
}

void lcd_init()
{
    DDRB = 0xFF;
    DDRD = 0xFF;

    // modo 8 bits, 2 lineas, celda 8x5
    PORTD = 0x38; // 0b0011_1000
    // RS=0 ; RW=0 ; E=1
    PORTB = 0x04; // 0b0000_0100
    _delay_us(100);
    PORTB = 0x00;

    // display, cursor, blink on
    PORTD = 0x0F;
    PORTB = 0x04;
    _delay_us(100);
    PORTB = 0x00;

    // clear display
    PORTD = 0x01;
    PORTB = 0x04;
    _delay_us(100);
    PORTB = 0x00;

    // entry mode set
    // cursor moves right and incremented, doesnt shift display
    PORTD = 0x06; // 0b0000_0110
    PORTB = 0x04;
    _delay_us(100);
    PORTB = 0x00;
}


/// @brief Envia un caracter al lcd
/// @param data 
void lcd_data(char data)
{
    set_lcd_mode(Char);
    DDRD = 0xFF;
    PORTD = data;
    lcd_set_enable(1);
    _delay_us(100);
    lcd_set_enable(0);
}

/// @brief Ejecuta una instruccion del lcd
/// @param cmd 
void lcd_cmd(char cmd)
{
    set_lcd_mode(Instruction);
    DDRD = 0xFF;
    PORTD = cmd;
    lcd_set_enable(1);
    _delay_us(100);
    lcd_set_enable(0);
}

/// @brief Genera un caracterer en la memoria del lcd
/// @param pos posicion en memoria (1-8)
/// @param arr 
void lcd_create_char(int pos, char* arr)
{
    if (pos < 8)
    {
        lcd_cmd(pos * 8 + 0x40);
        for (int i = 0; i < 8; i++)
            lcd_data(arr[i]);
    }
}

/// @brief Imprime un string
/// @param str 
void lcd_print(char* str)
{
    unsigned int i = 0;
    while (str[i] != '\0')
    {
        lcd_data(str[i]);
        i++;
    }
}
