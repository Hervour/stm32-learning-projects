#ifndef __ADCAPP_H__
#define __ADCAPP_H__
#include "common.h"
#include "adc.h"

////////////////////////////////////////////////////////
#define     SAMPLE_FREQ         40000   // 采样频率
#define     ADC_MAX_SIZE        4000    // 一次采集的样点数

#define     ADC_MAX_VALUE       4096    // ADC 的最大值
#define     ADC_TRIG_N          32      // 将4096等分成32份
#define     TRIG_LEVEL_DIV2     ((uint16_t)(ADC_MAX_VALUE / 2 / ADC_TRIG_N ) ) // 1/2的分层电平, 四舍五入用

extern  uint16_t  ADC_TRIG_LEVEL[ADC_TRIG_N]; // 把ADC的均值等分为 N 份, 自动获取固定的触发电平


////////////////////////////////////////////////////////
extern uint16_t  ADC1_Value[ADC_MAX_SIZE];
extern double    ADC_Vdc_Volt;
extern double    ADC_Vpp_Volt;
extern int       ADC_Sin_Freq;

////////////////////////////////////////////////////////

#endif
