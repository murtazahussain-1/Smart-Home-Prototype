#include "tm4c123gh6pm.h"
# include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
# include <stdint.h>
void delayMs(int n);
void intrupt_init()
{
	
}

int main(void)
{
	
	intrupt_init();
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