#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void StartPulse(void);
void StopPulse(void);
void SetPulsePeriod(uint32_t us);
void Motor_Puls(void);
void Motor_SetDir(MotorDir dir);

#endif
