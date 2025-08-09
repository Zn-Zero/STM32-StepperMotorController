#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "ExternParam_def.h"
#include "Motor.h"
#include "OLED.h"
#include "Delay.h"
#include "Buzzer.h"

void Button_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = BTN_START_STOP_PIN;
    GPIO_Init(BTN_START_STOP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BTN_DIRECTION_PIN;
    GPIO_Init(BTN_DIRECTION_PORT, &GPIO_InitStructure);

    OLED_ShowString(2, 1, "State: STOP");
    OLED_ShowString(3, 1, "Dir: CW");

    // 配置按钮中断
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  // 中断模式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  // 启用中断线

    // 启停按钮中断(EXTI0)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_Init(&EXTI_InitStructure);

    // 方向按钮中断(EXTI2)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // 启用中断通道

    // 启停按钮中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
    NVIC_Init(&NVIC_InitStructure);

    // 方向按钮中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void KeyEvent_BtnStartStop(void) {
    Buzzer_ms(100);

    if (Motor_ENA == MOTOR_STOP) {
        Motor_ENA = MOTOR_RUN;
        StartPulse();
        OLED_ShowString(2, 8, "RUNNING");
    } else {
        Motor_ENA = MOTOR_STOP;
        StopPulse();
        OLED_ShowString(2, 8, "STOP   ");
    }
}

void KeyEvent_BtnDirection(void) {
    Buzzer_ms(100);

    if (Motor_DIR == DIR_CW) {
        Motor_DIR = DIR_CCW;
        OLED_ShowString(3, 6, "CCW");
    } else {
        Motor_DIR = DIR_CW;
        OLED_ShowString(3, 6, "CW ");
    }

    Motor_SetDir(Motor_DIR);
}

void Read_Button(void) {
	if (GPIO_ReadInputDataBit(BTN_START_STOP_PORT, BTN_START_STOP_PIN) == 0) {
		Delay_ms(10);
		while (GPIO_ReadInputDataBit(BTN_START_STOP_PORT, BTN_START_STOP_PIN) == 0)
		Delay_ms(10);

        KeyEvent_BtnStartStop();
	}

	if (GPIO_ReadInputDataBit(BTN_DIRECTION_PORT, BTN_DIRECTION_PIN) == 0) {
		Delay_ms(10);
		while (GPIO_ReadInputDataBit(BTN_DIRECTION_PORT, BTN_DIRECTION_PIN) == 0)
		Delay_ms(10);

        KeyEvent_BtnDirection();
	}
}
