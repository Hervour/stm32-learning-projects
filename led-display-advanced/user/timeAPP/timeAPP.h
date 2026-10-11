#ifndef __TIMEAPP_H__
#define __TIMEAPP_H__

#include "common.h"
#include "tim.h"
//#include "led.h"

///////////////////////////////////////////////////////////////////////
extern FunctionalState  Read_Key_Enable;
 
#define TIM_MAX_CLK     168000000   // TIM1 TIM8-TIM11的最大频率值 为168MHz
#define TIM_OTH_CLK     84000000    // TIM2-TIM7,TIM2-TIM14 的最大频率值 为 84MHz

#define TIMx_CHN1_FLAG  0x01  
#define TIMx_CHN2_FLAG  0x02
#define TIMx_CHN3_FLAG  0x04
#define TIMx_CHN4_FLAG  0x08

///////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void My_TIMx_Init(TIM_HandleTypeDef *, uint16_t , uint32_t );
void Set_TIMx_Duty(TIM_HandleTypeDef *, uint32_t , double );
void Set_TIMx_Freq_Duty(TIM_HandleTypeDef *, int , uint32_t , double );
void Set_TIMx_Freq(TIM_HandleTypeDef *_htim, int _freq);
void Set_TIMx_TO_DAC_Freq(TIM_HandleTypeDef *_htim, int _freq, uint32_t n_dot_dac);

void TIMx_CHNx_PWM_Out(TIM_HandleTypeDef *, uint32_t , int , double , TIM_HandleTypeDef *, int );

///////////////////////////////////////////////////////////////////////

#endif
