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
    Buzzer_Init();

    Encoder_AB_Init();
    Encoder_C_Init();

    Keys_Init();

    LED_Init();

    Motor_Init(1000);

    // 临时用法(模拟I2C)
    OLED_Init();
}
