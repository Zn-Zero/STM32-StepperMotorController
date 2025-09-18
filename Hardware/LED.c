// 指示灯 PB1
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // LED PB1 - 推挽输出
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    LED_OFF();
}
