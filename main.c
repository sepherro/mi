
#include <stdio.h>
#include "stm32f4_discovery.h"
#include "usbd_cdc_vcp.h"

/* Private macro */
/* Private variables */
uint32_t button_sts;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
uint32_t i = 0;
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**  Main
**===========================================================================
*/

int main(void)
{

	SystemInit();

	STM32F4_Discovery_LEDInit(LED3); //Orange
	STM32F4_Discovery_LEDInit(LED4); //Green
	STM32F4_Discovery_LEDInit(LED5); //Red
	STM32F4_Discovery_LEDInit(LED6); //Blue

	STM32F4_Discovery_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);

	while (1){

		if(usb_cdc_kbhit()){
			char c, buffer_out[15];
			c = usb_cdc_getc();
			switch(c){
				case '3':
					STM32F4_Discovery_LEDToggle(LED3);
					sprintf(buffer_out,"LED%c = %u\r\n",c,GPIO_ReadInputDataBit(GPIOD,LED3_PIN));
					usb_cdc_printf(buffer_out);
					break;
				case '4':
					STM32F4_Discovery_LEDToggle(LED4);
					sprintf(buffer_out,"LED%c = %u\r\n",c,GPIO_ReadInputDataBit(GPIOD,LED4_PIN));
					usb_cdc_printf(buffer_out);
					break;
				case '5':
					STM32F4_Discovery_LEDToggle(LED5);
					sprintf(buffer_out,"LED%c = %u\r\n",c,GPIO_ReadInputDataBit(GPIOD,LED5_PIN));
					usb_cdc_printf(buffer_out);
					break;
				case '6':
					STM32F4_Discovery_LEDToggle(LED6);
					sprintf(buffer_out,"LED%c = %u\r\n",c,GPIO_ReadInputDataBit(GPIOD,LED6_PIN));
					usb_cdc_printf(buffer_out);
					break;
			}
		}

		button_sts = STM32F4_Discovery_PBGetState(BUTTON_USER);

		if(button_sts){
			STM32F4_Discovery_LEDOff(LED3);
			STM32F4_Discovery_LEDOff(LED5);
			STM32F4_Discovery_LEDOff(LED3);
			STM32F4_Discovery_LEDOff(LED5);
		}
	}
}
