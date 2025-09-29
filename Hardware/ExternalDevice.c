// 外接设备（推拉力计）
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Motor.h"
#include "OLED.h"
#include "Delay.h"

void PD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 步骤 2: 配置GPIO
    // 配置PB10为浮空输入或上拉输入
    // 如果外部信号源在空闲时有明确的电平，可以用浮空输入
    // 如果信号源可能悬空，建议使用上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 步骤 3: 配置AFIO，将PB10连接到EXTI10中断线
    // 对于GPIOB的Pin 10，对应的EXTI线是EXTI10
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);

    EXTI_InitTypeDef EXTI_InitStructure;
    // 步骤 4: 配置EXTI (外部中断/事件控制器)
    // 选择要配置的EXTI线
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    // 使能该中断线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    // 模式为中断模式 (区别于事件模式)
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    // 触发方式为上升沿触发
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    // 应用配置
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    // 步骤 5: 配置NVIC (嵌套向量中断控制器)
    // EXTI线 10-15 共用一个中断通道: EXTI15_10_IRQn
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    // 中断优先级分组需要先设置，这里假设已在别处设置好
    // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 优先级值越小，优先级越高
    // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    // 使能该中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // 应用配置
    NVIC_Init(&NVIC_InitStructure);
}
