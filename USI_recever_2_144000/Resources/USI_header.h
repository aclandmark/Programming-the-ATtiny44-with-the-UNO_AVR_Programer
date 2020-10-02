

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


/**********************************************************************************************************************/
#define T0_delay_5ms 5,220
#define T1_delay_4sec 5,0x85F7
#define T1_delay_500ms 5,0xF0BE
#define T1_delay_250ms 5,0xF85F
#define T1_delay_50ms 3, 0xCF31
#define T1_delay_100ms 3, 0x9E62


/********************************************************************************************************************/
unsigned char ReverseByte (unsigned char);
void Initialise_USI_Tx (void);
void Tx_data_byte(unsigned char);							//Transmit single characters
unsigned char Char_from_USI (void);							//Receive single characters as typed in at keyboard						
void String_from_USI (unsigned char*);						//Receive strings (were there is no pause between characters)
void String_to_USI(const char*);							//The equivalent of sendString
void Echo_string(unsigned char*);							//Echo string received from keyboard

/**********************************************************************************************************************/


int EEP_MAX = 0x2000;
int sig_byte_2, sig_byte_3;
int EE_size;
unsigned int FlashSZ;

unsigned char OSCCAL_DV;

//unsigned char Rx_clock = 55;
//unsigned char Tx_clock = 67;

/**********************************************************************************************************************/
///1440: works for polling: Theoretical value is 55
#define Tx_clock_144000						55
#define Rx_clock_144000						50
#define Start_clock_144000					TCCR0B = (1 << CS00);


///1280: works for polling: Theoretical value is 62
#define Tx_clock_128000						62
#define Rx_clock_128000						57
#define Start_clock_128000					TCCR0B = (1 << CS00);


///1152: works for polling: Theoretical value is 69
#define Tx_clock_115200						67							
#define Rx_clock_115200						63						
#define Start_clock_115200					TCCR0B = (1 << CS00);


#define Set_baud_rate_Tx_76800				85;		
#define Set_baud_rate_Rx_76800				80;				
#define Start_clock_76800					TCCR0B = (1 << CS00);

#define Set_baud_rate_Tx_57600				117;	
#define Set_baud_rate_Rx_57600				112;
#define Start_clock_57600					TCCR0B = (1 << CS00);
	

///384: works for polling: Theoretical value is 208
#define Set_baud_rate_Tx_38400				209;					
#define Set_baud_rate_Rx_38400				204;
#define Start_clock_38400					TCCR0B = (1 << CS00);


#define Set_baud_rate_Tx_19200				50;	
#define Set_baud_rate_Rx_19200				45;	
#define Start_clock_19200					TCCR0B = (1 << CS01);


#define Set_baud_rate_Tx_9600				104;	
#define Set_baud_rate_Rx_9600				100;
#define Start_clock_9600					TCCR0B = (1 << CS01);


#define receiveBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 |= (1 << OCF0A);\
OCR0A +=  Rx_clock;

#define transmitBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 |= (1 << OCF0A);\
OCR0A +=  Tx_clock;


/***********************************************************************/

#define fetch_char \
TCNT0 = 0;\
OCR0A = Rx_clock/2;\
Start_clock;\
USICR = (1 << USIWM0);\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;\
keypress =  USIDR;\
receiveBit;\
TCCR0B = 0;


/***********************************************************************/

#define setup_ATtiny_HW \
setup_watchdog;\
setup_IO;\
set_device_type_and_memory_size;\
Initialise_USI_Tx ();\
cal_device;



/***********************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


#define setup_IO \
MCUCR &= (~(1 << PUD));\
DDRA = 0;\
PORTA = 0xFF;\
DDRB = 0;\
PORTB = 0x0F;



#define cal_device \
eeprom_write_byte((uint8_t*)(EE_size - 3), OSCCAL);\
if ((eeprom_read_byte((uint8_t*)(EE_size - 2)) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)(EE_size - 2)) < 0xF0) && (eeprom_read_byte((uint8_t*)(EE_size - 2))\
== eeprom_read_byte((uint8_t*)(EE_size - 1)))) OSCCAL = eeprom_read_byte((uint8_t*)(EE_size - 2));





/*****************************************************************************/
const char *Device_type[8], *Device_family[2];
int device_ptr, family_ptr;


/*****************************************************************************/
#define Set_device_signatures \
Device_type[0] = "48/P";\
Device_type[1] = "88/P";\
Device_type[2] = "168/P";\
Device_type[3] = "328/P";\
Device_type[4] = "644";\
Device_type[5] = "32A";\
Device_type[6] = "44A";\
\
\
Device_family[0] = "Atmega ";\
Device_family[1] = "ATtiny ";



/*****************************************************************************/
#define set_device_type_and_memory_size \
Set_device_signatures;\
sig_byte_2 = eeprom_read_byte((uint8_t*)(EEP_MAX - 4));\
sig_byte_3 = eeprom_read_byte((uint8_t*)(EEP_MAX - 5));\
\
switch(sig_byte_2){\
	\
	case 0x92: FlashSZ = 0x800;  EE_size = 0x100;\
	switch (sig_byte_3)\
	{case 0x05: \
		case 0x0A: device_ptr = 0; family_ptr = 0; break;\
	case 0x07: device_ptr = 6; family_ptr = 1; break;}\
	break;\
	\
	case 0x93: FlashSZ = 0x1000; EE_size = 0x200;\
	switch (sig_byte_3)\
	{case 0x0A:\
	case 0x0F: device_ptr = 1; family_ptr = 0; break;}\
	break;\
	\
	case 0x94: FlashSZ = 0x2000; EE_size = 0x200;\
	switch (sig_byte_3)\
	{case 0x06: \
	case 0x0B: device_ptr = 2; family_ptr = 0; break;}\
	break;\
	\
	case 0x95: FlashSZ = 0x4000; EE_size = 0x400;\
	switch (sig_byte_3)\
	{case 0x14:\
		case 0x0F: device_ptr = 3; family_ptr = 0; break;\
	case 0x02: device_ptr = 5; family_ptr = 0; break;}\
	break;\
	\
	case 0x96: FlashSZ = 0x8000; EE_size = 0x800;\
	switch (sig_byte_3)\
	{case 0x09: device_ptr = 4; family_ptr = 0; break;}\
break;}
