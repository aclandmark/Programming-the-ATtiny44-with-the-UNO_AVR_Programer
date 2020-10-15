


/************************************************************************************************/
long compute_error(char Num_Av, char sign)
{long error;
	
	error_sum = 0;
	int_counter = 0;													//Initialise all registers
	TCNT1_sum = 0;
	TCCR1B = 0;															//Ensure T1 is halted
	TCNT1 = 0;															//clear Timer 1
	GIFR |= 1 << PCIF0;													//Clear spurious interrupts
	GIMSK |= 1 << PCIE0;												//Set Pin change interrupt
	PCMSK1 |= 1 << PCINT10;												//Pin PB2 the clock input pin
	TIMSK |= (1 << TOIE1);												//Set timer 1 interrupt on overflow
	
	while (int_counter < Num_Av);										//Pause here for interrupts: Average the result over several 32.768mS periods
	GIMSK &= (~(1 << PCIE0));											//Measurement complete: Reset registers
	while (GIMSK & (1 << PCIE0));										//Disable PCI
	TIMSK &= (~(1 << TOIE1));											//Disable Timer 1 interrupt
	
	error = error_sum/Num_Av;											//Obtain average result
	if ((sign) && error < 0) error = error * (-1);						//Set sign if required
	return error;
	}




/************************************************************************************************/
void save_cal_values(unsigned char OSCCAL_user){
	eeprom_write_byte((uint8_t*)(EE_size - 1), OSCCAL_user);
	eeprom_write_byte((uint8_t*)(EE_size - 2), OSCCAL_user);
eeprom_write_byte((uint8_t*)(EE_size - 3), OSCCAL_DV);}




/********************************************************************************************************************************/
void printout_cal_values(void){
	String_to_USI("\r\nValues saved to EEPROM  ");
	Num_to_PC(10,eeprom_read_byte((uint8_t*)(EE_size-3))); String_to_USI("  ");
	Num_to_PC(10,eeprom_read_byte((uint8_t*)(EE_size-2))); String_to_USI("  ");
Num_to_PC(10,eeprom_read_byte((uint8_t*)(EE_size-1))); newline();}
