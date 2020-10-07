

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

#define PCI_vector \
PCINT0_vect

#define Disable_PCI_on_DI_pin \
GIMSK &= (~(1 << PCIE0));

#define setup_IO \
setup_IO_26



/*****************************************************************************/
#define Set_device_signatures \
Device_type[0] = "26/L";\
Device_type[1] = "44A";\
Device_type[2] = "84A";\
Device_type[3] = "? Unknown device! Op halted.";


/*****************************************************************************/
#define set_device_type_and_memory_size \
Set_device_signatures;\
sig_byte_2 = eeprom_read_byte((uint8_t*)(EEP_MAX - 4));\
sig_byte_3 = eeprom_read_byte((uint8_t*)(EEP_MAX - 5));\
\
switch(sig_byte_2){\
	\
	case 0x91: FlashSZ = 0x400;  EE_size = 0x80;\
	switch (sig_byte_3)\
	{case 0x09: device_ptr = 0; break;\
	default: device_ptr = 3; break;}\
	break;\
	\
	case 0x92: FlashSZ = 0x800;  EE_size = 0x100;\
	switch (sig_byte_3)\
	{case 0x07: device_ptr = 1; break;\
	default: device_ptr = 3; break;	}\
	break;\
	\
	case 0x93: FlashSZ = 0x1000;  EE_size = 0x200;\
	switch (sig_byte_3)\
	{case 0x0C: device_ptr = 2; break;\
	default: device_ptr = 3; break;	}\
	break;\
	\
default:  device_ptr = 3;	break;}
