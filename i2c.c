#include<LPC21xx.h>
#include "types.h"
 #include "i2c_def.h"
void init_i2c(void)
{
//configure I/O pins for SCL and SDA pins using pinsel0
//PINSEL0=0x00000050;
  PINSEL0|=SCL_EN|SDA_EN;
//configure I2C speed
//using I2SCLL
  I2SCLL=loadval;
//and I2CSCLH
  I2SCLH=loadval;
//i2c peripheral enable for communication
  I2CONSET=1<<(I2EN_BIT);
}

void i2c_start(void)
{
//start condition
  I2CONSET=1<<STA_BIT;
//wait for start bit status
  while(((I2CONSET>>SI_BIT)&1)==0);
//clear start condition
  I2CONCLR=1<<STA_BIT; 
}

void i2c_restart(void)
{
//start condition
  I2CONSET=1<<STA_BIT;
//clr SI_BIT
  I2CONCLR=1<<SI_BIT;
//wait for SI bit status
  while(((I2CONSET>>SI_BIT)&1)==0);
//clear start condition
  I2CONCLR=1<<STA_BIT;
}

void i2c_write(u8 dat)
{
//put data into I2DAT
  I2DAT=dat;
//clr SI_BIT
  I2CONCLR=1<<SI_BIT;
//wait for SI bit status
  while(((I2CONSET>>SI_BIT)&1)==0);
}

void i2c_stop(void)
{
//issue stop condition
  I2CONSET=1<<STO_BIT;
//clr SI bit status
  I2CONCLR=1<<SI_BIT;
//stop will cleared automatically
}

u8 i2c_nack(void)
{
 I2CONSET=0x00;//assert not to ACK
 I2CONCLR=1<<SI_BIT;
 while(((I2CONSET>>SI_BIT)&1)==0);
 return I2DAT;
}
u8 i2c_masterack(void)
{
I2CONSET=0x04;//assert ACK
I2CONCLR=1<<SI_BIT;
while(((I2CONSET>>SI_BIT)&1)==0);
I2CONCLR=0x04;//clear assert ACK
return I2DAT;
}
