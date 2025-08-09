#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "Delay.h"

void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

void Buzzer_On(void)
{
    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}

void Buzzer_Off(void)
{
    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

void Buzzer_Turn(void)
{
    GPIO_ReadOutputDataBit(BUZZER_PORT, BUZZER_PIN) == 1 ? Buzzer_On() : Buzzer_Off();
}

void Buzzer_ms(uint8_t ms)
{
    Buzzer_On();
    Delay_ms(ms);
    Buzzer_Off();
}
