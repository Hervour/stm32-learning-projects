#ifndef  __COMMON_H__
#define  __COMMON_H__
//////////////////////////////////////////////////////////////
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((__IO uint32_t *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
// GPIOx->ODR寄存器所在的位置
#define GPIOA_ODR_Addr    (GPIOA_BASE + 20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE + 20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE + 20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE + 20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE + 20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE + 20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE + 20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE + 20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE + 20) //0x40022014 
#define GPIOJ_ODR_ADDr    (GPIOJ_BASE + 20) //0x40022414
#define GPIOK_ODR_ADDr    (GPIOK_BASE + 20) //0x40022814

// GPIOx->IDR寄存器所在的位置
#define GPIOA_IDR_Addr    (GPIOA_BASE + 16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE + 16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE + 16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE + 16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE + 16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE + 16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE + 16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE + 16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE + 16) //0x40022010 
#define GPIOJ_IDR_Addr    (GPIOJ_BASE + 16) //0x40022410 
#define GPIOK_IDR_Addr    (GPIOK_BASE + 16) //0x40022810 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //A端口n号引脚输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //A端口n号引脚输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //B端口n号引脚输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //B端口n号引脚输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //C端口n号引脚输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //C端口n号引脚输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //D端口n号引脚输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //D端口n号引脚输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //E端口n号引脚输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //E端口n号引脚输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //F端口n号引脚输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //F端口n号引脚输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //G端口n号引脚输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //G端口n号引脚输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //H端口n号引脚输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //H端口n号引脚输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //I端口n号引脚输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //I端口n号引脚输入

#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  //J端口n号引脚输出 
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  //J端口n号引脚输入

#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  //K端口n号引脚输出 
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  //K端口n号引脚输入

//////////////////////////////////////////////////////////////
#define PIN_0      0
#define PIN_1      1
#define PIN_2      2
#define PIN_3      3
#define PIN_4      4
#define PIN_5      5
#define PIN_6      6
#define PIN_7      7
#define PIN_8      8
#define PIN_9      9
#define PIN_10     10
#define PIN_11     11
#define PIN_12     12
#define PIN_13     13
#define PIN_14     14
#define PIN_15     15 
//////////////////////////////////////////////////////////////

////////////////////////////////////////
// 将接收到的按IEEE741存储的float型的数转换为真实的浮点数
typedef union{
    float     f;
    uint32_t  uint32;
}U32toFloat;
////////////////////////////////////////
typedef  enum{
    FALSE = 0,
    TRUE  = !FALSE,
}BOOL;

#define PI      (3.14159)
////////////////////////////////////////
// 一个字节中的未定义
typedef struct {
    uint8_t  b0:1;
    uint8_t  b1:1;
    uint8_t  b2:1;
    uint8_t  b3:1;
    uint8_t  b4:1;
    uint8_t  b5:1;
    uint8_t  b6:1;
    uint8_t  b7:1;
}bitDef;

typedef  union{
    bitDef   bits8;
    uint8_t  abyte;
}uniBit8Byte;



///////////////////////////////////////////
void  Delay_ms(uint32_t );
void  Delay(volatile uint32_t );

ErrorStatus String_Match_OK(char *str1, char *str2);
ErrorStatus Get_String_Match_First_End_Addr(char *str1, char *str2, uint32_t *first_addr, uint32_t *end_addr);

uint8_t  Swap_bit7_bit0(uint8_t );

#endif
