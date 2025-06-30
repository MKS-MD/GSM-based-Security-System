#ifndef __I2C_DEF_H__
#define __I2C_DEF_H__

//defines pin function selection 
#define SCL_EN 0x00000010 
#define SDA_EN 0x00000040

//defines i2c speed configuration
#define cclk      60000000
#define	pclk      cclk/4
#define i2c_speed 100000
#define loadval	  ((pclk/i2c_speed)/2)

//bit defines for I2CONSET sfr
#define AA_BIT	 2
#define	SI_BIT	 3
#define	STO_BIT	 4
#define	STA_BIT	 5
#define I2EN_BIT 6

#endif
