#ifndef  __KEY_H__
#define  __KEY_H__

#include "common.h"
#include "main.h"
#include "led.h"
#include "timeAPP.h"

////////////////////////////////////////////////////////////
// KEY1 参数设置
#define  KEY1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define  KEY1_GPIOx                 GPIOE
#define  KEY1in                     PEin
#define  pinKEY1                    PIN_0 

#define  KEY1_PIN                  (1 << pinKEY1)
#define  KEY1_READ()                KEY1in(pinKEY1)

// KEY2 参数设置
#define  KEY2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define  KEY2_GPIOx                 GPIOE
#define  KEY2in                     PEin
#define  pinKEY2                    PIN_1 

#define  KEY2_PIN                  (1 << pinKEY2)
#define  KEY2_READ()                KEY2in(pinKEY2)

// KEY3 参数设置
#define  KEY3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define  KEY3_GPIOx                 GPIOE
#define  KEY3in                     PEin
#define  pinKEY3                    PIN_2 

#define  KEY3_PIN                  (1 << pinKEY3)
#define  KEY3_READ()                KEY3in(pinKEY3)

// KEY4 参数设置
#define  KEY4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define  KEY4_GPIOx                 GPIOE
#define  KEY4in                     PEin
#define  pinKEY4                    PIN_3 

#define  KEY4_PIN                  (1 << pinKEY4)
#define  KEY4_READ()                KEY4in(pinKEY4)

////////////////////////////////////////////////////////////
#define  FREQmin     100
#define  FREQmax     100000

typedef  enum{
    KEY_NO_PRESSED      = 0,  // 键没有按下状态
    KEY_JUST_PRESSED       ,  // 键刚按下状态
    KEY_ALREADY_PRESSED    ,  // 键已经按下状态
    KEY_JUST_POPUP         ,  // 键刚弹出状态
}enuKEYStatus;

typedef  struct{
    enuKEYStatus  key1;
    enuKEYStatus  key2;
    enuKEYStatus  key3;
    enuKEYStatus  key4;
}struKEY;

extern  struKEY  Key; // 声明全局变量为外部
////////////////////////////////////////////////////////////

void  KEY_GPIO_Init(void);
void  KEY_Param_Init(struKEY *);
void  Key_Read(struKEY *);
//void  Key_Process(struKEY *, int *, double *, BOOL *);
//void  Key_Process(struKEY *, int *, FunctionalState *);
void  Key_Process(struKEY *, int *, double *, BOOL *);
//void  Key_Process(struKEY *, struPulse *);

#endif

