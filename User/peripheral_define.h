#ifndef PERIPHERAL_DEFINE_H
#define PERIPHERAL_DEFINE_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

/*-----------------------按键定义---------------------------*/
// 启停
#define KEY_START_STOP_PORT    GPIOA
#define KEY_START_STOP_PIN     GPIO_Pin_2
#define KEY_START_STOP_EXTI    EXTI_Line2
#define KEY_START_STOP_IRQ     EXTI2_IRQn

// 转向
#define KEY_DIR_PORT           GPIOA
#define KEY_DIR_PIN            GPIO_Pin_3
#define KEY_DIR_EXTI           EXTI_Line3
#define KEY_DIR_IRQ            EXTI3_IRQn

//三档变速
#define KEY_SPEED_LEVEL_PORT   GPIOA
#define KEY_SPEED_LEVEL_PIN    GPIO_Pin_4
#define KEY_SPEED_LEVEL_EXTI   EXTI_Line4
#define KEY_SPEED_LEVEL_IRQ    EXTI4_IRQn

// 上限位按钮
#define KEY_UP_LIMIT_PORT      GPIOA
#define KEY_UP_LIMIT_PIN       GPIO_Pin_5
#define KEY_UP_LIMIT_EXTI      EXTI_Line5
#define KEY_UP_LIMIT_IRQ       EXTI9_5_IRQn

// 下限位按钮
#define KEY_DOWN_LIMIT_PORT    GPIOA
#define KEY_DOWN_LIMIT_PIN     GPIO_Pin_6
#define KEY_DOWN_LIMIT_EXTI    EXTI_Line6
#define KEY_DOWN_LIMIT_IRQ     EXTI9_5_IRQn

/*-----------------------编码器定义---------------------------*/
#define ENCODER_PORT           GPIOA
#define ENCODER_A_PIN          GPIO_Pin_0
#define ENCODER_B_PIN          GPIO_Pin_1
#define ENCODER_TIM            TIM2

/*-----------------------电机驱动定义---------------------------*/
#define MOTOR_PUL_PORT         GPIOA
#define MOTOR_PUL_PIN          GPIO_Pin_8
#define MOTOR_PUL_TIM          TIM1
 
// #define MOTOR_PUL_PORT GPIOB
// #define MOTOR_PUL_PIN GPIO_Pin_6
// #define MOTOR_PUL_TIM TIM4

#define MOTOR_DIR_PORT         GPIOA
#define MOTOR_DIR_PIN          GPIO_Pin_9

/*-----------------------传感器定义---------------------------*/
#define SENSOR_PORT            GPIOA
#define SENSOR_PIN             GPIO_Pin_7
#define SENSOR_ADC             ADC1
#define SENSOR_ADC_CHANNEL     ADC_Channel_7

/*-----------------------其他外设定义---------------------------*/
// 蜂鸣器定义
#define BUZZER_PORT            GPIOB
#define BUZZER_PIN             GPIO_Pin_0

// LED定义
#define LED_PORT               GPIOB
#define LED_PIN                GPIO_Pin_1

// 触摸屏I2C定义
#define TOUCH_SCL_PORT         GPIOB
#define TOUCH_SCL_PIN          GPIO_Pin_10 // I2C2_SCL
#define TOUCH_SDA_PORT         GPIOB
#define TOUCH_SDA_PIN          GPIO_Pin_11 // I2C2_SDA
#define TOUCH_I2C              I2C2

/*-------------------------外设操作宏定义------------------------*/
// 蜂鸣器操作
#define BUZZER_OFF()           GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_ON()            GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_TOGGLE()        BUZZER_PORT->ODR ^= BUZZER_PIN
// LED操作
#define LED_OFF()              GPIO_SetBits(LED_PORT, LED_PIN)
#define LED_ON()               GPIO_ResetBits(LED_PORT, LED_PIN)
#define LED_TOGGLE()           LED_PORT->ODR ^= LED_PIN
// 电机转向
#define MOTOR_DIR_FORWARD()    GPIO_SetBits(MOTOR_DIR_PORT, MOTOR_DIR_PIN)
#define MOTOR_DIR_REVERSE()    GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_DIR_PIN)
#define MOTOR_DIR_TOGGLE()     MOTOR_DIR_PORT->ODR ^= MOTOR_DIR_PIN

/*-------------------------全局变量定义------------------------*/
// 运行参数
// 电机方向枚举
typedef enum {
    DIR_CW = 0, // 顺时针
    DIR_CCW = 1 // 逆时针
} MotorDir;

// 电机状态枚举
typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_RUN = 1
} MotorState;

extern MotorDir MOTOR_DIR;
extern MotorState MOTOR_ENA;

extern uint16_t set_speed;     // 当前设置速度
extern uint16_t current_speed; // 当前正在运行的速度

extern uint16_t spd_preset[];
extern uint8_t spd_idx;

// 参数宏定义
#define MIN_SPEED              1                          // 最慢 mm/min INIT_SPEE -> 0
#define MAX_SPEED              600                        // 最快 mm/min INIT_SPEE -> 10000
#define ROUND_STEP             5000                       // 一圈步数

// 以下参数互补，选一使用
#define INIT_FRE               1000                       // 初始PWM频率      (换算速度 -> (INIT_FRE / ROUND_STEP) * SCREW_LEAD * 60)
#define INIT_SPEED             60                         // 初始速度 mm/min  (换算频率 -> INIT_SPEED / 60 / SCREW_LEAD * ROUND_STEP)

#define SCREW_LEAD             5                          // 丝杆导程


#define TIMER_CLOCK            72000000                   // 时钟频率 72MHz

#endif /* PERIPHERAL_DEFINE_H */
