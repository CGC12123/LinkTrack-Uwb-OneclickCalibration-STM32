#include "TIMER.h"

void TIM2_IRQHandler_SendOneClickCalibrationStatusReading(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        // 在这里执行你的函数或功能
        SendOneClickCalibrationStatusReading();

        // 检测到一键标定结束
        if(EndCalibrationStatusCheck != 0)
        {
            Timer_Configuration_stop();
        }

        // 清除中断标志位
        TIM2->SR &= ~TIM_SR_UIF;
    }
}

void Timer_Configuration_SendOneClickCalibrationStatusReading(void)
{
    // 启用定时器时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 7199;  // 假设定时器时钟频率为72MHz，预分频7199，得到10kHz的计数频率
    TIM2->ARR = 1999;  // 10kHz的计数频率下，计数1999次等于200毫秒

    // 启用定时器中断
    TIM2->DIER |= TIM_DIER_UIE;

    // 配置中断优先级
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);

    // 启动定时器
    TIM2->CR1 |= TIM_CR1_CEN;
}

void Timer_Configuration_stop(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}
