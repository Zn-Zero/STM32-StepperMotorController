#include "stm32f10x.h" // Device header
#include "peripheral_define.h"
#include "Delay.h"
#include "Buzzer.h"
#include "OLED.h"

#define MAX_FREQ MAX_SPEED / 60 / SCREW_LEAD *ROUND_STEP // 最大频率10kHz -> 600mm/min
#define MIN_FREQ MIN_SPEED / 60 / SCREW_LEAD *ROUND_STEP // 最小频率0Hz -> 600mm/min

MotorDir MOTOR_DIR = DIR_CW;
MotorState MOTOR_ENA = MOTOR_STOP;

// 当前ARR值
uint16_t current_arr = 0;
// 当前设置的pwm频率
uint16_t set_fre = INIT_FRE;
// 当前设置速度
uint16_t set_speed = INIT_SPEED;
// 当前正在运行的速度
uint16_t current_speed = 0;

uint16_t spd_preset[] = {60, 300, 600};
uint8_t spd_idx = 0;

// 电机PWM定时器初始化
void Motor_Init(uint32_t speed)
{
    // 使能定时器时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // --- 配置方向引脚 PA9 ---
    GPIO_InitStructure.GPIO_Pin = MOTOR_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_DIR_PORT, &GPIO_InitStructure);

    // --- 配置PWM引脚 PA8 ---
    GPIO_InitStructure.GPIO_Pin = MOTOR_PUL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PUL_PORT, &GPIO_InitStructure);

    // --- 配置刹车引脚 PB12 ---
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 指定定时器使用内部时钟(72MHz)
    // 可以不写，因为上电后默认使用内部时钟
    TIM_InternalClockConfig(MOTOR_PUL_TIM);
    
    // 限制速度范围
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    if (speed < MIN_SPEED)
        speed = MIN_SPEED;

    // 计算预分频器和自动重装载值
    uint16_t prescaler = 0;
    uint32_t arr_value;
    double temp, frequency;

    frequency = speed / 60.0 / SCREW_LEAD * ROUND_STEP;

    // 计算并四舍五入取整
    temp = (double)(TIMER_CLOCK / (frequency * (prescaler + 1)) - 1 + 0.5);
    arr_value = (uint32_t)temp;

    // 确保ARR不超过最大值
    while (arr_value > 0xFFFF)
    {
        prescaler++;
        temp = (double)(TIMER_CLOCK / (frequency * (prescaler + 1)) - 1 + 0.5);
        arr_value = (uint32_t)temp;
    }

    current_arr = arr_value; // 保存当前ARR值

    OLED_ShowNum(4, 1, prescaler, 5);
    OLED_ShowNum(4, 7, arr_value, 5);

    // 配置定时器时基
    // 定义时基初始化参数结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    // 配置滤波采样周期的时钟分频。分频越多，信号越稳定，但延迟越大
    // [TIM_CKD_DIV1 -> 1分频(不分频) | TIM_CKD_DIV2 -> 2分频 | TIM_CKD_DIV4 -> 4分频]
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // 计数模式：向上计数 | 向下计数 | 对称计数
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 计数器 (需要-1)
    TIM_TimeBaseInitStructure.TIM_Period = current_arr; // ARR(Auto Reload Register) -> 自动重装载寄存器

    // 预分频器 (需要-1)
    TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler; // PSC(Prescaler) —> 预分频器

    // 重复计数器，仅限高级定时器(TIM1)。不使用时给0
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_PUL_TIM, &TIM_TimeBaseInitStructure);

    // 清除由于初始化时被立即执行的影响
    TIM_ClearFlag(MOTOR_PUL_TIM, TIM_FLAG_Update);

    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 并非所有参数都需要使用，未使用的参数需要有默认值
    TIM_OCStructInit(&TIM_OCInitStructure);

    // 配置PWM模式，固定50%占空比
    // 输出模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 设置输出比较的极性
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // 设置CCR(Capture/Compare Register) —> 捕获 / 比较寄存器
    TIM_OCInitStructure.TIM_Pulse = current_arr / 2; // 50%占空比
    // 设置输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    /*-----------TIM1高级定时器所需配置---------------*/
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; // 关闭互补输出
    /*----------------------------------------------*/

    TIM_OC1Init(MOTOR_PUL_TIM, &TIM_OCInitStructure);
    

    // 使能预装载
    TIM_OC1PreloadConfig(MOTOR_PUL_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(MOTOR_PUL_TIM, ENABLE);

    /*-----------TIM1高级定时器刹车功能配置---------------*/
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    TIM_BDTRStructInit(&TIM_BDTRInitStructure);

    // 核心修改在这里
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;                      // 使能刹车
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;         // 低电平有效
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; // 刹车后不自动恢复

    TIM_BDTRConfig(MOTOR_PUL_TIM, &TIM_BDTRInitStructure);
    /*---------------------------------------------------*/

    // OLED_ShowString(4,1,"DEBUG");
    // 高级定时器必须使能主输出，否则无PWM输出
    TIM_CtrlPWMOutputs(MOTOR_PUL_TIM, ENABLE);
}

// 重新设置PWM频率（控制步进电机速度）
void Motor_SetSpeed(uint32_t speed)
{
    if (set_speed == speed)
    {
        return;
    }
    

    // 限制速度范围
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    if (speed < MIN_SPEED)
        speed = MIN_SPEED;

    // 计算预分频器和自动重装载值
    uint16_t prescaler = 0;
    uint32_t arr_value;
    double temp, frequency;

    frequency = speed / 60.0 / SCREW_LEAD * ROUND_STEP;

    // 计算并四舍五入取整
    temp = (double)(TIMER_CLOCK / (frequency * (prescaler + 1)) - 1 + 0.5);
    arr_value = (uint32_t)temp;

    // 确保ARR不超过最大值
    while (arr_value > 0xFFFF)
    {
        prescaler++;
        temp = (double)(TIMER_CLOCK / (frequency * (prescaler + 1)) - 1 + 0.5);
        arr_value = (uint32_t)temp;
    }

    current_arr = arr_value; // 保存当前ARR值

    // 更新定时器参数
    MOTOR_PUL_TIM->PSC = prescaler;
    MOTOR_PUL_TIM->ARR = arr_value;
    MOTOR_PUL_TIM->CCR1 = arr_value / 2; // 保持50%占空比

    set_speed = speed;

    // OLED_ShowNum(4, 7, arr_value, 5);
}

uint32_t step = 1;     // 步长（），可减小以更平滑
uint32_t delay_ms = 20; // 步间延时（ms），增大以降低加速度

// 梯形加速 需要修改为根据速度
void Trapezoidal_Acceleration(uint32_t target)
{
    // 若当前频率已高于目标，直接跳到目标（或报错）
    if (current_speed >= target)
    {
        Motor_SetSpeed(target);
        return;
    }

    for (uint32_t speed = current_speed; speed < target;)
    {
        // 最后一步可能不足一个步长，直接跳到目标
        uint32_t next_speed = (speed + step) > target ? target : (speed + step);
        Motor_SetSpeed(next_speed);
        speed = next_speed;

        Delay_ms(delay_ms); // 用毫秒级延时降低加速度
    }

    current_speed = target;
}

// 梯形减速
void Trapezoidal_Deceleration(uint32_t target)
{
    // 若当前频率已低于目标，直接跳到目标
    if (current_speed <= target)
    {
        Motor_SetSpeed(target);
        return;
    }

    for (uint32_t speed = current_speed; speed > target;)
    {
        // 最后一步可能不足一个步长，直接跳到目标
        uint32_t next_speed = (speed - step) < target ? target : (speed - step);
        Motor_SetSpeed(next_speed);
        speed = next_speed;

        Delay_ms(delay_ms); // 用毫秒级延时降低加速度
    }

    current_speed = target;
}

// 缓慢加速
void Motor_Start(void)
{
    TIM_Cmd(MOTOR_PUL_TIM, ENABLE);
    Trapezoidal_Acceleration(set_speed);
    Buzzer_beep_ms(100);
    LED_ON();
    MOTOR_ENA = MOTOR_RUN;
}

// 立即启动
void Motor_Start_Instantly(void)
{
    // TIM_CtrlPWMOutputs(MOTOR_PUL_TIM, ENABLE);
    TIM_Cmd(MOTOR_PUL_TIM, ENABLE);
    Buzzer_beep_ms(100);
    LED_ON();
    MOTOR_ENA = MOTOR_RUN;
}

// 缓慢减速
void Motor_Stop(void)
{
    Trapezoidal_Deceleration(set_speed);
    TIM_Cmd(MOTOR_PUL_TIM, DISABLE);
    Buzzer_beep_ms(100);
    LED_OFF();
    MOTOR_ENA = MOTOR_STOP;
}

// 立即停止
void Motor_Stop_Instantly(void)
{
    // TIM_CtrlPWMOutputs(MOTOR_PUL_TIM, DISABLE);
    TIM_Cmd(MOTOR_PUL_TIM, DISABLE);
    Buzzer_beep_ms(100);
    LED_OFF();
    MOTOR_ENA = MOTOR_STOP;
}

// 变速
// void Motor_Change_Speed(uint32_t speed)
// {
    
// }
