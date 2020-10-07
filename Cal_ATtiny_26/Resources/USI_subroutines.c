
unsigned char ReverseByte (unsigned char x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
return x;}





void Char_to_USI(unsigned char Txdata){					//Transmit data
	
	Txdata = ReverseByte(Txdata);
	Start_clock;										//Start T0 running  
		
	while (!(char_transmitted));						//wait for USI to be initialised
	char_transmitted = 0;
	USISR = 0xF0 | (0x10 - 0x05);						//First half of data
	USIDR = (Txdata >> 2) | 0x80;

	while (!(char_transmitted));						//wait for USI counter overflow
	char_transmitted = 0;
	USISR = 0xF0 | (0x10 - 0x05);						//Second half of data
	USIDR = (Txdata << 3) | 0x07;

	while (!(char_transmitted));						//wait for USI counter overflow
	char_transmitted = 0;
	TCCR0 = 0;	}										//halt baud rate clock



void Initialise_USI_Tx (void)
	{	
		WPU_on_DI_pin;
		
		USICR = 0;										//Reset USI
		TCCR0 = 0;										//Ensure that T0 is not running.
		TCNT0 =  Tx_clock;								//Uses Interrupt on T0 compare match
		TIFR = (1 << TOV0);							//Clear spurious T0 interrupts
		TCCR0 |= 1 << PSR0;			\\ TCNT0 = 0;			//Clear TCNT0
		TIMSK |= (1 << TOIE0);						//Enable T0 interrupt on output compare
		
		USICR |= ( 1 << USIOIE);						//Enable USI counter interrupt
		USICR |= (1 << USIWM0);							//Select USI 3-wire mode
		USICR |= (1 << USICS0);							//Select USI clock source (timer0 compare match)
		Configure_DO_pin_as_Output
		
		USIDR = 0xFF;									//Load USIDR with 0xFF
		USISR = 0xFF;}									//clear bit counter
		



/************************************************************************************************/


unsigned char Char_from_USI (char timeout)						//zero: wait indefinitively; one wait with timeout
{int p = 8000;
	unsigned char keypress;
		
	set_USI_ports_to_WPU;
	
	USICR = 0;													//Reset USI
	
	GIFR |= 1 << PCIF0;											//clear spurious interrupts on DI pin
	GIMSK |= 1 << PCIE0;										//Set PCI on DI pin
	PCMSK0 |= 1 << PCINT6;
	
	if (timeout)
	while((!(char_received)) && p--);							//Wait for USI overflow ISR
	else while(!(char_received));
	if(char_received){
		char_received = 0;
	keypress = ReverseByte(USIBR);}
	else keypress = 0;
	Initialise_USI_Tx ();										//Leave USI ready to transmit char
return keypress;}
	
