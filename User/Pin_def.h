#ifndef __PIN_DEF_H
#define __PIN_DEF_H

#include "stm32f10x.h"                  // Device header

// 按钮引脚定义
#define BTN_START_STOP_PIN    GPIO_Pin_0
#define BTN_START_STOP_PORT   GPIOA
#define BTN_DIRECTION_PIN     GPIO_Pin_2
#define BTN_DIRECTION_PORT    GPIOA

// 驱动器控制引脚定义
#define PUL_PIN               GPIO_Pin_5
#define PUL_PORT              GPIOA
#define DIR_PIN               GPIO_Pin_6
#define DIR_PORT              GPIOA
#define ENA_PIN               GPIO_Pin_7
#define ENA_PORT              GPIOA

// 编码器引脚定义
#define ENC_A_PIN             GPIO_Pin_14
#define ENC_A_PORT            GPIOB
#define ENC_B_PIN             GPIO_Pin_12
#define ENC_B_PORT            GPIOB
#define ENC_C_PIN             GPIO_Pin_13
#define ENC_C_PORT            GPIOB

// 蜂鸣器引脚定义
#define BUZZER_PIN            GPIO_Pin_15
#define BUZZER_PORT           GPIOB

// 电机方向枚举
typedef enum {
    DIR_CW = 0,   // 顺时针
    DIR_CCW = 1   // 逆时针
} MotorDir;

// 电机状态枚举
typedef enum {
    MOTOR_STOP = 0,
    MOTOR_RUN = 1
} MotorState;

// 编码器参数宏定义
#define ENCODER_MIN_SPEED     0         // 最慢mm/min
#define ENCODER_MAX_SPEED     600       // 最快mm/min
#define ROUND_STEP            5000      // 一圈步数
#define ENCODER_INIT_SPEED    100       // 初始速度计数
#define SCREW_LEAD            5         // 丝杆导程

#endif /* __PIN_DEF_H */
