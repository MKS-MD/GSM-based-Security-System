#include<LPC21xx.h>
#include<stdlib.h>
void generate_otp(int);
void pincon(void)
{
PINSEL0|=0x5;
}
void UARTcon(void)
{
U0LCR=0x83;
U0DLL=97;
U0DLM=0;
U0LCR=0x03;
}
void UART0_Tx(unsigned char d)
{
U0THR=d;
while(((U0LSR>>5)&1)==0);
}
void string(char *s)
{
  int i;
  for(i=0;s[i];i++)
  UART0_Tx(s[i]);
}
unsigned char UART_RX(void)
{
while((U0LSR&1)==0);
return U0RBR;
}
void uart_gsm(u8 *phnum,u8 *otp)
{
//communicating with GSM 
string("AT\r\n"); //command to check whether GSM module is connected 
delay_ms(5000);	  //waiting for response

//changing gsm for sms to text mode
string("AT+CMGF=1\r\n"); //command to gsm for sms to text mode 
delay_ms(5000);          //waiting for response

//set receiver phone number
string("AT+CMGS=\""); 
string(phnum); //sending phone number to gsm via uart
string("\"\r\n");    //end of above command
delay_ms(5000);      //waiting for response

//send message content
string(otp);      //send otp content
UART0_Tx(26);     //Send Ctrl+Z (ASCII 26) to indicate end of message
delay_ms(5000);   //waiting for response
}

void generate_otp(int t)
{
	char otp_str[50];
	//int otp =(rand()%9000)+1000;
	snprintf(otp_str,sizeof(otp_str),"Your OTP is %d for unlocking the door.",t);
	uart_gsm("+918610813437",otp_str);
}
