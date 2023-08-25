#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "UWB.h"
#include "TIMER.h"
#include "LED.h"

int main(void)
{
	LED1_OFF();
	OneclickCalibration();

	// 定时发送一键标定状态读取指令
	Timer_Configuration_SendOneClickCalibrationStatusReading();

	if(EndCalibrationStatusCheck != 0)
	{
		// 点个灯标记一下
		LED1_ON();
	}
}
