

unsigned char ReverseByte (unsigned char x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
return x;}



/**************************************************************************************************************/
void Initialise_USI_Tx (void)
{DDRA &= (!(1 << DDA6));							//Put WPU on PA6 (DI pin)
	PORTA |= 1 << PA6;
	
	USICR = 0;										//Reset USI
	OCR0A = Tx_clock;
	TIFR0 = (1 << OCF0A);							//Clear spurious interrupts
	TCNT0 = 0;										//Clear TCNT0
	USICR |= (1 << USIWM0);							//Select USI 3-wire mode
	DDRA |= (1 << DDA5);							//Configure DO (PA5) as output
	USIDR = 0xFF;									//Load USIDR with 0xFF
USISR = 0xFF;}										//Clear spurious interrupt flags




/**************************************************************************************************************/
void Tx_data_byte(unsigned char Txdata){				//USI already initialised for transmit by default
	
	Txdata = ReverseByte(Txdata);
	
	Start_clock;	
	transmitBit;										//Initialise transmission
	USIDR = (Txdata >> 2) | 0x80;						//Add start bit

	for(int p = 0; p <= 4; p++){transmitBit;}			//First half of byte
		
	USIDR = (Txdata << 3) | 0x07;						//Add stop bit
	for(int p = 0; p <= 4; p++){transmitBit;}			//Second half of byte
	
	TCCR0B = 0;	}										//Halt baud rate clock

	


/************************************************************************************************************/
unsigned char Char_from_USI (void)									//Receive char
{unsigned char keypress;
	
	DDRA &= (~((1 << DDA4) | (1 << DDA5) | (1 << DDA6)));			//Set USI IO to WPU
	PORTA |= (1 << PA4) | (1 << PA5) | (1 << PA6);
	USICR = 0;														//Reset USI
	
	while(PINA & (1 << PA6));										//wait for start bit
	
	fetch_char;	
	keypress = ReverseByte(keypress);
	Initialise_USI_Tx ();											//Leave USI ready to transmit char
return (keypress);}													//Take char from buffer register





/**********************************************************************************************************/
void String_from_USI (unsigned char* string)						//Receive string from PC
{ int p = 8000;
	unsigned char keypress;
	
	DDRA &= (~((1 << DDA4) | (1 << DDA5) | (1 << DDA6)));			//Set USI IO to WPU
	PORTA |= (1 << PA4) | (1 << PA5) | (1 << PA6);
	
	USICR = 0;														//Reset USI
	while(PINA & (1 << PA6));										//wait for start bit
	fetch_char;														//Detects 1 start bit, 8 data bits and ONE stop bit
	string[0] = keypress;
	for(int n = 1; n <= 45; n++)									//Max permissible string length 44 chars
	{USICR = 0;														//Reset USI
	while ((PINA & (1 << PA6)) && (p--));							//Wait for start bit for limited time only 
		
	if (!(PINA & (1 << PA6)))										//Start bit detected
		{fetch_char;
		string[n] = keypress;
		p = 8000;}													//Clear TCNT0
	
	else{string[n] = '\0';											//receiver times out: terminate string
	break;}}
	
	for(int n = 0; n <= 45; n++)
	{if (string[n]){string[n] = ReverseByte(string[n]);}
		else break;}
	Initialise_USI_Tx ();}											//Leave USI ready to transmit char



/******************************************************************************************************/
void newline(void){String_to_USI ("\r\n");}



/*******************************************************************************************************/
void String_to_USI(const char* s){									//Send predefined string to PC
	int i = 0;
	while(i < 200){
		if(s[i] == '\0') break;
	Tx_data_byte(s[i++]);} }
	

	
	
/*******************************************************************************************************/
	void Echo_string(unsigned char* s){								//Send variable string to PC
		int i = 0;
		while(i < 200){
			if(s[i] == '\0') break;
		Tx_data_byte(s[i++]);} }