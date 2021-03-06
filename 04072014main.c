////// uREDNIANIE DLA 10 MINUT DZIALA, TRZEBA DOROBNIC EKRANY //////////

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
	#define PORTF_PIN0 (*((volatile unsigned long*)0x40025004))
	#define PORTF_PIN1 (*((volatile unsigned long*)0x40025008))
	#define PORTF_PIN2 (*((volatile unsigned long*)0x40025010))
	volatile uint8_t SW1on=0;
	int16_t	tabWsp[3][5]={{0,0,0,0,0},{-6,24,34,24,-6},{0,0,0,0,0}},	// tabWsp[numer_kanalu][nr_sekwencera]
			tabWspDt[3][5]={{0,0,0,0,0},{-14,-7,0,7,14},{0,0,0,0,0}},
			tabWspDt2[5]={20,-10,-20,-10,20};
	int32_t	tabI[3], 	// tablica z pomiarem I co 10kHz I1,I2,I3
			tabU[3], 	// tablica z pomiarem U co 10kHz I1,I2,I3
			tabdI[3], 	// tablica z obliczeniem dI co 10kHz
			tabdU[3], 	// tablica z obliczeniem dU co 10kHz

			tablicaTestowa[200],

			P[10][3],		// tablica mocy czynnych
			Q[10][3],		// tablica mocy biernych
			Pcz[3],		// tablica mocy czynnych
			Qbie[3],		// tablica mocy biernych
			// tablica sinx 0;2pi 201 probek, trzeba podzielic przez 4096
			tabsinx[201]={0,65,129,193,257,321,384,447,510,572,633,694,754,814,872,930,987,1043,1098,1151,1204,1255,1306,1355,1402,1448,1493,1536,1578,1618,1657,1694,1729,1763,1795,1825,1853,1880,1904,1927,1948,1967,1984,1999,2012,2023,2032,2039,2044,2047,2048,2047,2044,2039,2032,2023,2012,1999,1984,1967,1948,1927,1904,1880,1853,1825,1795,1763,1729,1694,1657,1618,1578,1536,1493,1448,1402,1355,1306,1255,1204,1151,1098,1043,987,930,872,814,754,694,633,572,510,447,384,321,257,193,129,65,0,-65,-129,-193,-257,-321,-384,-447,-510,-572,-633,-694,-754,-814,-872,-930,-987,-1043,-1098,-1151,-1204,-1255,-1306,-1355,-1402,-1448,-1493,-1536,-1578,-1618,-1657,-1694,-1729,-1763,-1795,-1825,-1853,-1880,-1904,-1927,-1948,-1967,-1984,-1999,-2012,-2023,-2032,-2039,-2044,-2047,-2048,-2047,-2044,-2039,-2032,-2023,-2012,-1999,-1984,-1967,-1948,-1927,-1904,-1880,-1853,-1825,-1795,-1763,-1729,-1694,-1657,-1618,-1578,-1536,-1493,-1448,-1402,-1355,-1306,-1255,-1204,-1151,-1098,-1043,-987,-930,-872,-814,-754,-694,-633,-572,-510,-447,-384,-321,-257,-193,-129,-65,0},
			tabcosx[201]={2048,2047,2044,2039,2032,2023,2012,1999,1984,1967,1948,1927,1904,1880,1853,1825,1795,1763,1729,1694,1657,1618,1578,1536,1493,1448,1402,1355,1306,1255,1204,1151,1098,1043,987,930,872,814,754,694,633,572,510,447,384,321,257,193,129,65,0,-65,-129,-193,-257,-321,-384,-447,-510,-572,-633,-694,-754,-814,-872,-930,-987,-1043,-1098,-1151,-1204,-1255,-1306,-1355,-1402,-1448,-1493,-1536,-1578,-1618,-1657,-1694,-1729,-1763,-1795,-1825,-1853,-1880,-1904,-1927,-1948,-1967,-1984,-1999,-2012,-2023,-2032,-2039,-2044,-2047,-2048,-2047,-2044,-2039,-2032,-2023,-2012,-1999,-1984,-1967,-1948,-1927,-1904,-1880,-1853,-1825,-1795,-1763,-1729,-1694,-1657,-1618,-1578,-1536,-1493,-1448,-1402,-1355,-1306,-1255,-1204,-1151,-1098,-1043,-987,-930,-872,-814,-754,-694,-633,-572,-510,-447,-384,-321,-257,-193,-129,-65,0,65,129,193,257,321,384,447,510,572,633,694,754,814,872,930,987,1043,1098,1151,1204,1255,1306,1355,1402,1448,1493,1536,1578,1618,1657,1694,1729,1763,1795,1825,1853,1880,1904,1927,1948,1967,1984,1999,2012,2023,2032,2039,2044,2047,2048};
	int32_t	tabIs20ms[10][3],	// tablica I*sin za 20 ms
			tabIc20ms[10][3],	// tablica I*cos za 20 ms
			tabdIs20ms[10][3],	// tablica dI*sin za 20 ms
			tabdIc20ms[10][3],	// tablica dI*cos za 20 ms
			tabUs20ms[10][3],	// tablica U*sin za 20 ms
			tabUc20ms[10][3];	// tablica U*cos za 20 ms

	uint32_t Irms[3]={0,0,0},
			dIrms[3]={0,0,0},
			Urms[3]={0,0,0};

	int16_t	Ih1[3]={0,0,0},
			dIh1[3]={0,0,0},
			Uh1[3]={0,0,0},
			Ph1[3]={0,0,0},
			Qh1[3]={0,0,0},
			Phn[3]={0,0,0},
			Qhn[3]={0,0,0};

	uint32_t Ih1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej pradu
			dIh1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej pochodnej pradu
			Uh1kwadrat[3]={0,0,0},	// tablica wartoci pierwszej harmonicznej napiecia
			Ihnkwadrat[3]={0,0,0},	// tablica wartoci reszty harmonicznych pradu
			dIhnkwadrat[3]={0,0,0},	// tablica wartoci reszty harmonicznych pradu
			Uhnkwadrat[3]={0,0,0};	// tablica wartoci pierwszej harmonicznych napiecia

	int16_t	Ihn[3]={0,0,0},
			dIhn[3]={0,0,0},
			Uhn[3]={0,0,0};

	uint64_t I2[10][3]={0,0,0}, 	// tablica kwadratow pradow, suma dla 20ms
			U2[10][3]={0,0,0},	// tablica kwadratow napiec, suma dla 20ms
			dI2[10][3]={0,0,0}; 	// tablica kwadratow pochodnych pradow, suma dla 20ms

	unsigned long	pomiar_prad[3][5],		// tablica od Paw�a z pradami z kolejki
					pomiar_napiecie[3][5];	// tablica od Paw�a z napieciami z kolejki
	uint32_t ADCvalue[16], ui32Period, numerProbki=0, n=0, n10min=0;
	unsigned short key_klawisz2;

	int a=0, i=0, j=0, bufor=0, flaga20ms=0, stanPortuF = 0, wyborWyswietlania =1, wyborFazyOkresu = 0,
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
			Pvalue[3]={0,0,0},
			Qvalue[3]={0,0,0},
			Ph1value[3]={0,0,0},
			Qh1value[3]={0,0,0},
			Phnvalue[3]={0,0,0},
			Qhnvalue[3]={0,0,0},
			diffI[3],
			diffIsum[3]={0,0,0},
			diffIsum3sek[3],
			diffU[3],
			diffUsum[3]={0,0,0},
			diffUsum3sek[3],
			THDi[3],
			THDu[3],
						Uh1value10min[3]={0,0,0},
						Urmsvalue10min[3]={0,0,0},
						Uhnvalue10min[3]={0,0,0},
						Ih1value10min[3]={0,0,0},
						Irmsvalue10min[3]={0,0,0},
						Ihnvalue10min[3]={0,0,0},
						dIh1value10min[3]={0,0,0},
						dIrmsvalue10min[3]={0,0,0},
						dIhnvalue10min[3]={0,0,0},
						Pvalue10min[3]={0,0,0},
						Qvalue10min[3]={0,0,0},
						Ph1value10min[3]={0,0,0},
						Qh1value10min[3]={0,0,0},
						Phnvalue10min[3]={0,0,0},
						Qhnvalue10min[3]={0,0,0},
						THDi10min[3]={0,0,0},
						THDu10min[3]={0,0,0},

			napiecieRzeczywiste=2.62*442.5,
			pradRzeczywisty=2.86*20;

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
	
	// Konfiguracja peryferi�w //
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
		SysCtlDelay(1);
		//////// obsluga wyswietlanych danych ///////////
		if(!key_klawisz2 && !PORTF_PIN0){
			key_klawisz2=60000;
			stanPortuF = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
			SW1on = stanPortuF & 0x01;
			if(!SW1on){
				wyborWyswietlania=wyborWyswietlania<<1;
			}
		}else if(key_klawisz2 && (PORTF_PIN0)){key_klawisz2++;}
		if(wyborWyswietlania>32)wyborWyswietlania=1;
		switch(wyborWyswietlania){
			case 1:
				wyborFazyOkresu=0;
				break;
			case 2:
				wyborFazyOkresu=1;
				break;
			case 4:
				wyborFazyOkresu=2;
				break;
			case 8:
				wyborFazyOkresu=0;
				break;
			case 16:
				wyborFazyOkresu=1;
				break;
			case 32:
				wyborFazyOkresu=2;
				break;
		}
		///////////////////////////////////////////////////

		if(flaga20ms==1){
				PORTF_PIN2 |=0xFF;
				int temp=bufor-1;
				flaga20ms=0;
				for(i=0;i<3;i++){
					// wartosc skuteczna pradu
					I2[temp][i]=I2[temp][i]/200;
					Irms[i]=SquareRootRounded(I2[temp][i]);
					// wartosc skuteczna pochodnej pradu
					dI2[temp][i]=dI2[temp][i]/200;
					dIrms[i]=SquareRootRounded(dI2[temp][i]);
					// wartosc skuteczna napiecia
					U2[temp][i]=U2[temp][i]/200;
					Urms[i]=SquareRootRounded(U2[temp][i]);
					// calka mocy czynnej i biernej
					Pcz[i]=P[temp][i]/200;
					Qbie[i]=Q[temp][i]/200;
					// usrednione za okres 20 ms sygnal*sin oraz sygnal*cos
					tabIs20ms[temp][i]=(tabIs20ms[temp][i]/2048/100);
					tabIc20ms[temp][i]=(tabIc20ms[temp][i]/2048/100);
					tabdIs20ms[temp][i]=(tabdIs20ms[temp][i]/2048/100);
					tabdIc20ms[temp][i]=(tabdIc20ms[temp][i]/2048/100);
					tabUs20ms[temp][i]=(tabUs20ms[temp][i]/2048/100);
					tabUc20ms[temp][i]=(tabUc20ms[temp][i]/2048/100);
					// kwadrat pierwszej harmonicznej pradu | pierwsza harmoniczna pradu
					Ih1kwadrat[i]=(tabIs20ms[temp][i]*tabIs20ms[temp][i]+tabIc20ms[temp][i]*tabIc20ms[temp][i])/2;		//2 oznacza dzielenie Ih1 przez sqrt(2)
					Ih1[i]=SquareRootRounded(Ih1kwadrat[i]);
					// kwadrat pierwszej harmonicznej pochodnej pradu | pierwsza harmoniczna pochodnej pradu
					dIh1kwadrat[i]=(tabdIs20ms[temp][i]*tabdIs20ms[temp][i]+tabdIc20ms[temp][i]*tabdIc20ms[temp][i])/2;
					dIh1[i]=SquareRootRounded(dIh1kwadrat[i]);
					// kwadrat pierwszej harmonicznej napiecia | pierwsza harmoniczna napiecia
					Uh1kwadrat[i]=(tabUs20ms[temp][i]*tabUs20ms[temp][i]+tabUc20ms[temp][i]*tabUc20ms[temp][i])/2;
					Uh1[i]=SquareRootRounded(Uh1kwadrat[i]);
					// pierwsza harmoniczna mocy czynnej
					Ph1[i]=(tabUs20ms[temp][i]*tabIs20ms[temp][i]+tabUc20ms[temp][i]*tabIc20ms[temp][i])/2;
					// pierwsza harmoniczna mocy biernej
					Qh1[i]=(tabUs20ms[temp][i]*tabdIs20ms[temp][i]+tabUc20ms[temp][i]*tabdIc20ms[temp][i])/2;
					// reszta harmonicznych mocy czynnej
					Phn[i]=Pcz[i]/4096-Ph1[i];
					// reszta harmonicznych mocy biernej
					Qhn[i]=Qbie[i]/4096-Qh1[i];
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
					Ih1value[i] += (Ih1[i] * pradRzeczywisty)/4096;
					Irmsvalue[i] += (Irms[i] *pradRzeczywisty)/4096;
					Ihnvalue[i] += (Ihn[i] * pradRzeczywisty)/4096;
					dIh1value[i] += (dIh1[i] * pradRzeczywisty)/4096;
					dIrmsvalue[i] += (dIrms[i] * pradRzeczywisty)/4096;
					dIhnvalue[i] += (dIhn[i] * pradRzeczywisty)/4096;
					Uh1value[i] += (Uh1[i] * napiecieRzeczywiste)/4096;
					Urmsvalue[i] += (Urms[i] * napiecieRzeczywiste)/4096;
					Uhnvalue[i] += (Uhn[i] * napiecieRzeczywiste)/4096;

					Pvalue[i] += (Pcz[i] * pradRzeczywisty * napiecieRzeczywiste)/4096/4096;
					Qvalue[i] += (Qbie[i] * pradRzeczywisty * napiecieRzeczywiste)/4096/4096;
					Ph1value[i] += (Ph1[i] * napiecieRzeczywiste)/4096;
					Qh1value[i] += (Qh1[i] * napiecieRzeczywiste)/4096;
					Phnvalue[i] += (Phn[i] * napiecieRzeczywiste)/4096;
					Qhnvalue[i] += (Qhn[i] * napiecieRzeczywiste)/4096;
				}
				//////////////// wyznaczenie Parametr�w dla 3 sekund i 10 minut ///////////////
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
					n10min++;

	//				sprintf((char*)&buffer1,"\nUrms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",Urmsvalue[1],Uh1value[1],Uhnvalue[1],THDu[1]);
	//				UART_OutString(buffer1);

					for(a=0;a<3;a++){
						Ih1value[a] /= 150;
						Irmsvalue[a] /= 150;
						Ihnvalue[a] /= 150;
						dIh1value[a] /= 150;
						dIrmsvalue[a] /= 150;
						dIhnvalue[a] /= 150;
						Uh1value[a] /= 150;
						Urmsvalue[a] /= 150;
						Uhnvalue[a] /= 150;
						Pvalue[a] /= 150;
						Qvalue[a] /= 150;
						Ph1value[a] /= 150;
						Qh1value[a] /= 150;
						Phnvalue[a] /= 150;
						Qhnvalue[a] /= 150;

						/////////// 10 MINUT 10 MINUT 10 MINUT 10 MINUT 10 MINUT /////////////////

						Ih1value10min[a] += Ih1value[a];
						Irmsvalue10min[a] += Irmsvalue[a];
						Ihnvalue10min[a] += Ihnvalue[a];
						dIh1value10min[a] += dIh1value[a];
						dIrmsvalue10min[a] += dIrmsvalue[a];
						dIhnvalue10min[a] += Ihnvalue[a];
						Uh1value10min[a] += Uh1value[a];
						Urmsvalue10min[a] += Urmsvalue[a];
						Uhnvalue10min[a] += Uhnvalue[a];
						Pvalue10min[a] += Pvalue[a];
						Qvalue10min[a] += Qvalue[a];
						Ph1value10min[a] += Ph1value[a];
						Qh1value10min[a] += Qh1value[a];
						Phnvalue10min[a] += Phnvalue[a];
						Qhnvalue10min[a] += Qhnvalue[a];
						THDi10min[a] = (Uhnvalue10min[a]/Uh1value10min[a])*100;
						THDu10min[a] = (Ihnvalue10min[a]/Ih1value10min[a])*100;

						//////////////////////////////////////////////////////////////////////////

					}

					//////////// 3 SEKUNDY 3 SEKUNDY 3 SEKUNDY  3 SEKUNDY - wyswietlanie/////
					if(wyborWyswietlania>4){
						sprintf((char*)&buffer1,"\fFaza%d (3sek):\n\r      Urms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",wyborFazyOkresu+1,Urmsvalue[wyborFazyOkresu],Uh1value[wyborFazyOkresu],Uhnvalue[wyborFazyOkresu],THDu[wyborFazyOkresu]);
						UART_OutString(buffer1);
						SysCtlDelay(10000);
						sprintf((char*)&buffer1,"      Irms: %.2lf, Ih1: %.2lf, Ihn: %.2lf, THD I: %.2lf%%\n\r",Irmsvalue[wyborFazyOkresu],Ih1value[wyborFazyOkresu],Ihnvalue[wyborFazyOkresu],THDi[wyborFazyOkresu]);
						UART_OutString(buffer1);
						sprintf((char*)&buffer1,"      dIrms: %.4lf, dIh1: %.4lf, dIhn: %.4lf\n\r",dIrmsvalue[wyborFazyOkresu],dIh1value[wyborFazyOkresu],dIhnvalue[wyborFazyOkresu]);
						UART_OutString(buffer1);
						SysCtlDelay(10000);
						sprintf((char*)&buffer1,"      P: %.2lf, Q: %.2lf, Ph1: %.2lf, Qh1: %.2lf, Phn: %.2lf, Qhn: %.2lf\n\r",Pvalue[wyborFazyOkresu],Qvalue[wyborFazyOkresu],Ph1value[wyborFazyOkresu],Qh1value[wyborFazyOkresu],Phnvalue[wyborFazyOkresu],Qhnvalue[wyborFazyOkresu]);
						UART_OutString(buffer1);
						SysCtlDelay(10000);
					}

					/////////// 10 MINUT 10 MINUT 10 MINUT 10 MINUT 10 MINUT - wyswietlanie///
					if(wyborWyswietlania<8){
						sprintf((char*)&buffer1,"\fFaza%d (10min, odswiezenie za %d sekund):\n\r      Urms: %.2lf, Uh1: %.2lf, Uhn: %.2lf, THD U: %.2lf%%\n\r",wyborFazyOkresu+1,(200-n10min)*3,Urmsvalue10min[wyborFazyOkresu]/n10min,Uh1value10min[wyborFazyOkresu]/n10min,Uhnvalue10min[wyborFazyOkresu]/n10min,THDu10min[wyborFazyOkresu]);
						UART_OutString(buffer1);
						sprintf((char*)&buffer1,"      Irms: %.2lf, Ih1: %.2lf, Ihn: %.2lf, THD I: %.2lf%%\n\r",Irmsvalue10min[wyborFazyOkresu]/n10min,Ih1value10min[wyborFazyOkresu]/n10min,Ihnvalue10min[wyborFazyOkresu]/n10min,THDi10min[wyborFazyOkresu]);
						UART_OutString(buffer1);
						sprintf((char*)&buffer1,"      dIrms: %.4lf, dIh1: %.4lf, dIhn: %.4lf\n\r",dIrmsvalue10min[wyborFazyOkresu]/n10min,dIh1value10min[wyborFazyOkresu]/n10min,dIhnvalue10min[wyborFazyOkresu]/n10min);
						UART_OutString(buffer1);
						sprintf((char*)&buffer1,"      P: %.2lf, Q: %.2lf, Ph1: %.2lf, Qh1: %.2lf, Phn: %.2lf, Qhn: %.2lf\n\r",Pvalue10min[wyborFazyOkresu]/n10min,Qvalue10min[wyborFazyOkresu]/n10min,Ph1value10min[wyborFazyOkresu]/n10min,Qh1value10min[wyborFazyOkresu]/n10min,Phnvalue10min[wyborFazyOkresu]/n10min,Qhnvalue10min[wyborFazyOkresu]/n10min);
						UART_OutString(buffer1);
					}

					//////////////////////////////////////////////////////////////////////////

					for(a=0;a<3;a++){
						Ih1value[a] = 0;
						Irmsvalue[a] = 0;
						Ihnvalue[a] = 0;
						dIh1value[a] = 0;
						dIrmsvalue[a] = 0;
						dIhnvalue[a] = 0;
						Uh1value[a] = 0;
						Urmsvalue[a] = 0;
						Uhnvalue[a] = 0;
						Pvalue[a] = 0;
						Qvalue[a] = 0;
						Ph1value[a] = 0;
						Qh1value[a] = 0;
						Phnvalue[a] = 0;
						Qhnvalue[a] = 0;
					}
					if(n10min>199){
						n10min=0;
						for(a=0;a<3;a++){

							Ih1value10min[a] = 0;
							Irmsvalue10min[a] = 0;
							Ihnvalue10min[a] = 0;
							dIh1value10min[a] = 0;
							dIrmsvalue10min[a] = 0;
							dIhnvalue10min[a] = 0;
							Uh1value10min[a] = 0;
							Urmsvalue10min[a] = 0;
							Uhnvalue10min[a] = 0;
							Pvalue10min[a] = 0;
							Qvalue10min[a] = 0;
							Ph1value10min[a] = 0;
							Qh1value10min[a] = 0;
							Phnvalue10min[a] = 0;
							Qhnvalue10min[a] = 0;
							THDi10min[a] = 0;
							THDu10min[a] = 0;
						}
					}

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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);															// odblokowanie peryferi�w portu F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);							// typ pin�w - wyjscia
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);											// typ pin�w - wejscia		// PORTF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);		// konfiguracja portow WEJSCIOWYCH

	/******** PORT E ADC0 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // odblokowanie peryferi�w portu A
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
		tabdI[i]=(tabdI[i]/70-(1783*dTsekwencera));
		tabI[i]=(tabI[i]/70-(1783*dTsekwencera)); 			// dzielenie przez 70 wynika z usredniania
		tabU[i]=(tabU[i]/70-(1580*dTsekwencera));

//		 SPRAWDZENIE przesuniecia w UART
//		if(i==1&&numerProbki<200) {
//			sprintf((char*)&buffer1,"%d\r",tabU[1]);
//			UART_OutString(buffer1);
//		}
//		------------------------

		// sumy dla I oraz U
		I2[bufor][i]+=tabI[i]*tabI[i]/(dTsekwencera*dTsekwencera);
		U2[bufor][i]+=tabU[i]*tabU[i]/(dTsekwencera*dTsekwencera);
		dI2[bufor][i]+=tabdI[i]*tabdI[i]/(dTsekwencera*dTsekwencera);
		tabIs20ms[bufor][i]+=(tabI[i]*tabsinx[numerProbki])/dTsekwencera;
		tabIc20ms[bufor][i]+=(tabI[i]*tabcosx[numerProbki])/dTsekwencera;
		tabdIs20ms[bufor][i]+=(tabdI[i]*tabsinx[numerProbki])/dTsekwencera;
		tabdIc20ms[bufor][i]+=(tabdI[i]*tabcosx[numerProbki])/dTsekwencera;
		tabUs20ms[bufor][i]+=(tabU[i]*tabsinx[numerProbki])/dTsekwencera;
		tabUc20ms[bufor][i]+=(tabU[i]*tabcosx[numerProbki])/dTsekwencera;
		P[bufor][i]+=tabI[i]*tabU[i]/(dTsekwencera*dTsekwencera); 		// podzieli jeszcze przez dTsekwencera
		Q[bufor][i]+=tabdI[i]*tabU[i]/(dTsekwencera*dTsekwencera);		// podzieli jeszcze przez dTsekwencera
		// czyszczenie tablic
		tabI[i]=0;
		tabdI[i]=0;
		tabU[i]=0;
	}

	PORTF_PIN1 &=(~0x02);
}
