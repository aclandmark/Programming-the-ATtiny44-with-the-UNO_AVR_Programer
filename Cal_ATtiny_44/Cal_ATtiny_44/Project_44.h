


//Baud rate set for 38400: replace 38400 with wanted baud rate

#define Tx_clock						Tx_clock_38400
#define Rx_clock						Rx_clock_38400
#define Start_clock						Start_clock_38400

#define DI_pin				6
#define DO_pin				5
#define USCK_pin			4
#define setup_IO			setup_IO_44

#include "../Resources/ATtiny_cal.h"
#include "../Resources/USI_subroutines.c"
#include "../Resources/ASKII_and_timer_subroutines.c"
#include "../Resources/cal_subroutines.c"







