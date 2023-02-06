#include "TM4C123GH6PM.h"
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
double x=0;
char array[8];
void UART0Tx(char c)
{
	while((UART0->FR & 0x0020) !=0){}
	UART0->DR = c;
}


void delayMs(int n);
void sender(char arrr[])
{
int i=0;
for (i=0;i<strlen(arrr);i++)
	{
		UART0Tx(arrr[i]);
		
	}
}


void UART_init()
{
	SYSCTL->RCGCUART |=1;
	SYSCTL->RCGCGPIO |=1;
	
	//Now we will initialize the Uart0
	UART0->CTL=0;
	UART0->IBRD=8;
	UART0->FBRD=44;
	UART0->CC=0;
	UART0->LCRH=0X60;
	UART0->CTL=0X301;
	
	//SETTING UP THE PINS
	GPIOA->DEN=0X03;
	GPIOA->AFSEL=0X03;
	GPIOA->PCTL=0X11;


}	



void ADC_init()
{
	
	SYSCTL->RCGCGPIO |=0x10;
	
	SYSCTL->RCGCADC |=2;

	
	
	//PE5
	GPIOE->AFSEL |=0x20;
//	GPIOE->DEN &=~0x20;
//	GPIOE->DATA =0x20;
//	GPIOE->AMSEL |=0x20;

	//INITIALIZING ADC
	ADC1->ACTSS &=~8;
	
	ADC1->EMUX &=0x4000;

	ADC1->SSCTL3 |=6;
	
	
	ADC1->SSMUX3 =1;
	
	ADC1->ACTSS |=8;
	
	
	ADC1->IM |=0x0008;
	
	NVIC->IP[51]=3<<5;
	
	//
	NVIC->ISER[1] |=0x80000;
	//
	
	ADC1->PSSI |=8;
	//GPIOE->IM=0xE;
	

		
	__enable_irq();

}


int main(void)
{
	UART_init();
ADC_init();		
	
		
	SYSCTL->RCGCGPIO |=0x20;
	
	GPIOF->DIR |=0x1e;
	GPIOF->DEN |=0x1e;
	
	GPIOF->DATA=0X04;

	
	

	while(1)
	{
		
		GPIOF->DATA^=0X04;
			ADC1->PSSI |=8;
	delayMs(1000);
	
	}
	
}
void  ADC1Seq3_Handler ()
{
	volatile int readback;
	
	
	
	x=ADC1->SSFIFO3;

		
	x=(3.3*x*100)/(4096.0);
		
		sprintf(array, "%f", x);
		sender(array);
		sender("\n\r");
		
	ADC1->ISC |=0x0E;
	readback=ADC1->ISC;

	
	
}


void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++)
{} /* do nothing for 1 ms */
}

void  SystemInit(void)
{
	__disable_irq();
	SCB->CPACR |=0X00F00000;
}