


//Baud rate set for 38400: replace 38400 with wanted baud rate

#define Tx_clock						Tx_clock_38400
#define Rx_clock						Rx_clock_38400
#define Start_clock						Start_clock_38400



#define	TCNT0				TCNT0L
#define	TIFR0				TIFR
#define	WDTCSR				WDTCR
#define	PCIF0				PCIF
#define TIMSK0				TIMSK
#define TIFR1				TIFR
#define PCINT0_vect			PCINT_vect
#define PCINT6				PCINT0
#define PCINT4				PCINT2
#define TIM0_COMPA_vect		TIMER0_COMPA_vect
#define DDRA				DDRB
#define PORTA				PORTB
#define PINA				PINB
#define DI_pin				0
#define DO_pin				1
#define USCK_pin			2
#define setup_IO			setup_IO_461





#include "../Resources/ATtiny_cal.h"
#include "../Resources/USI_subroutines.c"
#include "../Resources/ASKII_and_timer_subroutines.c"
#include "../Resources/cal_subroutines.c"



const char message_1[] PROGMEM = {"\r\n\r\nCalibrating ATtiny"};
const char message_2[] PROGMEM = "\r\nDV/WV, previous OSCCAL values  ";
const char message_3[] PROGMEM = "User cal? Enter 1 to F then x if OK\r\n";







