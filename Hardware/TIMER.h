#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "UWB.h"

extern uint8_t temp_EndCalibrationStatusCheck;

void Timer_Configuration_SendOneClickCalibrationStatusReading(void); // 定时发送一键标定状态读取指令定时器配置函数
void TIM2_IRQHandler_SendOneClickCalibrationStatusReading(void); // 定时发送一键标定状态读取指令中断函数
void Timer_Configuration_stop(void); // 关闭定时器2

#endif
