


#define receiveBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 |= (1 << OCF0A);\
OCR0A +=  Rx_clock;

/*
Waits for T0 to count to the first clock period
Triggers the USI clock
Clears the compare match flag 
Increments OCR0A by one clock period
*/



#define transmitBit \
while (!(TIFR0 & (1 << OCF0A)));\
USICR |= (1 << USICLK);\
TIFR0 |= (1 << OCF0A);\
OCR0A +=  Tx_clock;


/***********************************************************************/

#define fetch_char \
TCNT0 = 0;\
OCR0A = Rx_clock/2;\
Start_clock;\
USICR = (1 << USIWM0);\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;receiveBit;receiveBit;receiveBit;\
receiveBit;\
keypress =  USIDR;\
receiveBit;\
TCCR0B = 0;





unsigned char ReverseByte (unsigned char x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
return x;}



/**************************************************************************************************************/
void Initialise_USI_Tx (void)
{	WPU_on_DI_pin;
	USICR = 0;										//Reset USI
	TCCR0B = 0;										//Ensure USI clock is halted
	OCR0A = Tx_clock;								//Define clock period
	TIFR0 = (1 << OCF0A);							//Clear spurious interrupts
	TCNT0 = 0;										//Clear TCNT0
	USICR |= (1 << USIWM0);							//Select USI 3-wire mode
	Configure_DO_pin_as_Output;
	USIDR = 0xFF;									//Load USIDR with 0xFF
USISR = 0xFF;}										//Clear spurious interrupt flags




/**************************************************************************************************************/
void Char_to_USI(unsigned char Txdata){				//USI already initialised for transmit by default
	
	Txdata = ReverseByte(Txdata);
	
	Start_clock;	
	
	//TCCR0B = Pre_scaler;
	transmitBit;										//Initialise transmission
	USIDR = (Txdata >> 2) | 0x80;						//Add start bit

	for(int p = 0; p <= 4; p++){transmitBit;}			//First half of byte
		
	USIDR = (Txdata << 3) | 0x07;						//Add stop bit
	for(int p = 0; p <= 4; p++){transmitBit;}			//Second half of byte
	
	TCCR0B = 0;	}										//Halt baud rate clock

	


/************************************************************************************************************/
unsigned char Char_from_USI (char timeout)				//Receive char
{int p = 8000;											//Time out variable
	unsigned char keypress;
	
	set_USI_ports_to_WPU;
	USICR = 0;											//Reset USI
	if (timeout)										//Timeout required
	while ((!(start_bit)) && (p--));					//Wait here for start bit or timeout
	else
	while (!(start_bit));								//Or just wait indefinitely for start bit
	if(start_bit){	
	fetch_char;	
	keypress = ReverseByte(keypress);}
	else keypress = 0;									//Timeout occurred
	Initialise_USI_Tx ();								//Leave USI ready to transmit char
return (keypress);}										//Take char from buffer register





/**********************************************************************************************************/
void String_from_USI (unsigned char* string)			//Receive string from PC
{ int p = 8000;
	unsigned char keypress;
	
	set_USI_ports_to_WPU;
	USICR = 0;											//Reset USI
	while (!(start_bit));								//Wait for start bit 
	fetch_char;											//Detects 1 start bit, 8 data bits and ONE stop bit
	string[0] = keypress;
	for(int n = 1; n <= 45; n++)						//Max permissible string length 44 chars
	{USICR = 0;											//Reset USI
	while 	((!(start_bit)) && (p--));					//Wait here for next start bit or end of string 
	if(start_bit)
		{fetch_char;
		string[n] = keypress;
		p = 8000;}										//Reset timeout period
	else{string[n] = '\0';								//receiver times out: terminate string
	break;}}
	
	for(int n = 0; n <= 45; n++)
	{if (string[n])
		{string[n] = ReverseByte(string[n]);}
		else break;}
	Initialise_USI_Tx ();}								//Leave USI ready to transmit char


