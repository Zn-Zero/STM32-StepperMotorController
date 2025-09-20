// 按键
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Motor.h"

extern MotorDir MOTOR_ENA;

// 启停按键 PA0 - 上拉输入
void Key_StartStop_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_START_STOP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_START_STOP_PORT, &GPIO_InitStructure);

    // 配置中断线映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitTypeDef EXTI_InitStructure;

    // 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_START_STOP_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = KEY_START_STOP_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 三档定速按键 PB13 - 下拉输入
void Key_SpeedLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_SPEED_LEVEL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY_SPEED_LEVEL_PORT, &GPIO_InitStructure);

    // 配置中断线映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    EXTI_InitTypeDef EXTI_InitStructure;

    // 下降沿触发
    EXTI_InitStructure.EXTI_Line = KEY_SPEED_LEVEL_EXTI;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = KEY_SPEED_LEVEL_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_Init(&NVIC_InitStructure);
}

void Keys_Init(void)
{
    // 启停按键 PA0
    Key_StartStop_Init();
    // 三档定速按键 PB13
    Key_SpeedLevel_Init();
}

void EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        if (MOTOR_ENA == MOTOR_STOP)
        {
            Motor_Start_Instantly();
        }
        else
        {
            Motor_Stop_Instantly();
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line15) == SET)
    {


        
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
