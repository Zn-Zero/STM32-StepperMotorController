// 传感器 PA7
#include "stm32f10x.h" // Device header
#include "peripheral_define.h"

void Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 传感器输入 PA7 - 模拟输入
    GPIO_InitStructure.GPIO_Pin = SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(SENSOR_PORT, &GPIO_InitStructure);

    // 使能ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    // ADC时钟分频 (最大14MHz)
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_InitTypeDef ADC_InitStructure;
    // ADC配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // 单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // 转换通道数量
    ADC_Init(SENSOR_ADC, &ADC_InitStructure);

    // 配置ADC通道转换顺序和采样时间
    ADC_RegularChannelConfig(SENSOR_ADC, SENSOR_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);

    // 使能ADC
    ADC_Cmd(SENSOR_ADC, ENABLE);

    // ADC校准
    ADC_ResetCalibration(SENSOR_ADC);
    while (ADC_GetResetCalibrationStatus(SENSOR_ADC));

    ADC_StartCalibration(SENSOR_ADC);
    while (ADC_GetCalibrationStatus(SENSOR_ADC));

    // 开始ADC转换
    ADC_SoftwareStartConvCmd(SENSOR_ADC, ENABLE);
}

// 读取传感器值 (0-4095对应0-3.3V)
uint16_t Sensor_ReadValue(void)
{
    // 等待ADC转换完成
    while (!ADC_GetFlagStatus(SENSOR_ADC, ADC_FLAG_EOC));
    return ADC_GetConversionValue(SENSOR_ADC);
}
