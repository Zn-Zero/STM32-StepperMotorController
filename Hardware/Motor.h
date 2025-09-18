#ifndef __MOTOR_H
#define __MOTOR_H

/*初始化电机相关端口*/
void Motor_Init(uint32_t frequency);
/*重新设置PWM频率（控制步进电机速度）*/
void Motor_SetFrequency(uint32_t frequency);
/*梯形加速*/
void Trapezoidal_Acceleration(uint32_t current, uint32_t target);
/*梯形减速*/
void Trapezoidal_Deceleration(uint32_t current, uint32_t target);
#endif
