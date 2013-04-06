#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>
#include "usbd_cdc_vcp.h"
#include "led_driver.h"
#include "core_cm4.h"
#include "stm32f4_discovery.h"
#include "motor_driver.h"

/* Private macro */
/* Private variables */
uint32_t button_sts;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
uint32_t i = 0;
/* Private function prototypes */
/* Private functions */

volatile uint32_t flag_send_sensor_data, flag_new_command_received;
uint8_t usb_rx_buffer[10], usb_tx_buffer[10];
uint8_t usb_rx_ptr=0;
uint8_t character;

/**
**===========================================================================
**  Main
**===========================================================================
*/


void SysTick_Handler (void)
{
	flag_send_sensor_data = 1;
}


int main(void)
{

	SystemInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//

	STM32F4_Discovery_LEDInit(LED3); //Orange
	STM32F4_Discovery_LEDInit(LED4); //Green
	STM32F4_Discovery_LEDInit(LED5); //Red
	STM32F4_Discovery_LEDInit(LED6); //Blue

	STM32F4_Discovery_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);

	led_driver_init();
	motor_driver_init();

	SysTick_Config(2100000);										//10 times a second: 168 MHz / 8 = 21 MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	//while(1)
	//{
	//led_driver_write_data(50, 10);

	//}
	while (1)
	{

		if( usb_cdc_kbhit() )
		{
			character = usb_cdc_getc();
			usb_rx_buffer[usb_rx_ptr] = character;
			usb_rx_ptr++;

			if( usb_rx_ptr == 10 )															//upon receiving 10 bytes
			{
				if( ( usb_rx_buffer[0] == 0xAA ) && ( usb_rx_buffer[9] == 0xCC ) )			//check head and tail for correctness
				{
					usb_rx_ptr = 0;															//reset data pointer if correct
					flag_new_command_received = 1;											//set the flag if packet correct
				}
				else
				{
					usb_rx_ptr = 0;															// if packet incorrect, reset data pointer
				}
			}
		}

		if(flag_new_command_received)
		{



			flag_new_command_received = 0;
		}

		if(flag_send_sensor_data)
		{


			motor_set_speed(0x55, 0x55);

			flag_send_sensor_data = 0;
		}

		// here's how to send data through USB VCP
		/*
		sprintf(buffer_out,"LED%c = %u\r\n",c,GPIO_ReadInputDataBit(GPIOD,LED6_PIN));
		usb_cdc_printf(buffer_out);
		*/
	}
}
