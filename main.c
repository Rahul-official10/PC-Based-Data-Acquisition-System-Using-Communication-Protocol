//pc based data acquisition
#include <LPC21XX.H>
#include "delay.h"
#include "lcd.h"
#include "spi.h"
#include "rtc.h"
#include "uart.h"

int main()
{
	unsigned char hr,min,sec,con1,con2;
	float f;
	uart_config();
	lcd_init();
	spi0_init();
	i2c_init();
	lcd_command(0x80);
	lcd_str("   PCB DAQ");
	delay_ms(2000);
	i2c_rtc_write(0x68,0x00,0x50);
	i2c_rtc_write(0x68,0x01,0x59);
	i2c_rtc_write(0x68,0x02,0x71);
	lcd_command(0x01);
	while(1)
	{
		lcd_command(0x80);
		lcd_str("Time:");
		uart_str("Time:");
		
		con1=i2c_rtc_read(0X68,0X02);
		con2=con1 & 0x1f;
		hr=bcd_conv(con2);
		
		lcd_data((hr/10)+48);
		lcd_data((hr%10)+48);
		uart_tx((hr/10)+48);
		uart_tx((hr%10)+48);
		uart_tx(':');

		lcd_data(':');
		
		min=bcd_conv(i2c_rtc_read(0X68,0X01));
		lcd_data((min/10)+48);
		lcd_data((min%10)+48);
		uart_tx((min/10)+48);
		uart_tx((min%10)+48);
		uart_tx(':');
		lcd_data(':');
	  
		sec=bcd_conv(i2c_rtc_read(0X68,0X00));
		lcd_data((sec/10)+48);
		lcd_data((sec%10)+48);
		uart_tx((sec/10)+48);
		uart_tx((sec%10)+48);

		if((con1>>5)&1==1)
		{
			lcd_str(" PM");
			uart_str(" PM");
		}
		else
		{
			lcd_str(" AM");
			uart_str(" AM");
		}
		
		f=read_adc(0);
		f=f*10;
		lcd_command(0xc0);
		uart_str("\r\n");
		lcd_str("Temp: ");
		uart_str("Temp: ");
		lcd_float(f);
		uart_float(f);
		uart_str("\r\n");
	}
	
}

