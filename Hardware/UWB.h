#ifndef __UWB_H_
#define __UWB_H_

#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"

void OneclickCalibration(void);
void SendOneClickCalibrationStatusReading(void);
uint8_t EndCalibrationStatusCheck(void);

#endif
