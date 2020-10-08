


/************************************************************************************************/
long compute_error(char precision, char sign)
{long error;
	char Av_counter;
	
	TCCR1B = 0;															//Ensure T1 is halted
	TCNT1 = 0;															//clear Timer 1
	GIFR |= 1 << PCIF;													//Clear spurious interrupts
	GIMSK |= 1 << PCIE0;
	PCMSK0 |= 1 << PCINT4;												//Interrupt on clock pin
	
	if (precision == 1){Warmup_counter = 1; Av_counter = 1;}			//Num_2: av_counter	Num_1: Total counter
	if (precision == 2){Warmup_counter = 2; Av_counter = 4;}
	if (precision == 3){Warmup_counter = 5; Av_counter = 10;}
	
	error_counter = 0;													//Compute error for each value of OSCCAL 10 times
	error_sum = 0;
	while(error_counter < (Warmup_counter + Av_counter));				//Wait here while error is calculated
	
	error_counter = 0;
	error = error_sum;
	if ((!sign) && (error < 0)) error *= (-1);
	GIMSK &= (~(1 << PCIE0));											//halt interrupts
	PCMSK0 &= (~(1 << PCINT4));
	
return error/Av_counter;}




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
