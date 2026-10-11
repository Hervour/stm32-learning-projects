#include "adcapp.h"
#include  "math.h"
#include  "tftDisp.h"

uint16_t  ADC1_Value[ADC_MAX_SIZE];
uint16_t  ADC_TRIG_LEVEL[ADC_TRIG_N]; // 把ADC的均值等分为 N 份, 自动获取固定的触发电平

// 获取触发点的坐标值， 返回是否找到匹配值
ErrorStatus Find_Match_Trig_Value(uint16_t *scr, uint16_t start, uint16_t end, uint16_t matchval, uint16_t *trigX)
{    
    int         i;
    ErrorStatus result = ERROR;
    
    for(i=(start+1); i<end; i++)
    {
        if( (scr[i-1] < (matchval-1)) && (scr[i+1] > (matchval+1)) ) 
        {
            *trigX  = i;
            result = SUCCESS;
            break;
        }
    }
    return result;
}

// 求均值
uint16_t  Get_Average(uint16_t *scr, uint16_t start, uint16_t end)
{
    int  i, sum;
    uint16_t average;
    
    sum =0;
    for(i=start; i<end; i++)
    {
        sum += scr[i];
    }
    average = (uint16_t)(sum / (end - start)); // 求均值
    return average;
}

// 求峰峰值，返回电压等级值
uint16_t  Get_Vpp(uint16_t *scr, uint16_t start, uint16_t end)
{
    uint16_t    i, maxvalue, minvalue;
    
    maxvalue = 0;
    minvalue = 50000;
    for(i=start; i<end; i++)
    {
        if(scr[i] > maxvalue)
        {
            maxvalue = scr[i];
        }
            
        if(scr[i] < minvalue)
        {
            minvalue = scr[i];
        }
    }
    return (maxvalue - minvalue);
}

// 将电压等级值转换为电压值
double  Get_Voltage(uint16_t level)
{
    double  volt;
    volt = (double)level * 3.3 / 4095;
    return volt;
}

// 根据均值计算选择一个固定的触发电平，避免均值抖动引起波形的抖动
uint16_t Get_Now_Trig_Level(uint16_t avg)
{
    uint16_t  i, triglevel;
    

    if(avg < (ADC_TRIG_LEVEL[0] + TRIG_LEVEL_DIV2))
    {
        triglevel = ADC_TRIG_LEVEL[0] + TRIG_LEVEL_DIV2 / 2;
    }
    else if(avg > (ADC_TRIG_LEVEL[ADC_TRIG_N-1] + TRIG_LEVEL_DIV2))
    {
        triglevel = ADC_TRIG_LEVEL[ADC_TRIG_N-1] + TRIG_LEVEL_DIV2 + TRIG_LEVEL_DIV2 / 2;
    }
    else
    {
        for(i=1; i<ADC_TRIG_N; i++)
        {
            if( (avg >= (ADC_TRIG_LEVEL[i] - TRIG_LEVEL_DIV2 - 2)) && \
                (avg <  (ADC_TRIG_LEVEL[i] + TRIG_LEVEL_DIV2 + 2)) 
              )
            {
                triglevel = ADC_TRIG_LEVEL[i];
                break;
            }
        }
    }
    return triglevel;
}

static uint16_t lasttriglevel=0;
static uint16_t lastavglevel=0;
static uint16_t newtrig=0;
uint16_t  Get_Trig_Level(uint16_t *scr, uint16_t scrsize)
{
    uint16_t  i, nowtriglevel, nowavglevel;
    int       dltavg;
    
    nowavglevel = Get_Average(scr, 0, scrsize);
    if(newtrig == 0) // 第一次执行
    {
        newtrig = 1;
        
        for(i=0; i<ADC_TRIG_N; i++)
        {   // 计算各个触发电平的等级值, 只计算一次
            ADC_TRIG_LEVEL[i] = ADC_MAX_VALUE * i / ADC_TRIG_N;
        }
        nowtriglevel  = Get_Now_Trig_Level(nowavglevel);
    }
    else
    {   // 第一次以后的所有执行次数
        nowtriglevel = Get_Now_Trig_Level(nowavglevel);
        if(nowtriglevel != lasttriglevel)
        {   // 如果前后2次的触发电平不一样，但是电平等级差在+-20之内，就不改变触发电平，避免波形在触发电平位于交界点时抖动
            dltavg = (int)nowavglevel - (int)lastavglevel;
            if( (dltavg < 20) || (dltavg > -20) )
            {
                nowtriglevel = lasttriglevel;
            }
        }
    }
    lasttriglevel = nowtriglevel;
    lastavglevel  = nowavglevel;
    
    return nowtriglevel;
}



// ADC 转换完成中断回调函数 ,不是指ADC一次中断一次而是ADC转换填满了数组 ADC1_Value 后，触发DMA中断后的回调函数
double  ADC_Vdc_Volt;  // ADC测量得到的直流电压
double  ADC_Vpp_Volt;  // ADC测量得到的峰峰电压
int     ADC_Sin_Freq;  // ADC测量得到的正弦波频率
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{   
    int         i, average;
    uint16_t    adcbuf[ADC_MAX_SIZE], st_X1, st_X2, adc_trig_level;
    ErrorStatus result1, result2;
    
    if(hadc->DMA_Handle->Instance == DMA2_Stream0)
    {
        for (i=0; i<ADC_MAX_SIZE; i++)
        {
            adcbuf[i] = ADC1_Value[i];  // 保存原始数据
        }
        
        ////////////////////////////////////////////////////////////////////////////
        // 计算触发电平, 自动选择一个合适的触发电平
        adc_trig_level = Get_Trig_Level(adcbuf, ADC_MAX_SIZE);
        
        // 计算第一个上升沿的触发点 st_X1
        result1 = Find_Match_Trig_Value(adcbuf, 0, (ADC_MAX_SIZE/2), adc_trig_level, &st_X1);
        
        // 计算 第二个上升沿的触发点 st_X2
        if(result1 == SUCCESS) // 找到第一个匹配点
        {
            result2 = Find_Match_Trig_Value(adcbuf, (st_X1+3), ADC_MAX_SIZE, adc_trig_level, &st_X2);;
        }
        
        if((result1 == SUCCESS) && (result2 == SUCCESS)) 
        {   // 2个匹配点都找到了，计算以下4个参数的值
            average      = Get_Average(adcbuf, st_X1, st_X2); // 均值
            ADC_Vdc_Volt = Get_Voltage(average);              // 计数直流电压值 V
            ADC_Sin_Freq = (uint16_t)(SAMPLE_FREQ / (st_X2 - st_X1));   // 计算频率值 Hz
            ADC_Vpp_Volt = Get_Voltage(Get_Vpp(adcbuf, st_X1, st_X2));  // 计算峰峰值电压 V
        }
        else
        {   // 没有找到匹配点，给出一个不符合常理的值
            average      = 4096;
            ADC_Vdc_Volt = 5.0;  // 计数直流电压值 V
            ADC_Sin_Freq = 65535;//
            ADC_Vpp_Volt = 5.0;  // 
            st_X1        = 100;  // 画出随机波形
        }
        
        // 以下显示 ADC 测量参数和波形
//        Disp_Avg_Volt(average, 10, ADC_WORDS_Y0);           // 显示采样均值和均值对应的电压
//        Disp_ADC_Freq_Vpp(50, 450, ADC_Sin_Freq, ADC_Vpp_Volt); // 显示 ADC 测量的参数
        Disp_ADC_Freq_Vpp_Avg(50, 450, ADC_Sin_Freq, ADC_Vpp_Volt, ADC_Vdc_Volt); // 显示 ADC 测量的参数
        Draw_Wave(adcbuf, st_X1); // 画出波形
    }
}
  







