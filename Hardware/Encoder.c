// 编码器 PA1 -> A PA2 -> C PA3 -> B
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

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
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0F; // 滤波
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    // 配置编码器模式
    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // 清除计数器
    TIM_SetCounter(ENCODER_TIM, 0);

    // 使能定时器
    TIM_Cmd(ENCODER_TIM, ENABLE);
}

// 编码器C相
void Encoder_C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 编码器按键 PA2
    GPIO_InitStructure.GPIO_Pin = KEY_ENCODER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_ENCODER_PORT, &GPIO_InitStructure);

    // 配置中断线映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2); // 编码器按键

    EXTI_InitTypeDef EXTI_InitStructure;

    // 编码器按键中断配置 - 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_ENCODER_EXTI;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    // 编码器按键中断
    NVIC_InitStructure.NVIC_IRQChannel = KEY_ENCODER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_Init(&NVIC_InitStructure);
}
