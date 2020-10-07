

//Baud rate set for 38400: replace 38400 with wanted baud rate

#define Tx_clock						Tx_clock_9600
#define Rx_clock						Rx_clock_9600
#define Start_clock						Start_clock_9600


#include "../Resources/ATtiny_cal.h"
#include "../Resources/USI_subroutines.c"
#include "../Resources/ASKII_and_timer_subroutines.c"
#include "../Resources/cal_subroutines.c"



#define WPU_on_DI_pin \
DDRB &= (!(1 << DDB0));\
PORTB |= 1 << PB0;

#define Configure_DO_pin_as_Output \
DDRB |= (1 << DDB1);

#define set_USI_ports_to_WPU \
DDRB &= (~((1 << DDB0) | (1 << DDB1) | (1 << DDB2)));\
PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

#define Transmitter_active \
DDRB & (1 << DDB1)


#define DI_pin_low \
!(PINB & (1 << PB0))

#define PCI_vector
PCINT0_vect

#define Disable_PCI_on_DI_pin \
GIMSK &= (~(1 << PCIE0));

#define setup_IO 	
setup_IO_26

