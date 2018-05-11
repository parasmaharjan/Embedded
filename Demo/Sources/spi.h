#ifndef __SPI_H__
#define __SPI_H__

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define SPI_SS_PORT DDRS
#define DPI_SS_PIN  PTS 

void spi_init();
void spi_slave_select(int ss);
unsigned char spi_transfer(unsigned char data);

#endif