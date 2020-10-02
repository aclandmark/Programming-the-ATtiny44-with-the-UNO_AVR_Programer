							

#include "Project.h"

void Timer_T1_sub(void);



volatile char test,  bit_flag, test_2;


int main (void)
{	
	unsigned char Keypress = 'A', char_buffer[47];					//Receive buffer for 32 chars
	
		
setup_ATtiny_HW;													//Configures USI as a transmitter

DDRA |= 1 << DDA2;
PORTA &= (~(1 << PA2));

	//sei();
	//String_from_USI (char_buffer);									//Wait while user changes baud rate if required
	Char_from_USI();
	//while(1){Tx_data_byte(Keypress);}

/************Get Rx values using LED****************************************/


/*while(1){Keypress = Char_from_USI ();
	Tx_data_byte(Keypress);
	Rx_clock += 1;}*/
	
	

/************Get Tx values using PC screen****************************************/
	/*while(1){Tx_data_byte('A');
	String_from_USI (char_buffer);
	Tx_clock += 1;}*/
	
	
	set_device_type_and_memory_size;									//confirm device type and set EEPROM size
	String_to_USI("\r\n\r\nProgram running on ");
	String_to_USI (Device_family[family_ptr]);
	String_to_USI (Device_type[device_ptr]);
		
	newline();
			
		String_to_USI
		("\r\nEcho single keypresses ~ to escape.\r\n");				//Receive individual keypresses from PC
		while ((Keypress = Char_from_USI ()) != '~'){					//and echo to screen (~ is escape char)
		Tx_data_byte(Keypress);}										
		
		newline();
		
		while(1){
		String_from_USI (char_buffer);
		Echo_string(char_buffer);}
		
	return 1;}



	
		
	
	
	
/************************************************************************************************/
void Timer_T1_sub(void){
	TCNT1 = 0xF0BE;
	TIFR1 = 0xFF;
	TCCR1B = 5;
	while(!(TIFR1 & (1<<TOV1))){if (TCNT1 >= 0xF85F)PORTA &= (~(1 << PA2));}
	TIFR1 |= (1<<TOV1);
TCCR1B = 0;}