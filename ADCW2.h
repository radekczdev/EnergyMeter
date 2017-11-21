//konfiguracja 
/*AIN0-PE3
AIN1-PE2
AIN2-PE1
AIN3-PE0
AIN4-PD3
AIN5-PD2
AIN6-PD1
AIN7-PD0
*/
#define GPIO_PORTF_DATA_R		    (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_PDR_R        (*((volatile uint32_t *)0x40025514))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile uint32_t *)0x4002552C))
//REJESTRY PORTFE
#define GPIO_PORTE_DATA_R		    (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile uint32_t *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))

#define GPIO_PORTD_DATA_R		    (*((volatile uint32_t *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile uint32_t *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile uint32_t *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile uint32_t *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile uint32_t *)0x4000751C))
#define GPIO_PORTD_AMSEL_R      (*((volatile uint32_t *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile uint32_t *)0x4000752C))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))

//rejestry ADC0
#define SYSCTL_RCGC0_R          (*((volatile uint32_t *)0x400FE100))
#define ADC0_ACTSS_R            (*((volatile uint32_t *)0x40038000))
#define ADC0_RIS_R              (*((volatile uint32_t *)0x40038004))
#define ADC0_IM_R               (*((volatile uint32_t *)0x40038008))
#define ADC0_ISC_R              (*((volatile uint32_t *)0x4003800C))
#define ADC0_EMUX_R             (*((volatile uint32_t *)0x40038014))
#define ADC0_SSPRI_R            (*((volatile uint32_t *)0x40038020))
#define ADC0_PSSI_R             (*((volatile uint32_t *)0x40038028))
#define ADC0_SPC             (*((volatile uint32_t *)0x40038024))
#define ADC0_SSMUX0_R           (*((volatile uint32_t *)0x40038040))
#define ADC0_SSMUX1_R           (*((volatile uint32_t *)0x40038060))
#define ADC0_SSMUX2_R           (*((volatile uint32_t *)0x40038080))
#define ADC0_SSMUX3_R           (*((volatile uint32_t *)0x400380A0))
#define ADC0_SSCTL0_R           (*((volatile uint32_t *)0x40038044))
#define ADC0_SSCTL1_R           (*((volatile uint32_t *)0x40038064))
#define ADC0_SSCTL2_R           (*((volatile uint32_t *)0x40038084))
#define ADC0_SSCTL3_R           (*((volatile uint32_t *)0x400380A4))
#define ADC0_SSFIFO0_R          (*((volatile uint32_t *)0x40038048))
#define ADC0_SSFIFO1_R          (*((volatile uint32_t *)0x40038068))
#define ADC0_SSFIFO2_R          (*((volatile uint32_t *)0x40038088))
#define ADC0_SSFIFO3_R          (*((volatile uint32_t *)0x400380A8))
#define ADC0_PC_R               (*((volatile uint32_t *)0x40038FC4))
#define ADC0SAC              		(*((volatile uint32_t *)0x40038030))
#define ADC0CTL              		(*((volatile uint32_t *)0x40038038))

//rejestry ADC1
#define ADC1_ACTSS_R            (*((volatile uint32_t *)0x40039000))
#define ADC1_RIS_R              (*((volatile uint32_t *)0x40039004))
#define ADC1_IM_R               (*((volatile uint32_t *)0x40039008))
#define ADC1_ISC_R              (*((volatile uint32_t *)0x4003900C))
#define ADC1_EMUX_R             (*((volatile uint32_t *)0x40039014))
#define ADC1_SSPRI_R            (*((volatile uint32_t *)0x40039020))
#define ADC1_PSSI_R             (*((volatile uint32_t *)0x40039028))
#define ADC1_SPC             (*((volatile uint32_t *)0x40039024))
#define ADC1_SSMUX0_R           (*((volatile uint32_t *)0x40039040))
#define ADC1_SSMUX1_R           (*((volatile uint32_t *)0x40039060))
#define ADC1_SSMUX2_R           (*((volatile uint32_t *)0x40039080))
#define ADC1_SSMUX3_R           (*((volatile uint32_t *)0x400390A0))
#define ADC1_SSCTL0_R           (*((volatile uint32_t *)0x40039044))
#define ADC1_SSCTL1_R           (*((volatile uint32_t *)0x40039064))
#define ADC1_SSCTL2_R           (*((volatile uint32_t *)0x40039084))
#define ADC1_SSCTL3_R           (*((volatile uint32_t *)0x400390A4))
#define ADC1_SSFIFO0_R          (*((volatile uint32_t *)0x40039048))
#define ADC1_SSFIFO1_R          (*((volatile uint32_t *)0x40039068))
#define ADC1_SSFIFO2_R          (*((volatile uint32_t *)0x40039088))
#define ADC1_SSFIFO3_R          (*((volatile uint32_t *)0x400390A8))
#define ADC1_PC_R               (*((volatile uint32_t *)0x40039FC4))
#define ADC1SAC              		(*((volatile uint32_t *)0x40039030))
#define ADC1CTL              		(*((volatile uint32_t *)0x40039038))

//bity konfiguracyjne
#define ADC_ACTSS_ASEN3         0x00000008  // ADC SS3 Enable;steruje praca sekwencera enable/disable
#define ADC_ACTSS_ASEN0         0x00000001  // ADC SS3 Enable;steruje praca sekwencera enable/disable
#define ADC_RIS_INR3            0x00000008  // SS3 Raw Interrupt Status;sygnalizacja ze konwersja jest kompletna po kazdej probce sekw
#define ADC_IM_MASK3            0x00000008  // SS3 Interrupt Mask;sygnal wysylany od komparatora do controlera przerwan
#define ADC_ISC_IN3             0x00000008  // SS3 Interrupt Status and Clear;
#define ADC_EMUX_EM3_M          0x0000F000  // SS3 Trigger Select mask;zdarzenie w wyniku ktorego nastepuje pobranie probki
#define ADC_EMUX_EM3_PROCESSOR  0x00000000  // Processor (default)
#define ADC_SSPRI_SS3_4TH       0x00003000  // fourth priority
#define ADC_SSPRI_SS2_3RD       0x00000200  // third priority
#define ADC_SSPRI_SS1_2ND       0x00000010  // second priority
#define ADC_SSPRI_SS0_1ST       0x00000000  // first priority
#define ADC_PSSI_SS3            0x00000008  // SS3 Initiate;sterowanie praca poszczegolnych sekwencerow
#define ADC_SSMUX3_MUX0_M       0x0000000F  // 1st Sample Input Select mask
#define ADC_SSMUX3_MUX0_S       0           // 1st Sample Input Select lshift
#define ADC_SSCTL3_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL3_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL3_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL3_D0           0x00000001  // 1st Sample Diff Input Select
#define ADC_SSFIFO3_DATA_M      0x00000FFF  // Conversion Result Data mask
#define SYSCTL_RCGC0_ADC        0x00010000  // ADC0 Clock Gating Control
#define SYSCTL_RCGC0_ADCSPD_M   0x00000300  // ADC Sample Speed mask
#define SYSCTL_RCGC0_ADCSPD125K 0x00000000  // 125K samples/second
void ADC0_InitSeq1(void);
void ADC1_InitSeq2(void);
void ADC_0_1_InitSeq012(void);
unsigned long ADC0_InSeq0(unsigned long data[3]);
unsigned long ADC1_InSeq1(unsigned long data[3]);
unsigned long ADC1_InSeq2(unsigned long data1[4]);
unsigned long ADC0_1_InSeq1_2(unsigned long prad[3][5],unsigned long napiecie[3][5]);


