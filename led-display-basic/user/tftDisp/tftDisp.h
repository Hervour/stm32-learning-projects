#ifndef __TFTDISP_H__
#define __TFTDISP_H__ 

#include  "GUI.h"


// 显示2个选择框的坐标及数值的坐标位置
#define    WIDTH        300   // 框的宽度
#define    HEIGHT       100   // 框的高度
#define    INTERVAL_1    50   // 框的高间距
#define    INTERVAL_2    50   // 框的宽间距
#define    PIXEL_2        4   // 缩进的点数 

// 框1坐标
#define    X1_BK1       100   // 框1的左上X坐标
#define    Y1_BK1       200   // 框1的左上Y坐标
#define    X2_BK1      (X1_BK1 + WIDTH)     // 框1的右下X坐标
#define    Y2_BK1      (Y1_BK1 + HEIGHT)    // 框1的右下Y坐标

// 框2坐标
#define    X1_BK2      (X2_BK1 + INTERVAL_2)// 框2的左上X坐标
#define    Y1_BK2       Y1_BK1              // 框2的左上Y坐标
#define    X2_BK2      (X1_BK2 + WIDTH)     // 框2的右下X坐标
#define    Y2_BK2      (Y1_BK2 + HEIGHT)    // 框2的右下Y坐标

// 框3坐标
#define    X1_BK3       50                  // 框3的左上X坐标
#define    Y1_BK3       350                 // 框3的左上Y坐标
#define    X2_BK3       800                 // 框3的右下X坐标
#define    Y2_BK3      (Y2_BK1 + HEIGHT)    // 框3的右下Y坐标

//// 框4坐标
//#define    X1_BK4      (X2_BK3 + INTERVAL_2)// 框4的左上X坐标
//#define    Y1_BK4       Y1_BK1              // 框4的左上Y坐标
//#define    X2_BK4      (X1_BK4 + WIDTH)     // 框4的右下X坐标
//#define    Y2_BK4      (Y1_BK4 + HEIGHT)    // 框4的右下Y坐标

#define    DLT_X_WORD   (WIDTH/2 - 30)
#define    DLT_Y_WORD   (HEIGHT/2 -12)

//////////////////////////////////////////////////////////////////////////////////////////////
// 显示触摸坐标的参数
#define    LINE_Y0              (Y_MAX_PIXEL - 41) // 横线分割触摸坐标的显示
#define    X_X0                  660               // x坐标显示参数
#define    X_Y0                 (Y_MAX_PIXEL - 32)
#define    X_TOUCHX             (X_X0 + 4*ASC24_12PIXELperROW)

#define    Y_X0                 (X_X0 + 100 )      // y坐标显示参数
#define    Y_Y0                  X_Y0
#define    Y_TOUCHX             (Y_X0 + 4*ASC24_12PIXELperROW)

#define    TOUCH_DIG_FORCOLOR    YELLOW_4_4
#define    TOUCH_DIG_BKCOLOR     BLUE_4_4

//////////////////////////////////////////////////////////////////////////////////////////////

#define    ADC_WORDS_Y0          450
#define    ADC_WAVE_AMPL_RATIO   15  // 按ADC 值画波形时的压缩比例
#define    ADC_WAVE_X0           20
#define    ADC_WAVE_Y0           200
#define    ADC_WAVE_WIDTH        800
#define    ADC_WAVE_HEIGHT       190
#define    ADC_WAVE_WIN_PIXEL    5

#define    ADC_WAVE_BK_COLOR     BLUE_1_4
#define    ADC_WAVE_FOR_COLOR    YELLOW_4_4

//////////////////////////////////////////////////////////////////////////////////////////////////

#define WORD40X40_WIDTH   40
#define WORD40X40_HEIGHT  40
#define ASC20X40_WIDTH    20
#define ASC20X40_HEIGHT   40
#define BYTES_ASC20x40    120 //3字节/行 * 40行

#define R0                25  // 圆的半径
#define D0                65  // 两个圆直接的距离
#define X0_CIRCLE        750  // 指示电平的圆的X坐标
#define Y0_CIRCLE        380  // 指示电平的圆的y坐标, 开始的第一个圆

#define X_LEVEL_WORD    (X0_CIRCLE-50)          // 显示电平值的X坐标
#define Y_LEVEL_WORD    (Y0_CIRCLE + R0 + 20)   // 显示电平值的Y坐标
#define X_LEVEL_VALUE   (X_LEVEL_WORD + WORD40X40_WIDTH * 2 + 20)   // 显示电平值的X坐标
#define Y_LEVEL_VALUE   (Y_LEVEL_WORD)          // 显示电平值的Y坐标
#define MAX_LEVEL          4                    // 最大的电平值

//////////////////////////////////////////////////////////////////////
void  Disp_Freq(int _freq);
void  Disp_Duty(double _duty);
void  Draw_Wave(uint16_t *adcval, uint16_t stX);
void  Disp_Avg_Volt(int _avgvalue, uint16_t x0, uint16_t y0);
void  Disp_ADC_Sin_Freq(uint16_t _freq, uint16_t x0, uint16_t y0);
void  Disp_ADC_Sin_Vpp(uint16_t _vpp, uint16_t x0, uint16_t y0);
void  Disp_ADC_Sin_Vrms(uint16_t _vpp, uint16_t x0, uint16_t y0);

void  tftDisp_Touch_XY(uint16_t, uint16_t );
void  tftDisp_noTouch_XY(void);
void  Disp_Digital(int , uint16_t, uint16_t );
void  Disp_Double(double ftmp, uint16_t x, uint16_t y);
void  Disp_ADC_Freq_Vpp(uint16_t x0, uint16_t y0, int _freq, double _vpp);

void LCD_Disp_Name(void);

void Draw_Main_Win(void);
void Draw_Freq_Flag(int );
void Disp_Freq_Duty(uint16_t , uint16_t , uint16_t , uint16_t , int , double );

void Disp_ADC_Freq_Vpp_Avg(uint16_t x0, uint16_t y0, int _freq, double _vpp, double _vdc);
void Draw_ADC_Win(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
