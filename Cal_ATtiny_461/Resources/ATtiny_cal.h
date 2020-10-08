

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


/***********************************************************************/
#define T0_delay_5ms 5,220
#define T1_delay_4sec 5,0x85F7
#define T1_delay_500ms 5,0xF0BE
#define T1_delay_250ms 5,0xF85F
#define T1_delay_50ms 3, 0xCF31
#define T1_delay_100ms 3, 0x9E62


/***********************************************************************/
unsigned char ReverseByte (unsigned char);
void Initialise_USI_Tx (void);
void Char_to_USI(unsigned char);
unsigned char Char_from_USI (char);
void String_to_USI(const char*);
void NumericString_to_PC(unsigned char*);
void SBtoAL(unsigned char*, long, char);
void Num_to_PC(char, long);
long compute_error(char, char);

void Timer_T0_sub(char, unsigned char);
void Timer_T1_sub(char, unsigned int);
void Initialise_T0(void);

void save_cal_values(unsigned char );
void printout_cal_values(void);


/***********************************************************************/
unsigned char Txdata;
volatile long error_sum;
volatile unsigned char error_counter;
volatile char Warmup_counter;
volatile char char_transmitted;
volatile char char_received;

int sig_byte_2, sig_byte_3;
int EE_size;
unsigned int FlashSZ;

unsigned char OSCCAL_DV;

//unsigned char Rx_clock = 60;
//unsigned char Tx_clock = 90;


/***********************************************************************/
///76800: works for polling: Theoretical value is 105	
#define Tx_clock_76800						78
#define Rx_clock_76800						74
#define Start_clock_76800					TCCR0B = (1 << CS00);


///57600: works for polling: Theoretical value is 139
#define Tx_clock_57600						112
#define Rx_clock_57600						110
#define Start_clock_57600					TCCR0B = (1 << CS00);


///38400: works for polling: Theoretical value is 208	
#define Tx_clock_38400						185	
#define Rx_clock_38400						185	
#define Start_clock_38400					TCCR0B = (1 << CS00);


///19200: works for polling: Theoretical value is 416 (52 x 8)	
#define Tx_clock_19200						48	
#define Rx_clock_19200						48	
#define Start_clock_19200					TCCR0B = (1 << CS01);


///9600: works for polling: Theoretical value is 833 (104 X 8)		
#define Tx_clock_9600						100	
#define Rx_clock_9600						100
#define Start_clock_9600					TCCR0B = (1 << CS01);




/***********************************************************************/
#define setup_Attiny_HW \
setup_watchdog;\
setup_IO;\
set_device_type_and_memory_size;\
Initialise_USI_Tx ();




/***********************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCR |= (1 <<WDCE) | (1<< WDE);\
WDTCR = 0;


#define setup_IO_44 \
MCUCR &= (~(1 << PUD));\
DDRA = 0;\
PORTA = 0xFF;\
DDRB = 0;\
PORTB = 0x0F;


/*****************************************************************************/
const char *Device_type[4];
int device_ptr;





