#include<LPC21xx.h>
#include "keypad123.h"
int main()
{
unsigned char res;
lcdcon();
lcdstr1("4x4 Matrix");
lcdcmd(0x01);
while(1)
{
res=keyscan();
delay_ms(150);
lcddata(res);
}
}
