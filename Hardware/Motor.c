#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "ExternParam_def.h"
#include "OLED.h"
#include "Buzzer.h"
#include "Delay.h"

void Motor_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = PUL_PIN;
    GPIO_Init(PUL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DIR_PIN;
    GPIO_Init(DIR_PORT, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin = ENA_PIN;
    // GPIO_Init(ENA_PORT, &GPIO_InitStructure);

    
    // 定时器基本配置
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 滤波模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;               // 预分频器，72MHz/7200=10kHz
    TIM_TimeBaseStructure.TIM_Period = ENCODER_INIT_SPEED - 1;  // 自动重装载值 CK_CNT_OV(定时频率) = 72M/(PSC+1)/(ARR+1)  100 -> 5000step/s
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            // 重复计数器，仅限高级定时器。通用定时器赋0即可。

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    Tim
    // 使能定时器更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    

    // 配置中断优先级
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 默认先关闭定时器
    TIM_Cmd(TIM2, DISABLE);
}

// 启动脉冲输出
void StartPulse(void) {
    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

// 停止脉冲输出
void StopPulse(void) {
    // 停止定时器
    TIM_Cmd(TIM2, DISABLE);
}

// 设置脉冲周期，单位：us
void SetPulsePeriod(uint32_t us) {
    // 停止定时器
    TIM_Cmd(TIM2, DISABLE);
    
    // 更新自动重装载值
    TIM_SetAutoreload(TIM2, us - 1);
    
    Motor_Speed = (1*1000*1000 * SCREW_LEAD * 60)/(2 * Encoder_Count * ROUND_STEP);
    OLED_ShowNum(1, 1, Encoder_Count, 16);
    OLED_ShowNum(4, 1, Motor_Speed, 16);
    
    // 重新启动定时器
    if (Motor_ENA == MOTOR_RUN) TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief 产生一个脉冲信号驱动电机
 */
void Motor_Puls(void) {
    // 产生高电平脉冲(至少2.5us)
    GPIO_SetBits(PUL_PORT, PUL_PIN);
    Delay_us(5);  // 至少2.5us高电平

    // 产生低电平(至少2.5us)
    GPIO_ResetBits(PUL_PORT, PUL_PIN);
    Delay_us(5); // 至少2.5us低电平

    // if (Motor_Speed) Delay_us(Motor_Speed);  // 低电平时间决定速度
}

/**
 * @brief 设置电机旋转方向
 * @param dir 旋转方向(DIR_CW或DIR_CCW)
 */
void Motor_SetDir(MotorDir dir) {
    if (dir == DIR_CW) {
        GPIO_ResetBits(DIR_PORT, DIR_PIN);  // 低电平顺时针
    } else {
        GPIO_SetBits(DIR_PORT, DIR_PIN);    // 高电平逆时针
    }
    Delay_us(5);  // DIR提前PUL至少5us的时序要求
}



