
/*
Introduces subroutines necessary to read text from EEPROM and Flash
that has been programmed using the "8_UNO_programmer".

At user prompt press 'r' to read flash and 'R' to read EEPROM.

CONVERT FOR ATtiny


*/


#include "Text_reader.h"

unsigned char line_length = 0;
char next_char;



int main (void){

	unsigned char  Num_strings;											//The number of strings written to flash or EEPROM
	int  next_address;													//Address in memory of next character,
	int  text_num;														//The number of the string to be printed out
	
	setup_HW;
	sei();
	
	User_prompt;

	String_to_USI("\r\nATtiny");
	
	String_to_USI (Device_type[device_ptr]);



	if(User_response == 'r')

	{char_counter = 0;													//counts the number of characters in the text file (excludes \r & \n)
		next_address = FlashSZ*2 - 1;										//start address of text

		if(Char_from_flash(next_address) == 0xFF){
			String_to_USI("\r\nNo text!\r\n");
		wdt_enable(WDTO_60MS); while(1);}
		String_to_USI("\r\nReading strings.");

		Num_strings = string_counter(next_address);							//Count the number of strings
		
		String_to_USI("\r\nString number? 0 to exit");              //Request string

		while(1){
			text_num = Num_from_KBD();
			if(!(text_num))break;
			if (text_num > Num_strings){String_to_USI("\r\nNo string!\r\n"); continue;}
			String_to_USI("\r\n");
			String_to_USI("\r\n");
			print_string_num(text_num,next_address);
		}}
		wdt_enable(WDTO_60MS); while(1);
	return 1;}



	/***************************************************************************************************************************************************/
	unsigned char string_counter(int next_address){                //Scroll through text section of flash counting the '\0' chars
		char next_char, previous = 0;								//until '\0' '\0' is detected to indicate the end of the last string
		unsigned char counter = 0;
		
		while(1){
			next_char = Char_from_flash(next_address);             //result provided by assembly subroutine
			if(next_char == 0){counter += 1; char_counter += 1;
			if (!(previous)) return counter-1;}
			else char_counter += 1;
			previous = next_char;
		next_address -= 1;}}



		/***************************************************************************************************************************************************/
		void print_string_num(int text_num, int next_address){      //scroll through text section of flash counting '\0' chars
			int null_counter = 1;									 //until the start of the required string
			
			line_length = 0;
			while(1){
				if(null_counter == text_num)break;
				next_char = Char_from_flash(next_address);
				if(next_char == 0)null_counter += 1;
			next_address -= 1;}

			while(1){												//Print string; exit at '\0' character
				next_char = Char_from_flash(next_address);
				if(next_char)Char_to_USI(next_char); else break;
				line_length += 1;
				if ((next_char == ' ') && (line_length > 90))		//avoid long lines of print
				{String_to_USI("\r\n");line_length = 0;}
			next_address -= 1; }}




			/***************************************************************************************************************************************************/
			char Char_from_flash(int next_address){

				Prog_mem_address_H = next_address >> 8;
				Prog_mem_address_L = next_address;

				asm volatile ("push r0") ;									//Save registers before inserting assembly code
				asm volatile ("push r31") ;
				asm volatile ("push r30") ;

				asm volatile ("lds r31, Prog_mem_address_H");				//load ZH & L pointer from static RAM
				asm volatile ("lds r30, Prog_mem_address_L");

				asm volatile ("lpm r0, Z");									//load program memory into register r0

				asm volatile ("sts Flash_readout,  r0");					//Store contents of r0 in static RAM

				asm volatile ("pop r30");									//Restore registers
				asm volatile ("pop r31");
				asm volatile ("pop r0");

			return Flash_readout;}



			/************************************************************************************************/
			ISR(TIM0_COMPA_vect)												//Clock signal for USI: shifts data in the USIDataRegister
			{
				if(Transmitter_active)OCR0A = Tx_clock;
				else OCR0A = Rx_clock;											//Necessary because receiver initially sets half the baud rate
			TCNT0 = 0;}															//Reset T0


			/************************************************************************************************/
			ISR(USI_OVF_vect)													//USI counter overflows indicating the end of a transmission or reception

			{USISR |= (1 << USIOIF);											//Clear USI interrupt flag

				if(Transmitter_active)
				char_transmitted = 1;
				else
			char_received = 1;}													//USI receiver active



			/************************************************************************************************/
			ISR (PCINT0_vect){
				
				if(DI_pin_low)
				{TCNT0 = 0;
					OCR0A = Rx_clock/2;
					Start_clock;											//Start baud rate clock (Half period)
					TIFR0 = (1 << OCF0A);									//Clear spurious interrupts
					
					TIMSK0 |= (1 << OCIE0A);								//Enable interrupt on output compare
					USICR |= ( 1 << USIOIE);								//Enable USI counter interrupt
					USICR |= (1 << USIWM0);									//Select USI 3-wire mode
					USICR |= (1 << USICS0);									//Select USI clock source (timer0 compare match)

					char_received = 0;

					USISR = 0xF0 | 0x07;									//8 data bits + start bit
				Disable_PCI_on_DI_pin;}}
					
