#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "OLED.h"
#include "Encoder.h"
#include "Buzzer.h"
#include "Button.h"
#include "Motor.h"

uint32_t Encoder_Count = ENCODER_INIT_SPEED;
uint32_t Motor_Speed = (1*1000*1000 * SCREW_LEAD * 60)/(2 * ENCODER_INIT_SPEED * ROUND_STEP);
uint32_t Motor_Step = 0;
MotorDir Motor_DIR = DIR_CW;
MotorState Motor_ENA = MOTOR_STOP;

void Init_All(void) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 使能定时器2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
    // 配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 2位抢占优先级, 2位响应优先级

    OLED_Init();
    Button_Init();
    Encoder_Init();
	  Buzzer_Init();
    Motor_Init();

    OLED_ShowNum(1,1,Encoder_Count,16);
    OLED_ShowNum(4,1,Motor_Speed,16);
}
