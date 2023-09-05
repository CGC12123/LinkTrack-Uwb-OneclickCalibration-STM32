#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "UWB.h"
#include "TIMER.h"
#include "LED.h"

int main(void)
{
	Serial_Init(); // UWB serial 初始化
	while(1)
	{
		SendReadInputFrames();
		if (Serial_GetRxFlag() == 1)
		{
			OneclickCalibration();
			break;
		}
	}
}
