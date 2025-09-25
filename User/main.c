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

