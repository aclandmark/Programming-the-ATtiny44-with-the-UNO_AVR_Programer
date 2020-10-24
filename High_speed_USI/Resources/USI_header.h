

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
unsigned char Char_from_USI (char);							//Receive single characters as typed in at keyboard						
void String_from_USI (unsigned char*);						//Receive strings (were there is no pause between characters)
void newline(void);
void String_to_USI(const char*);							//The equivalent of sendString
void Echo_string(unsigned char*);							//Echo string received from keyboard
char wait_for_return_key(void);
char isCharavailable (int m);
/**********************************************************************************************************************/


int EEP_MAX = 0x2000;
int sig_byte_2, sig_byte_3;
int EE_size;
unsigned int FlashSZ;
char User_response;
unsigned char OSCCAL_DV;

//unsigned char Rx_clock = 90;
//unsigned char Tx_clock = 90;

/**********************************************************************************************************************/

///2560: works for polling: Theoretical value is 31			Tested OK
#define Tx_clock_256000						33									
#define Rx_clock_256000						29
#define Start_clock_256000					TCCR0B = (1 << CS00);
#define Half_Rx_clock_256000				14


///144000: works for polling: Theoretical value is 55		Tested OK
#define Tx_clock_144000						58	
#define Rx_clock_144000						53
#define Start_clock_144000					TCCR0B = (1 << CS00);



///128000 works for polling: Theoretical value is 62		Tested OK
#define Tx_clock_128000						64
#define Rx_clock_128000						62
#define Start_clock_128000					TCCR0B = (1 << CS00);



///115200: works for polling: Theoretical value is 69		Tested OK
#define Tx_clock_115200						68					
#define Rx_clock_115200						66						
#define Start_clock_115200					TCCR0B = (1 << CS00);



///76800: works for polling: Theoretical value is 105		Tested OK
#define Tx_clock_76800						106			
#define Rx_clock_76800						102					
#define Start_clock_76800					TCCR0B = (1 << CS00);



///57600: works for polling: Theoretical value is 139		Tested OK
#define Tx_clock_57600						138		
#define Rx_clock_57600						134	
#define Start_clock_57600					TCCR0B = (1 << CS00);
	
	
	
///38400: works for polling: Theoretical value is 208		Tested OK
#define Tx_clock_38400						210													
#define Rx_clock_38400						206	
#define Start_clock_38400					TCCR0B = (1 << CS00);


///19200: works for polling: Theoretical value is 416 (52 x 8)		Tested OK
#define Tx_clock_19200						52		
#define Rx_clock_19200						50		
#define Start_clock_19200					TCCR0B = (1 << CS01);


///9600: works for polling: Theoretical value is 833 (104 X 8)		Tested OK
#define Tx_clock_9600						106		
#define Rx_clock_9600						102
#define Start_clock_9600					TCCR0B = (1 << CS01);


#define receiveBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 = 0xFF;\
OCR0A +=  Rx_clock;


#define transmitBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 = 0xFF;\
OCR0A +=  Tx_clock;


/***********************************************************************/

#define fetch_char \
Start_clock;\
OCR0A = Half_Rx_clock;\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;\
keypress =  USIDR;\
receiveBit;\
TCCR0B = 0;\
TCNT0 = 0;


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

#define setup_IO_44 \
MCUCR &= (~(1 << PUD));\
DDRA = 0;\
PORTA = 0xFF;\
DDRB = 0;\
PORTB = 0x0F;

#define setup_IO_461 \
MCUCR &= (~(1 << PUD));\
DDRA = 0;\
PORTA = 0xFF;\
DDRB = 0;\
PORTB = 0xFF;


#define cal_device \
eeprom_write_byte((uint8_t*)(EE_size - 3), OSCCAL);\
if ((eeprom_read_byte((uint8_t*)(EE_size - 2)) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)(EE_size - 2)) < 0xF0) && (eeprom_read_byte((uint8_t*)(EE_size - 2))\
== eeprom_read_byte((uint8_t*)(EE_size - 1)))) OSCCAL = eeprom_read_byte((uint8_t*)(EE_size - 2));


/**********************************************************************************/
#define User_prompt \
while(1){\
	do{String_to_USI("R?    ");}	 while(!(User_response = isCharavailable(125)));\
	\
if((User_response == 'R') || (User_response == 'r'))break;} String_to_USI("\r\n");




/*****************************************************************************/
const char *Device_type[8];
int device_ptr;



/*****************************************************************************/
#define Set_device_signatures \
Device_type[0] = "24A";\
Device_type[1] = "44A";\
Device_type[2] = "84A";\
Device_type[3] = "261A";\
Device_type[4] = "461A";\
Device_type[5] = "861A";\
Device_type[6] = "26/L";\
Device_type[7] = "? Unknown device! Op halted.";


/***************************************************************************************************************/
#define set_device_type_and_memory_size \
Set_device_signatures;\
sig_byte_2 = eeprom_read_byte((uint8_t*)(EEP_MAX - 4));\
sig_byte_3 = eeprom_read_byte((uint8_t*)(EEP_MAX - 5));\
\
switch(sig_byte_2){\
	\
	case 0x91: FlashSZ = 0x400;  EE_size = 0x80;\
	switch (sig_byte_3)\
	{case 0x09: device_ptr = 6; break;\
		case 0x0B: device_ptr = 0; break;\
		case 0x0C: device_ptr = 3; break;\
	default: device_ptr = 7; break;}\
	break;\
	\
	case 0x92: FlashSZ = 0x800;  EE_size = 0x100;\
	switch (sig_byte_3)\
	{case 0x07: device_ptr = 1; break;\
		case 0x08: device_ptr = 4; break;\
	default: device_ptr = 7; break;	}\
	break;\
	\
	case 0x93: FlashSZ = 0x1000;  EE_size = 0x200;\
	switch (sig_byte_3)\
	{case 0x0C: device_ptr = 2; break;\
		case 0x0D: device_ptr = 5; break;\
	default: device_ptr = 7; break;	}\
	break;\
	\
default:  device_ptr = 7;	break;}

