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
#include "driverlib/uart.h"

// EXTERNAL DRIVER LIBS //
//#include "lcd44780_LP.h"
//#include "ds3231.h"
#include "ADCW2.h"
#include "mathFunctions.h"
#include "uart.h"

// prototypy funkcji //
void portConfig(void);				// Konfiguracja GPIO
void setFPU(void);					// Uruchomienie FPU
void setTimer(void);				// Ustawienie Timera

// deklaracja zmiennych //
	#define PORTF_PIN1 (*((volatile unsigned long*)0x40025008))
	#define PORTF_PIN2 (*((volatile unsigned long*)0x40025010))
	volatile uint32_t SW1on=0;
	int16_t	tabWsp[3][5]={{0,0,0,0,0},{-6,24,34,24,-6},{0,0,0,0,0}},	// tabWsp[numer_kanalu][nr_sekwencera]
			tabWspDt[3][5]={{0,0,0,0,0},{-14,-7,0,7,14},{0,0,0,0,0}},
			tabWspDt2[5]={20,-10,-20,-10,20};
	int32_t	tabI[3], 	// tablica z pomiarem I co 10kHz I1,I2,I3
			tabU[3], 	// tablica z pomiarem U co 10kHz I1,I2,I3
			tabdI[3], 	// tablica z obliczeniem dI co 10kHz
			tabdU[3], 	// tablica z obliczeniem dU co 10kHz
			P[10][3],		// tablica mocy czynnych
			Q[10][3],		// tablica mocy biernych
			// tablica sinx 0;2pi 201 probek, trzeba podzielic przez 4096
			tabsinx[201]={0,129,257,385,513,641,768,894,1019,1143,1266,1387,1508,1627,1744,1860,1973,2085,2195,2302,2408,2510,2611,2709,2804,2896,2986,3072,3156,3236,3314,3388,3458,3526,3589,3650,3706,3759,3808,3854,3896,3933,3967,3997,4023,4046,4064,4078,4088,4094,4096,4094,4088,4078,4064,4046,4023,3997,3967,3933,3896,3854,3808,3759,3706,3650,3589,3526,3458,3388,3314,3236,3156,3072,2986,2896,2804,2709,2611,2510,2408,2302,2195,2085,1973,1860,1744,1627,1508,1387,1266,1143,1019,894,768,641,513,385,257,129,0,-129,-257,-385,-513,-641,-768,-894,-1019,-1143,-1266,-1387,-1508,-1627,-1744,-1860,-1973,-2085,-2195,-2302,-2408,-2510,-2611,-2709,-2804,-2896,-2986,-3072,-3156,-3236,-3314,-3388,-3458,-3526,-3589,-3650,-3706,-3759,-3808,-3854,-3896,-3933,-3967,-3997,-4023,-4046,-4064,-4078,-4088,-4094,-4096,-4094,-4088,-4078,-4064,-4046,-4023,-3997,-3967,-3933,-3896,-3854,-3808,-3759,-3706,-3650,-3589,-3526,-3458,-3388,-3314,-3236,-3156,-3072,-2986,-2896,-2804,-2709,-2611,-2510,-2408,-2302,-2195,-2085,-1973,-1860,-1744,-1627,-1508,-1387,-1266,-1143,-1019,-894,-768,-641,-513,-385,-257,-129,0},
			tabcosx[201]={4096,4094,4088,4078,4064,4046,4023,3997,3967,3933,3896,3854,3808,3759,3706,3650,3589,3526,3458,3388,3314,3236,3156,3072,2986,2896,2804,2709,2611,2510,2408,2302,2195,2085,1973,1860,1744,1627,1508,1387,1266,1143,1019,894,768,641,513,385,257,129,0,-129,-257,-385,-513,-641,-768,-894,-1019,-1143,-1266,-1387,-1508,-1627,-1744,-1860,-1973,-2085,-2195,-2302,-2408,-2510,-2611,-2709,-2804,-2896,-2986,-3072,-3156,-3236,-3314,-3388,-3458,-3526,-3589,-3650,-3706,-3759,-3808,-3854,-3896,-3933,-3967,-3997,-4023,-4046,-4064,-4078,-4088,-4094,-4096,-4094,-4088,-4078,-4064,-4046,-4023,-3997,-3967,-3933,-3896,-3854,-3808,-3759,-3706,-3650,-3589,-3526,-3458,-3388,-3314,-3236,-3156,-3072,-2986,-2896,-2804,-2709,-2611,-2510,-2408,-2302,-2195,-2085,-1973,-1860,-1744,-1627,-1508,-1387,-1266,-1143,-1019,-894,-768,-641,-513,-385,-257,-129,0,129,257,385,513,641,768,894,1019,1143,1266,1387,1508,1627,1744,1860,1973,2085,2195,2302,2408,2510,2611,2709,2804,2896,2986,3072,3156,3236,3314,3388,3458,3526,3589,3650,3706,3759,3808,3854,3896,3933,3967,3997,4023,4046,4064,4078,4088,4094,4096};
	int64_t	tabIs20ms[10][3],	// tablica I*sin za 20 ms
			tabIc20ms[10][3],	// tablica I*cos za 20 ms
			tabdIs20ms[10][3],	// tablica dI*sin za 20 ms
			tabdIc20ms[10][3],	// tablica dI*cos za 20 ms
			tabUs20ms[10][3],	// tablica U*sin za 20 ms
			tabUc20ms[10][3];	// tablica U*cos za 20 ms

	int32_t Irms[3]={0,0,0},
			dIrms[3]={0,0,0},
			Urms[3]={0,0,0};

	int32_t	Ih1[3]={0,0,0},
			dIh1[3]={0,0,0},
			Uh1[3]={0,0,0};

	uint32_t Ih1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej pradu
			dIh1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej pochodnej pradu
			Uh1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej napiecia
			Ihnkwadrat[3]={0,0,0},	// tablica wartoci reszty harmonicznych pradu
			dIhnkwadrat[3]={0,0,0},	// tablica wartoci reszty harmonicznych pradu
			Uhnkwadrat[3]={0,0,0};	// tablica wartoci pierwszej harmonicznych napiecia

	int32_t	Ihn[3]={0,0,0},
			dIhn[3]={0,0,0},
			Uhn[3]={0,0,0};

	uint64_t I2[10][3]={0,0,0}, 	// tablica kwadratow pradow, suma dla 20ms
			U2[10][3]={0,0,0},	// tablica kwadratow napiec, suma dla 20ms
			dI2[10][3]={0,0,0}; 	// tablica kwadratow pochodnych pradow, suma dla 20ms

	unsigned long	pomiar_prad[3][5],		// tablica od Paw³a z pradami z kolejki
			pomiar_napiecie[3][5];	// tablica od Paw³a z napieciami z kolejki
	uint32_t ADCvalue[16], ui32Period, numerProbki=0, n=0;

	int a=0, i=0, j=0, bufor=0, flaga20ms=0,
			dTkanalu=1,
			dTsekwencera=8;


	volatile char time[], temp[5], tekstLCD[16];
	char buffer1[10000];
	float 	Uh1value[3]={0,0,0},
			Urmsvalue[3]={0,0,0},
			Uhnvalue[3]={0,0,0},
			Ih1value[3]={0,0,0},
			Irmsvalue[3]={0,0,0},
			Ihnvalue[3]={0,0,0},
			dIh1value[3]={0,0,0},
			dIrmsvalue[3]={0,0,0},
			dIhnvalue[3]={0,0,0},
			diffI[3],
			diffIsum[3]={0,0,0},
			diffIsum3sek[3],
			diffU[3],
			diffUsum[3]={0,0,0},
			diffUsum3sek[3],
			THDi[3],
			THDu[3];

	void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
	{
	    // petla, dopoki sa jeszcze znaki do przeslania.
	    while(ui32Count--)
	    {
	        // zapisz znak do UART.
	        UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
	    }
	}

int main(void) {
	// Konfiguracja zegara //
	SysCtlClockSet(SYSCTL_SYSDIV_4_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); 				// 400MHz / 2 / 5 (SYSCTL_SYSDIV_5) = 40MHz
	
	// Konfiguracja peryferiów //
	portConfig();	
//	lcdConfig();
	//rtcStartSet();			// funkcja ustawia zegar RTC poprzez I2C
	setFPU();
	setTimer();

	// obliczenie wspolczynnikow usredniajacych
	for(i=0;i<5;i++){
		tabWsp[0][i]=tabWsp[1][i]*dTsekwencera+tabWspDt[1][i]*dTkanalu;
		tabWsp[2][i]=tabWsp[1][i]*dTsekwencera-tabWspDt[1][i]*dTkanalu;
		tabWspDt[0][i]=tabWspDt[1][i]*dTsekwencera+tabWspDt2[i]*dTkanalu;
		tabWspDt[2][i]=tabWspDt[1][i]*dTsekwencera-tabWspDt2[i]*dTkanalu;
		tabWsp[1][i]=tabWsp[1][i]*dTsekwencera;
		tabWspDt[1][i]=tabWspDt[1][i]*dTsekwencera;
	}


	while(1){
		uint8_t stanPortuF = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
		SW1on = stanPortuF & 0x01;
		SysCtlDelay(1);
		if(flaga20ms==1){
				PORTF_PIN2 |=0xFF;
				int temp=bufor-1;
				flaga20ms=0;
				for(i=0;i<3;i++){
					// wartosc skuteczna pradu
					I2[temp][i]=I2[temp][i]/(dTsekwencera*dTsekwencera*200);
					Irms[i]=SquareRootRounded((int32_t)I2[temp][i]);
					// wartosc skuteczna pochodnej pradu
					dI2[temp][i]=dI2[temp][i]/(dTsekwencera*dTsekwencera*200);
					dIrms[i]=SquareRootRounded((int32_t)dI2[temp][i]);
					// wartosc skuteczna napiecia
					U2[temp][i]=U2[temp][i]/dTsekwencera/dTsekwencera/200;
					Urms[i]=SquareRootRounded((int32_t)U2[temp][i]);
					// usrednione za okres 20 ms sygnal*sin oraz sygnal*cos
					tabIs20ms[temp][i]=(tabIs20ms[temp][i]/4096/100/70);
					tabIc20ms[temp][i]=(tabIc20ms[temp][i]/4096/100/70);
					tabdIs20ms[temp][i]=(tabdIs20ms[temp][i]/4096/100/70);
					tabdIc20ms[temp][i]=(tabdIc20ms[temp][i]/4096/100/70);
					tabUs20ms[temp][i]=(tabUs20ms[temp][i]/4096/100/70);
					tabUc20ms[temp][i]=(tabUc20ms[temp][i]/4096/100/70);
					// kwadrat pierwszej harmonicznej pradu | pierwsza harmoniczna pradu
					Ih1kwadrat[i]=(tabIs20ms[temp][i]*tabIs20ms[temp][i]+tabIc20ms[temp][i]*tabIc20ms[temp][i]);
					Ih1[i]=SquareRootRounded(Ih1kwadrat[i]);
					// kwadrat pierwszej harmonicznej pochodnej pradu | pierwsza harmoniczna pochodnej pradu
					dIh1kwadrat[i]=(tabdIs20ms[temp][i]*tabdIs20ms[temp][i]+tabdIc20ms[temp][i]*tabdIc20ms[temp][i]);
					dIh1[i]=SquareRootRounded(dIh1kwadrat[i]);
					// kwadrat pierwszej harmonicznej napiecia | pierwsza harmoniczna napiecia
					Uh1kwadrat[i]=(tabUs20ms[temp][i]*tabUs20ms[temp][i]+tabUc20ms[temp][i]*tabUc20ms[temp][i]);
					Uh1[i]=SquareRootRounded(Uh1kwadrat[i]);
					// wartosc reszty harmonicznych
					Ihnkwadrat[i]=I2[temp][i]-Ih1kwadrat[i];
					dIhnkwadrat[i]=dI2[temp][i]-dIh1kwadrat[i];
					Uhnkwadrat[i]=U2[temp][i]-Uh1kwadrat[i];
					Ihn[i]=SquareRootRounded(Ihnkwadrat[i]);
					dIhn[i]=SquareRootRounded(dIhnkwadrat[i]);
					Uhn[i]=SquareRootRounded(Uhnkwadrat[i]);
					I2[temp][i]=0;
					dI2[temp][i]=0;
					U2[temp][i]=0;
					tabIs20ms[temp][i]=0;
					tabIc20ms[temp][i]=0;
					tabdIs20ms[temp][i]=0;
					tabdIc20ms[temp][i]=0;
					tabUs20ms[temp][i]=0;
					tabUc20ms[temp][i]=0;
					P[temp][i]=0;
					Q[temp][i]=0;
					// wartosci po przeliczeniu na V
					Ih1value[i] = (Ih1[i] * 3.3)/4096;
					Irmsvalue[i] = (Irms[i] * 3.3)/4096;
					Ihnvalue[i] = (Ihn[i] * 3.3)/4096;
					dIh1value[i] = (dIh1[i] * 3.3)/4096;
					dIrmsvalue[i] = (dIrms[i] * 3.3)/4096;
					dIhnvalue[i] = (dIhn[i] * 3.3)/4096;
					Uh1value[i] = (Uh1[i] * 3.3)/4096;
					Urmsvalue[i] = (Urms[i] * 3.3)/4096;
					Uhnvalue[i] = (Uhn[i] * 3.3)/4096;
				}
		//////////////// wyznaczenie THD dla 3 sekund ///////////////
				if(n>149){
					for(a=0;a<3;a++){
						diffIsum3sek[a]=diffIsum[a];
						diffUsum3sek[a]=diffUsum[a];
						diffIsum[a]=0;
						diffUsum[a]=0;
						THDi[a]=diffIsum3sek[a]*100/150;
						THDu[a]=diffUsum3sek[a]*100/150;
					}
					n=0;
					sprintf((char*)&buffer1,"\fFaza1:\nUrms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",Urmsvalue[0],Uh1value[0],Uhnvalue[0],THDu[0]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      Irms: %.2lf, Ih1: %.2lf, Ihn: %.2lf, THD I: %.2lf%%\n\r",Irmsvalue[0],Ih1value[0],Ihnvalue[0],THDi[0]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      dIrms: %.4lf, dIh1: %.4lf, dIhn: %.4lf, THD dI: %.4lf%%\n\r",Urmsvalue[0],Uh1value[0],Uhnvalue[0],THDu[0]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"\nFaza2:\nUrms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",Urmsvalue[1],Uh1value[1],Uhnvalue[1],THDu[1]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      Irms: %.2lf, Ih1: %.2lf, Ihn: %.2lf, THD I: %.2lf%%\n\r",Irmsvalue[1],Ih1value[1],Ihnvalue[1],THDi[1]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      dIrms: %.4lf, dIh1: %.4lf, dIhn: %.4lf, THD dI: %.4lf%%\n\r",Urmsvalue[1],Uh1value[1],Uhnvalue[1],THDu[1]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"\nFaza3:\nUrms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",Urmsvalue[2],Uh1value[2],Uhnvalue[2],THDu[2]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      Irms: %.2lf, Ih1: %.2lf, Ihn: %.2lf, THD I: %.2lf%%\n\r",Irmsvalue[2],Ih1value[2],Ihnvalue[2],THDi[2]);
					UART_OutString(buffer1);
					sprintf((char*)&buffer1,"      dIrms: %.4lf, dIh1: %.4lf, dIhn: %.4lf, THD dI: %.4lf%%\n\r",Urmsvalue[2],Uh1value[2],Uhnvalue[2],THDu[2]);
					UART_OutString(buffer1);
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

//				sprintf((char*)&buffer,"%.3lf/r", Urmsvalue[1]);



//				sprintf(buffer1,"Urms: %.2lf, Irms: %.2lf, Uhn: %.2lf\n\r",Urmsvalue[1],Irmsvalue[1],Uhnvalue[1]);
//	    		UARTSend((uint8_t *)buffer1, 255);

				PORTF_PIN2 &=~0xFF;
			}
	}
}

void portConfig(){
	/******** PORT A UART0 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 500000, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//	UART_Init();

	/******** PORT F GPIO ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);															// odblokowanie peryferiów portu F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);							// typ pinów - wyjscia

	/******** PORT E ADC0 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // odblokowanie peryferiów portu A
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ADC_0_1_InitSeq012();
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
		if(bufor>8)bufor=0;
	}
	ADC0_1_InSeq1_2(pomiar_prad, pomiar_napiecie);
	// usrednienie pradow

	for(i=0;i<3;i++){
		/////// usrednianie pomiaru ///////
		for(j=0;j<5;j++){
			tabdI[i]+=pomiar_prad[i][j]*tabWspDt[i][j];
			tabI[i]+=pomiar_prad[i][j]*tabWsp[i][j]; 			// dzielenie przez 70 wynika z usredniania
			tabU[i]+=pomiar_napiecie[i][j]*tabWsp[i][j];
		}
		///////////////////////////////////

		// sumy dla I oraz U
		I2[bufor][i]+=(tabI[i]/70)*(tabI[i]/70);
		U2[bufor][i]+=(tabU[i]/70)*(tabU[i]/70);
		dI2[bufor][i]+=(tabdI[i]/70)*(tabdI[i]/70);
		tabIs20ms[bufor][i]+=(tabI[i]/dTsekwencera*tabsinx[numerProbki]);
		tabIc20ms[bufor][i]+=(tabI[i]/dTsekwencera*tabcosx[numerProbki]);
		tabdIs20ms[bufor][i]+=(tabdI[i]/dTsekwencera*tabsinx[numerProbki]);
		tabdIc20ms[bufor][i]+=(tabdI[i]/dTsekwencera*tabcosx[numerProbki]);
		tabUs20ms[bufor][i]+=(tabU[i]/dTsekwencera*tabsinx[numerProbki]);
		tabUc20ms[bufor][i]+=(tabU[i]/dTsekwencera*tabcosx[numerProbki]);
		P[bufor][i]+=(tabI[i]/70)*(tabU[i]/70); 		// podzieli jeszcze przez dTsekwencera
		Q[bufor][i]+=(tabdI[i]/70)*(tabU[i]/70);		// podzieli jeszcze przez dTsekwencera
		// czyszczenie tablic
		tabI[i]=0;
		tabdI[i]=0;
		tabU[i]=0;

	}

	PORTF_PIN1 &=(~0x02);
}
