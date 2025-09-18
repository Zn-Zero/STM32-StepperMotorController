#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"
#include "peripheral_define.h"

// GPIO初始化
void GPIO_InitAll(void)
{   
    // 使能所有用到的GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
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

    // 初始状态设置
    
    
    MOTOR_DIR_FORWARD();
}

// 编码器定时器初始化 (编码器模式)
void Encoder_TIM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    
    // 使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基本配置
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  // 最大计数范围
    TIM_TimeBaseStructure.TIM_Prescaler = 0;     // 不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER_TIM, &TIM_TimeBaseStructure);
    
    // 编码器模式配置
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0F;    // 滤波
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);
    
    // 配置编码器模式
    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    // 清除计数器
    TIM_SetCounter(ENCODER_TIM, 0);
    
    // 使能定时器
    TIM_Cmd(ENCODER_TIM, ENABLE);
}





// 所有外设初始化
void Peripheral_InitAll(void)
{
    // 初始化GPIO
    // GPIO_InitAll();

    // 初始化外部中断
    // EXTI_InitAll();

    // 初始化编码器定时器
    // Encoder_TIM_Init();
    
    // 初始化电机PWM定时器 (50kHz: 72MHz / (71+1)/(19+1) = 50kHz)
    // MotorPWM_TIM_Init(20 - 1, 72 - 1); //arr psc
    
    // 初始化传感器ADC
    // ADC_InitSensor();
    
    // 初始化触摸屏I2C
    // I2C_InitTouchScreen();
}

// 读取编码器值
int16_t Encoder_GetCount(void)
{
    int16_t count = TIM_GetCounter(ENCODER_TIM);
    TIM_SetCounter(ENCODER_TIM, 0);  // 读取后清零
    return count;
}

