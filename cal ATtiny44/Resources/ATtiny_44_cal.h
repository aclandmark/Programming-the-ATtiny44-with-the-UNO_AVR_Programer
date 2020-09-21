

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

/***********************************************************************/
#define Set_Baud_Rate_1152		OCR0A = 56;
#define Start_1152_clock		TCCR0B = (1 << CS00);


#define Set_Baud_Rate_768		OCR0A = 90;
#define Start_768_clock			TCCR0B = (1 << CS00);

#define Set_Baud_Rate_576		OCR0A = 115;
#define Start_576_clock			TCCR0B = (1 << CS00);

#define Set_Baud_Rate_384		OCR0A = 194;
#define Start_384_clock			TCCR0B = (1 << CS00);


#define Set_Baud_Rate_192		OCR0A = 50;
#define Start_192_clock			TCCR0B = (1 << CS01);


#define Set_Baud_Rate_96		OCR0A = 104;
#define Start_96_clock			TCCR0B = (1 << CS01);



/***********************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


/***********************************************************************/
#define T1_delay_4sec 5,0x85F7
#define T1_delay_500ms 5,0xF0BE
#define T1_delay_250ms 5,0xF85F
#define T1_delay_50ms 3, 0xCF31
#define T1_delay_100ms 3, 0x9E62




/***********************************************************************/
unsigned char ReverseByte (unsigned char);
void Initialise_USI_Tx (void);
void Tx_data_byte(unsigned char);
void Timer_T1_sub(char, unsigned int);
void Initialise_T0(void);
void String_to_USI(const char*);
void NumericString_to_PC(unsigned char*);
void SBtoAL(unsigned char*, long, char);
void Num_to_PC(char, long);
long compute_error(char, char);


/***********************************************************************/
unsigned char Txdata, TxdataR;
volatile long error_sum;
volatile unsigned char error_counter;
volatile char Warmup_counter;
volatile char bit_counter_ovf;