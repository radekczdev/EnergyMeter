// C INCLUDES //
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// TI INCLUDES //
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"

// TI DRIVER LIBS //
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/adc.h"
#include "driverlib/fpu.h"	// Floating-Point Unit - FPU
#include "driverlib/rom.h"
#include "driverlib/timer.h"

// EXTERNAL DRIVER LIBS //
#include "lcd44780_LP.h"
#include "ds3231.h"
#include "ADCW2.h"
#include "mathFunctions.h"

// prototypy funkcji //
void portConfig(void);				// Konfiguracja GPIO
void lcdConfig(void);				// Inicjalizacja HD4470
void rtcStartSet(void);				// ustawienie pocz¹tkowe zegara RTC
void getAdcData(uint32_t *);				// Pobranie danych z przetwornika ADC
void setFPU(void);					// Uruchomienie FPU
void setTimer(void);				// Ustawienie Timera

// deklaracja zmiennych //
	#define PORTF_PIN1 (*((volatile unsigned long*)0x40025008))
	#define PORTF_PIN2 (*((volatile unsigned long*)0x40025010))
	volatile uint32_t SW1on=0;
	int32_t	tabWspolczynnikiUsredn[5]={-6,24,34,24,-6},
			tabWspolczynnikiUsrednDt[5]={14,7,0,-7,-14},
			tabWspolczynnikiUsrednDt2[5]={20,-10,-20,-10,20},
			tabI[3], 	// tablica z pomiarem I co 10kHz I1,I2,I3
			tabU[3], 	// tablica z pomiarem U co 10kHz I1,I2,I3
			tabdI[3], 	// tablica z obliczeniem dI co 10kHz
			tabdU[3], 	// tablica z obliczeniem dU co 10kHz
			// tablica sinx 0;2pi 201 probek, trzeba podzielic przez 4096
			tabsinx[201]={0,129,257,385,513,641,768,894,1019,1143,1266,1387,1508,1627,1744,1860,1973,2085,2195,2302,2408,2510,2611,2709,2804,2896,2986,3072,3156,3236,3314,3388,3458,3526,3589,3650,3706,3759,3808,3854,3896,3933,3967,3997,4023,4046,4064,4078,4088,4094,4096,4094,4088,4078,4064,4046,4023,3997,3967,3933,3896,3854,3808,3759,3706,3650,3589,3526,3458,3388,3314,3236,3156,3072,2986,2896,2804,2709,2611,2510,2408,2302,2195,2085,1973,1860,1744,1627,1508,1387,1266,1143,1019,894,768,641,513,385,257,129,0,-129,-257,-385,-513,-641,-768,-894,-1019,-1143,-1266,-1387,-1508,-1627,-1744,-1860,-1973,-2085,-2195,-2302,-2408,-2510,-2611,-2709,-2804,-2896,-2986,-3072,-3156,-3236,-3314,-3388,-3458,-3526,-3589,-3650,-3706,-3759,-3808,-3854,-3896,-3933,-3967,-3997,-4023,-4046,-4064,-4078,-4088,-4094,-4096,-4094,-4088,-4078,-4064,-4046,-4023,-3997,-3967,-3933,-3896,-3854,-3808,-3759,-3706,-3650,-3589,-3526,-3458,-3388,-3314,-3236,-3156,-3072,-2986,-2896,-2804,-2709,-2611,-2510,-2408,-2302,-2195,-2085,-1973,-1860,-1744,-1627,-1508,-1387,-1266,-1143,-1019,-894,-768,-641,-513,-385,-257,-129,0},
			tabcosx[201]={4096,4094,4088,4078,4064,4046,4023,3997,3967,3933,3896,3854,3808,3759,3706,3650,3589,3526,3458,3388,3314,3236,3156,3072,2986,2896,2804,2709,2611,2510,2408,2302,2195,2085,1973,1860,1744,1627,1508,1387,1266,1143,1019,894,768,641,513,385,257,129,0,-129,-257,-385,-513,-641,-768,-894,-1019,-1143,-1266,-1387,-1508,-1627,-1744,-1860,-1973,-2085,-2195,-2302,-2408,-2510,-2611,-2709,-2804,-2896,-2986,-3072,-3156,-3236,-3314,-3388,-3458,-3526,-3589,-3650,-3706,-3759,-3808,-3854,-3896,-3933,-3967,-3997,-4023,-4046,-4064,-4078,-4088,-4094,-4096,-4094,-4088,-4078,-4064,-4046,-4023,-3997,-3967,-3933,-3896,-3854,-3808,-3759,-3706,-3650,-3589,-3526,-3458,-3388,-3314,-3236,-3156,-3072,-2986,-2896,-2804,-2709,-2611,-2510,-2408,-2302,-2195,-2085,-1973,-1860,-1744,-1627,-1508,-1387,-1266,-1143,-1019,-894,-768,-641,-513,-385,-257,-129,0,129,257,385,513,641,768,894,1019,1143,1266,1387,1508,1627,1744,1860,1973,2085,2195,2302,2408,2510,2611,2709,2804,2896,2986,3072,3156,3236,3314,3388,3458,3526,3589,3650,3706,3759,3808,3854,3896,3933,3967,3997,4023,4046,4064,4078,4088,4094,4096},
			tabIs20ms[200][3],	// tablica I*sin za 20 ms
			tabIc20ms[200][3],	// tablica I*cos za 20 ms
			tabUs20ms[200][3],	// tablica U*sin za 20 ms
			tabUc20ms[200][3];	// tablica U*cos za 20 ms

	uint32_t Irms[3],
			Urms[3],
	 	 	I2[200][3]={0,0,0}, 	// tablica kwadratow pradow, suma dla 20ms
			U2[200][3]={0,0,0},	// tablica kwadratow napiec, suma dla 20ms
			Ih1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej pradu
			Uh1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej napiecia
			Ih1[3]={0,0,0},
			Uh1[3]={0,0,0},
			Ihnkwadrat[3]={0,0,0},	// tablica wartoci reszty harmonicznych pradu
			Uhnkwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznych napiecia
			Ihn[3]={0,0,0},
			Uhn[3]={0,0,0},
			tablicaTEST[3][200];
	unsigned long	pomiar_prad[3][5],		// tablica od Paw³a z pradami z kolejki
			pomiar_napiecie[3][5];	// tablica od Paw³a z napieciami z kolejki
	uint32_t ADCvalue[16], ui32Period, numerProbki=0, n=0;

	int a=0, i=0, j=0, bufor=0, flaga20ms=0, h=56; // h - wspolczynnik do pochodnych

	volatile char time[], temp[5], tekstLCD[16];
	float 	Uh1value[3]={0,0,0},
			Urmsvalue[3]={0,0,0},
			Uhnvalue[3]={0,0,0},
			diffI[3],
			diffIsum[3]={0,0,0},
			diffIsum3sek[3],
			diffU[3],
			diffUsum[3]={0,0,0},
			diffUsum3sek[3],
			THDi[3],
			THDu[3];


int main(void) {
	// Konfiguracja zegara //
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); 				// 400MHz / 2 / 5 (SYSCTL_SYSDIV_5) = 40MHz
	
	// Konfiguracja peryferiów //
	portConfig();	
	lcdConfig();
	//rtcStartSet();			// funkcja ustawia zegar RTC poprzez I2C
	setFPU();
	setTimer();

	while(1){
		SysCtlDelay(1);
		if(flaga20ms==1){
				PORTF_PIN2 |=0xFF;
				int temp=bufor;
				flaga20ms=0;
				for(i=0;i<3;i++){
					// wartosc skuteczna pradu
					Irms[i]=SquareRootRounded(I2[temp][i]/200);
					// wartosc skuteczna napiecia
					Urms[i]=SquareRootRounded(U2[temp][i]/200);
					// usrednione za okres 20 ms sygnal*sin oraz sygnal*cos
					tabIs20ms[temp][i]=(tabIs20ms[temp][i]/100);
					tabIc20ms[temp][i]=(tabIc20ms[temp][i]/100);
					tabUs20ms[temp][i]=(tabUs20ms[temp][i]/100);
					tabUc20ms[temp][i]=(tabUc20ms[temp][i]/100);
					// kwadrat pierwszej harmonicznej pradu | pierwsza harmoniczna pradu
					Ih1kwadrat[i]=(tabIs20ms[temp][i]*tabIs20ms[temp][i]+tabIc20ms[temp][i]*tabIc20ms[temp][i]);
					Ih1[i]=SquareRootRounded(Ih1kwadrat[i]);
					// kwadrat pierwszej harmonicznej napiecia | pierwsza harmoniczna napiecia
					Uh1kwadrat[i]=(tabUs20ms[temp][i]*tabUs20ms[temp][i]+tabUc20ms[temp][i]*tabUc20ms[temp][i]);
					Uh1[i]=SquareRootRounded(Uh1kwadrat[i]);
					// wartosc reszty harmonicznych
					Ihnkwadrat[i]=Irms[i]*Irms[i]-Ih1kwadrat[i];
					Uhnkwadrat[i]=Urms[i]*Urms[i]-Uh1kwadrat[i];
					Ihn[i]=SquareRootRounded(Ihnkwadrat[i]);
					Uhn[i]=SquareRootRounded(Uhnkwadrat[i]);
					I2[temp][i]=0;
					U2[temp][i]=0;
					tabIs20ms[temp][i]=0;
					tabIc20ms[temp][i]=0;
					tabUs20ms[temp][i]=0;
					tabUc20ms[temp][i]=0;
				}
		//////////////// wyznaczenie THD dla 3 sekund ///////////////
				if(n>149){
					for(a=0;a<3;a++){
						diffIsum3sek[a]=diffIsum[a];
						diffUsum3sek[a]=diffUsum[a];
						diffIsum[a]=0;
						diffUsum[a]=0;
						Uh1value[a] = (Uh1[a] * 3.31)/4096;
						Urmsvalue[a] = (Urms[a] * 3.31)/4096;
						Uhnvalue[a] = (Uhn[a] * 3.31)/4096;
						THDi[a]=(diffIsum3sek[a]/150)*100;
						THDu[a]=(diffUsum3sek[a]/150)*100;
					}
					n=0;
				}
				else{
					for(a=0;a<3;a++){
						diffI[a]=(float)Ihn[a]/(float)Ih1[a];
						diffU[a]=(float)Uhn[a]/(float)Uh1[a];
						diffIsum[a]+=diffI[a];
						diffUsum[a]+=diffU[a];
					}
					n++;

				}
		//////////////////////////////////////////////////////////////
//				sprintf((char*)&tekstLCD, "Uh1:%.2lf r2:%.2lf", Uh1value[1], Urmsvalue[1]);
//				LCDWriteText(tekstLCD,0,0);
//				sprintf((char*)&tekstLCD, "THD:%.2lf%%", THDu[1]);
//				LCDWriteText(tekstLCD,1,0);
				PORTF_PIN2 &=~0xFF;
			}
	}
}

void portConfig(){
	/******** PORT F GPIO ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);															// odblokowanie peryferiów portu F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);							// typ pinów - wyjscia

	/******** PORT E ADC0 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // odblokowanie peryferiów portu A
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ADC_0_1_InitSeq012();

	/******** PORT A SPI  ******/
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/******** PORT A I2C1 ******/
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
//	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
//	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
//	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
//	I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
}

void lcdConfig(){
	// Uruchomienie zapisu do wywietlacza //
	initLCD();
	LCDCommand(0x0c);
}

void rtcStartSet(){
	setDs3231DecBCD(SECONDS,00);
	setDs3231DecBCD(MINUTES,06);
	setDs3231DecBCD(HOURS,23);
}

void setFPU(void){
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
}

void setTimer(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ui32Period = (SysCtlClockGet()/10000);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void){
	PORTF_PIN1 |=0x02;

	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// licznik wywolanych przerwan
	if(numerProbki==200){
		flaga20ms=1;
		numerProbki=0;
		bufor++;
	}else{
		numerProbki++;
		if(bufor>200)bufor=0;
	}
	ADC0_1_InSeq1_2(pomiar_prad, pomiar_napiecie);
	// usrednienie pradow

	for(i=0;i<3;i++){
/////////////// usrednianie pomiaru //////////////////////////
		for(j=0;j<5;j++){
			tabdI[i]+=pomiar_prad[i][j]*tabWspolczynnikiUsrednDt[j];
			tabdU[i]+=pomiar_napiecie[i][j]*tabWspolczynnikiUsrednDt[j];
			tabI[i]+=pomiar_prad[i][j]*tabWspolczynnikiUsredn[j]; 			// dzielenie przez 70 wynika z usredniania
			tabU[i]+=(pomiar_napiecie[i][j]*tabWspolczynnikiUsredn[j]);
		}
///////////////////////////////////////////////////////////////

		I2[bufor][i]+=((tabI[i]/70)*(tabI[i]/70));
		U2[bufor][i]+=((tabU[i]/70)*(tabU[i]/70));
		tabIs20ms[bufor][i]+=(tabI[i]/70*tabsinx[numerProbki])/4096;
		tabIc20ms[bufor][i]+=(tabI[i]/70*tabcosx[numerProbki])/4096;
		tabUs20ms[bufor][i]+=(tabU[i]/70*tabsinx[numerProbki])/4096;
		tabUc20ms[bufor][i]+=(tabU[i]/70*tabcosx[numerProbki])/4096;
		tabI[i]=0;
		tabU[i]=0;
		tabdI[i]=0;
		tabdU[i]=0;

	}

	PORTF_PIN1 &=(~0x02);
}
