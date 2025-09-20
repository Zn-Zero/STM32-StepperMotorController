#ifndef __MOTOR_H
#define __MOTOR_H

/*初始化电机相关端口*/
void Motor_Init(uint32_t speed);

/*重新设置PWM频率（控制步进电机速度）*/
void Motor_SetSpeed(uint32_t speed);

/**
 * @brief   步进电机梯形加速控制
 *
 * @details 通过线性增加PWM频率实现恒定加速度，每次频率调整后保持固定延时，
 *          使电机转速平稳上升，避免因加速过快导致失步。
 *
 * @param[in]  current  起始频率（单位：Hz），需在[MIN_FREQ, MAX_FREQ]范围内
 * @param[in]  target   目标频率（单位：Hz），需大于current，且≤MAX_FREQ（200000）
 *
 * @return  void
 *
 * @note    1. 调用前需确保定时器已初始化且PWM已启动
 *          2. 加速度由步长（100Hz）和延时（10ms）决定，可修改源码调整：
 *             加速度 = 步长 / 延时 = 100Hz / 0.01s = 10000Hz/s
 *          3. 若current ≥ target，函数会直接将频率设为target并退出
 *
 * @warning 目标频率对应速度有上限，不可超过600mm/min，否则会被强制截断为MAX_FREQ，可能导致电机异常
 *
 * @example
 *          // 从初始1000Hz加速到5000Hz
 *          Trapezoidal_Acceleration(1000, 5000);
 */
void Trapezoidal_Acceleration(uint32_t current, uint32_t target);

/**
 * @brief   步进电机梯形减速速控制
 *
 * @details 通过线性增加PWM频率实现恒定加速度，每次频率调整后保持固定延时，
 *          使电机转速平稳下降，避免因加速过快导致失步。
 *
 * @param[in]  current  起始频率（单位：Hz），需在[MIN_FREQ, MAX_FREQ]范围内
 * @param[in]  target   目标频率（单位：Hz），需大于current，且≤MAX_FREQ（200000）
 *
 * @return  void
 *
 * @note    1. 调用前需确保定时器已初始化且PWM已启动
 *          2. 加速度由步长（100Hz）和延时（10ms）决定，可修改源码调整：
 *             加速度 = 步长 / 延时 = 100Hz / 0.01s = 10000Hz/s
 *          3. 若current ≤ target，函数会直接将频率设为target并退出
 *
 * @example
 *          // 从5000Hz减速到0Hz
 *          Trapezoidal_Acceleration(5000, 0);
 */
void Trapezoidal_Deceleration(uint32_t current, uint32_t target);

/*缓慢加速启动*/
void Motor_Start(void);

/*立即启动*/
void Motor_Start_Instantly(void);

/*缓慢减速停止*/
void Motor_Stop(void);

/*立即停止*/
void Motor_Stop_Instantly(void);

#endif
