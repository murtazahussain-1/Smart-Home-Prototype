#include "tm4c123gh6pm.h"
# include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"

# include <stdint.h>

#include <string.h>
#include <stdio.h>



void UART_init();


void UART0Tx(char c);

void sender(char arrr[]);



void delayMs(int n);


int main(void)
{
	
	UART_init();

	
	//sprintf(array, "%f", x);
	while(1)
	{sender("What's your Name");
	sender("\n\r");
	delayMs(2000);}

}


void UART0Tx(char c)
{
	while((UART0->FR & 0x0020) !=0){}
	UART0->DR = c;
}


void sender(char arrr[])
{
int i=0;
for (i=0;i<strlen(arrr);i++)
	{
		UART0Tx(arrr[i]);
		//UART0Rx();
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