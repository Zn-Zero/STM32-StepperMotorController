#include "stm32f10x.h"                  // Device header
#include "Pin_def.h"
#include "ExternParam_def.h"
#include "Init.h"
#include "Motor.h"
#include "Encoder.h"
#include "OLED.h"
#include "Delay.h"
#include "Button.h"

int main(void) {
   Init_All();
    
    while (1) {

    }
}

// 外部中断0处理函数(启停按钮)
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        Delay_ms(20);  // 消抖
        if (GPIO_ReadInputDataBit(BTN_START_STOP_PORT, BTN_START_STOP_PIN) == 0) {
            KeyEvent_BtnStartStop();
        }
        
        EXTI_ClearITPendingBit(EXTI_Line0);  // 清除中断标志
    }
}

// 外部中断2处理函数(方向按钮)
void EXTI2_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line2) == SET) {
        Delay_ms(20);  // 消抖
        if (GPIO_ReadInputDataBit(BTN_DIRECTION_PORT, BTN_DIRECTION_PIN) == 0) {
            KeyEvent_BtnDirection();
        }
        
        EXTI_ClearITPendingBit(EXTI_Line2);  // 清除中断标志
    }
}

// 外部中断15-10处理函数(编码器)
void EXTI15_10_IRQHandler(void) {

    if (EXTI_GetITStatus(EXTI_Line14) == SET) {
        ENC_A_IRQHandler();
        EXTI_ClearITPendingBit(EXTI_Line14);  // 清除中断标志
    }

    // if (EXTI_GetITStatus(EXTI_Line12) == SET) {
    //     ENC_B_IRQHandler();
    //     EXTI_ClearITPendingBit(EXTI_Line12);  // 清除中断标志
    // }
}

/**
 * @brief 定时器2中断，产生一个脉冲信号驱动电机
 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        // if (Motor_Step < ROUND_STEP)
        // {
            // Motor_Step++;
            Motor_Puls();
        // }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
