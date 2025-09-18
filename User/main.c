#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"

int main(void) {

    // 使能所有用到的GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // 初始化
    Peripheral_InitAll();

    while (1)
    {
    }
}

