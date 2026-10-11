#include "dacapp.h"
#include <math.h>

struDacParam DAC_Param;
//////////////////////////////////////////////////////

void DAC_Init(struDacParam *signal)
{
    signal->DAC_Type        = DAC_TYPE_SIN; // DAC输出信号类型为正弦波
    signal->DAC_Freq        = 200;          // Hz
    signal->DAC_Vpp         = 3.0;          // V
    signal->DAC_DC_Voltage  = DC_VOLTAGE;   // V
    signal->DAC_Refresh     = TRUE;         // 允许刷新DAC输出新的信号
}

// 生成一个周期的正弦波
void  Set_DAC_Sine(struDacParam *signal)
{
    double   ampl_level;  // 振幅     的电压等级值
    double   dc_level;    // 直流电平 的电压等级值
    double   ftmp;
    int      i;
        
    ampl_level = signal->DAC_Vpp * 4095 / 3.3 / 2;  //振幅 = 峰峰值/2
    dc_level   = signal->DAC_DC_Voltage  * 4095 / 3.3;      // 直流电平
    
    for (i=0; i<DAC_NOW_SIZE; i++)
    {
        ftmp = ampl_level * sin(2.0 * PI / DAC_NOW_SIZE * i) + dc_level;   // 
        signal->DAC_Value[i] = (uint16_t)ftmp;
    }
}

// 生成一个周期的方波
void  Set_DAC_Square(struDacParam *signal)
{
    double   ampl;  // 振幅     的电压等级值
    double   dc;    // 直流电平 的电压等级值
    uint16_t ampl_level,dc_level,levellow, levelhigh;
    int      i, period_div2;
        
    ampl = (double)(signal->DAC_Vpp) * 4095 / 3.3 / 2;     //振幅 = 峰峰值/2
    dc   = (double)(signal->DAC_DC_Voltage)  * 4095 / 3.3; // 直流电平
    ampl_level = (uint16_t)ampl; // 振幅     等级值 取整
    dc_level   = (uint16_t)dc;   // 直流电平 等级值 取整
    
    if(ampl_level > dc_level)
    {
        ampl_level = dc_level;
    }
    levellow  = dc_level - ampl_level; // 方波低电平
    levelhigh = dc_level + ampl_level; // 方波高电平
    
    period_div2 = DAC_NOW_SIZE/2;
    for (i=0; i<period_div2; i++)
    {
        signal->DAC_Value[i]             = levellow; // 前半周期=低电平
        signal->DAC_Value[i+period_div2] = levelhigh;// 前半周期=高电平
    }
}

// 生产一个周期的信号
void  Setup_DAC_Signal(struDacParam *_sig)  
{
    if(_sig->DAC_Type == DAC_TYPE_SIN)
    {
        Set_DAC_Sine(_sig);
    }
    else if(_sig->DAC_Type == DAC_TYPE_SQU)
    {
        Set_DAC_Square(_sig);
    }
}












