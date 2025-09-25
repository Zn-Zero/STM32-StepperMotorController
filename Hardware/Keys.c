// 按键
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Motor.h"
#include "OLED.h"
#include "Delay.h"

// 启停按键 PA2 - 上拉输入
void Key_StartStop_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_START_STOP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_START_STOP_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = KEY_START_STOP_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = KEY_START_STOP_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 电机方向控制 PA3
void Key_DIR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_DIR_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = KEY_DIR_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = KEY_DIR_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 三档定速按键 PA4 - 上拉输入
void Key_SpeedLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_SPEED_LEVEL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_SPEED_LEVEL_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = KEY_SPEED_LEVEL_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = KEY_SPEED_LEVEL_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_Init(&NVIC_InitStructure);
}

// 上限位控制 PA5
void Key_Up_Limit_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_UP_LIMIT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_UP_LIMIT_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = KEY_UP_LIMIT_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = KEY_UP_LIMIT_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 下限位控制 PA6
void Key_Down_Limit_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_DOWN_LIMIT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_DOWN_LIMIT_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = KEY_DOWN_LIMIT_EXTI;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = KEY_DOWN_LIMIT_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Keys_Init(void)
{
    // 启停按键 PA2
    Key_StartStop_Init();
    // 转向按键 PA3
    Key_DIR_Init();
    // 三档定速按键 PA4
    Key_SpeedLevel_Init();
    // 上限位控制 PA5
    Key_Up_Limit_Init();
    // 下限位控制 PA6
    Key_Down_Limit_Init();
}

void EXTI2_IRQHandler()
{
    // 电机启停
    if (EXTI_GetITStatus(KEY_START_STOP_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_START_STOP_PORT, KEY_START_STOP_PIN) == 0)
        {
            Delay_ms(10);
        }

        // OLED_ShowString(4, 1, "EXTI2");

        if (MOTOR_ENA == MOTOR_STOP)
        {
            Motor_Start_Instantly();
        }
        else
        {
            Motor_Stop_Instantly();
        }

        EXTI_ClearITPendingBit(KEY_START_STOP_EXTI);
    }
}

void EXTI3_IRQHandler()
{
    if (EXTI_GetITStatus(KEY_DIR_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_DIR_PORT, KEY_DIR_PIN) == 0)
        {
            Delay_ms(10);
        }

        if (MOTOR_DIR == DIR_CW)
        {
            MOTOR_DIR_FORWARD();
            MOTOR_DIR = DIR_CCW;
        }
        else
        {
            MOTOR_DIR_REVERSE();
            MOTOR_DIR = DIR_CW;
        }

        EXTI_ClearITPendingBit(KEY_DIR_EXTI);
    }
}

void EXTI4_IRQHandler()
{
    // 三档定速切换
    if (EXTI_GetITStatus(KEY_SPEED_LEVEL_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_SPEED_LEVEL_PORT, KEY_SPEED_LEVEL_PIN) == 0)
        {
            Delay_ms(10);
        }

        spd_idx++;
        if (spd_idx > 2)
        {
            spd_idx = 0;
        }

        TIM_SetCounter(ENCODER_TIM, spd_preset[spd_idx]);
        // Motor_SetSpeed(spd_preset[spd_idx]);

        EXTI_ClearITPendingBit(KEY_SPEED_LEVEL_EXTI);
    }
}


void EXTI9_5_IRQHandler()
{
    // 上限位
    if (EXTI_GetITStatus(KEY_UP_LIMIT_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_UP_LIMIT_PORT, KEY_UP_LIMIT_PIN) == 0)
        {
            Delay_ms(10);
        }

        // todo 上限位

        EXTI_ClearITPendingBit(KEY_UP_LIMIT_EXTI);
    }

    // 下限位
    if (EXTI_GetITStatus(KEY_DOWN_LIMIT_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_DOWN_LIMIT_PORT, KEY_DOWN_LIMIT_PIN) == 0)
        {
            Delay_ms(10);
        }

        // todo 下限位

        EXTI_ClearITPendingBit(KEY_DOWN_LIMIT_EXTI);
    }
}

