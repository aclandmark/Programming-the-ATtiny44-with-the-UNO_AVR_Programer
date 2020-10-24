

unsigned char ReverseByte (unsigned char x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
return x;}



/**************************************************************************************************************/
void Initialise_USI_Tx (void)
{		
	USICR = 0;										//Reset USI
	OCR0A = Tx_clock;
	TIFR0 = 0xFF;									//Clear spurious interrupts
	TCNT0 = 0;										//Clear TCNT0
	USICR |= (1 << USIWM0);							//Select USI 3-wire mode
	Configure_DO_pin_as_Output;
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
	
	TCCR0B = 0;											//Halt baud rate clock
	TCNT0 = 0;}
	


/************************************************************************************************************/
unsigned char Char_from_USI (char timeout)				//Receive char
{int p = 8000;											//Time out variable
	unsigned char keypress;
	
	 WPU_on_DO_pin;
	if (timeout)										//Timeout required
	while (DI_paused && (p--));					//Wait here for start bit or timeout
	else
	while (DI_paused);								//Or just wait indefinitely for start bit
	if(start_bit){
		fetch_char;
		
	keypress = ReverseByte(keypress);}
	else keypress = 0;									//Timeout occurred
	Initialise_USI_Tx ();								//Leave USI ready to transmit char
return (keypress);}





/**********************************************************************************************************/
void String_from_USI (unsigned char* string)						//Receive string from PC
{ unsigned int p = 0;
	unsigned char keypress;
	
	 WPU_on_DO_pin;
		
	while(DI_paused);
	fetch_char;														//Detects 1 start bit, 8 data bits and ONE stop bit
	string[0] = keypress;
	for(int n = 1; n <= 45; n++)									//Max permissible string length 44 chars
	{USICR = 0;														//Reset USI
	
	while(DI_paused && (++p));
	
	if(start_bit)
		
		{fetch_char;
		string[n] = keypress;
		}													//Clear TCNT0
	
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
		
		
		
/******************************************************************************************************/
		char wait_for_return_key(void){								//Keep receiving keypresses until the return key is pressed
			char keypress;
			keypress = Char_from_USI (0);							//Wait indefinitely
			if((keypress == '\r') || (keypress == '\n')){			//If carriage return
			Char_from_USI (1);keypress = '\r';}						//Ignore companion char if present
		return keypress;}



/******************************************************************************************************/
char isCharavailable (int m){								//Wait for keypress with a timeout
	char keypress = 0;										//Return 0 is there is no keypress
	while (!(keypress = Char_from_USI(1)) && m--);
return keypress;}