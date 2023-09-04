#include "UWB.h"

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

struct _Uwb* UwbRight; // 用于校验后储存

// 发送 “读输入帧”
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

static uint8_t packet_ID[2] = { 0x54, 0x00 };
uint8_t ReadInputFrames_mode; // 记录读返回帧的mode

// 发送读输入帧 请求数据
void SendReadInputFrames(void)
{
    Serial_SendArray(ReadInputFrames, 128); // 发送
}

// 读取返回帧
void ReadReturnFrame(void)
{
    // 状态机
    struct _Uwb Uwb;
    static unsigned char rc_counter = 0;
    signed char sum = 0;

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
                ++rc_counter;
                sum += RxData;
                // Serial_SendByte(RxData);
            }
        }
        else if( rc_counter < sizeof(packet_ID) + sizeof(Uwb) )
        {	
            //接收数据
            ( (unsigned char*)&Uwb )[ rc_counter - sizeof(packet_ID) ] = RxData;
            sum += RxData;
            ++rc_counter;
            // Serial_SendByte(RxData);
        }
        else
        {
            // 包尾校验
            if( sum == RxData )
            {
                ReadInputFrames_mode = Uwb.mode;
                *UwbRight = Uwb; // 赋值
                // Serial_SendByte(RxData);
            }
            rc_counter = 0;
            sum = 0;
        }
    // }
}

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

// 一键标定
void OneclickCalibration(void)
{
    // SerialUWB_Init(); // UWB serial 初始化
    // SendReadInputFrames(); // 发送读输入帧
    ReadReturnFrame(); // 读取返回帧
    // SendOneClickCalibration(); // 发送一键标定指令
}
