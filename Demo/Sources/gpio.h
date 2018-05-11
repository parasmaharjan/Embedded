#ifndef __GPIO_H__
#define __GPIO_H__

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define HIGH 0xFF
#define LOW  0x0


enum 
{	
	GPIOA = 0, 
	GPIOB, 
	GPIOE, 
	GPIOH, 
	GPIOJ, 
	GPIOK, 
	GPIOM, 
	GPIOP, 
	GPIOS, 
	GPIOT
};

enum 
{
	PIN0 = 0x01,
	PIN1 = 0x02,
	PIN2 = 0x04,
	PIN3 = 0x08,
	PIN4 = 0x10,
	PIN5 = 0x20,
	PIN6 = 0x40,
	PIN7 = 0x80
};

void gpio_init();
unsigned char gpio_inport(unsigned char gpio, unsigned char pin);
void gpio_outport(unsigned char gpio, unsigned char pin, unsigned char value);

#endif