

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


volatile long error_sum;
volatile long TCNT1_sum;
volatile char int_counter;

int sig_byte_2, sig_byte_3;
int EE_size;
unsigned int FlashSZ;

unsigned char OSCCAL_DV;

void USART_init (unsigned char, unsigned char);
void Timer_T0_sub(char, unsigned char);
#define T0_delay_10ms 5,183
#define T0_delay_5ms 5,220


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
void Char_to_PC(char);
void String_to_PC(const char*);
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