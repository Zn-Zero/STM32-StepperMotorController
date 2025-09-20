#include "stm32f10x.h"                  // Device header
#include "peripheral_define.h"
#include "Buzzer.h"
#include "Encoder.h"
#include "Keys.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h" // 临时
// #include "Sensor.h"
// #include "Touch.h"

// 所有外设初始化
void Peripheral_InitAll(void)
{
    // 使能所有用到的GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 临时用法(模拟I2C)
    OLED_Init();

    Buzzer_Init();

    Encoder_AB_Init();
    Encoder_C_Init();

    Keys_Init();

    LED_Init();

    Motor_Init(INIT_SPEED);
}
