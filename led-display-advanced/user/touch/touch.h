#ifndef __TOUCH_H__
#define __TOUCH_H__ 
////////////////////////////////////////////////////////////////////////////////// 
#include "common.h"
#include "main.h"
#include "led.h"
#include "timeAPP.h"

// 电容屏 FT6336芯片驱动
//////////////////////////////////////////////////////////////////////////////////
// SCL 引脚配置
#define     FT6336_SCL_GPIO_CLK_EN()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define     FT6336_SCL_GPIOx            GPIOG
#define     FT6336SCLout                PGout
#define     pinFT6336SCL                PIN_1

#define     FT6336_SCL_PIN             (1 << pinFT6336SCL) // GPIO_PIN_1
#define     I2C1_SCL_OUT_1()            FT6336SCLout(pinFT6336SCL)=1
#define     I2C1_SCL_OUT_0()            FT6336SCLout(pinFT6336SCL)=0
//#define     I2C1_SCL_OUT(x)            (x? (FT6336SCLout(pinFT6336SCL)=1) : (FT6336SCLout(pinFT6336SCL)=0))

// SDA引脚配置
#define     FT6336_SDA_GPIO_CLK_EN()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define     FT6336_SDA_GPIOx            GPIOG
#define     FT6336SDAout                PGout 
#define     FT6336SDAin                 PGin
#define     pinFT6336SDA                PIN_0

#define     FT6336_SDA_PIN             (1 << pinFT6336SDA) // GPIO_PIN_0
#define     I2C1_SDA_OUT_1()            FT6336SDAout(pinFT6336SDA)=1
#define     I2C1_SDA_OUT_0()            FT6336SDAout(pinFT6336SDA)=0
#define     I2C1_SDA_IN()               FT6336SDAin(pinFT6336SDA)
//#define     I2C1_SDA_OUT(x)            (x? (FT6336SDAout(pinFT6336SDA)=1) : (FT6336SDAout(pinFT6336SDA)=0))

                                
#define     NOP()  __ASM("NOP")

#define     FT6336_ADDR         0x70
#define     IIC_DLY3            40
#define     IIC_DLY2            25
#define     IIC_DLY1            15
#define     freq_min            10
#define     freq_max            50000
#define     dfreq               (freq_max-freq_max)
#define     duty_max            3
#define     duty_min            0.2
//////////////////////////////////////////////////////////////////////////////////
                                
typedef  enum{
    NO_TOUCH   = 0,    // 触摸屏没有按下
    JUST_TOUCH    ,    // 触摸屏刚按下
    TOUCHED       ,    // 触摸屏已经触摸到了
    JUST_POP      ,    // 触摸屏刚弹出
}TouchMainStatus;

typedef  enum{  
    Y_NO_MOVING=0 ,
    UP_MOVING     ,   // 触摸屏已经按下，并且向上移动
    DOWN_MOVING   ,   // 触摸屏已经按下，并且向下移动
}YSubStatus;

typedef  enum{ 
    X_NO_MOVING=0 ,
    LEFT_MOVING   ,   // 触摸屏已经按下，并且向左移动
    RIGHT_MOVING  ,   // 触摸屏已经按下，并且向右移动
}XSubStatus;

typedef struct 
{
    uint16_t ctp_x;     // 触摸屏的X坐标值
    uint16_t ctp_y;     // 触摸屏的Y坐标值
}struCTPxy;

typedef  struct
{
    struCTPxy       ctpxy;           // 触摸屏的坐标XY
    TouchMainStatus ctpmainstatus;   // 触摸屏的主要状态
    XSubStatus      xmove;           // X方向的移动状态
    YSubStatus      ymove;           // Y方向的移动状态
    int16_t         dx;              // X方向移动的像素 +值:表示向右移动; -值:表示向左移动
    int16_t         dy;              // Y方向移动的像素 +值:表示向下移动; -值:表示向上移动
    uint16_t        notouchedcount;  // 触摸屏没有被触屏1秒后，坐标显示---，用于计数10mS计数一次
}struTouch;


extern  struTouch  Touch;
//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////

void  FT6336_I2C_GPIO_Init(void);
void  ReadCTP(struTouch *);

void  Touch_Disp_XY(struTouch *);
void  Touch_Process(struTouch *, int *, double *, BOOL *);


#endif

