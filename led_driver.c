#include "stm32f4xx_gpio.h"

//LED driver 0, PORTE

#define LED0_C GPIO_Pin_5
#define LED0_D GPIO_Pin_6


void write_delay(void)
{
	volatile unsigned int i;

	for (i=0;i<100;i++);
}


void led_driver_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	// SCK, MOSI
	GPIO_InitStructure.GPIO_Pin = LED0_C | LED0_D;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


}


void led_driver_write_data(unsigned char address, unsigned char data)
{
	volatile int i;


	GPIO_ResetBits(GPIOE, LED0_C);

	for (i=0;i<100;i++)			//hold clock input low for at least 500 us
	{
		write_delay();
	}


	for(i=7; i>=0; i--)
	{
		spiDelay();
		if( ( data >> i ) & 0x01 )					// data bit i
			GPIO_SetBits(GPIOE, LED0_D);
		else
			GPIO_ResetBits(GPIOE, LED0_D);
		spiDelay();
		GPIO_SetBits(GPIOE, LED0_C);		// tick
		spiDelay();
		GPIO_ResetBits(GPIOE, LED0_C);		// tock
	}

	spiDelay();

	GPIO_SetBits(GPIOE, LED0_C);		// pull clock high

}
