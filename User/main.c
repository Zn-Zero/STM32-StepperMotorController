#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"
#include "peripheral_define.h"
#include "Buzzer.h"
#include "Encoder.h"
#include "Keys.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h"

int main(void) {

    // 初始化
    Peripheral_InitAll();

    while (1)
    {
        Encoder_GetSpeed();

        OLED_ShowString(1, 1, "ENA: ");
        if (MOTOR_ENA == MOTOR_RUN)
        {
            OLED_ShowString(1, 6, "RUN ");
        }
        else
        {
            OLED_ShowString(1, 6, "STOP");
        }
        OLED_ShowString(2, 1, "DIR: ");
        if (MOTOR_DIR == DIR_CW)
        {
            OLED_ShowString(2, 6, "CW ");
        }
        else
        {
            OLED_ShowString(2, 6, "CCW");
        }
        
        OLED_ShowString(3, 1, "SPD: ");
        OLED_ShowNum(3, 6, set_speed, 3);
        OLED_ShowString(3, 9, "mm/min");

        // OLED_ShowNum(4, 1, Encoder_GetCount(), 3);
        // OLED_ShowNum(4, 3, spd_preset[spd_idx], 3);

        // OLED_ShowNum(4, 1, 21231, 9);
    }
}

void EXTI2_IRQHandler()
{
    // 电机启停
    if (EXTI_GetITStatus(KEY_START_STOP_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_START_STOP_PORT, KEY_START_STOP_PIN) == 0)
        {
            Delay_ms(10);
        }

        // OLED_ShowString(4, 1, "EXTI2");

        if (MOTOR_ENA == MOTOR_STOP)
        {
            Motor_Start_Instantly();
        }
        else
        {
            Motor_Stop_Instantly();
        }

        EXTI_ClearITPendingBit(KEY_START_STOP_EXTI);
    }
}

void EXTI3_IRQHandler()
{
    if (EXTI_GetITStatus(KEY_DIR_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_DIR_PORT, KEY_DIR_PIN) == 0)
        {
            Delay_ms(10);
        }

        if (MOTOR_DIR == DIR_CW)
        {
            MOTOR_DIR_FORWARD();
            MOTOR_DIR = DIR_CCW;
        }
        else
        {
            MOTOR_DIR_REVERSE();
            MOTOR_DIR = DIR_CW;
        }

        EXTI_ClearITPendingBit(KEY_DIR_EXTI);
    }
}

void EXTI4_IRQHandler()
{
    // 三档定速切换
    if (EXTI_GetITStatus(KEY_SPEED_LEVEL_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_SPEED_LEVEL_PORT, KEY_SPEED_LEVEL_PIN) == 0)
        {
            Delay_ms(10);
        }

        spd_idx = (spd_idx + 1) % 3;

        TIM_SetCounter(ENCODER_TIM, spd_preset[spd_idx]);
        // Motor_SetSpeed(spd_preset[spd_idx]);

        EXTI_ClearITPendingBit(KEY_SPEED_LEVEL_EXTI);
    }
}

void EXTI9_5_IRQHandler()
{
    // 上限位
    if (EXTI_GetITStatus(KEY_UP_LIMIT_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_UP_LIMIT_PORT, KEY_UP_LIMIT_PIN) == 0)
        {
            Delay_ms(10);
            TIM_Cmd(MOTOR_PUL_TIM, DISABLE);
        }

        // 上限位
        if (MOTOR_ENA == MOTOR_RUN)
        {
            MOTOR_DIR_REVERSE();
            MOTOR_DIR = DIR_CW;
        
            TIM_Cmd(MOTOR_PUL_TIM, ENABLE);
        }

        EXTI_ClearITPendingBit(KEY_UP_LIMIT_EXTI);
    }

    // 下限位
    if (EXTI_GetITStatus(KEY_DOWN_LIMIT_EXTI) == SET)
    {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(KEY_DOWN_LIMIT_PORT, KEY_DOWN_LIMIT_PIN) == 0)
        {
            Delay_ms(10);
            TIM_Cmd(MOTOR_PUL_TIM, DISABLE);
        }

        // 下限位
        if (MOTOR_ENA == MOTOR_RUN)
        {
            MOTOR_DIR_FORWARD();
            MOTOR_DIR = DIR_CCW;
        
            TIM_Cmd(MOTOR_PUL_TIM, ENABLE);
        }

        EXTI_ClearITPendingBit(KEY_DOWN_LIMIT_EXTI);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        // 外设信号传入，暂停电机
        Motor_Stop_Instantly();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}
