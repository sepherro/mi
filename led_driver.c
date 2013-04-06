#include "stm32f4xx_gpio.h"

//LED driver 4, PORTD

#define LED0_C GPIO_Pin_6
#define LED0_D GPIO_Pin_7


void write_delay(void)
{
	volatile unsigned int i;

	for(i=0;i<50;i++);
}


void led_driver_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	// SCK, MOSI
	GPIO_InitStructure.GPIO_Pin = (LED0_C | LED0_D);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, LED0_C);
	GPIO_SetBits(GPIOD, LED0_D);

}


void led_driver_write_byte(unsigned char data)
{
	volatile int i;

	GPIO_ResetBits(GPIOD, LED0_C);
	write_delay();

	for(i=7; i>=0; i--)
	{
		write_delay();
		if( ( data >> i ) & 0x01 )					// data bit i
			GPIO_SetBits(GPIOD, LED0_D);
		else
			GPIO_ResetBits(GPIOD, LED0_D);
		write_delay();
		GPIO_SetBits(GPIOD, LED0_C);		// tick
		write_delay();
		GPIO_ResetBits(GPIOD, LED0_C);		// tock
	}

	write_delay();

	//GPIO_SetBits(GPIOD, LED0_C);		// pull clock high

}



void led_driver_write_data(unsigned char data, unsigned char howmany)
{
	volatile unsigned int i, j;

/*
	GPIO_ResetBits(GPIOD, LED0_C);

	for(i=0;i<10;i++)			//hold clock input low for at least 500 us
	{
		write_delay();
	}
*/



	for(j=0;j<howmany;j++)
	{
		led_driver_write_byte(255);
		led_driver_write_byte(255);
		led_driver_write_byte(255);

	}

	GPIO_ResetBits(GPIOD, LED0_C);

	for(i=0;i<200;i++)			//hold clock input low for at least 500 us
	{
		write_delay();
	}

	GPIO_SetBits(GPIOD, LED0_C);
	for(i=0;i<200;i++)			//hold clock input low for at least 500 us
	{
		write_delay();
	}
}
