// 蜂鸣器 PB0
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Delay.h"

void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 蜂鸣器 PB0 - 推挽输出
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    BUZZER_OFF();
}

void Buzzer_beep_ms(uint8_t ms)
{
    if (ms > 0)
    {
        BUZZER_ON();
        Delay_ms(ms);
        BUZZER_OFF();
    }
}
