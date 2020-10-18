


#include "Project.h"
void USART_init (unsigned char, unsigned char);
void Timer_T0_sub(char, unsigned char);
#define T0_delay_10ms 5,183
#define T0_delay_5ms 5,220

int main (void)
{
	unsigned char OSCCAL_DV, OSCCAL_WV;
	long error;
	char counter = 0;
	
	setup_watchdog;
	USART_init(0,25);
	OCR1A = 0xFF;
	sei();
	
	OSCCAL_DV = OSCCAL;
	OSCCAL_WV = OSCCAL;
	OSCCAL = 0xF0;
	while (OSCCAL>= 0x0F){OSCCAL--;
		if(!(OSCCAL%2))Char_to_PC('.');
		error = compute_error(1,0);
		if(error < 200) OSCCAL_WV = OSCCAL;
		if(error < 1000) counter++; else counter =0;
	if (counter == 8)break;	}
	
	newline();
	String_to_PC("Default OSCCAL value  ");
	Num_to_PC(10, OSCCAL_DV);
	newline();
	
	for (int m = 0x0F; m <=0xF0; m++)
	{
		Timer_T0_sub(T0_delay_5ms);
		OSCCAL = m;
		error = compute_error(3,1);
		OSCCAL = OSCCAL_WV;
		Num_to_PC(10, m); Char_to_PC('\t');
		Num_to_PC(10, error);
		newline();
		
	}
	cli();
	
	String_to_PC("Repeat cal using Xtal algorithm");
	
	
	
	while(1);
return 1;}


ISR (PCINT0_vect){													//Pin change interrupt on DI pin or SCK pins
	
	if (!(TCCR1B)) {
		TCNT1_sum = 0;
		TCNT1 = 0;
	TCCR1B =2;	}											//1MHz clock
	
	else {TCCR1B = 0; int_counter += 1;
		error_sum = error_sum + TCNT1_sum - 32768 + TCNT1;
	}}

	

	ISR(TIMER1_COMPA_vect){TCNT1_sum += 0x100; }




void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
	UCSR0B = 0;
	UBRR0H = UBRROH_N;
	UBRR0L = UBRR0L_N;
	UCSR0A = (1 << U2X0);
	UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}




void Timer_T0_sub(char Counter_speed, unsigned char Start_point){
	TCNT0 = Start_point;
	TCCR0B = Counter_speed;
	while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0); TCCR0B = 0;}


/************************************************************************************************/


long compute_error(char Num_Av, char sign)
{long error;
	
	error_sum = 0;
	int_counter = 0;													//Initialise all registers
	TCNT1_sum = 0;
	TCCR1B = 0;															//Ensure T1 is halted
	TCNT1 = 0;															//clear Timer 1
	
	//GIFR |= 1 << PCIF0;												//Clear spurious interrupts
	PCIFR |= (1 << PCIF0);
	
	//GIMSK |= 1 << PCIE0;												//Set Pin change interrupt
	PCICR |= 1 << PCIE0;
	
	PCMSK0	= (1 << PCINT5);											//Set mask on clock pin
	TIMSK0 |= (1 << OCIE1A);											//Set Timer 1 Interrupt on output compare
	
	while (int_counter < Num_Av);										//Pause here for interrupts: Average the result over several 32.768mS periods
	
	//GIMSK &= (~(1 << PCIE0));											//Measurement complete: Reset registers
	PCICR &= (~(1 << PCIE0));
	
	PCMSK0 &= (~(1 << PCINT5));
	TIMSK0 &= (~(1 << TOIE1));											//Disable Timer 1 interrupt
	
	error = error_sum/Num_Av;											//Obtain average result
	if (!(sign) && error < 0) error = error * (-1);						//Set sign if required
	return error;
}


