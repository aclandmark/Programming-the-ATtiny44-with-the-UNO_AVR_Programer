							

							#include "Project.h"



							int main (void)
							{
								unsigned char Keypress, char_buffer[11], letter = 'A', test_buffer[11];
								
								setup_ATtiny_HW;													//Configures USI as a transmitter

								sei();
								String_from_USI (char_buffer);

								for(int m = 0; m <= 9; m++)test_buffer[m] = letter++;
								test_buffer[10] = '\0';
								
								
								
								while(1){Echo_string(test_buffer); String_from_USI (char_buffer); if (char_buffer[0] == '~')break;}	//adjust tx rate
								
								set_device_type_and_memory_size;									//confirm device type and set EEPROM size
								String_to_USI("\r\n\r\nProgram running on ");
								String_to_USI (Device_family[family_ptr]);
								String_to_USI (Device_type[device_ptr]);
								
								newline();
								//if receive char === A echo it or adjust rx br
								String_to_USI
								("\r\nEcho single keypresses ~ to escape.\r\n");				//Receive individual keypresses from PC
								while ((Keypress = Char_from_USI ()) != '~'){					//and echo to screen (~ is escape char)
								Tx_data_byte(Keypress);}
								
								newline();
								
								while(1){
									String_from_USI (char_buffer);									//Receive string from PC
								Echo_string(char_buffer);}
								
							return 1;}



							/************************************************************************************************/
							ISR(TIM0_COMPA_vect)												//Clock signal for USI
							{	Set_baud_rate_Rx;
							TCNT0 = 0;}														//Reset T0



							/************************************************************************************************/
							ISR(USI_OVF_vect)													//Reception or transmission complete

							{USISR |= (1 << USIOIF);											//Clear USI interrupt flag
								if(DDRA & (1 << DDA5))												//If USI transmitter active
								char_transmitted = 1;
								else
							char_received = 1;}													//If USI receiver active




							/************************************************************************************************/
							ISR (PCINT0_vect){												//Start detector for chars from PC
								{if (!(PINA & (1 << PA6)))								//Low on USI DI pin
									{
										TCNT0 = 0;
										Set_half_rate_clock_Rx;
										Start_clock;
										
										TIFR0 = (1 << OCF0A);									//Clear spurious interrupts
										
										TIMSK0 |= (1 << OCIE0A);								//Enable interrupt on output compare
										USICR |= ( 1 << USIOIE);								//Enable USI counter interrupt
										USICR |= (1 << USIWM0);									//Select USI 3-wire mode
										USICR |= (1 << USICS0);									//Select USI clock source (timer0 compare match)			char_received = 0;
										USISR = 0xF0 | 0x07;									//8 data bits + start bit
										GIMSK &= (~(1 << PCIE0));								//Disable PCI on DI pin
									}}}
									

