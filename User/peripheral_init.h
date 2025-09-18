#ifndef PERIPHERAL_INIT_H
#define PERIPHERAL_INIT_H

// 所有外设初始化函数
// void Peripheral_InitAll(void);

// 各模块单独初始化函数
void GPIO_InitAll(void);
void EXTI_InitAll(void);
void Encoder_TIM_Init(void);
// void ADC_InitSensor(void);
// void I2C_InitTouchScreen(void);

#endif /* PERIPHERAL_INIT_H */
