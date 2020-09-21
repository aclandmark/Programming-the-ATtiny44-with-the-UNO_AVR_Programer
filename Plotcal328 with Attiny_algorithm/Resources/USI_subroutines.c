




void Tx_data_byte(unsigned char Txdata){
	
	Txdata = ReverseByte(Txdata);
	Start_96_clock;									//Start clock
		
	while (!(bit_counter_ovf));							//wait for USI counter overflow
	bit_counter_ovf = 0;
	USISR = 0xF0 | (0x10 - 0x05);						//First half of data
	USIDR = (Txdata >> 2) | 0x80;

	while (!(bit_counter_ovf));							//wait for USI counter overflow
	bit_counter_ovf = 0;
	USISR = 0xF0 | (0x10 - 0x05);						//Second half of data
	USIDR = (Txdata << 3) | 0x07;

	while (!(bit_counter_ovf));							//wait for USI counter overflow
	bit_counter_ovf = 0;
	TCCR0B = 0;	}										//halt baud rate clock



void Initialise_USI_Tx (void)
	{	USICR = 0;										//Reset USI
		Set_Baud_Rate_96;
		TIFR0 = (1 << OCF0A);
		TCNT0 = 0;
		TIMSK0 |= (1 << OCIE0A);						//Enable interrupt on output compare
		USICR |= ( 1 << USIOIE);						//Enable USI counter interrupt
		USICR |= (1 << USIWM0);							//Select USI 3-wire mode
		USICR |= (1 << USICS0);							//Select USI clock source (timer0 compare match)
		DDRA |= (1 << DDA5);							//Configure DO (PA5) as output
		USIDR = 0xFF;									//Load USIDR with 0xFF
		USISR = 0xFF;}									//clear bit counter
		




unsigned char ReverseByte (unsigned char x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
return x;}
