#ifndef PERIPHERAL_INIT_H
#define PERIPHERAL_INIT_H

// 所有外设初始化函数
void Peripheral_InitAll(void);

// 各模块单独初始化函数
void GPIO_InitAll(void);
void EXTI_InitAll(void);
void Encoder_TIM_Init(void);
void MotorPWM_TIM_Init(uint16_t arr, uint16_t psc);
void ADC_InitSensor(void);
void I2C_InitTouchScreen(void);

// 外设操作函数
void Motor_SetPWM(uint16_t duty);
int16_t Encoder_GetCount(void);
uint16_t Sensor_ReadValue(void);

#endif /* PERIPHERAL_INIT_H */
