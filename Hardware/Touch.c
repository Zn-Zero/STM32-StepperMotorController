// 触摸屏 PB10 PB11
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

void Touch_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 触摸屏I2C引脚初始化 - 复用开漏输出
    GPIO_InitStructure.GPIO_Pin = TOUCH_SCL_PIN | TOUCH_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 使能I2C时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    I2C_InitTypeDef I2C_InitStructure;
    // I2C配置
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; // 主机模式不需要自己的地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000; // 400kHz高速模式
    I2C_Init(TOUCH_I2C, &I2C_InitStructure);

    // 使能I2C
    I2C_Cmd(TOUCH_I2C, ENABLE);
}
