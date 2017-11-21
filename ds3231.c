#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "ds3231.h"
#include "mathFunctions.h"

uint8_t tempINT,tempFRA;
char temper[5];

uint8_t getDs3231BCDDec(uint8_t option){
	// wyslanie komendy //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, false);
	I2CMasterDataPut(I2C1_BASE,option);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C1_BASE)){
	};

	// odebranie danych //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, true);
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C1_BASE)){
	};

	// sprawdzenie czy transmisja zakonczona powodzeniem //
	if(I2CMasterErr(I2C1_BASE) != I2C_MASTER_ERR_NONE)	{
		return I2CMasterErr(I2C1_BASE);
	}
	return BCDToDecimal(I2CMasterDataGet(I2C1_BASE));
}

void setDs3231DecBCD(uint8_t option, uint8_t value){
	// wyslanie komendy //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, false);
	I2CMasterDataPut(I2C1_BASE, option);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE)){
	};
	I2CMasterDataPut(I2C1_BASE,DecimalToBCD(value));
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
}

void getDs3231Temp(char* temp){
	// wyslanie komendy //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, false);
	I2CMasterDataPut(I2C1_BASE, TEMP_H);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C1_BASE)){
	};

	// odebranie danych //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, true);
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C1_BASE)){
	};
	tempINT = I2CMasterDataGet(I2C1_BASE);

		// wyslanie komendy //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, false);
	I2CMasterDataPut(I2C1_BASE, TEMP_L);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C1_BASE)){
	};
	// odebranie danych //
	I2CMasterSlaveAddrSet(I2C1_BASE, DS3231_ADDRESS, true);
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C1_BASE)){
	};
	tempFRA = I2CMasterDataGet(I2C1_BASE);
	
	// sprawdzenie czy transmisja zakonczona powodzeniem //
	if(I2CMasterErr(I2C1_BASE) != I2C_MASTER_ERR_NONE)	{
		temp="error";
	}
	if(25*(tempFRA>>6)==0){
		sprintf(temp,"%d,0%d%cC",tempINT,25*(tempFRA>>6),223);
	}
	else
		sprintf(temp,"%d,%d%cC",tempINT,25*(tempFRA>>6),223);
}
