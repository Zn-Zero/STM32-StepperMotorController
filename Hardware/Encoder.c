#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "ExternParam_def.h"
#include "Encoder.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"

void Encoder_Init(void) {
    // 编码器引脚初始化
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = ENC_A_PIN;
    GPIO_Init(ENC_A_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ENC_B_PIN;
    GPIO_Init(ENC_B_PORT, &GPIO_InitStructure);

    // 配置编码器A相中断(EXTI14 -> A相)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    // 配置编码器B相中断(EXTI12 -> B相)
    // GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

    EXTI_InitTypeDef EXTI_InitStructure;

    // EXTI_InitStructure.EXTI_Line = EXTI_Line14 | EXTI_Line12;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发

    EXTI_Init(&EXTI_InitStructure);

    // 编码器中断优先级
    
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStructure);
}

/**
 * 调速考虑梯形算法
 */

void ENC_A_IRQHandler(void) {
    if (GPIO_ReadInputDataBit(GPIOB, ENC_B_PIN) == 0) {
        // 正转加速
        if (Motor_Speed < ENCODER_MAX_SPEED) {
            Encoder_Count -= 10;
        } else {
            // Encoder_Count = ENCODER_MAX_SPEED / ENCODER_MIN_SPEED;
        }
    } else {
        // 反转减速
        if (Motor_Speed > ENCODER_MIN_SPEED) {
            Encoder_Count += 10;
        } else {
            // Encoder_Count = ENCODER_MAX_SPEED / ENCODER_SPEED_STEP;
        }
    }

    SetPulsePeriod(Encoder_Count);
}

void ENC_B_IRQHandler(void) {
    if (GPIO_ReadInputDataBit(GPIOB, ENC_A_PIN) == 0) {
        // 反转减速
        if (Motor_Speed < ENCODER_MAX_SPEED) {
            Encoder_Count++;
            // Motor_Speed = Encoder_Count * ENCODER_SPEED_STEP;
        } else {
            // Encoder_Count = ENCODER_MAX_SPEED / ENCODER_SPEED_STEP;
        }
    }
}
