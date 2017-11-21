#include <stdint.h>

/* funkcje przeksztalcania z systemu BCD na dziesietny i odwrotnie */

uint8_t BCDToDecimal (uint8_t bcdByte)
{
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}
 
uint8_t DecimalToBCD (uint8_t decimalByte)
{
  return (((decimalByte / 10) << 4) | (decimalByte % 10));
}


/* funkcja pierwiastkowania */

uint32_t Pierwiastek(uint32_t liczba_wejsciowa)
{
    uint32_t temp_wartosc  = liczba_wejsciowa;
    uint32_t reszta = 0;
    uint32_t rozdzielczosc = 1uL << 30;


    while (rozdzielczosc > temp_wartosc)
    {
        rozdzielczosc >>= 2;
    }

    while (rozdzielczosc != 0)
    {
        if (temp_wartosc >= reszta + rozdzielczosc)
        {
            temp_wartosc = temp_wartosc - (reszta + rozdzielczosc);
            reszta = reszta +  2 * rozdzielczosc;
        }
        reszta >>= 1;
        rozdzielczosc >>= 2;
    }

    if (temp_wartosc > reszta)
    {
        reszta++;
    }

    return reszta;
}
