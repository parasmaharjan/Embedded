#include "adc.h"

void adc_init()
{
	ATD0CTL2 = 0x80;     //Turn on ADC,..No Interrupt
	delay_ms(5);
	ATD0CTL3 = 0x08;  //one conversion, no FIFO
	ATD0CTL4 = 0x6B;  //8-bit resolu, 16-clock for 2nd phase,
	                  //prescaler of 24 for Conversion Freq=1MHz
}

int adc_conversion(unsigned char channel)
{
	ATD0CTL5 = 0x80 | channel;  //Channel x (right justified, unsigned,single-conver,one chan only) 

	while(!(ATD0STAT0 & 0x80));
	
	return ATD0DR0;
}