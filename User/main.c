#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "UWB.h"
#include "TIMER.h"
#include "LED.h"

int main(void)
{
    SerialUWB_Init(); // UWB serial 初始化
	while(1)
	{
		OneclickCalibration();
	}

	// 定时发送一键标定状态读取指令
	// Timer_Configuration_SendOneClickCalibrationStatusReading();
}
