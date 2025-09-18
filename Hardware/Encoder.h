#ifndef __ENCODER_H
#define __ENCODER_H

/*编码器AB相初始化*/
void Encoder_AB_Init(void);
/*编码器C相初始化*/
void Encoder_C_Init(void);
/*读取编码器值*/
int16_t Encoder_GetCount(void);

#endif
