#include "define.h"

#define MASTER 1
#define SLAVE  0
#define SD     1

/* Fat File system */
FATFS FatFs;		                 // FatFs work area needed for each volume
FIL Fil;			                 // File object needed for each open file 
FRESULT fr;
FILINFO fno;



/* UART1 INTERRUPT HANDLER */
char rx_buffer[16];
int rx_flag = 0;
int buffer_ptr = 0;
#pragma CODE_SEG NON_BANKED
interrupt void UART1_RX_IRS(void)     // Interrupt Service Routine for /IRQ pin
{
	char c;
	
	if(SCI1SR1 & SCI1SR1_RDRF_MASK)	
		c = SCI1DRL;
	//PORTB = c; // For debuging
	
	if(c == 0x0d)
	{
		buffer_ptr = 0;
		rx_flag = 1;	
	}
	else
	{
		rx_buffer[buffer_ptr] = c;
		buffer_ptr++;
	}	
}

/* Interrupt Service Routine for Timer */
unsigned int task1 = 0, task2 = 0;
#pragma CODE_SEG NON_BANKED
interrupt void TIMER_OVERFLOW(void) // 'Interrupt' keyword: tells the C compiler that this function is an ISR
{
		task1++;
		task2++;
		TFLG2 = 0x80;
}

#pragma CODE_SEG NON_BANKED
/* Interrupt Service Routine for Unimplemented ISR */
interrupt void UnimplementedISR(void)
{
   /* Unimplemented ISRs trap.*/
   asm BGND;
}

/* Setting my own interrupt vector table */
#pragma CODE_SEG DEFAULT /* change code section to DEFAULT (for Small Memory Model, this is $C000) */
typedef void (*near tIsrFunc)(void);   // Interrupt Vector Table
const tIsrFunc _vect[] @0xFFD4 = {     // 0xFFD4 is the address to store the IRQ interrupt vector
	UART1_RX_IRS, 					   // UART1 Interrupt
	UnimplementedISR,
	UnimplementedISR,
	UnimplementedISR,
	UnimplementedISR,
	TIMER_OVERFLOW                     // Timer Overflow
};

void timer_init()
{
	TSCR1 = 0x80;  		//enable timer
	TSCR2 = 0x83;  		//Prescaler: 8 = 128ms delay
}

byte custom_char[] = {
	0b00100, // 0: temperature
	0b01010,
	0b01010,
	0b01010,
	0b01110,
	0b11111,
	0b11111,
	0b01110,
	
	0b01000, // 1: farenhiet
	0b10100,
	0b01000,
	0b00011,
	0b00100,
	0b00111,
	0b00100,
	0b00100,
		
	0b01000, // 2: centigrade
	0b10100,
	0b01000,
	0b00011,
	0b00100,
	0b00100,
	0b00100,
	0b00011,
	
	0b01001, // 3: sun 1
	0b00100,
	0b00001,
	0b11010,
	0b00010,
	0b00001,
	0b00100,
	0b01001,
	
	0b00010, // 4: sun 2
	0b00100,
	0b10000,
	0b01000,
	0b01011,
	0b10000,
	0b00100,
	0b00010
};

void main(void) 
{
	#if MASTER == 1
	
		char * filename = "data.csv";
		char buffer[100]="";
		const char s[2] = ",";
		char spi_data = 0xAA;
		char * spi_data_ptr = &spi_data;
		char * token;
		int k = 0;
		UINT bw;
	    float temp = 0;
	    int log_flag = 1;
	    
		DDRB = 0xFF;    //PORTB as output
		DDRJ = 0xFF;    //PTJ as output for Dragon12+ LEDs
		PTJ  = 0x0;     //Allow the LEDs to dsiplay data on PORTB pins
		DDRP = 0xFF;
		PTP  = 0x0F;
		DDRH = 0x00;
		
		adc_init();
		lcd_init();
		lcd_custom_char(custom_char, 5);
		uart1_init();
		timer_init();
		
		__asm CLI;
		
		#if SD == 1
			/* Mount the file */
			f_mount(&FatFs, "", 0);
				
			/* Writing to SD card and read it back */
			fr = f_open(&Fil, filename, FA_WRITE|FA_CREATE_NEW);
			sprintf(buffer, "address,temperature,luminious\r\n");
			f_write(&Fil, buffer, sizeof(buffer), 0);
			f_close(&Fil);
		#endif		
		
		lcd_set_cursor(0,0);
		lcd_send_str("Sensor ");
		
		lcd_set_cursor(0,1);
		lcd_send_char(0x00);
		lcd_send_char(0x01); // f
		//lcd_send_char(0x02); // c
		
		lcd_set_cursor(8,1);
		lcd_send_char(0x03);
		lcd_send_char(0x04);
		for(;;)
		{
			if(task1 > 2)
			{
				task1 = 0;
				PORTB ^= (1 << 0);
			}
		    if(rx_flag)
	        {
	        	rx_flag = 0;
	        	f_write(&Fil, rx_buffer, sizeof(rx_buffer), 0);
	        	lcd_set_cursor(7,0);
	        	token = strtok(rx_buffer, s);
	        	lcd_set_cursor(3,1);
	        	token = strtok(NULL, s);
	        	lcd_send_str(token);
	        	lcd_set_cursor(11,1);
	        	token = strtok(NULL, s);
	        	lcd_send_str(token);
	        }
	        if(!(PTH & 0x01))
	        {
	        	lcd_clear();
	        	lcd_set_cursor(0,0);
	        	lcd_send_str("Removing...");
	        	f_close(&Fil);
	        }
		}
		
	#elif SLAVE == 1
	
		int address = 1;
		float temperature;
		int  luminious;
		char buffer[16]="";
		
		DDRB = 0xFF;    //PORTB as output
		DDRJ = 0xFF;    //PTJ as output for Dragon12+ LEDs
		PTJ  = 0x0;     //Allow the LEDs to dsiplay data on PORTB pins
		DDRP = 0xFF;
		PTP  = 0x0F;
		
		
		lcd_init();
		lcd_send_str("Sensor 1");
		lcd_set_cursor(0,1);
		lcd_send_str("Initializing...");
		lcd_custom_char(custom_char, 5);
		adc_init();
		uart1_init();
		timer_init();
		
		lcd_clear();
		lcd_set_cursor(0,0);
		lcd_send_char(0x00);
		lcd_send_char(0x01);
		
		lcd_set_cursor(0,1);
		lcd_send_char(0x03);
		lcd_send_char(0x04);
		
		lcd_set_cursor(3,0);
		lcd_send_str("NA");
		lcd_set_cursor(3,1);
		lcd_send_str("NA");
		__asm CLI;
		
		for(;;)
		{
		 	if(task1 >= 2)
			{
				task1 = 0;
				PORTB ^= (1 << 0);
			}
			if(task2 >= 10)
			{
				task2 = 0;
				luminious = adc_conversion(4);
				sprintf(buffer, "%d      ", luminious);
				lcd_set_cursor(3,1);
				lcd_send_str(buffer);
				temperature = adc_conversion(5) * 0.4887;
				sprintf(buffer, "%.1f    ", temperature);
				lcd_set_cursor(3,0);
				lcd_send_str(buffer);
				sprintf(buffer, "%d,%.1f,%d\r", address, temperature, luminious);
				uart1_send_str(buffer);
			}
		}
	#endif
	
}
