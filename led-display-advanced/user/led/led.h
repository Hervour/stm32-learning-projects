#ifndef __LED_H__
#define __LED_H__
#include  "common.h"

// LED1引脚配置
//#define RCC_LED1_GPIO_CLK_EN()  __HAL_RCC_GPIOE_CLK_ENABLE()
//#define LED1_GPIOx   GPIOE 
//#define LED1_PINx    GPIO_PIN_7

//#define LED1_ON()    HAL_GPIO_WritePin(LED1_GPIOx, LED1_PINx, GPIO_PIN_SET)   // LED1亮
//#define LED1_OFF()   HAL_GPIO_WritePin(LED1_GPIOx, LED1_PINx, GPIO_PIN_RESET) // LED1暗


// LED1引脚配置
#define RCC_LED1_GPIO_CLK_EN()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIOx   GPIOF   //F端口 
#define pinLED1      13      //引脚号13
#define LED1out      PFout
#define LED1in       PFin

#define LED1_PINx   (1 << pinLED1)
#define LED1_ON()    LED1out(pinLED1)=1 // LED1亮
#define LED1_OFF()   LED1out(pinLED1)=0 // LED1暗
#define LED1_TOGGLE()  LED1out(pinLED1)=(~LED1in(pinLED1)) //LED1取反

// LED2引脚配置
#define RCC_LED2_GPIO_CLK_EN()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED2_GPIOx   GPIOF   //F端口 
#define pinLED2      12      //引脚号12
#define LED2out      PFout
#define LED2in       PFin

#define LED2_PINx   (1 << pinLED2)
#define LED2_ON()    LED2out(pinLED2)=1 // LED2亮
#define LED2_OFF()   LED2out(pinLED2)=0 // LED2暗
#define LED2_TOGGLE()  LED2out(pinLED2)=(~LED2in(pinLED2)) //LED2取反

// LED3引脚配置
#define RCC_LED3_GPIO_CLK_EN()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED3_GPIOx   GPIOF   //F端口 
#define pinLED3      11      //引脚号11
#define LED3out      PFout
#define LED3in       PFin

#define LED3_PINx   (1 << pinLED3)
#define LED3_ON()    LED3out(pinLED3)=1 // LED3亮
#define LED3_OFF()   LED3out(pinLED3)=0 // LED3暗
#define LED3_TOGGLE()  LED3out(pinLED3)=(~LED3in(pinLED3)) //LED3取反

// LED4引脚配置
#define RCC_LED4_GPIO_CLK_EN()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED4_GPIOx   GPIOB   //B端口 
#define pinLED4      1       //引脚号1
#define LED4out      PBout
#define LED4in       PBin

#define LED4_PINx   (1 << pinLED4)
#define LED4_ON()    LED4out(pinLED4)=1 // LED4亮
#define LED4_OFF()   LED4out(pinLED4)=0 // LED4暗
#define LED4_TOGGLE()  LED4out(pinLED4)=(~LED4in(pinLED4)) //LED4取反

#define LED_ALL_ON()   LED1_ON();LED2_ON();LED3_ON();LED4_ON()
#define LED_ALL_OFF()  LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF()

// PG10引脚配置
#define RCC_PG10_GPIO_CLK_EN()  __HAL_RCC_GPIOG_CLK_ENABLE()
#define PG10_GPIOx   GPIOG   //G端口 
#define pinPG10      10       //引脚号10
#define PG10out      PGout
#define PG10in       PGin

#define PG10_PINx   (1 << pinPG10)
#define PG10_ON()    PG10out(pinPG10)=1 // PG10=1
#define PG10_OFF()   PG10out(pinPG10)=0 // PG10=0
#define PG10_TOGGLE()  PG10out(pinPG10)=(~PG10in(pinPG10)) //PG10取反

// 函数的声明
void LED_GPIO_Init(void);

#endif
