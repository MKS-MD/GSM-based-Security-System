#include<LPC21xx.h>
#include<string.h>
#include "keypad123.h"
#include "i2c_eeprom.c"
#include "i2c_def.h"
#include "types.h"
#include "uart.h"
//#include "i2c.c"
#define swt 12
#define sw 1<<14
#define motor_h	1<<10
#define motor_l 1<<11
typedef unsigned char u8;
void enter(u8 *x)
{
char i;
for(i=0;i<4;i++)
{
delay_ms(20);
lcdcmd(0xc0);
x[i]=keyscan();
lcddata('*');
}
}
void motor_init(void)
{
	IODIR0 |= motor_h | motor_l;
	IOCLR0 |= motor_h | motor_l;
}
void door_open(void)
{
	IOSET0 = motor_h;
	IOCLR0 = motor_l;
}

void door_close(void)
{
	IOSET0 = motor_l;
	IOCLR0 = motor_h;
}

void door_off(void)
{
	IOCLR0 |= motor_l | motor_h;
}
int main()
{
char i=0,j=0;
int attempt=0,t=1000;
unsigned int otp, entered_otp=0;
u8 p[4],s1[4],s[4]="3813";

char mess[]="Multi level security system";
char mess1[]={"Enter the pin"};

while((IOPIN0>>swt&1)==1);

while(i<1)
{ 
lcdcon();
lcdcmd(0x01);
	str_rot(mess);
	delay_ms(400);
	i++;
}
lcdcon();
delay_ms(20);

while(j<1)
{
lcdstr1(mess1);
j++;
}

while(1)
{
init_i2c();
pincon();
UARTcon();
motor_init();				  
i2c_eeprom_page_write(0x50,0x00,s,4);
i2c_eeprom_seq_read(0x50,0x00,p,4);

for(attempt=1;attempt<=3;)
{
enter(s1);
if(strncmp(p,s1,4)==0)
{
lcdcmd(0x01);
lcdstr1("pin accepted");
lcdcmd(0x01);
lcdstr1("Transmitting OTP");
delay_ms(250);
lcdcmd(0x01);
	lcdcmd(0x80);
	lcdstr1("Enter OTP :");
	while(t)
	{
	t++;
	if(t==9999)
	{ t= 1000;}
	if((IOPIN0&sw)==0)
	 {
	 t++;
	 break;
	 }
	}
	lcdcmd(0xc0);
	otp=t;
	generate_otp(t);
	for(i=0;i<4;i++)
	{
		unsigned char key= keyscan();
		lcddata(key);
		entered_otp=entered_otp *10+ (key-48) ;				
	}
	delay_ms(100);
	if(otp==entered_otp)
	{
		
	   	lcdcmd(0x01);
		lcdcmd(0x80);
		lcdstr1("OTP Matched");
		door_open();
		delay_ms(5000);
		door_close();          
		door_off();
		uart_gsm("+918610813437","Door Unlocked");

	}
	else
	{
	 	lcdcmd(0x01);
		lcdcmd(0x80);
		lcdstr1("Wrong OTP");
	}
}

else
{
lcdcon();
lcddata(attempt+48);
attempt++;

if(attempt>3)
{
lcdstr1("Attempt exceeded");
lcdstr1("PIN Incorrect");
lcdcmd(0x01);
lcdstr1("Door locked");
uart_gsm("+918610813437","Intruder alert...");
return 0;  
}
delay_ms(300);
}
}
}
}
									