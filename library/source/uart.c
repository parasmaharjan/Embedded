#include "uart.h"

void uart0_init()
{
    SCI0BDH=0x00;   //Serial Monitor used for LOAD works at 48MHz  
    SCI0BDL=26;     //8MHz/2=4MHz, 4MHz/16=250,000 and 250,000/9600=26 
    
    SCI0CR1=0x00;
    SCI0CR2=0x2C;   // Enable interrupt and TX/RX
}

void uart1_init()
{
	SCI1BDH=0x00;   //Serial Monitor used for LOAD works at 48MHz  
    SCI1BDL=26;     //8MHz/2=4MHz, 4MHz/16=250,000 and 250,000/9600=26 
    
    SCI1CR1=0x00;
    SCI1CR2=0x2C;	// Enable interrupt and TX/RX
}

void uart0_send_char(unsigned char c)  //SCI0 (COM0 of HCS12 chip)
{
	while(!(SCI0SR1 & 0x80));  //make sure the last bit is gone
	SCI0DRL = c;		       //before giving it another byte	
}

void uart0_send_str(char * str)
{
	while(*str)
	{
		uart_send_char(*str);
		str++;
	}
}

void uart1_send_char(unsigned char c)  //SCI0 (COM0 of HCS12 chip)
{
	while(!(SCI1SR1 & 0x80));  //make sure the last bit is gone
	SCI1DRL = c;		       //before giving it another byte	
}

void uart1_send_str(char * str)
{
	while(*str)
	{
		uart1_send_char(*str);
		str++;
	}
}


