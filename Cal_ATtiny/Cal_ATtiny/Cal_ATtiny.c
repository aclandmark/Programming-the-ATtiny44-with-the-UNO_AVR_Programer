						

#include "Project.h"

int main (void)
{
	unsigned char  OSCCAL_WV, OSCCAL_min = 0, offset, OSCCAL_UV;
	long error;
	char counter = 0;
	unsigned char Rx_data_byte;	
	int EEP_MAX = 0x2000;
	
	setup_Attiny_HW;
	
	
	sei();															//Required by USI (Rx and Tx) and calibration subroutines
	OSCCAL_DV = OSCCAL;												//Calibrate OSCCAL
	OSCCAL_WV = OSCCAL;
	OSCCAL_UV = OSCCAL;
	OSCCAL = 0xF0;
	while (OSCCAL>= 0x0F){OSCCAL;
		if(!(OSCCAL%3))Char_to_USI('.');
		error = compute_error(1,0);
		if(error < 500) OSCCAL_WV = OSCCAL;
		if(error < 1000) counter += 1; else counter =0;				//should be < 500 not 1000
		OSCCAL -= 2;		
		if (counter == 4)break;	}
			
	newline();
	set_device_type_and_memory_size;
	String_to_USI("\r\n\r\nCalibrating ");
	String_to_USI (Device_family[family_ptr]);
	String_to_USI (Device_type[device_ptr]);
	
	
	
	String_to_USI("\r\nDV/WV, previous OSCCAL values  ");
	Num_to_PC(10, OSCCAL_DV);  String_to_USI("  "); Num_to_PC(10, OSCCAL_WV);String_to_USI("  "); 
	Num_to_PC(10, eeprom_read_byte((uint8_t*)(EE_size - 2)));
	newline();
		
	counter = 0;
	for (int m = -15; m <=15; m++)								//Print out results
	{Timer_T0_sub(T0_delay_5ms);
		OSCCAL = OSCCAL_WV + m;
		error = compute_error(1,1);
		if ((error < -1000) ||
		(error > 1000))counter = 0; else counter += 1;
		if(counter == 1)OSCCAL_min = OSCCAL;
						
		OSCCAL = OSCCAL_WV;
		
		Num_to_PC(16, counter); Char_to_USI('\t');
		Num_to_PC(10, OSCCAL_WV + m); Char_to_USI('\t');
		Num_to_PC(10, error);
		newline();}
		
		String_to_USI("User cal? Enter 1 to F then x if OK\r\n");
		while(1){
		if ((Rx_data_byte = Char_from_USI ()) == 'x')break;
		offset = Rx_data_byte - '1';
		if(offset > 9)offset -= 7;
		OSCCAL_UV = OSCCAL_min + offset;
		Num_to_PC(10, OSCCAL_UV);
		newline();
		}
		save_cal_values(OSCCAL_UV);
		printout_cal_values();
		while(1);
	return 1;}



/************************************************************************************************/
ISR(TIM0_COMPA_vect)												//Clock signal for USI
{	Set_Baud_Rate_384;
	TCNT0 = 0;}														//Reset T0



/************************************************************************************************/
ISR(USI_OVF_vect)

{USISR |= (1 << USIOIF);											//Clear USI interrupt flag
if(DDRA & (1 << DDA5))												//USI transmitter active
	char_transmitted = 1;
else
char_received = 1;}													//USI receiver active			




/************************************************************************************************/
	ISR (PCINT0_vect){
		long TCNT1_BKP;
		
		if(DDRA & (1 << DDA5)){										//USI transmitter active: Calculate OSCCAL errors
		if (!(TCCR1B)) {TCCR1B = (1 << CS11);}						//start T1 counter 1MHz clock
	else {
		TCNT1_BKP = TCNT1;
		TCNT1 = 0;
		if (error_counter < Warmup_counter);
		else {error_sum = error_sum - 32768 + TCNT1_BKP;}
		error_counter +=1;}}
		
	else{															//USI receiver active
		{if (!(PINA & (1 << PA6)))									//Low on USI DI pin
			{
			TCNT0 = 0;
			Set_half_rate_clock_384;
			Start_384_clock;										//Start baud rate clock (Half period)
			TIFR0 = (1 << OCF0A);									//Clear spurious interrupts
			
			TIMSK0 |= (1 << OCIE0A);								//Enable interrupt on output compare
			USICR |= ( 1 << USIOIE);								//Enable USI counter interrupt
			USICR |= (1 << USIWM0);									//Select USI 3-wire mode
			USICR |= (1 << USICS0);									//Select USI clock source (timer0 compare match)

			char_received = 0;

			USISR = 0xF0 | 0x07;									//8 data bits + start bit
			GIMSK &= (~(1 << PCIE0));								//Disable PCI on DI pin
			}}}
	}

	
	







