

void newline(void){String_to_USI ("\r\n");}

void String_to_USI(const char* s){
	int i = 0;
	while(i < 200){
		if(s[i] == '\0') break;
	Char_to_USI(s[i++]);} }


char wait_for_return_key(void){
	char keypress;
	keypress = Char_from_USI (0);										//Wait indefinitely
	if((keypress == '\r') || (keypress == '\n')){						//If carriage return
		Char_from_USI (1);keypress = '\r';}								//Ignore companion char if present  
	return keypress;}





char isCharavailable (int m){	
char keypress;	
while (!(keypress = Char_from_USI(1)) && m--);
return keypress;}





/*
void Num_to_PC(char radix, long int_num){
char array[12];												//long has 10 chars + sign + null terminator	
SBtoAL(array, int_num, radix);								//calls the Binary to askii subroutine
NumericString_to_PC(array);}								//Prints characters in reverse order


void SBtoAL(char array[], long num, char radix){					//Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';			//0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';			//0x80000000 * -1 = 0
array[3] = '0';array[4] = '0';array[5] = '0'; array[6] = '0';
array[7] = '8';array[8] = '-';array[9] = '\0';
array[10] = '\0'; array[11] = '\0'; break; } return;}

for (int n = 0; n <=11; n++)array[n] = 0;	
if ((sign = num) < 0) num = num * (-1);

do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);
if (sign < 0) {array[m] = '-';m++;}}

void NumericString_to_PC(char* s){					
int n=0;
while (s[n] != '\0')n++;									//scroll to end of string counting the number of characters
for(int m = n; m; m--)Char_to_USI(*(s + m-1));}	
*/

char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}



int Num_from_KBD(void){
char keypress;
int number;

do
{keypress =  Char_from_USI(0);} while(!(decimal_digit(keypress)));

keypress -= '0';
number =  keypress;

while(1){
if ((keypress = wait_for_return_key())  =='\r')break;
if (decimal_digit (keypress))		
{
keypress -= '0';
number = number * 10 + keypress;}}
return number;}
















