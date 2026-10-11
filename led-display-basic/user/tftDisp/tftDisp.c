/*************************************************
// 文件说明:      此文件为针对用户层面的函数集
//                建立在在底层驱动文件和基本显示函数之上，用户可以增加相应的显示功能
//                因为这里的文件是针对用户层面的，所以软件有很强的移植性
// 创 建 人：     ZHAOWL
// 日期：         2023-5-10
// 修 改 人：                     
// 修改日期：     201x-xx-xx
*************************************************/

/* --------------------------Includes ---------------------------------------------*/
#include "tftDisp.h"
#include "led.h"
/* ----------------------End of Includes ---------------------------------------------*/

/************************************************************************
// 函 数 名:      tftDisp_Touch_XY(struCTPxy *cptxy, struCTPxy *lastxy)
// 函数功能描述:  显示XY的坐标值
// 参数描述:      x,y 坐标;  forcolor:前景; bkcolor:背景; char *string:显示的字符串   
// 返 回 值:      无       
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     面向用户的--软件功能层

// 创 建 人：     zhaoweiliang
// 日期：         2023-5-10
// 修 改 人：                     
// 修改日期：     201x-xx-xx
************************************************************************/


static  uint16_t last_touch_x=0;
static  uint16_t last_touch_y=0;
void  tftDisp_Touch_XY(uint16_t now_touch_x, uint16_t now_touch_y)
{
    uint8_t   disp_str[20];
    if(now_touch_x != last_touch_x) // 当数值变化后, 显示, 不变则不显示维持原状态
    {
        memset(disp_str, 0, 20);
        sprintf((char*)disp_str, "%3d", now_touch_x);
        Gui_Draw_Asc24_Hzk24(X_TOUCHX, X_Y0, TOUCH_DIG_FORCOLOR, TOUCH_DIG_BKCOLOR, disp_str);
    }
    if(now_touch_y != last_touch_y)  // 当数值变化后, 显示, 不变则不显示维持原状态
    {
        memset(disp_str, 0, 20);
        sprintf((char*)disp_str, "%3d", now_touch_y);
        Gui_Draw_Asc24_Hzk24(Y_TOUCHX, Y_Y0, TOUCH_DIG_FORCOLOR, TOUCH_DIG_BKCOLOR, disp_str);
    }
    
    last_touch_x = now_touch_x;
    last_touch_y = now_touch_y;
}

/////////////////////////////////////////////

void  tftDisp_noTouch_XY(void)
{
    Gui_Draw_Asc24_Hzk24(X_X0, X_Y0, TOUCH_DIG_FORCOLOR, TOUCH_DIG_BKCOLOR, (uint8_t*)"X = ---");
    Gui_Draw_Asc24_Hzk24(Y_X0, X_Y0, TOUCH_DIG_FORCOLOR, TOUCH_DIG_BKCOLOR, (uint8_t*)"Y = ---");
}


void  Disp_Digital(int dig, uint16_t x, uint16_t y)
{
    char  str1[20];
    
    memset(str1, '\0', 20);
    sprintf(str1, "%6d", dig);
    Gui_Draw_Asc24_Hzk24(x, y, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

// 显示频率和占空比值
void  Disp_Freq_Duty(uint16_t x0, uint16_t y0, uint16_t forcolor, uint16_t backcolor, int _freq, double _duty)
{
    char  str1[50];
    
    memset(str1, '\0', 50);
    sprintf(str1, "Freq=%6dHz, duty=%5.1f%%", _freq, (_duty*100));
    Gui_Draw_Asc24_Hzk24(x0, y0, forcolor, backcolor, (uint8_t*)str1);
}    

void  Disp_Freq(int _freq)
{
    char  str1[30];
    
    memset(str1, '\0', 30);
    sprintf(str1, "频率=%6dHz", _freq);
    Gui_Draw_Asc24_Hzk24(640, 410, YELLOW_4_4, WHITE_1_4, (uint8_t*)str1);
}

void  Disp_Duty(double _duty)
{
    char  str1[30];
    
    memset(str1, '\0', 30);
    sprintf(str1, "Duty=%6.2f", (_duty * 100));
    strcat(str1,"%");
    Gui_Draw_Asc24_Hzk24(100, 250, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

void  Disp_Avg_Volt(int _avgvalue, uint16_t x0, uint16_t y0)
{
    double  ftmp;
    char    str1[50];
    
    ftmp = (double)_avgvalue * 3.3 / 4095;
    memset(str1, '\0', 50);
    sprintf(str1, "ADC均值=%4d, V=%5.3fV", _avgvalue, ftmp);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

void  Disp_ADC_Sin_Freq(uint16_t _freq, uint16_t x0, uint16_t y0)
{
    char    str1[50];

    memset(str1, '\0', 50);
    sprintf(str1, "频率=%5dHz", _freq);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

void  Disp_ADC_Sin_Vpp(uint16_t _vpp, uint16_t x0, uint16_t y0)
{
    double  ftmp;
    char    str1[50];
    
    ftmp = (double)_vpp * 3.3 / 4095;
    memset(str1, '\0', 50);
    sprintf(str1, "峰峰值=%5.3fV", ftmp);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

void  Disp_ADC_Sin_Vrms(uint16_t _vpp, uint16_t x0, uint16_t y0)
{
    double  ftmp;
    char    str1[50];
    
    ftmp = (double)_vpp * 3.3 / 4095 * 1.104;
    memset(str1, '\0', 50);
    sprintf(str1, "RMS=%6.4fV", ftmp);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}    

static uint16_t  lastADC[ADC_WAVE_WIDTH];
void  Draw_Wave(uint16_t *adcval, uint16_t stX)
{
    int     i;
    double  ftmp;
    
    for (i=0; i<ADC_WAVE_WIDTH; i++)
    {
        Gui_Draw_aPixel((i + ADC_WAVE_X0), lastADC[i], ADC_WAVE_BK_COLOR);
        
        ftmp = (double)adcval[i + stX];
        ftmp = ADC_WAVE_Y0 - (ftmp - 2048.0) / ADC_WAVE_AMPL_RATIO;
        lastADC[i] = (uint16_t)ftmp;
        
        Gui_Draw_aPixel((i + ADC_WAVE_X0), lastADC[i], ADC_WAVE_FOR_COLOR);
    }
    Gui_Draw_Line(ADC_WAVE_X0, ADC_WAVE_Y0 ,ADC_WAVE_X0 + ADC_WAVE_WIDTH, ADC_WAVE_Y0 , YELLOW_4_4);
}

// 显示 ADC 测量的参数
void  Disp_ADC_Freq_Vpp(uint16_t x0, uint16_t y0, int _freq, double _vpp)
{
    char    str1[50],str2[20],str3[20];
    
    if(_freq < 65535)
    {
        sprintf(str2, "%5dHz", _freq);
    }
    else
    {
        sprintf(str2, "-----Hz");
    }
    
    if(_vpp < 3.4)
    {
        sprintf(str3, "%4.2fVpp", _vpp);
    }
    else
    {
        sprintf(str3, "-.--Vpp");
    }
    sprintf(str1, "ADC测量: %s, %s", str3, str2);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

// 显示姓名学号
//void LCD_Disp_Name(void)
//{
//    uint16_t  x0, y0;
//    x0 = 750;
//    y0 = 40;
//    Gui_Draw_A_Word_32x32(x0, y0, WHITE_4_4, BLUE_4_4, Word_32x32_Shang); // 显示 "上"
//    x0 += 32;
//    Gui_Draw_A_Word_32x32(x0, y0, WHITE_4_4, BLUE_4_4, Word_32x32_Hai);   // 显示 "海"
//    x0 += 32;
//    Gui_Draw_A_Word_32x32(x0, y0, WHITE_4_4, BLUE_4_4, Word_32x32_Da);    // 显示 "大"
////    x0 += 32;
////    Gui_Draw_A_Word_32x32(x0, y0, WHITE_4_4, BLUE_4_4, Word_32x32_Xue);   // 显示 "学"

//    x0 = 720;
//    y0 = 80;
//    Gui_Draw_Asc32_String(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)"12345678"); // 显示学号
//}


void Draw_Main_Win(void)
{
    Lcd_Clear_Area(X1_BK1,Y1_BK1, X2_BK1,Y2_BK1, GREEN_4_4);
    Lcd_Clear_Area(X1_BK2,Y1_BK2, X2_BK2,Y2_BK2, GREEN_4_4);
    
    Gui_Draw_Asc24_Hzk24((X1_BK1 + DLT_X_WORD), (Y1_BK1 + DLT_Y_WORD), WHITE_4_4, BLACK, (uint8_t*)"  500Hz ");
    Gui_Draw_Asc24_Hzk24((X1_BK2 + DLT_X_WORD), (Y1_BK2 + DLT_Y_WORD), WHITE_4_4, BLACK, (uint8_t*)" 2000Hz ");
    
//    // 以下画出滑块条框
//    Lcd_Clear_Area(X1_BK3,Y1_BK3, X2_BK3,Y2_BK3, GREEN_2_4);
//    Gui_Draw_Rect((X1_BK3-1), (Y1_BK3-1),(X2_BK3 + 4),(Y2_BK3+1), WHITE_4_4);
//    Gui_Draw_Rect((X1_BK3-2), (Y1_BK3-2),(X2_BK3 + 5),(Y2_BK3+2), WHITE_4_4);
}

static uint16_t last_X=50;
void Draw_Freq_Flag(int freq)
{
    uint16_t x;
    double   ftmp;
    ftmp = ((double)freq - 1000) / 4000 * 750 +50;
    x    = (uint16_t)ftmp;
    
    Lcd_Clear_Area(last_X,Y1_BK3, (last_X+2),Y2_BK3, GREEN_2_4);
    
    Lcd_Clear_Area(x,Y1_BK3, (x+2),Y2_BK3, RED_4_4);
    last_X = x;
}

// 显示 ADC 测量的参数
void Disp_ADC_Freq_Vpp_Avg(uint16_t x0, uint16_t y0, int _freq, double _vpp, double _vdc)
{
    char    str1[60],str2[20],str3[20],str4[20];
    
    memset(str1, 0, 60);
    memset(str2, 0, 20);
    memset(str3, 0, 20);
    memset(str4, 0, 20);
    
    if(_freq < 65535)
    {
        sprintf(str2, "%5dHz", _freq);
    }
    else
    {
        sprintf(str2, "-----Hz");
    }
    
    if(_vpp < 3.4)
    {
        sprintf(str3, "%4.2fVpp", _vpp);
    }
    else
    {
        sprintf(str3, "-.--Vpp");
    }
    
    if(_vdc < 3.4)
    {
        sprintf(str4, "%4.2fVdc", _vdc);
    }
    else
    {
        sprintf(str4, "-.--Vdc");
    }
    
    sprintf(str1, "ADC测量: %s, %s, %s", str4, str3, str2);
    Gui_Draw_Asc24_Hzk24(x0, y0, WHITE_4_4, BLUE_4_4, (uint8_t*)str1);
}

void Draw_ADC_Win(void)
{
    // 画出ADC波形显示框
    Gui_Draw_Rect((ADC_WAVE_X0 - ADC_WAVE_WIN_PIXEL), (ADC_WAVE_Y0 - ADC_WAVE_HEIGHT - ADC_WAVE_WIN_PIXEL), \
        (ADC_WAVE_X0 + ADC_WAVE_WIDTH + ADC_WAVE_WIN_PIXEL), (ADC_WAVE_Y0 + ADC_WAVE_HEIGHT + ADC_WAVE_WIN_PIXEL), WHITE_4_4);
    Gui_Draw_Rect((ADC_WAVE_X0 - ADC_WAVE_WIN_PIXEL-1), (ADC_WAVE_Y0 - ADC_WAVE_HEIGHT - ADC_WAVE_WIN_PIXEL -1), \
        (ADC_WAVE_X0 + ADC_WAVE_WIDTH + ADC_WAVE_WIN_PIXEL +1 ), (ADC_WAVE_Y0 + ADC_WAVE_HEIGHT + ADC_WAVE_WIN_PIXEL + 1), WHITE_4_4);
    // 画出ADC波形的中心横轴
    Gui_Draw_Line(ADC_WAVE_X0, ADC_WAVE_Y0 ,ADC_WAVE_X0 + ADC_WAVE_WIDTH, ADC_WAVE_Y0 , RED_4_4);
}

