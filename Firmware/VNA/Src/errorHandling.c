#include "main.h"
#include "errorHandling.h"
#include "txChain.h"
#include "max2871.h"
#include "dwt_stm32_delay.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

void statusFucked(char *str)
{
	printUSB((uint8_t *) str);

	HAL_GPIO_WritePin(STATUS_LED_R_GPIO_Port,STATUS_LED_R_Pin,0);
	HAL_GPIO_WritePin(STATUS_LED_G_GPIO_Port,STATUS_LED_G_Pin,1);
	HAL_GPIO_WritePin(STATUS_LED_B_GPIO_Port,STATUS_LED_B_Pin,1);

	while (1);
}

void statusThinking(char *str)
{
	#if DEBUG == 1
		printUSB((uint8_t *) str);
	#endif

	HAL_GPIO_WritePin(STATUS_LED_R_GPIO_Port,STATUS_LED_R_Pin,1);
	HAL_GPIO_WritePin(STATUS_LED_G_GPIO_Port,STATUS_LED_G_Pin,1);
	HAL_GPIO_WritePin(STATUS_LED_B_GPIO_Port,STATUS_LED_B_Pin,0);
}

void statusNominal(char *str)
{
	#if DEBUG == 1
		printUSB((uint8_t *) str);
	#endif

	HAL_GPIO_WritePin(STATUS_LED_R_GPIO_Port,STATUS_LED_R_Pin,1);
	HAL_GPIO_WritePin(STATUS_LED_G_GPIO_Port,STATUS_LED_G_Pin,0);
	HAL_GPIO_WritePin(STATUS_LED_B_GPIO_Port,STATUS_LED_B_Pin,1);
}

void logError(char *str)
{
	#if DEBUG == 1
		printUSB((uint8_t *) str);
	#endif
	#if DEBUG == 2
		printUSB((uint8_t *) str);
	#endif

	HAL_GPIO_WritePin(STATUS_LED_R_GPIO_Port,STATUS_LED_R_Pin,0);
	HAL_GPIO_WritePin(STATUS_LED_G_GPIO_Port,STATUS_LED_G_Pin,1);
	HAL_GPIO_WritePin(STATUS_LED_B_GPIO_Port,STATUS_LED_B_Pin,1);
}
