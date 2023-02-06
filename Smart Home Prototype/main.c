#include "tm4c123gh6pm.h"
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

volatile int buzzer_value = 8000;
void main_init(void);
void UART_init(void);
void intrupt_init(void);
void UART0Tx(char c);
void sender(char arrr[]);
void PWM_INIT();
void ADC_init();
void delayMs(int n);

volatile float x = 0;
volatile float y = 0;
char array[4];
char array1[4];

// Temp (PE3) > smoke  (PE2) > IR (PF1,PF2)
int main(void)
{
	main_init();
	UART_init();
	intrupt_init();
	PWM_INIT();
	ADC_init();
	ADC0->PSSI |= 8;
	ADC1->PSSI |= 8;

	while (1)
	{

		PWM1->_3_CMPA = 0;
		sender("\x1b[2J");
		sender("\x1b[0;0H");
		sender("Intruder not at IR-1");
		sender("\n\r");
		sender("Intruder not at IR-2");
		sender("\n\r");

		sender("Temperature: ");
		x = (3.3 * x * 100.00) / (4096.0);
		sprintf(array, "%.2f", x);
		sender(array);
		sender("\n\r");

		sender("Smoke: ");
		sprintf(array1, "%.2f", y);
		sender(array1);

		sender("\n\r");

		//
		if (x > 30)
		{
			PWM1->_3_CMPA = buzzer_value;
			ADC0->PSSI |= 8;
			sender("\x1b[2J");
			sender("\x1b[0;0H");
			sender("Temperature High");
			sender("\n\r");
			sender("Temperature: ");
			sender(array);
			sender("\n\r");
			// delayMs(300);
		}

		if (y > 900)
		{
			PWM1->_3_CMPA = buzzer_value;
			sender("\x1b[2J");
			sender("\x1b[0;0H");
			sender("Smoke Level High");
			sender("\n\r");
			sender("Smoke: ");
			sender(array1);
			sender("\n\r");
			// delayMs(300);
			// ADC1->PSSI |=8;
		}
		delayMs(300);

		ADC0->PSSI |= 8;
		ADC1->PSSI |= 8;
	}
}

void main_init()
{
	SYSCTL->RCGCUART |= 1;

	// CLOCK FOR PORT A,E,F

	SYSCTL->RCGCGPIO |= 0x31;

	SYSCTL->RCGCADC |= 3;

	GPIOF->LOCK = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0X01;
	GPIOF->LOCK = 0;

	GPIOF->DIR = 4;
	GPIOF->DEN = 0X7;
	GPIOF->PUR = 0X3;
	GPIOF->DATA |= 4;

	// PORT E

	GPIOE->AFSEL |= 0xE;
	GPIOE->DEN &= ~0xE;
}

void UART0Tx(char c)
{
	while ((UART0->FR & 0x0020) != 0)
	{
	}
	UART0->DR = c;
}

void sender(char arrr[])
{
	int i = 0;
	for (i = 0; i < strlen(arrr); i++)
	{
		UART0Tx(arrr[i]);
	}
}

void UART_init()
{

	// Now we will initialize the Uart0
	UART0->CTL = 0;
	UART0->IBRD = 8;
	UART0->FBRD = 44;
	UART0->CC = 0;
	UART0->LCRH = 0X60;

	UART0->IM |= -0X0010;
	UART0->CTL = 0X301;

	// PORT A
	GPIOA->DEN = 0X03;
	GPIOA->AFSEL = 0X03;
	GPIOA->PCTL = 0X11;

	NVIC->IP[5] = 3 << 5;
	NVIC->ISER[0] |= 0X00000020;
}

void intrupt_init()
{
	GPIOF->IS &= ~0X3;
	GPIOF->IBE &= ~0X3;

	GPIOF->IEV &= 0X3;
	GPIOF->ICR |= 0X3;
	GPIOF->IM |= 0X3;

	NVIC->IP[30] = 1 << 5;
	NVIC->ISER[0] |= 0X40000000;

	__enable_irq();
}

void GPIOPortF_Handler()
{

	volatile int readback;

	while (GPIOF->MIS != 0)
	{
		sender("\x1b[2J");
		sender("\x1b[0;0H");

		PWM1->_3_CMPA = buzzer_value;
		if (GPIOF->MIS & 0x2)
		{

			sender("Intruder at IR-2");
			sender("\n\r");

			GPIOF->ICR |= 0X02;
			readback = GPIOF->ICR;
		}
		if (GPIOF->MIS & 0x1)
		{

			sender("Intruder at IR-1");
			sender("\n\r");

			GPIOF->ICR |= 0X01;
			readback = GPIOF->ICR;
		}
		delayMs(2000);
	}

	GPIOF->ICR |= 0X11;
	readback = GPIOF->ICR;
}

void PWM_INIT()
{
	SYSCTL->RCGCPWM |= 2;

	SYSCTL->RCC &= ~0X00100000;

	GPIOF->AFSEL = 4;
	GPIOF->PCTL |= 0X00000500;

	PWM1->_3_CTL = 0;
	PWM1->_3_GENA = 0X0000008C;
	PWM1->_3_LOAD = 16000;
	PWM1->_3_CMPA = 0;

	PWM1->_3_CTL = 1;
	PWM1->ENABLE = 0X40;
}

void ADC_init()
{
	// INITIALIZING ADC

	ADC0->ACTSS &= ~8;
	ADC1->ACTSS &= ~8;

	ADC0->EMUX &= 0x0000;
	ADC1->EMUX &= 0x0000;

	ADC0->SSCTL3 |= 0X6;
	ADC1->SSCTL3 |= 0X6;

	ADC0->SSMUX3 = 0;
	ADC1->SSMUX3 = 1;

	ADC0->ACTSS |= 8;
	ADC1->ACTSS |= 8;

	ADC0->IM |= 0x0008;
	ADC1->IM |= 0x0008;

	NVIC->IP[17] = 2 << 5; // PE3 (Temp Sensor)  High Priority
	NVIC->IP[51] = 3 << 5; // PE2 (Smoke Sensor)  Low Priority

	NVIC->ISER[0] |= 0x20000;
	NVIC->ISER[1] |= 0x80000;

	ADC0->PSSI |= 8;
	ADC1->PSSI |= 8;
	//
}
void ADC0Seq3_Handler()
{
	volatile int readback;

	x = ADC0->SSFIFO3;

	ADC0->ISC |= 0x08;
	readback = ADC0->ISC;
}

void ADC1Seq3_Handler()
{

	volatile int readback;

	y = ADC1->SSFIFO3;
	ADC1->ISC |= 0x08;
	readback = ADC1->ISC;
}

void UART0_Handler(void)
{
	volatile int readback;
	char c;

	if (UART0->MIS & 0x0010)
	{
		c = UART0->DR;
		if (c == 'a')
		{
			buzzer_value = 8000;
		}
		else if (c == 's')
			buzzer_value = 0;

		UART0->ICR = 0X0010;
		readback = UART0->ICR;
	}
	else
	{

		UART0->ICR = UART0->MIS;
		readback = UART0->ICR;
	}
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < 3180; j++)
		{
		} /* do nothing for 1 ms */
}
void SystemInit()
{
	SCB->CPACR |= 0XF00000;
}