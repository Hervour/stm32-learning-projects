#ifndef __DACAPP_H__
#define __DACAPP_H__
#include "common.h"
#include "dac.h"

//////////////////////////////////////////////////
typedef enum{
    DAC_TYPE_SIN  = 1,  // 正弦波
    DAC_TYPE_SQU     ,  // 方波
}enuSigType;

#define         DAC_NOW_SIZE     200    // DAC 信号一个周期波形所占的数组大小
#define         DC_VOLTAGE      (1.65) // 直流电压值，单位：V

typedef struct{
    uint16_t    DAC_Value[DAC_NOW_SIZE]; // DAC数组
    uint16_t    DAC_Freq;          // dac 输出信号的频率, 单位：Hz
    double      DAC_Vpp;           // dac 输出信号的峰峰值，单位：V
    double      DAC_DC_Voltage;    // dac 输出信号的直流电压，单位：V
    BOOL        DAC_Refresh;       // dac 输出的正弦波刷新使能 
    enuSigType  DAC_Type;       // dac 输出的正弦波刷新使能
}struDacParam;

extern  struDacParam  DAC_Param;

//////////////////////////////////////////////////
void  DAC_Init(struDacParam *);
void  Set_DAC_Sine(struDacParam *);
void  Set_DAC_Square(struDacParam *);
void  Setup_DAC_Signal(struDacParam *); 

//////////////////////////////////////////////////
#endif
