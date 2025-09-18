#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

#define MAX_FREQ 200000 // 最大频率200kHz
#define MIN_FREQ 10     // 最小频率10Hz，可根据需要调整

// 电机PWM定时器初始化
void Motor_Init(uint32_t frequency)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 限制频率范围
    if (frequency > MAX_FREQ) frequency = MAX_FREQ;
    if (frequency < MIN_FREQ) frequency = MIN_FREQ;

    // 计算预分频器和自动重装载值
    uint32_t timer_clock = 72000000; // TIM2时钟为72MHz
    uint16_t prescaler = 0;
    uint16_t arr_value;

    // 计算初始值
    arr_value = (timer_clock / (frequency * (prescaler + 1))) - 1;

    // 确保ARR值不超过16位最大值
    while (arr_value > 0xFFFF)
    {
        prescaler++;
        arr_value = (timer_clock / (frequency * (prescaler + 1))) - 1;
    }

    current_arr = arr_value; // 保存当前ARR值

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 配置定时器时基
    TIM_TimeBaseStructure.TIM_Period = arr_value;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    // 配置PWM模式，固定50%占空比
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = arr_value / 2; // 50%占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // 使能预装载
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE); // 使能PWM输出
}