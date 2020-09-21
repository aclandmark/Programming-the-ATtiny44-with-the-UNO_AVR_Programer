


/*
Baud rate settings for OCROA	9.6						832					832 +/- 40		Error 36
								19.2					416					400 +16/-8		Error 16 Checked
								38.4KHz		calculate	208 In practice use 194 +10/-9		Error 14 Checked
								57.6					139					115 +/- 7		Error 24 Checked
								76.8KHz					104					90  +4/-5		Error 14 Checked
								115.2					69					56 +/- zero		Error 13 Checked

	*/									
void USART_init (unsigned char, unsigned char);
void Timer_T0_sub(char, unsigned char);
#define T0_delay_10ms 5,183
#define T0_delay_5ms 5,220


#include "Project.h"

int main (void)
{
	unsigned char OSCCAL_DV, OSCCAL_WV;
	long error;
	char counter = 0;
	
	setup_watchdog;
	USART_init(0,25);
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
		error = compute_error(1,1);
		OSCCAL = OSCCAL_WV;
		Num_to_PC(10, m); Char_to_PC('\t');
		Num_to_PC(10, error);
		newline();
		
		}
		cli();
	
	
	
	while(1);
	return 1;}





void Timer_T0_sub(char Counter_speed, unsigned char Start_point){
	TCNT0 = Start_point;
	TCCR0B = Counter_speed;
	while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0); TCCR0B = 0;}



/************************************************************************************************/
void Timer_T1_sub(char Counter_speed, unsigned int Start_point){
	TCNT1H = (Start_point >> 8);
	TCNT1L = Start_point & 0x00FF;
	TIFR1 = 0xFF;
	TCCR1B = Counter_speed;
	while(!(TIFR1 & (1<<TOV1)));
	TIFR1 |= (1<<TOV1);
TCCR1B = 0;}






/************************************************************************************************/
	ISR (PCINT0_vect){
		long TCNT1_BKP;
		
		if (!(TCCR1B)) {TCCR1B = (1 << CS11);}					//start T1 counter 1MHz clock
	
	else {
		TCNT1_BKP = TCNT1;
		TCNT1 = 0;
		if (error_counter < Warmup_counter);
		else {error_sum = error_sum - 32768 + TCNT1_BKP;}
		error_counter +=1;}
		}

	
	
/************************************************************************************************/	
long compute_error(char precision, char sign)
{long error;
char Av_counter;
		
	TCCR1B = 0;														//Ensure T1 is halted
	TCNT1 = 0;														//clear Timer 1
	PCIFR |= (1 << PCIF0);											//Clear spurious interrupts
	PCICR |= 1 << PCIE0;
	PCMSK0 |= 1 << PCINT5;											//Interrupt on clock pin
		
	if (precision == 1){Warmup_counter = 1; Av_counter = 1;}		//Num_2: av_counter	Num_1: Total counter
	if (precision == 2){Warmup_counter = 2; Av_counter = 4;}
	if (precision == 3){Warmup_counter = 5; Av_counter = 10;}
	
	error_counter = 0;												//Compute error for each value of OSCCAL 10 times
	error_sum = 0;
	while(error_counter < (Warmup_counter + Av_counter));			//Wait here while error is calculated
		
	error_counter = 0;
	error = error_sum;
	if ((!sign) && (error < 0)) error *= (-1);
	//cli();
	PCICR &= (~(1 << PCIE0));											//halt interrupts
	PCMSK0 &= (~(1 << PCINT5));
	
return error/Av_counter;}	



/**********************************************************************************************/
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
	UCSR0B = 0;
	UBRR0H = UBRROH_N;
	UBRR0L = UBRR0L_N;
	UCSR0A = (1 << U2X0);
	UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}


