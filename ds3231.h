#ifndef ds3231_H_
#define ds3231_H_

#define NUM_I2C_DATA 8
#define DS3231_ADDRESS 0x68
#define SECONDS 0x00
#define MINUTES 0x01
#define HOURS 0x02
#define DAYS 0x04
#define WEEKDAYS 0x03
#define C_MONTHS 0x05
#define YEARS 0x06
#define TEMP_H 0x11
#define TEMP_L 0x12

// funkcja pobiera dane z jednego z rejestrów zdefiniowanych powyzej
extern uint8_t getDs3231BCDDec(uint8_t register);
// funkcja pobiera wartosc aktualna Temperatury
void getDs3231Temp(char*);
// funkcja ustawia wartosc jednego z rejestrow zdefiniowanych wyzej
extern void setDs3231DecBCD(uint8_t register, uint8_t value);

#endif
