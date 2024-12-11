// uart 
#include <LPC21XX.H>
void uart_config(void)
{
	PINSEL0|=0X00000005;
	U0LCR=0X83;
	U0DLL=97;
	U0LCR=0X03;
}

void uart_tx(unsigned ch)
{
	U0THR=ch;
	while(((U0LSR>>5)&1)==0);
}

unsigned char uart_rx(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}

void uart_str(unsigned char *s)
{
	while(*s)
	{
		uart_tx(*s++);
	}
}
void uart_integer(int n)
{
int arr[5],i=0;
if (n==0)
{
uart_tx('0');
}
else
{
if(n<0)
{
uart_tx('-');
n=-n;
}
while(n>0)
{
arr[i++]=n%10;
n=n/10;
}
for(--i;i>=0;i--)
{
uart_tx(arr[i]+48);
}
}
}
void uart_float(float f)
{
int num;
num=f;
uart_integer(num);
uart_tx('.');
num=(f-num)*100;
uart_integer(num);
}
