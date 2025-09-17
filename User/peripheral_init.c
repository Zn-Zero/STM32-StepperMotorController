#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"
#include "peripheral_define.h"

// GPIO初始化
void GPIO_InitAll(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能所有用到的GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                          RCC_APB2Periph_AFIO, ENABLE);
    
    // 按键初始化 - 上拉输入
    // 启停按键 PA0
    GPIO_InitStructure.GPIO_Pin = KEY_START_STOP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_START_STOP_PORT, &GPIO_InitStructure);
    
    // 编码器按键 PA2
    GPIO_InitStructure.GPIO_Pin = KEY_ENCODER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_ENCODER_PORT, &GPIO_InitStructure);
    
    // 三档定速按键 PB13
    GPIO_InitStructure.GPIO_Pin = KEY_SPEED_LEVEL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_SPEED_LEVEL_PORT, &GPIO_InitStructure);
    
    // 编码器A/B相初始化 - 复用推挽输入 (定时器模式)
    GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN | ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
    
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
    
    // 传感器输入 PA7 - 模拟输入
    GPIO_InitStructure.GPIO_Pin = SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(SENSOR_PORT, &GPIO_InitStructure);
    
    // 蜂鸣器 PB0 - 推挽输出
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);
    
    // LED PB1 - 推挽输出
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // 触摸屏I2C引脚初始化 - 复用开漏输出
    GPIO_InitStructure.GPIO_Pin = TOUCH_SCL_PIN | TOUCH_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始状态设置
    BUZZER_OFF();
    LED_OFF();
    MOTOR_DIR_FORWARD();
}

// 外部中断初始化
void EXTI_InitAll(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 配置中断线映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  // 启停按键
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);  // 编码器按键
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13); // 三档定速按键
    
    // 启停按键中断配置 - 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_START_STOP_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // 编码器按键中断配置 - 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_ENCODER_EXTI;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    // 三档定速按键中断配置 - 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_SPEED_LEVEL_EXTI;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置中断优先级
    // 启停按键中断
    NVIC_InitStructure.NVIC_IRQChannel = KEY_START_STOP_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 编码器按键中断
    NVIC_InitStructure.NVIC_IRQChannel = KEY_ENCODER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_Init(&NVIC_InitStructure);
    
    // 三档定速按键中断
    NVIC_InitStructure.NVIC_IRQChannel = KEY_SPEED_LEVEL_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_Init(&NVIC_InitStructure);
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

// 电机PWM定时器初始化
void MotorPWM_TIM_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器基本配置
    TIM_TimeBaseStructure.TIM_Period = arr;      // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;   // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(MOTOR_PUL_TIM, &TIM_TimeBaseStructure);
    
    // PWM模式配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;           // 初始占空比0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(MOTOR_PUL_TIM, &TIM_OCInitStructure);
    
    // 使能PWM输出比较预装载
    TIM_OC1PreloadConfig(MOTOR_PUL_TIM, TIM_OCPreload_Enable);
    // 使能定时器预装载寄存器
    TIM_ARRPreloadConfig(MOTOR_PUL_TIM, ENABLE);
    
    // 使能定时器
    TIM_Cmd(MOTOR_PUL_TIM, ENABLE);
}

// ADC初始化 (用于传感器)
void ADC_InitSensor(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    
    // 使能ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    // ADC时钟分频 (最大14MHz)
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    // ADC配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // 单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // 转换通道数量
    ADC_Init(SENSOR_ADC, &ADC_InitStructure);
    
    // 配置ADC通道转换顺序和采样时间
    ADC_RegularChannelConfig(SENSOR_ADC, SENSOR_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    
    // 使能ADC
    ADC_Cmd(SENSOR_ADC, ENABLE);
    
    // ADC校准
    ADC_ResetCalibration(SENSOR_ADC);
    while(ADC_GetResetCalibrationStatus(SENSOR_ADC));
    
    ADC_StartCalibration(SENSOR_ADC);
    while(ADC_GetCalibrationStatus(SENSOR_ADC));
    
    // 开始ADC转换
    ADC_SoftwareStartConvCmd(SENSOR_ADC, ENABLE);
}

// I2C初始化 (用于触摸屏)
void I2C_InitTouchScreen(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    
    // 使能I2C时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    
    // I2C配置
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;  // 主机模式不需要自己的地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;  // 400kHz高速模式
    I2C_Init(TOUCH_I2C, &I2C_InitStructure);
    
    // 使能I2C
    I2C_Cmd(TOUCH_I2C, ENABLE);
}

// 所有外设初始化
void Peripheral_InitAll(void)
{
    // 配置系统时钟等（通常在main函数中）
    // SystemInit();
    
    // 初始化GPIO
    GPIO_InitAll();
    
    // 初始化外部中断
    EXTI_InitAll();
    
    // 初始化编码器定时器
    Encoder_TIM_Init();
    
    // 初始化电机PWM定时器 (50kHz: 72MHz / (71+1)/(19+1) = 50kHz)
    MotorPWM_TIM_Init(19, 71);
    
    // 初始化传感器ADC
    ADC_InitSensor();
    
    // 初始化触摸屏I2C
    I2C_InitTouchScreen();
}

// 设置电机PWM占空比 (0-1000对应0-100.0%)
void Motor_SetPWM(uint16_t duty)
{
    if(duty > 1000) duty = 1000;
    // 根据自动重装载值计算比较值
    // TIM_SetCompare1(MOTOR_PUL_TIM, (uint16_t)(duty * TIM_GetAutoreload(MOTOR_PUL_TIM) / 1000));
}

// 读取编码器值
int16_t Encoder_GetCount(void)
{
    int16_t count = TIM_GetCounter(ENCODER_TIM);
    TIM_SetCounter(ENCODER_TIM, 0);  // 读取后清零
    return count;
}

// 读取传感器值 (0-4095对应0-3.3V)
uint16_t Sensor_ReadValue(void)
{
    // 等待ADC转换完成
    while(!ADC_GetFlagStatus(SENSOR_ADC, ADC_FLAG_EOC));
    return ADC_GetConversionValue(SENSOR_ADC);
}
