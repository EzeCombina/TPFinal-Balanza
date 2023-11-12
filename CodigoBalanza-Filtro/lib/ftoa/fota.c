#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ftoa.h"

void ftoa(float f, char *str)
{
    const int precision = 2;

    bool isNeg = f < 0.f;
    if(isNeg)
        f *= -1;

    char *buf = str;

    int whole = f;

    // si tiene 1 digito entero pre incremento la posicion del buf con un espacio vacio
    if (whole < 10)
    {
        *buf++ = 0x20;
    }
    if(isNeg) 
    {
        *buf++ = '-';
    }

    itoa(whole, buf, 10);
    buf += strlen(buf);

    *buf++ = '.';

    float frac = f - whole;

    for (int i = 1; i <= precision; i++)
    {
        frac *= 10;

        int digit = frac;
        *buf++ = digit + '0';

        frac -= digit;
    }

    // nul-terminate string
    *buf = '\0';
}