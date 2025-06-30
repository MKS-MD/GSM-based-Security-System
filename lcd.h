#include<LPC21xx.h>
#include<string.h>
#include "delay-ms.h"
#define lcd 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19
typedef unsigned char u8;

void lcdcmd(u8 cmd)
{
IOCLR1=lcd;
IOSET1=(cmd&0xf0)<<16;
IOCLR1=rw;
IOCLR1=rs;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd;
IOSET1=(cmd&0x0f)<<20;
IOCLR1=rw;
IOCLR1=rs;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}
void lcdcon()
{
 PINSEL2=0;
 IODIR1=lcd|rs|rw|e;
 lcdcmd(0x01);
 lcdcmd(0x02);
 lcdcmd(0x0c);
 lcdcmd(0x28);
 lcdcmd(0x80);
}
void lcddata(u8 data)
{
IOCLR1=lcd;
IOSET1=(data&0xf0)<<16;
IOCLR1=rw;
IOSET1=rs;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd;
IOSET1=(data&0x0f)<<20;
IOCLR1=rw;
IOSET1=rs;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}
void lcdstr1(char *s)
{
 char pos = 0;
 while(*s)
 {	pos++;
 	lcddata(*s++);
	delay_ms(250);
	if(pos == 16)
	{
		lcdcmd(0x80);
		pos = 0;
	}
}
}
void lcd_int(int n)
{
signed char i=0;
u8 arr[5];
if(n==0)	     
lcddata('0');
else
{
if(n<0)
{
lcddata('-');
n=-n;
}
while(n>0)
{
arr[i++]=n%10;
n=n/10;
}
}
for(--i;i>0;i--)
lcddata(arr[i]);
}       
/*void str_rot(u8 *s)
{
lcdcon();
while(1)
{
int i,j,pos=0;
for(j=0;*s;j++)
lcddata(*s++);
for(i=0;i<=16;i++)
{
delay_ms(300);
lcdcmd(0x1c);
pos++;
}
if(pos==16)
lcdcmd(0x80);
}
}*/
void lcdstr(char *s)
{
while(*s)
lcddata(*s++);
delay_ms(150);
}

void str_rot(char *str)
{
int i;
char first=str[0];

lcdstr1(str);
for(i=0;i<strlen(str)-1;i++)
{
str[i]=str[i+1];
}		
str[i]=first;
}


