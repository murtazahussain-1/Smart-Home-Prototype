#include "tm4c123gh6pm.h"
# include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
# include <stdint.h>

#include <string.h>
#include <stdio.h>
void delayMs(int n);
void intrupt_init(void);
void UART0Tx(char c)
{
	while((UART0->FR & 0x0020) !=0){}
		UART0->DR = c;
}
void main_init()
{
	SYSCTL->RCGCUART |=1;
	
	
	
	SYSCTL->RCGCGPIO |=0x31;
	
	GPIOF->LOCK=0x4C4F434B;
	GPIO_PORTF_CR_R = 0X01;
	GPIOF->LOCK=0;
	
	GPIOF->DIR =0;
	GPIOF->DEN=0X1E;

	
}





void UART_init()
{
	

	//Now we will initialize the Uart0
	UART0->CTL=0;
	UART0->IBRD=8;
	UART0->FBRD=44;
	UART0->CC=0;
	UART0->LCRH=0X60;
	UART0->IM |=-0X0010;
	UART0->CTL=0X301;
	
	
	// PORT A
	GPIOA->DEN=0X03;
	GPIOA->AFSEL=0X03;
	GPIOA->PCTL=0X11;
	
	NVIC->IP[5]=3<<5;
	NVIC->ISER[0] |=0X00000020;
	
	__enable_irq();


}

int main(void)
{
	
	main_init();
	UART_init();


	while(1)

	{
		
	}

}

void UART0_Handler(void)
{
	volatile int readback;
	char c;
	
	if(UART0->MIS & 0x0010)
	{
		c=UART0->DR;
		UART0Tx(c);
		GPIOF->DATA=c<<1;
		UART0->ICR=0X0010;
		readback=UART0->ICR;
	}
	else
	{
		UART0->ICR=UART0->MIS;
		readback = UART0->ICR;
	}
	
}









/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++)
{} /* do nothing for 1 ms */
}
void SystemInit()
{
	SCB->CPACR|=0XF00000;
}