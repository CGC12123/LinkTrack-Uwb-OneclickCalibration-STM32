#include "stm32f10x.h"                  // Device header
#include <stdio.h>//为了移植printf

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[128];
uint8_t Serial_RxFlag = 0;
uint32_t sum = 0;
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 460800;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	for(uint16_t i = 0; i< Length ;i++)
	{
		Serial_SendByte(Array[i]);
	}
}
void Serial_SendArray_Uwb(uint8_t *Array,uint16_t Length)
{
	uint8_t sum = 0;
	for(uint16_t i = 0; i< Length - 1 ;i++)
	{
		Serial_SendByte(Array[i]);
		sum += Array[i];
	}
	Serial_SendByte(sum % 0x100); // 包尾
}


void Serial_SendString(char *String)
{
	for(uint8_t i=0;String[i]!='\0';i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t Result = 1;
	while(y--)
	{
		Result*=x;  
	}
	return Result;
}
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i = 0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket,4);
	Serial_SendByte(0xFE);

}

void USART1_IRQHandler(void)
{
	static uint8_t Rxstate = 0;
	static uint8_t pRxstate = 0;	
	if (USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET)
	{
		uint8_t ReceiveData = USART_ReceiveData(USART1);
		if(Rxstate == 0)
		{
			if(ReceiveData == 0x54)
			{
				Rxstate = 1;
				pRxstate = 0;
				Serial_RxPacket[0] = ReceiveData;
				sum += ReceiveData;
			}
		}
		else if(Rxstate == 1)
		{
			if(ReceiveData == 0x00)
			{
				Rxstate = 2;
				Serial_RxPacket[1] = ReceiveData;
				sum += ReceiveData;
			}
		}
		else if(Rxstate == 2)
		{
			Serial_RxPacket[pRxstate + 2] = ReceiveData;
			pRxstate++;
			sum += ReceiveData;
			if(pRxstate>=125)
			{
				Rxstate = 3;
			}
		}
		else if(Rxstate == 3)
		{
//			if(sum%0x100 == ReceiveData )
//			{
				Serial_RxPacket[127] = ReceiveData;
				Rxstate = 0;
				Serial_RxFlag = 1;
//			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
}

void OneclickCalibration(void)
{
	Serial_RxPacket[17] = 0x08;
	Serial_SendArray_Uwb(Serial_RxPacket,128);
}
