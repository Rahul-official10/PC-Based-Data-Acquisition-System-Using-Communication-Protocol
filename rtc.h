// rtc
#include <LPC21XX.H>
//#include "delay.h"
#define cclk 60000000
#define pclk (cclk/4)
#define i2c_speed 100000
#define loadval ((pclk/i2c_speed)/2)

void i2c_init(void)
{
	PINSEL0|= 0X00000050;
	I2SCLL= loadval;
	I2SCLH= loadval;
	
	// i2c peripheral enable communication
	I2CONSET=1<<6;
}

void i2c_start(void)
{
	//start condition
	I2CONSET=1<<5;
	// wait for start bit
	while(((I2CONSET>>3)&1)==0);
	// CLEAR START CONDITION
	I2CONCLR=1<<5;
}

void i2c_stop(void)
{
	I2CONSET= 1<<4;
	// CLEAR SI BIT STATUS
	I2CONCLR= 1<<3;
}

void i2c_restart(void)
{
	//START CONDT
	I2CONSET=1<<5;
	//CLR SI BIT
	I2CONCLR=1<<3;
	// WAIT FOR SI BIT STATUS
	while(((I2CONSET>>3)&1)==0);
	I2CONCLR=1<<5;
}

void i2c_write(unsigned char d)
{
	I2DAT=d;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
}

unsigned char no_ack(void)
{
	I2CONSET= 0X00; //assert not of ack
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	return I2DAT;
	
}

unsigned char i2c_rtc_read(unsigned char SAddr,unsigned char RBuff)
{
	unsigned char data;
	i2c_start();
	i2c_write(SAddr<<1);
	i2c_write(RBuff);
	i2c_restart();
	i2c_write(SAddr<<1|1);
	data=no_ack();
	i2c_stop();
	return data;
}
void i2c_rtc_write(unsigned char SAddr, unsigned char WBuff, unsigned char dat)
{
	i2c_start();
	i2c_write(SAddr<<1);
	i2c_write(WBuff);
	i2c_write(dat);
	i2c_stop();
	delay_ms(10);
}
unsigned char bcd_conv(unsigned char b_num)
{
	return (((b_num/16)*10)+(b_num%16));
}
