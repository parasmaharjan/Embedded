#include "spi.h"

void spi_init()
{
	SPI_SS_PORT = SPI_SS_PORT | 0xE0;   /* SS, SCLK, MOSI outputs */
	DPI_SS_PIN = DPI_SS_PIN | 0x80;     /* Bring SS high to deselect slave */
	SPI0CR1 = 0x50;	
}


void spi_slave_select(int ss)
{
	if(ss)
		DPI_SS_PIN = DPI_SS_PIN & 0x80; /* Bring SS low to select device */
	else
		DPI_SS_PIN = DPI_SS_PIN | 0x80; /* Bring SS high to deselect device */	
}


unsigned char spi_transfer(unsigned char data)
{
	spi_slave_select(1);
	SPI0DR = data; /* Write junk byte to device */
	while ((SPI0SR & 0x80)==0) ; /* Wait for transfer to finish */
	data = SPI0DR; /* Get data byte */
	spi_slave_select(0);
	return data;
}