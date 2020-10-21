
//Baud rate set for 38400: replace 38400 with wanted baud rate

#define TIFR0				TIFR
#define TCNT0				TCNT0L
#define WDTCSR				WDTCR	
#define TIFR1				TIFR
#define DI_pin				0
#define DO_pin				1
#define USCK_pin			2
#define setup_IO			setup_IO_461
#define DDR_USI				DDRB
#define PIN_USI				PINB
#define PORT_USI			PORTB

#define set_USI_ports_to_WPU \
DDR_USI &= (~((1 << DI_pin) | (1 << DO_pin) | (1 << USCK_pin)));\
PORT_USI |= (1 << DI_pin) | (1 << DO_pin) | (1 << USCK_pin);

#define WPU_on_DI_pin \
DDR_USI &= (!(1 << DI_pin));\
PORT_USI |= 1 << DI_pin;

#define Configure_DO_pin_as_Output \
DDR_USI |= (1 << DO_pin);

#define start_bit \
!(PIN_USI & (1 << DI_pin))


/***********************************************************
#define set_USI_ports_to_WPU \
DDRA &= (~((1 << DDA4) | (1 << DDA5) | (1 << DDA6)));\
PORTA |= (1 << PA4) | (1 << PA5) | (1 << PA6);

#define WPU_on_DI_pin \
DDRA &= (!(1 << DDA6));\
PORTA |= 1 << PA6;

#define Configure_DO_pin_as_Output \
DDRA |= (1 << DDA5);

#define start_bit \
!(PINA & (1 << PA6))
*************************************************************************/

#define Tx_clock						Tx_clock_256000
#define Rx_clock						Rx_clock_256000
#define Start_clock						Start_clock_256000

#include "../Resources/USI_header.h"
#include "../Resources/USI_subroutines.c"


