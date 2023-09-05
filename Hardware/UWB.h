#ifndef __UWB_H_
#define __UWB_H_

#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"

struct _Uwb
{
	uint8_t mix;
	uint8_t role;
	uint8_t math_model;
    uint8_t uart_baudrate[3];
    uint8_t system_ch;
    uint8_t id;
    uint8_t update_rate[2];
    uint8_t system_id;
    uint8_t reserved1[1];
    uint8_t on_off;
    uint8_t reserved2[1];
    uint8_t filter_property;
    uint8_t mode;
    uint8_t reserved3[1];
    uint8_t protocol;
    uint8_t tx_gain;
    uint8_t reserved4[3];
    uint8_t node_capacity;
    uint8_t reserved5[2];
    uint8_t local_time[4];
    uint8_t reserved6[5];
    uint8_t anchor_group_index;
    char a0_coordinate_x[3];  char a0_coordinate_y[3]; char a0_coordinate_z[3];
    char a1_coordinate_x[3];  char a1_coordinate_y[3]; char a1_coordinate_z[3];
    char a2_coordinate_x[3];  char a2_coordinate_y[3]; char a2_coordinate_z[3];
    char a3_coordinate_x[3];  char a3_coordinate_y[3]; char a3_coordinate_z[3];
    char a4_coordinate_x[3];  char a4_coordinate_y[3]; char a4_coordinate_z[3];
    char a5_coordinate_x[3];  char a5_coordinate_y[3]; char a5_coordinate_z[3];
    char a6_coordinate_x[3];  char a6_coordinate_y[3]; char a6_coordinate_z[3];
    char a7_coordinate_x[3];  char a7_coordinate_y[3]; char a7_coordinate_z[3];
    char a8_coordinate_x[3];  char a8_coordinate_y[3]; char a8_coordinate_z[3];
    char a9_coordinate_x[3];  char a9_coordinate_y[3]; char a9_coordinate_z[3];
};

static uint8_t ReadInputFrames[128] = { 
                0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
                0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a};

void SendReadInputFrames(void);
// void OneclickCalibration(void);
// void SendOneClickCalibrationStatusReading(void);
// uint8_t EndCalibrationStatusCheck(void);

#endif
