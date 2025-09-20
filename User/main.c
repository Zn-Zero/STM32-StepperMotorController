#include "stm32f10x.h"                  // Device header
#include "peripheral_init.h"
#include "peripheral_define.h"
#include "Buzzer.h"
#include "Encoder.h"
#include "Keys.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h"

// 电机方向
extern MotorDir MOTOR_DIR;
// 电机状态
extern MotorState MOTOR_ENA;

// 储当前设置的pwm频率
extern uint16_t set_fre;
// 当前设置速度
extern uint16_t set_speed;
// 当前正在运行的频率
extern uint16_t current_fre;

int main(void) {

    // 初始化
    Peripheral_InitAll();

    while (1)
    {
        OLED_ShowString(1, 1, "ENA: ");
        if (MOTOR_ENA == MOTOR_RUN)
        {
            OLED_ShowString(1, 6, "RUN ");
        }
        else
        {
            OLED_ShowString(1, 6, "STOP");
        }
        OLED_ShowString(2, 1, "DIR: CW");
        OLED_ShowString(3, 1, "SPD: ");
        OLED_ShowNum(3, 6, set_speed, 3);
        OLED_ShowString(3, 9, "mm/min");

        // OLED_ShowNum(4, 1, 21231, 9);
    }
}

