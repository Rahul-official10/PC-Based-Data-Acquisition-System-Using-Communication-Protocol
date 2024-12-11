// delay 
#include <LPC21XX.H>

void delay_ms(int sec)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<sec);
	T0TCR=0X03;
	T0TCR=0X00;
}
