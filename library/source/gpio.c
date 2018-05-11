/***************************************************
 * GPIO library for HCS12                          *
 *                                                 *
 * Example:                                        *
 *                                                 *
 * void main(void)                                 *
 * {                                               *
 * 	gpio_init();                                   *
 * 	gpio_outport(GPIOJ, (1 << 1), LOW);            *
 * 	for(;;)                                        *
 *	{                                              *
 *		if(gpio_inport(GPIOH, (1 << 1)))           *
 *		{                                          *
 *			gpio_outport(GPIOB, (1 << 1), HIGH);   *
 *			delay_ms(500);                         *
 *			gpio_outport(GPIOB, (1 << 1), LOW);    *
 *			delay_ms(500);                         *
 *		}                                          *
 *	}		                                       *
 * }                                               *
 *                                                 *
 ***************************************************/


#include "gpio.h"

void gpio_init()
{
	
}


unsigned char gpio_inport(unsigned char gpio, unsigned char pin)
{
	unsigned char * port;
	unsigned char value = 0xFF;
	switch(gpio)
	{
		case GPIOA:
			DDRA &= ~(pin);
			port = &PORTA;
			break;
		case GPIOB:
			DDRB &= ~(pin);
			port = &PORTB;
			break;
		case GPIOE:
			DDRE &= ~(pin);
			port = &PORTE;
			break;
		case GPIOH:
			DDRH &= ~(pin);
			port = &PTH;
			break;
		case GPIOJ:
			DDRJ &= ~(pin);
			port = &PTJ;
			break;
		case GPIOK:
			DDRK &= ~(pin);
			port = &PORTK;
			break;
		case GPIOM:
			DDRM &= ~(pin);
			port = &PTM;
			break;
		case GPIOP:
			DDRP &= ~(pin);
			port = &PTP;
			break;
		case GPIOS:
			DDRS &= ~(pin);
			port = &PTS;
			break;
		default:
			break;
			
					
	}
	value = *port & pin;
	return value;
}


void gpio_outport(unsigned char gpio, unsigned char pin, unsigned char value)
{
	unsigned char * port;
	switch(gpio)
	{
		case GPIOA:
			DDRA |= pin;
			port = &PORTA;
			break;
		
		case GPIOB:
			DDRB |= pin;
			port = &PORTB;
			break;
		case GPIOE:
			DDRE |= pin;
			port = &PORTE;
			break;
		case GPIOH:
			DDRH |= pin;
			port = &PTH;
			break;
		case GPIOJ:
			DDRJ |= pin;
			port = &PTJ;
			break;
		case GPIOK:
			DDRK |= pin;
			port = &PORTK;
		case GPIOM:
			DDRM |= pin;
			port = &PTM;
		case GPIOP:
			DDRP |= pin;
			port = &PTP;
		case GPIOS:
			DDRP |= pin;
			port = &PTS;
		default:
			break;
	}
	
	*port &= ~(pin);
	*port |= (value & pin);
}
