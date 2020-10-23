
/*
Program that tests the USI (Universal Serial Interface) when used as a USART.
Press R at the user prompt to send and receive single keypress and strings
Press r to read text from the flash
*/

							

#include "Project.h"

int main (void)
{	
	unsigned char  Num_strings;													//The number of strings written to flash or EEPROM
	int  next_address;															//Address in memory of next character,
	int  text_num;																//Number of string to be printed out
	unsigned char Keypress, char_buffer[47];									//Receive buffer for 32 chars
	
setup_ATtiny_HW;																//Configures USI as a transmitter

set_USI_clocks;

User_prompt;
	
	set_device_type_and_memory_size;											//confirm device type and set EEPROM size
	Flash_String_to_USI(message_1);												//"Program running on " 
	String_to_USI (Device_type[device_ptr]);
	
		if(User_response == 'R'){
		
		Flash_String_to_USI(message_2);											//"\r\nEcho single keypresses: return to escape.\r\n"
		String_to_USI("\r\nTEST1234567890abcdefghijklmnopqrstuvwxyz\r\n");
		while(( Keypress = wait_for_return_key()) != '\r'){
		Char_to_USI(Keypress);}										
		
		Flash_String_to_USI(message_3);											//\r\nSend String to the USI\r\n
		
		for (int m = 0; m < 10; m++){
		String_from_USI (char_buffer);
		Echo_string(char_buffer);}}
		
		
		
		
		if(User_response == 'r'){
		char_counter = 0;													//counts the number of characters in the text file (excludes \r & \n)
		next_address = FlashSZ*2 - 1;										//start address of text

		if(Char_from_flash(next_address) == 0xFF){
			Flash_String_to_USI(message_4);									//"\r\nNo text!\r\n"
		wdt_enable(WDTO_60MS); while(1);}
		Flash_String_to_USI(message_5);										//"\r\nReading strings."

		Num_strings = string_counter(next_address);							//Count the number of strings
		
		Flash_String_to_USI(message_6);										//"\r\nString number? 0 to exit"

		while(1){
			text_num = Num_from_KBD();
			if(!(text_num))break;
			if (text_num > Num_strings){Flash_String_to_USI(message_7);		//"\r\nNo string!\r\n"
				continue;}
			String_to_USI("\r\n");
			String_to_USI("\r\n");
			print_string_num(text_num,next_address);
		}	
			
		}wdt_enable(WDTO_60MS); while(1);
		
		
	return 1;}



		/***************************************************************************************************************************************************/
		unsigned char string_counter(int next_address){							//Scroll through text section of flash counting the '\0' chars
			char next_char, previous = 0;										//until '\0' '\0' is detected to indicate the end of the last string
			unsigned char counter = 0;
			
			while(1){
				next_char = Char_from_flash(next_address);						//result provided by assembly subroutine
				if(next_char == 0){counter += 1; char_counter += 1;
				if (!(previous)) return counter-1;}
				else char_counter += 1;
				previous = next_char;
			next_address -= 1;}}



			/***************************************************************************************************************************************************/
			void print_string_num(int text_num, int next_address){				//scroll through text section of flash counting '\0' chars
				int null_counter = 1;											//until the start of the required string
				
				line_length = 0;
				while(1){
					if(null_counter == text_num)break;
					next_char = Char_from_flash(next_address);
					if(next_char == 0)null_counter += 1;
				next_address -= 1;}

				while(1){														//Print string; exit at '\0' character
					next_char = Char_from_flash(next_address);
					if(next_char)Char_to_USI(next_char); else break;
					line_length += 1;
					if ((next_char == ' ') && (line_length > 90))				//avoid long lines of print
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