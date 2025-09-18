#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

#define MAX_FREQ 200000 // 最大频率200kHz
#define MIN_FREQ 10     // 最小频率10Hz，可根据需要调整

// 全局变量存储当前ARR值，用于计算固定占空比
static uint16_t current_arr = 0;

// 电机PWM定时器初始化
void Motor_Init(uint32_t frequency)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 电机方向控制 PA5 - 推挽输出
    GPIO_InitStructure.GPIO_Pin = MOTOR_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_DIR_PORT, &GPIO_InitStructure);

    // 电机PWM输出 PA6 - 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = MOTOR_PUL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PUL_PORT, &GPIO_InitStructure);

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

// 重新设置PWM频率（控制步进电机速度）
void Motor_SetFrequency(uint32_t frequency)
{
    // 限制频率范围，确保高低电平均>2.5us
    if (frequency > MAX_FREQ) frequency = MAX_FREQ;
    if (frequency < MIN_FREQ) frequency = MIN_FREQ;

    uint32_t timer_clock = 72000000;
    uint16_t prescaler = 0;
    uint16_t arr_value;

    // 停止定时器
    TIM_Cmd(TIM2, DISABLE);

    // 计算新的PSC和ARR值
    arr_value = (timer_clock / (frequency * (prescaler + 1))) - 1;

    while (arr_value > 0xFFFF)
    {
        prescaler++;
        arr_value = (timer_clock / (frequency * (prescaler + 1))) - 1;
    }

    current_arr = arr_value; // 更新当前ARR值

    // 更新定时器参数
    TIM2->PSC = prescaler;
    TIM2->ARR = arr_value;
    TIM2->CCR1 = arr_value / 2; // 保持50%占空比

    // 重新启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

// 梯形加速
void Trapezoidal_Acceleration(uint32_t current, uint32_t target)
{
    for (uint32_t freq = current; freq <= target; freq += 100)
    {
        Motor_SetFrequency(freq);
        Delay_us(10)
    }
}

// 梯形减速
void Trapezoidal_Deceleration(uint32_t current, uint32_t target)
{
    for (uint32_t freq = current; freq >= target; freq -= 100)
    {
        Motor_SetFrequency(freq);
        Delay_us(10)
    }
}
