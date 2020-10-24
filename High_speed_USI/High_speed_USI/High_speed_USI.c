							

							#include "Project.h"



							int main (void)
							{
								unsigned char Keypress = 'A', char_buffer[47];					//Receive buffer for 32 chars
								
								setup_ATtiny_HW;													//Configures USI as a transmitter
								User_prompt;

								
								set_device_type_and_memory_size;									//confirm device type and set EEPROM size
								String_to_USI("\r\n\r\nProgram running on ");
								String_to_USI("\r\nATtiny");
								String_to_USI (Device_type[device_ptr]);
								
								newline();
								
								String_to_USI
								("\r\nEcho single keypresses: Return key to escape.\r\n");				//Receive individual keypresses from PC
								while(( Keypress = wait_for_return_key()) != '\r'){
								Tx_data_byte(Keypress);}
								
								String_to_USI
								("\r\nSend strings to USI\r\n");
								
								while(1){
									String_from_USI (char_buffer);
								Echo_string(char_buffer);}
								
							return 1;}



							
							
