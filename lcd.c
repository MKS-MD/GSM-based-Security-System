#include<LPC21xx.h>
{
int res;
lcdconfig();
lcdstr("4x4 Matrix");
while(1)
{
lcdcmd(0xc0);
res=keyscan();
lcd_int(res);
delay_ms(500);
lcdcmd(0x01);
}
}