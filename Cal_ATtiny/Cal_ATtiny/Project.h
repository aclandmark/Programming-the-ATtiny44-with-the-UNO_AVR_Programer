

//Baud rate set for 38400: replace 38400 with wanted baud rate

#define Tx_clock						Tx_clock_76800
#define Rx_clock						Rx_clock_76800
#define Start_clock						Start_clock_76800


#define WPU_on_DI_pin \
DDRA &= (!(1 << DDA6));\
PORTA |= 1 << PA6;

#define Configure_DO_pin_as_Output \
DDRA |= (1 << DDA5);

#define set_USI_ports_to_WPU \
DDRA &= (~((1 << DDA4) | (1 << DDA5) | (1 << DDA6)));\
PORTA |= (1 << PA4) | (1 << PA5) | (1 << PA6);

#define Transmitter_active \
DDRA & (1 << DDA5)


#define DI_pin_low \
!(PINA & (1 << PA6))

#define PCI_vector \
PCINT0_vect

#define Disable_PCI_on_DI_pin \
GIMSK &= (~(1 << PCIE0));



#include "../Resources/ATtiny_cal.h"
#include "../Resources/USI_subroutines.c"
#include "../Resources/ASKII_and_timer_subroutines.c"
#include "../Resources/cal_subroutines.c"

