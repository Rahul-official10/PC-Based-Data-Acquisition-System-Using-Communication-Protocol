//spi header
#include <LPC21XX.H>
void spi0_init(void)
{
	PINSEL0|=0X00001500;
	S0SPCCR=150;
	S0SPCR=(1<<5|0X18);
	IODIR0=1<<7;//CS
}

unsigned char spi0(unsigned char data)
{
	unsigned char stat;
	stat=S0SPSR; //clear SPIF
	S0SPDR=data;
	while(((S0SPSR>>7)&1)==0); //WAIT FOR TRANSMISSION COMPLETE
	return S0SPDR;
}

float read_adc(unsigned char CN)
{
	unsigned char hbyte,lbyte;
	unsigned int adc_val=0;
	
	//activate chip
	IOCLR0|=1<<7; //CS=7
	
	spi0(0x06);
	hbyte=spi0(CN<<6);
	lbyte=spi0(0x00);
	
	//deselect chip
	IOSET0|= 1<<7; 
	
	adc_val=((hbyte & 0x0f)<<8)|lbyte;
	return ((adc_val*3.3)/4096);
}
