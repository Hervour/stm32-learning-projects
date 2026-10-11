#include "timeAPP.h"
#include "led.h"

// 所有定时器的计数溢出中断回调函数
FunctionalState Read_Key_Enable;        // 读取按键使能
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        LED1_TOGGLE();
    }
    
    if(htim->Instance == TIM7)
    {
        PG10_TOGGLE();
    }
}

///////////////////////////////////////////////////////////////////
// 以下调用的定时器，必须在STM32CubeMX中都完成过初始化
// 调整TIMx的计数参数，占空比等
void My_TIMx_Init(TIM_HandleTypeDef *_htim, uint16_t _prescaler, uint32_t _period)
{   
    _htim->Init.Prescaler = _prescaler;
    
    if( (_htim->Instance == TIM2) || (_htim->Instance == TIM5) )
    {   // STM32F407的 TIM2 TIM5 的 Period(ARR寄存器) 为32 位寄存器
        _htim->Init.Period = _period;
    }
    else
    {  // STM32F407 除了 TIM2 TIM5 的其他定时器 的 Period(ARR寄存器) 为 16 位寄存器
        _htim->Init.Period = (uint16_t)_period;
    }

    HAL_TIM_Base_Init(_htim); // 初始化PSR、ARR这2个寄存器
}

// 按频率值设置TIMx，计数器X的计数溢出频率
void  Set_TIMx_Freq(TIM_HandleTypeDef *_htim, int _freq)
{   
    double      ftmp, fTimx_clk;  
    uint32_t    _period;
    uint16_t    _prescaler, i;
    
    // 给各个定时器工作的频率 赋值
    if( (_htim->Instance == TIM1) || (_htim->Instance == TIM8)  || \
        (_htim->Instance == TIM9) || (_htim->Instance == TIM10) || \
        (_htim->Instance == TIM11)
      )
    {
        fTimx_clk = TIM_MAX_CLK;  // 168M
    }
    else
    {
        fTimx_clk = TIM_OTH_CLK;  // 84M
    }
    
    // 预分频值从0开始，根据频率值，计算period，直到获得位长符合要求的 period
    _prescaler = 0; // 预分频值从0开始
    if( (_htim->Instance == TIM2) || (_htim->Instance == TIM5) )
    {   // STM32F407的 TIM2 TIM5 的 Period(ARR寄存器) 为32 位寄存器
        ftmp    = fTimx_clk / (_prescaler + 1) / (double)_freq - 1;
        _period = (uint32_t)ftmp;
    }
    else
    {  // STM32F407 除了 TIM2 TIM5 的其他定时器 的 Period(ARR寄存器) 为 16 位寄存器
        for (i=0; i<=65535; i++)
        {
            ftmp    = fTimx_clk / (_prescaler + 1) / (double)_freq - 1;
            _period = (uint32_t)ftmp;
            if(_period < 65536)
            {
                break;  // _period符合位长的要求了，退出循环
            }
            else
            {
                _prescaler++; // _period数值大于65535，超过了2个字节的限制，预分频器+1，重新计算_period 直到符合要求
            }
        }
    }
    _htim->Instance->CNT = 0; // 定时器器初始计数值=0
    My_TIMx_Init(_htim, _prescaler, _period);  // 初始化定时器
}

// 按占空比设置TIMx
void  Set_TIMx_Duty(TIM_HandleTypeDef *_htim, uint32_t _chnx, double _duty)
{
    uint32_t    pulse;
    double      ftmp;
    
    ftmp  = _duty * (1 + _htim->Init.Period);
    if( (_htim->Instance == TIM2) || (_htim->Instance == TIM5) )
    {   // STM32F407的 TIM2 TIM5 的 Period(ARR寄存器) 为 32 位寄存器，Pulse(CCRx)也是 32 位寄存器
        pulse = (uint32_t)ftmp;
    }
    else
    {  // STM32F407 除了 TIM2 TIM5 的其他定时器 的 Period(ARR寄存器) 为 16 位寄存器，Pulse(CCRx)也是 16 位寄存器
        pulse = (uint16_t)ftmp;
    }
    
    switch(_chnx)      
    {
        case  TIM_CHANNEL_1: { _htim->Instance->CCR1 = pulse;  break; } 
        case  TIM_CHANNEL_2: { _htim->Instance->CCR2 = pulse;  break; }
        case  TIM_CHANNEL_3: { _htim->Instance->CCR3 = pulse;  break; }
        case  TIM_CHANNEL_4: { _htim->Instance->CCR4 = pulse;  break; }
    }
}


void Set_TIMx_Freq_Duty(TIM_HandleTypeDef *_htim, int _freq, uint32_t _chn, double _duty)
{
    Set_TIMx_Freq(_htim, _freq);
    Set_TIMx_Duty(_htim, _chn, _duty);
}

///////////////////////////////////////////////////////////////////
// 设置DAC输出正弦波的频率
void  Set_TIMx_TO_DAC_Freq(TIM_HandleTypeDef *_htim, int _freq, uint32_t n_dot_dac)
{
    int  _freq_timx;           // 定时器X的计数溢出频率
    _freq_timx = _freq * n_dot_dac; // 定时器X的计数溢出频率 = DAC输出正弦波的频率 * 一个正弦波周期的取样点数
    Set_TIMx_Freq(_htim, _freq_timx);// 设置定时器X的计数溢出频率
}
/////////////////////////////////////////////////
// 设置主从定时器输出设定频率和占空比的设定数量的脉冲波
void  TIMx_CHNx_PWM_Out(TIM_HandleTypeDef *master_tim, uint32_t _chnxflag, int _freq, double _duty, TIM_HandleTypeDef *slave_tim, int _pulse_numb)
{
    // 设置从定时器，用于控制脉冲数量
    __HAL_TIM_SET_AUTORELOAD(slave_tim, (_pulse_numb - 1)); // 从定时器对主定时器 输出的脉冲计数，当输出满了 _pulse_numb 个脉冲时,从定时器产生中断
    __HAL_TIM_CLEAR_IT(slave_tim, TIM_IT_UPDATE);           // 计数之前要清掉该标记位，否则会产生一个中断
    HAL_TIM_Base_Start_IT(slave_tim);                       // 使能从定时器计数，并产生中断
    
    // 设置主定时器，控制输出脉冲的频率和占空比
    Set_TIMx_Freq(master_tim, _freq); // 必须先设置频率，获得period的值，然后根据period的值，计算占空比
    // 设置各个通道的占空比，并允许计数输出
    if(_chnxflag & TIMx_CHN1_FLAG)    // 如果允许 TIM_CHANNEL_1 通道输出PWM波
    {
        Set_TIMx_Duty(master_tim, TIM_CHANNEL_1, _duty); // 设置通道1的占空比
        HAL_TIM_PWM_Start(master_tim, TIM_CHANNEL_1);    // 使能主定时器timx.ch1 输出脉冲信号
    }
    if(_chnxflag & TIMx_CHN2_FLAG)    // 如果允许 TIM_CHANNEL_2 通道输出PWM波
    {
        Set_TIMx_Duty(master_tim, TIM_CHANNEL_2, _duty); // 设置通道2的占空比
        HAL_TIM_PWM_Start(master_tim, TIM_CHANNEL_2);    // 使能主定时器timx.ch2 输出脉冲信号
    }
    if(_chnxflag & TIMx_CHN3_FLAG)    // 如果允许 TIM_CHANNEL_3 通道输出PWM波
    {
        Set_TIMx_Duty(master_tim, TIM_CHANNEL_3, _duty); // 设置通道3的占空比
        HAL_TIM_PWM_Start(master_tim, TIM_CHANNEL_3);    // 使能主定时器timx.ch3 输出脉冲信号
    }
    if(_chnxflag & TIMx_CHN4_FLAG)    // 如果允许 TIM_CHANNEL_4 通道输出PWM波
    {
        Set_TIMx_Duty(master_tim, TIM_CHANNEL_4, _duty); // 设置通道4的占空比
        HAL_TIM_PWM_Start(master_tim, TIM_CHANNEL_4);    // 使能主定时器timx.ch4 输出脉冲信号
    }
}

