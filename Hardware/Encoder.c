// 编码器
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Motor.h"
#include "OLED.h"

// 编码器AB相 
void Encoder_AB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 编码器A/B相初始化 - 复用推挽输入 (定时器模式)
    GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN | ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);

    // 使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 定时器基本配置
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF; // 最大计数范围
    TIM_TimeBaseStructure.TIM_Prescaler = 0;   // 不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER_TIM, &TIM_TimeBaseStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;

    // 编码器模式配置
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0F; // 滤波
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    // 配置编码器模式
    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // 清除计数器
    TIM_SetCounter(ENCODER_TIM, INIT_SPEED);

    // 使能定时器
    TIM_Cmd(ENCODER_TIM, ENABLE);
}

// 读取编码器值
uint32_t Encoder_GetSpeed(void)
{
    int16_t count = (int16_t)TIM_GetCounter(ENCODER_TIM);

    // OLED_ShowSignedNum(4, 1, count, 5);

    // TIM_SetCounter(ENCODER_TIM, 0); // 读取后清零

    if (count < 1)
    {
        count = 1;
        TIM_SetCounter(ENCODER_TIM, 1);
    }
    if (count > 600)
    {
        count = 600;
        TIM_SetCounter(ENCODER_TIM, 600);
    }
    Motor_SetSpeed(count);

    return count;
}


