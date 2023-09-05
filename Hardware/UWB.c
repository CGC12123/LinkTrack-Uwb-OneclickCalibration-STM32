#include "UWB.h"



// struct _Uwb* UwbRight; // 用于校验后储存

// 发送 “读输入帧”


// static uint8_t packet_ID[2] = { 0x54, 0x00 };
// uint8_t ReadInputFrames_mode; // 记录读返回帧的mode

// 发送读输入帧 请求数据
void SendReadInputFrames(void)
{
    Serial_SendArray(ReadInputFrames, 128); // 发送
}
/*
// 读取返回帧
void ReadReturnFrame(void)
{
    // 状态机
    static struct _Uwb Uwb;
    static unsigned char rc_counter = 0;
    static signed char sum = 0;

    // if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	// {
		uint8_t RxData = USART_ReceiveData(USART1);
        if( rc_counter < sizeof(packet_ID) )
        {
            //接收包头
            if( RxData != packet_ID[ rc_counter ] )
            {
                rc_counter = 0;
                sum = 0;
            }
            else
            {
                Serial_SendByte(RxData);
                ++rc_counter;
                sum += RxData;
            }
        }
        else if( rc_counter < sizeof(packet_ID) + sizeof(Uwb) )
        {	
            //接收数据
            Serial_SendByte(RxData);
            ( (unsigned char*)&Uwb )[ rc_counter - sizeof(packet_ID) ] = RxData;
            sum += RxData;
            ++rc_counter;
        }
        else
        {
            // 包尾校验
            if( sum == RxData )
            {
                Serial_SendByte(RxData);
                ReadInputFrames_mode = Uwb.mode;
                *UwbRight = Uwb; // 赋值
            }
            rc_counter = 0;
            sum = 0;
        }
    // }
}
*/
/*
// 发送一键标定指令
void SendOneClickCalibration(void)
{
    // 状态机
    unsigned char rc_counter = 0;
    signed char sum = 0;
    
    // 替换数据
    UwbRight->mode = 0x08;

    if( rc_counter < sizeof(packet_ID) )
    {
        Serial_SendByte(packet_ID[rc_counter]);
        ++rc_counter;
        sum += packet_ID[rc_counter];
    }
    else if( rc_counter < sizeof(packet_ID) + sizeof(UwbRight) && UwbRight->mix == 0x00)
    {	
        //发送指令数据
        Serial_SendByte(*(uint8_t*)&UwbRight[rc_counter - sizeof(packet_ID)]);
        sum += *((uint8_t*)&UwbRight[rc_counter - sizeof(packet_ID)]);
        ++rc_counter;
    }
    else
    {
        // 发送校验位
        Serial_SendByte(sum % 0x100);
    }
}

// 发送一键标定状态读取指令
void SendOneClickCalibrationStatusReading(void)
{
    // 状态机
    unsigned char rc_counter = 0;
    signed char sum = 0;

    if( rc_counter < sizeof(packet_ID) )
    {
        Serial_SendByte(packet_ID[rc_counter]);
        ++rc_counter;
        sum += packet_ID[rc_counter];
    }
    else if( rc_counter < sizeof(packet_ID) + sizeof(UwbRight) )
    {	
        // 替换数据
        UwbRight->mix = 0x20;
        UwbRight->mode = 0x08;
        
        //发送指令数据
        Serial_SendByte(*(uint8_t*)&UwbRight[rc_counter - sizeof(packet_ID)]);
        sum += *((uint8_t*)&UwbRight[rc_counter - sizeof(packet_ID)]);
        ++rc_counter;
    }
    else
    {
        // 发送校验位
        Serial_SendByte(sum);
    }
}

// 检查是否结束标定
uint8_t EndCalibrationStatusCheck(void)
{
    // 读输入帧
    unsigned char rc_counter = 0;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
        if( rc_counter < sizeof(packet_ID) )
        {
            //接收包头
            if( RxData != packet_ID[ rc_counter ] )
            {
                rc_counter = 0;
            }
            else
            {
                ++rc_counter;
            }
        }
        else if( rc_counter < 18 )
        {	
            //比较数据 mode 当重置为读数据帧中的数据表示一键标定自动结束
            if(ReadInputFrames_mode == RxData && rc_counter == 17)
            {
                return 1;
            }
        }
    }
    return 0;
}
*/
// 一键标定
/*
void OneclickCalibration(void)
{
    // SerialUWB_Init(); // UWB serial 初始化
    // SendReadInputFrames(); // 发送读输入帧
    // ReadReturnFrame(); // 读取返回帧
    // SendOneClickCalibration(); // 发送一键标定指令
}
*/
