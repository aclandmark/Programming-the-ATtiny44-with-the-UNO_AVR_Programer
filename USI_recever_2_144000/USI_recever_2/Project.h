
long Baud_rate = 115200;
char Pre_scaler;
long Rx_clock;
long Tx_clock;

#define set_USI_clocks \
Tx_clock = 8000000/Baud_rate;\
switch(Baud_rate){\
	case 9600:\
	case 1920:		Rx_clock = Tx_clock; Pre_scaler = 2;break;\
	case 38400:		Rx_clock = Tx_clock; Pre_scaler = 1;break;\
	case 57600:		Rx_clock = Tx_clock - 5; Pre_scaler = 1;break;\
	case 76800:		Rx_clock = Tx_clock - 3; Pre_scaler = 1;break;\
	case 115200:	Rx_clock = Tx_clock - 3; Pre_scaler = 1;break;\
	case 128000:	Rx_clock = Tx_clock; Pre_scaler = 1;break;\
	}

#define Start_clock \
TCCR0B = Pre_scaler;




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

/*#define Tx_clock						Tx_clock_38400
#define Rx_clock						Rx_clock_38400
#define Start_clock						Start_clock_38400
*/




#include "../Resources/USI_header.h"
#include "../Resources/USI_subroutines.c"
#include "../Resources/IO_subroutines.c"

const char message_1[] PROGMEM = "Program running on ";
const char message_2[] PROGMEM = "\r\nEcho single keypresses: return to escape.\r\n";
const char message_3[] PROGMEM = "\r\nSend String to the USI\r\n";
const char message_4[] PROGMEM = "\r\nNo text!\r\n";
const char message_5[] PROGMEM = "\r\nReading strings.";
const char message_6[] PROGMEM = "\r\nString number? 0 to exit";
const char message_7[] PROGMEM = "\r\nNo string!\r\n";


