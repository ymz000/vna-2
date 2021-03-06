#include "main.h"
#include "gainBlock.h"
#include "pe43711.h"
#include "ad8319.h"
#include "dwt_stm32_delay.h"
#include <string.h>
#include <stdlib.h>

struct gainStruct gainStatus;

void gainBlockSetup(struct gainStruct *gainStatus)
{
	setAttenuation(31.75, gainStatus);
}

void setOutputPowerUSB(struct gainStruct *gainStatus)
{
	int8_t str1[64] = "";
	float inputPower;
	printUSB("Enter an output power in dBm between -20 and 0 dBm: ");
	scanUSB(str1, 64);
	inputPower = atof(str1);
	printUSB(inputPower);
	printUSB("\n");

	if (inputPower >= -20 || inputPower <= 0)
	{
		setOutputPower(inputPower, gainStatus);
	}
	else
	{
		printUSB("\nBad input, try harder next time.\n\n");
	}

}

void setOutputPower(float setPower, struct gainStruct *gainStatus)
{
	int8_t str1[64] = "";

	setAttenuation(gainStatus->attenuation, gainStatus);
	gainStatus->setOutputPower = setPower;
	gainStatus->measOutputPower = readAD8319();

	sprintf((char *)str1, "Power = %0.2f dBm\n", gainStatus->measOutputPower);
	printUSB(str1);

	if (gainStatus->measOutputPower > gainStatus->setOutputPower)
	{
		while (gainStatus->measOutputPower > gainStatus->setOutputPower)
		{
			setAttenuation(gainStatus->attenuation += 0.25, gainStatus);
			HAL_Delay(10);
			gainStatus->measOutputPower = readAD8319();
			sprintf((char *)str1, "Power = %0.2f dBm\n", gainStatus->measOutputPower);
			printUSB(str1);
			if (gainStatus->attenuation >= 31.75 || gainStatus->attenuation <= 0)
				break;
		}
	}
	else if (gainStatus->measOutputPower < gainStatus->setOutputPower)
	{
		while (gainStatus->measOutputPower < gainStatus->setOutputPower)
		{
			setAttenuation(gainStatus->attenuation -= 0.25, gainStatus);
			HAL_Delay(10);
			gainStatus->measOutputPower = readAD8319();
			sprintf((char *)str1, "Power = %0.2f dBm\n", gainStatus->measOutputPower);
			printUSB(str1);
			if (gainStatus->attenuation >= 31.75 || gainStatus->attenuation <= 0)
				break;
		}
	}

}

void gainBlockPrintStatus(struct gainStruct *gainStatus)
{
	int8_t str1[64] = "";

	printUSB("----  Gain Block  ----\n\n");

	sprintf((char *)str1, "Set Power = %0.2f dBm\n", gainStatus->setOutputPower);
	printUSB(str1);

	printPower(gainStatus);

	sprintf((char *)str1, "Set Attenuation = %0.2f dB\n", gainStatus->attenuation);
	printUSB(str1);
}

