#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../lib/ftoa/ftoa.h"
#include "../lib/lcd/lcd.h"
// #include <Arduino.h>


/*
    PORTB -> Control
    PB2: E
    PB1: R/W -> 0: write
    PB0: RS
*/
/*
    PORTD -> D[7-0]
*/

const int pos_flechaAbajo = 1;
const int pos_flechaArriba = 2;
const int pos_logoUTN = 3;


void ADC_initialization(uint8_t pin)
{
    // voltage ref: AVCC
    // conversion left adjusted: 10 bits
    // ADMUX[3:0]: pin a5
    ADMUX = 0x45; // 0100_0101
    // ADCSRA[7]: enable adc
    // ADCSRA[3]: enable interrupts
    // ADCSRA[2:0]: prescaler (128)
    ADCSRA = 0x87; // 1000_0111
    DIDR0 |= 1 << pin;
}

/// @brief Compara la temp con la anterior y muestra una flecha indicando si sube o baja
int compare_result(int result, int old_weight)
{
    if (result == old_weight)
    {
        lcd_data(' ');
        return 0;
    }

    // muestra la flecha indicando si la temp subio o bajo
    if (result > old_weight)
    {
        lcd_data(pos_flechaArriba);
    }
    else
    {
        lcd_data(pos_flechaAbajo);
    }

    return 1;
}

void printUTN()
{
    // salta a 2da linea, pos 3
    // LCD_8bits_comando(0xC3);
    lcd_cmd(0xC3);

    lcd_print("UTN FR SFCO");
    lcd_data(pos_logoUTN);

    lcd_cmd(0b10001010);
    lcd_print("KG");
}

int main()
{    
    uint8_t lower_bits;
    uint8_t higher_bits;
    uint16_t adc_result;

    float weight;
    float old_weight = 0;
    float temp_weight = 0;

    int counter = 1;
    char temp_str[5];

    ADC_initialization(5);

    // LCD_8bits_on();
    lcd_init();

    // flecha 1
    char flechaAbajo[8] = {0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04, 0x00};
    char flechaArriba[8] = {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x00};
    // logo utn
    char logoUTN[8] = {0x15, 0x0E, 0x04, 0x1F, 0x04, 0xE, 0x15, 0x00};

    // LCD_8bits_Crea_Caracter(pos_flechaAbajo, flechaAbajo);
    lcd_create_char(pos_flechaArriba, flechaArriba);
    lcd_create_char(pos_flechaAbajo, flechaAbajo);
    lcd_create_char(pos_logoUTN, logoUTN);

    printUTN();

    while (1)
    {
        // ADCSRA[6] = 1 -> inicia una conversion
        ADCSRA |= 1 << ADSC;
        // ADIF -> interr flag, alta cuando termina la conversion
        while ((ADCSRA & (1 << ADIF)) == 0)
            ;

        lower_bits = ADCL;
        higher_bits = ADCH;
        adc_result = (higher_bits << 8) | lower_bits;
        weight = (adc_result + 0.01) * 30 / 1024;
        temp_weight += weight;

        if (counter++ % 4 != 0)
            continue;

        // entra cada 4 lecturas, entonces toma el promedio
        weight = temp_weight / 4;


        // LCD_8bits_comando(0b10000011);
        lcd_cmd(0b10000011);

        if (compare_result(weight, old_weight))
        {
            old_weight = weight;
            // actualiza el peso
            ftoa(weight, temp_str);
            lcd_print(temp_str);
        }

        // vuelve al inicio
        lcd_cmd(0x02);

        // reseteo el contador y el sumador
        counter = 1;
        temp_weight = 0;
        _delay_ms(500);
    }
}