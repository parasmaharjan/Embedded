#include "delay.h"


/* Delay function */
void delay_ms(unsigned int itime)
{
        unsigned int i; unsigned int j;
        for(i=0;i<itime;i++)
			for(j=0;j<4000;j++);
}
