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
	volatile uint8_t SW1on=0;
	int32_t	tabWspolczynnikiUsredn[5]={-6,24,34,24,-6},
			tabWspolczynnikiUsrednDt[5]={14,7,0,-7,-14},
			tabWspolczynnikiUsrednDt2[5]={20,-10,-20,-10,20},
			tabI[3], 	// tablica z pomiarem I co 10kHz I1,I2,I3
			tabU[3], 	// tablica z pomiarem U co 10kHz I1,I2,I3
			tabdI[3], 	// tablica z obliczeniem dI co 10kHz
			tabdU[3], 	// tablica z obliczeniem dU co 10kHz
			tabdI2[3],  // tablica z obliczeniem dI2 co 10kHz
			tabdU2[3],  // tablica z obliczeniem dU2 co 10kHz
			// tablica sinx 0;2pi 200 probek, trzeba podzielic przez 4095
			tabIs20ms[3],	// tablica I*sin za 20 ms
			tabIc20ms[3],	// tablica I*cos za 20 ms
			tabUs20ms[3],	// tablica U*sin za 20 ms
			tabUc20ms[3],	// tablica U*cos za 20 ms
			tabsinx[200]={0,129,258,387,516,644,771,898,1023,1148,1272,1394,1515,1634,1752,1868,1982,2094,2204,2312,2417,2521,2621,2719,2815,2907,2997,3083,3167,3247,3324,3398,3469,3536,3599,3659,3715,3768,3816,3861,3902,3940,3973,4002,4028,4049,4066,4080,4089,4094,4095,4092,4085,4073,4058,4039,4016,3988,3957,3922,3882,3839,3792,3742,3687,3629,3568,3502,3434,3362,3286,3207,3125,3040,2952,2861,2767,2671,2571,2469,2365,2258,2149,2038,1925,1810,1693,1575,1454,1333,1210,1086,961,835,708,580,452,323,194,65,-65,-194,-323,-452,-580,-708,-835,-961,-1086,-1210,-1333,-1454,-1575,-1693,-1810,-1925,-2038,-2149,-2258,-2365,-2469,-2571,-2671,-2767,-2861,-2952,-3040,-3125,-3207,-3286,-3362,-3434,-3502,-3568,-3629,-3687,-3742,-3792,-3839,-3882,-3922,-3957,-3988,-4016,-4039,-4058,-4073,-4085,-4092,-4095,-4094,-4089,-4080,-4066,-4049,-4028,-4002,-3973,-3940,-3902,-3861,-3816,-3768,-3715,-3659,-3599,-3536,-3469,-3398,-3324,-3247,-3167,-3083,-2997,-2907,-2815,-2719,-2621,-2521,-2417,-2312,-2204,-2094,-1982,-1868,-1752,-1634,-1515,-1394,-1272,-1148,-1023,-898,-771,-644,-516,-387,-258,-129,0},
			tabcosx[200]={4095,4093,4087,4077,4062,4044,4022,3995,3965,3931,3893,3850,3805,3755,3701,3644,3583,3519,3451,3380,3305,3227,3146,3062,2974,2884,2791,2695,2596,2495,2391,2285,2177,2066,1953,1839,1722,1604,1485,1363,1241,1117,992,866,739,612,484,355,226,97,-32,-162,-291,-419,-548,-676,-803,-929,-1055,-1179,-1302,-1424,-1545,-1664,-1781,-1896,-2010,-2122,-2231,-2338,-2443,-2546,-2646,-2743,-2838,-2930,-3019,-3104,-3187,-3267,-3343,-3416,-3486,-3552,-3614,-3673,-3729,-3780,-3828,-3872,-3912,-3948,-3981,-4009,-4033,-4054,-4070,-4082,-4090,-4094,-4094,-4090,-4082,-4070,-4054,-4033,-4009,-3981,-3948,-3912,-3872,-3828,-3780,-3729,-3673,-3614,-3552,-3486,-3416,-3343,-3267,-3187,-3104,-3019,-2930,-2838,-2743,-2646,-2546,-2443,-2338,-2231,-2122,-2010,-1896,-1781,-1664,-1545,-1424,-1302,-1179,-1055,-929,-803,-676,-548,-419,-291,-162,-32,97,226,355,484,612,739,866,992,1117,1241,1363,1485,1604,1722,1839,1953,2066,2177,2285,2391,2495,2596,2695,2791,2884,2974,3062,3146,3227,3305,3380,3451,3519,3583,3644,3701,3755,3805,3850,3893,3931,3965,3995,4022,4044,4062,4077,4087,4093,4095},
//			tabsygnalsin[200]={2048,2112,2177,2241,2305,2369,2433,2496,2559,2622,2683,2744,2805,2865,2923,2981,3038,3094,3150,3203,3256,3308,3358,3407,3455,3501,3546,3589,3631,3671,3710,3747,3782,3815,3847,3877,3905,3931,3956,3978,3999,4017,4034,4049,4061,4072,4081,4087,4092,4094,4095,4093,4090,4084,4077,4067,4055,4042,4026,4008,3989,3967,3944,3918,3891,3862,3831,3799,3764,3728,3691,3651,3610,3568,3524,3478,3431,3383,3333,3282,3230,3177,3122,3067,3010,2952,2894,2835,2775,2714,2652,2590,2528,2465,2401,2337,2273,2209,2144,2080,2015,1951,1886,1822,1758,1694,1630,1567,1505,1443,1381,1320,1260,1201,1143,1085,1028,973,918,865,813,762,712,664,617,571,527,485,444,404,367,331,296,264,233,204,177,151,128,106,87,69,53,40,28,18,11,5,2,0,1,3,8,14,23,34,46,61,78,96,117,139,164,190,218,248,280,313,348,385,424,464,506,549,594,640,688,737,787,839,892,945,1001,1057,1114,1172,1230,1290,1351,1412,1473,1536,1599,1662,1726,1790,1854,1918,1983,2048};
			tabsygnalsin[200]={0,129,258,387,516,644,771,898,1023,1148,1272,1394,1515,1634,1752,1868,1982,2094,2204,2312,2417,2521,2621,2719,2815,2907,2997,3083,3167,3247,3324,3398,3469,3536,3599,3659,3715,3768,3816,3861,3902,3940,3973,4002,4028,4049,4066,4080,4089,4094,4095,4092,4085,4073,4058,4039,4016,3988,3957,3922,3882,3839,3792,3742,3687,3629,3568,3502,3434,3362,3286,3207,3125,3040,2952,2861,2767,2671,2571,2469,2365,2258,2149,2038,1925,1810,1693,1575,1454,1333,1210,1086,961,835,708,580,452,323,194,65,-65,-194,-323,-452,-580,-708,-835,-961,-1086,-1210,-1333,-1454,-1575,-1693,-1810,-1925,-2038,-2149,-2258,-2365,-2469,-2571,-2671,-2767,-2861,-2952,-3040,-3125,-3207,-3286,-3362,-3434,-3502,-3568,-3629,-3687,-3742,-3792,-3839,-3882,-3922,-3957,-3988,-4016,-4039,-4058,-4073,-4085,-4092,-4095,-4094,-4089,-4080,-4066,-4049,-4028,-4002,-3973,-3940,-3902,-3861,-3816,-3768,-3715,-3659,-3599,-3536,-3469,-3398,-3324,-3247,-3167,-3083,-2997,-2907,-2815,-2719,-2621,-2521,-2417,-2312,-2204,-2094,-1982,-1868,-1752,-1634,-1515,-1394,-1272,-1148,-1023,-898,-771,-644,-516,-387,-258,-129,0};
	uint32_t Irms[3],
			Urms[3],
	 	 	I2[3]={0,0,0}, 	// tablica kwadratow pradow, suma dla 20ms
			U2[3]={0,0,0},	// tablica kwadratow napiec, suma dla 20ms
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
	uint32_t ADCvalue[8], testFdioda, ui32Period, numerProbki=0, n=0;

	volatile char time[], temp[5], ADCvalueChar[20];
	float ADCvalueVolts=0, ADCvalueVoltsP=0,diff,diffsum=0,diffsum3sek;


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
		PORTF_PIN2 |=0xFF;
	//	uint8_t stanPortuF = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
	//	SW1on = stanPortuF & 0x01;
		// pobranie i wywietlenie temperatury, czasu //
	//	getDs3231BCDDec(HOURS);getDs3231BCDDec(MINUTES);getDs3231BCDDec(SECONDS);
	//	sprintf((char*)&time, "%d:%d:%d", getDs3231BCDDec(HOURS),getDs3231BCDDec(MINUTES),getDs3231BCDDec(SECONDS));
	//	LCDWriteText(time,0,0);
		//getDs3231Temp((char*)&temp);
		//LCDWriteText(temp,1,0);
		////////////////////////////////////////
	//	SysCtlDelay(5000);

		if(numerProbki==0){

		}

		ADCvalueVolts = (Uh1[1] * 3.31)/4095;
		ADCvalueVoltsP = (Urms[1] * 3.31)/4095;


		sprintf((char*)&ADCvalueChar, "Uh1:%.2lf r2:%.2lf", ADCvalueVolts, ADCvalueVoltsP);
		LCDWriteText(ADCvalueChar,0,0);
		sprintf((char*)&ADCvalueChar, "THD:%.2lf%%", (diffsum3sek/150)*100);
		LCDWriteText(ADCvalueChar,1,0);

		PORTF_PIN2 &=~0xFF;
	}
}

void portConfig(){
	/******** PORT F GPIO ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);															// odblokowanie peryferiów portu F
//		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
//		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
//	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);											// typ pinów - wejscia		// PORTF
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);							// typ pinów - wyjscia
//	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);		// konfiguracja portow

	/******** PORT E ADC0 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // odblokowanie peryferiów portu A
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3);
//	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
//	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
//	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
//	ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
//	ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);
//	ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
//	ADCSequenceEnable(ADC0_BASE, 0);
	ADC_0_1_InitSeq012();

	/******** PORT A SPI  ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/******** PORT A I2C1 ******/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
	I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
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

//void getAdcData(uint32_t * ADCvalue){
//	ADCIntClear(ADC0_BASE, 0);
//	ADCProcessorTrigger(ADC0_BASE, 0);
//	while(!ADCIntStatus(ADC0_BASE, 0, false))
//	{
//	}
//	ADCSequenceDataGet(ADC0_BASE, 0, ADCvalue);
//}

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
	int i=0, j=0;
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

//	getAdcData(ADCvalue);
	// licznik wywolanych przerwan
	if(numerProbki==199){
		numerProbki=0;
		for(i=0;i<3;i++){
			// wartosc skuteczna pradu
			Irms[i]=SquareRootRounded(I2[i]/200);
			// wartosc skuteczna napiecia
			Urms[i]=SquareRootRounded(U2[i]/200);
			// usrednione za okres 20 ms sygnal*sin oraz sygnal*cos
			tabIs20ms[i]=(tabIs20ms[i]/200);
			tabIc20ms[i]=(tabIc20ms[i]/200);
			tabUs20ms[i]=(tabUs20ms[i]/200);
			tabUc20ms[i]=(tabUc20ms[i]/200);
			// kwadrat pierwszej harmonicznej pradu | pierwsza harmoniczna pradu
			Ih1kwadrat[i]=(tabIs20ms[i]*tabIs20ms[i]+tabIc20ms[i]*tabIc20ms[i]);
			Ih1[i]=SquareRootRounded(Ih1kwadrat[i]);
			// kwadrat pierwszej harmonicznej napiecia | pierwsza harmoniczna napiecia
			Uh1kwadrat[i]=(tabUs20ms[i]*tabUs20ms[i]+tabUc20ms[i]*tabUc20ms[i]);
			Uh1[i]=SquareRootRounded(Uh1kwadrat[i]);
			// wartosc reszty harmonicznych
			Ihnkwadrat[i]=Irms[i]*Irms[i]-Ih1kwadrat[i];
			Uhnkwadrat[i]=Urms[i]*Urms[i]-Uh1kwadrat[i];
			Ihn[i]=SquareRootRounded(Ihnkwadrat[i]);
			Uhn[i]=SquareRootRounded(Uhnkwadrat[i]);
			I2[i]=0;
			U2[i]=0;
			tabIs20ms[i]=0;
			tabIc20ms[i]=0;
			tabUs20ms[i]=0;
			tabUc20ms[i]=0;
		}
//////////////// wyznaczenie THD dla 3 sekund ///////////////
		if(n>149){
			diffsum3sek=diffsum;
			n=0;
			diffsum=0;
		}
		else{
			diff=(float)Uhn[1]/(float)Uh1[1];
			n++;
			diffsum+=diff;
		}
//////////////////////////////////////////////////////////////

	}else
		numerProbki++;
	ADC0_1_InSeq1_2(pomiar_prad, pomiar_napiecie);
	// usrednienie pradow
	for(i=0;i<3;i++){

/////////////// usrednianie pomiaru //////////////////////////
		for(j=0;j<5;j++){
			tabdI[i]+=pomiar_prad[i][j]*tabWspolczynnikiUsrednDt[j];
			tabdU[i]+=pomiar_napiecie[i][j]*tabWspolczynnikiUsrednDt[j];
			tabdI2[i]+=pomiar_prad[i][j]*tabWspolczynnikiUsrednDt2[j];
			tabdU2[i]+=pomiar_napiecie[i][j]*tabWspolczynnikiUsrednDt2[j];
			tabI[i]+=pomiar_prad[i][j]*tabWspolczynnikiUsredn[j]; 			// dzielenie przez 70 wynika z usredniania
			tabU[i]+=(pomiar_napiecie[i][j]*tabWspolczynnikiUsredn[j]);

		}
///////////////////////////////////////////////////////////////

//		tablicaTEST[i][numerProbki]=U2[i];
		I2[i]+=((tabI[i]/70)*(tabI[i]/70));
		U2[i]+=((tabU[i]/70)*(tabU[i]/70));
		tabIs20ms[i]+=2*(tabI[i]/40*tabsinx[numerProbki])/4095;
		tabIc20ms[i]+=2*(tabI[i]/70*tabcosx[numerProbki])/4095;
		tabUs20ms[i]+=2*(tabU[i]/70*tabsinx[numerProbki])/4095;
		tabUc20ms[i]+=2*(tabU[i]/70*tabcosx[numerProbki])/4095;
		tabI[i]=0;
		tabU[i]=0;
		tabdI[i]=0;
		tabdU[i]=0;
		tabdI2[i]=0;
		tabdU2[i]=0;
	}

	PORTF_PIN1 &=(~0x02);
}
