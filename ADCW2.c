#include "ADCW2.h"
#include <stdint.h>

unsigned long delay;
//ustawienie wejscie analogowych dla nozek PORTD0-3
void GPIOD_ANALOG_INIT(void){
	GPIO_PORTD_DIR_R &= ~0x07;      // 3) make PD3,PD2,PD1,PD0 input
  GPIO_PORTD_AFSEL_R |= 0x07;     // 4) enable alternate function on PD3,PD2,PD1,PD0
  GPIO_PORTD_DEN_R &= ~0x07;      // 5) disable digital I/O on PD3,PD2,PD1,PD0
                                  // 5a) configure PD3,PD2,PD1,PD0 as ?? (skip this line because PCTL is for digital only)
	GPIO_PORTD_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFF000)+0x00000000;
  GPIO_PORTD_AMSEL_R |= 0x07;     // 6) enable analog functionality on PD3,PD2,PD1,PD0
}
//wejscia analogowe PORTE
void GPIOE_ANALOG_INIT(void){
  GPIO_PORTE_DIR_R &= ~0x7;      // 3) make PE5,PE4,PE1,PE0 input
  GPIO_PORTE_AFSEL_R |= 0x7;     // 4) enable alternate function on PE5,PE4,PE1,PE0
  GPIO_PORTE_DEN_R &= ~0x7;      // 5) disable digital I/O on PE5,PE4,PE1,PE0
                                  // 5a) configure PE5,PE4,PE1,PE0 as ?? (skip this line because PCTL is for digital only)
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFF000)+0x00000000;
  GPIO_PORTE_AMSEL_R |= 0x7;     // 6) enable analog functionality on PE5,PE4,PE1,PE0
}

//inicjalizacja ADC0 dla sekwencera 1
void ADC0_InitSeq1(void){
	delay = SYSCTL_RCGC2_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGC2_R;
  GPIOD_ANALOG_INIT();
	GPIOE_ANALOG_INIT();
	SYSCTL_RCGC0_R |= 0x00030000;   // 7) activate ADC0 (legacy code)
	SYSCTL_RCGC0_R &= ~0x00000F00;   // 7) sample rate
	SYSCTL_RCGC0_R |= 0x00000F00;
	//SYSCTL_RCGC0_R = (SYSCTL_RCGC0_R&0xFFFFF000)+0x00000000;
  ADC0_SSPRI_R = 0x3201;
	ADC1_SSPRI_R = 0x3102;
  ADC0_ACTSS_R &= ~0x0002;        // 10) disable sample sequencer 0
	ADC0_SPC = 0x0;
  ADC0_EMUX_R = 0x0000;         // 11) seq0 is software trigger
  ADC0_SSMUX1_R &= ~0x00F0;       // 12) clear SS0 field
  ADC0_SSMUX1_R = 0x567;             //    set channel
  ADC0_SSCTL1_R = 0x0600;         // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0002;           // 14) disable SS0 interrupts
  ADC0_ACTSS_R |= 0x0002;         // 15) enable sample sequencer 0
	ADC1_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 1
	ADC1_SPC = 0x0;
  ADC1_EMUX_R = 0x0000;         // 11) seq1,seq2 is software trigger
	ADC1_SSMUX2_R &= ~0x0F00;
	ADC1_SSMUX2_R = 0x123;
	ADC1_SSCTL2_R = 0x0600;
  ADC1_IM_R &= ~0x0004;           // 14) disable SS0 interrupts
  ADC1_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 1
}
//inicjalizacja ADC1 dla sekwencera 2
/*void ADC1_InitSeq2(void){ unsigned long delay;
	delay = SYSCTL_RCGC2_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGC2_R;
	GPIOE_ANALOG_INIT();
	//SYSCTL_RCGC0_R |= 0x00020000;   // 7) activate ADC1 (legacy code),str.455
	//SYSCTL_RCGC0_R &= ~0x00000300;   // 7) activate ADC1 (legacy code)
  //SYSCTL_RCGC0_R |= 0x00000400;  // 8) configure for 125K (legacy code)
  ADC1_SSPRI_R = 0x3102;
  ADC1_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 1
	ADC1CTL = 0x40;
	ADC1_SPC = 0x0;
  ADC1_EMUX_R = 0x0000;         // 11) seq1,seq2 is software trigger
	ADC1_SSMUX2_R &= ~0x0F00;
	ADC1_SSMUX2_R = 0x123;
	ADC1_SSCTL2_R = 0x0600;
  ADC1_IM_R &= ~0x0004;           // 14) disable SS0 interrupts
  ADC1_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 1
}*/

void ADC_0_1_InitSeq012(void){
			ADC0_InitSeq1();
}
unsigned long k;
unsigned long ADC0_1_InSeq1_2(unsigned long prad[3][5],unsigned long napiecie[3][5]){			//odczyt ADC0(Sek1),ADC1(Sek2)
for(k=0;k<5;k++){
ADC0_PSSI_R = 0x08000002;						// 1) initiate SS1
ADC1_PSSI_R = 0x08000004;           // 2) initiate SS2
ADC0_PSSI_R = 0x80000002;           // 3) uruchomienie ADC0,ADC1
GPIO_PORTF_DATA_R |= 0x08;	
while((ADC0_RIS_R&0x02)==0 && (ADC1_RIS_R&0x04)==0){};   // 2) wait for conversion done
	GPIO_PORTF_DATA_R &=~ 0x08;
	//odczyt z Sek1 ADC0
	GPIO_PORTF_DATA_R |= 0x08;
	prad[0][k] = ADC0_SSFIFO1_R&0xFFF;		// 1) read result CH7 PD0
	GPIO_PORTF_DATA_R &=~ 0x08;
	GPIO_PORTF_DATA_R |= 0x08;
  prad[1][k] = ADC0_SSFIFO1_R&0xFFF;   // 2) read result	CH6 PD1
	GPIO_PORTF_DATA_R &=~ 0x08;
	GPIO_PORTF_DATA_R |= 0x08;
	prad[2][k] = ADC0_SSFIFO1_R&0xFFF;   // 3) read result	CH5 PD2
	GPIO_PORTF_DATA_R &=~ 0x08;

	//odczyt z Sek2 ADC1
	GPIO_PORTF_DATA_R |= 0x10;
	napiecie[0][k] = ADC1_SSFIFO2_R&0xFFF;   // 1) read result	CH3 PE0
	GPIO_PORTF_DATA_R &= ~0x10;
	GPIO_PORTF_DATA_R |= 0x10;
  napiecie[1][k] = ADC1_SSFIFO2_R&0xFFF;   // 2) read result	CH2 PE1
	GPIO_PORTF_DATA_R &= ~0x10;
	GPIO_PORTF_DATA_R |= 0x10;
  napiecie[2][k] = ADC1_SSFIFO2_R&0xFFF;   // 3) read result	CH1 PE2
	GPIO_PORTF_DATA_R &= ~0x10;

	ADC0_ISC_R = 0x0002;
	ADC1_ISC_R = 0x0004;
}
	return 1;
}



