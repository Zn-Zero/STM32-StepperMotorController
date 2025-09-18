#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"

int main(void) {

    // 初始化GPIO
    GPIO_InitAll();

    // 初始化外部中断
    EXTI_InitAll();

    // 初始化编码器定时器
    Encoder_TIM_Init();

    // 初始化电机PWM定时器 (50kHz: 72MHz / (71+1)/(19+1) = 50kHz)
    MotorPWM_TIM_Init(20 - 1, 72 - 1); // arr psc

    while (1) {

    }
}

