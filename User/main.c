#include "stm32f10x.h"
#include "lcd.h"


/*------------------------------------------------------------
PROJ Template
-------------------------------------------------------------*/



int main(void)
{

  while (1) {

  }
}


void Delayus(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}
