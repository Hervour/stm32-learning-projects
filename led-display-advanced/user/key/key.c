#include "key.h"
#include "uartapp.h"

struKEY  Key;

void  KEY_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // KEY1 的引脚初始化
    KEY1_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = KEY1_PIN;
    HAL_GPIO_Init(KEY1_GPIOx, &GPIO_InitStruct); 
    
    // KEY2 的引脚初始化
    KEY2_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = KEY2_PIN;
    HAL_GPIO_Init(KEY2_GPIOx, &GPIO_InitStruct); 
        
    // KEY3 的引脚初始化
    KEY3_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = KEY3_PIN;
    HAL_GPIO_Init(KEY3_GPIOx, &GPIO_InitStruct); 
        
    // KEY4 的引脚初始化
    KEY1_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = KEY4_PIN;
    HAL_GPIO_Init(KEY4_GPIOx, &GPIO_InitStruct); 

}

void  KEY_Param_Init(struKEY *_key)
{
    _key->key1 = KEY_NO_PRESSED;
    _key->key2 = KEY_NO_PRESSED;
    _key->key3 = KEY_NO_PRESSED;
    _key->key4 = KEY_NO_PRESSED;
}


// 根据前后电平获得按键状态
enuKEYStatus  Get_Keyx_Status(uint8_t lastk, uint8_t nowk)
{
    enuKEYStatus retval;
    
    if((lastk == 1) && (nowk == 1))
    {
        retval = KEY_NO_PRESSED;
    }
    else if((lastk == 0) && (nowk == 1))
    {
        retval = KEY_JUST_POPUP;
    }
    else if((lastk == 0) && (nowk == 0))
    {
        retval = KEY_ALREADY_PRESSED;
    }
    else
    {
        retval = KEY_JUST_PRESSED;
    }
    
    return retval;
}

/////////////////////////////////////////////////
//定义4个存放 按键引脚上一次的电平 的静态变量
static  uint8_t lastKey1=1; 
static  uint8_t lastKey2=1; 
static  uint8_t lastKey3=1; 
static  uint8_t lastKey4=1; 
void  Key_Read(struKEY *_key)
{
    uint8_t  nowkey;
    
    // 读取KEY1的状态
    nowkey     = KEY1_READ(); // 读取现在按键的电平
    _key->key1 = Get_Keyx_Status(lastKey1,nowkey); //根据前后2次按键的电平获得按键状态
    lastKey1   = nowkey;        // 保存现在的按键电平
    
    // 读取KEY2的状态
    nowkey     = KEY2_READ(); // 读取现在按键的电平
    _key->key2 = Get_Keyx_Status(lastKey2,nowkey);
    lastKey2 = nowkey;
    
    // 读取KEY3的状态
    nowkey     = KEY3_READ(); // 读取现在按键的电平
    _key->key3 = Get_Keyx_Status(lastKey3,nowkey);
    lastKey3 = nowkey;
    
    // 读取KEY4的状态
    nowkey     = KEY4_READ(); // 读取现在按键的电平
    _key->key4 = Get_Keyx_Status(lastKey4,nowkey);
    lastKey4 = nowkey;
}

/////////////////////////////////////
static  uint32_t lastTick=0;
void  Key_Process(struKEY *_key, int *_freq, double *_duty, BOOL *_refresh)
{   
    uint32_t dlt_tick_ms;
    
    dlt_tick_ms = uwTick - lastTick;
    if(dlt_tick_ms < 50)  // 50ms检测一次按键
    {
        return;
    }
    
    // 这里往下都是过了50ms
    lastTick = uwTick; // 下一次也要等待 50ms 再读键

    Key_Read(_key); // 读取四个按键的键值
    
    // 下面根据按键的键值做处理
    if(_key->key1 == KEY_JUST_PRESSED)   
    {   
//        *_freq    = 500;    // D、K1按下, 频率设为500Hz(占空比保持不变)
//        *_refresh = TRUE;   // 刷新波形使能
        
        LED1_TOGGLE();
        uint16_t  wordlength;
        strcpy((char*)U1_Tx_Buf, "上海大学");
        wordlength = strlen((char*)U1_Tx_Buf);
        HAL_UART_Transmit_DMA(&huart1, U1_Tx_Buf, wordlength);
        
    }    
        
    if(_key->key2 == KEY_JUST_PRESSED) 
    {        
//        *_freq    = 1000;   // D、K2按下, 频率设为1000Hz(占空比保持不变)
//        *_refresh = TRUE;   // 刷新波形使能
        LED2_TOGGLE();
        uint16_t  wordlength;
        strcpy((char*)U1_Tx_Buf, "罗浩文");
        wordlength = strlen((char*)U1_Tx_Buf);
        HAL_UART_Transmit(&huart1, U1_Tx_Buf, wordlength, 100);
    }
        
    if(_key->key3 == KEY_JUST_PRESSED)   
    { 
        *_freq    = 2000;   // D、K3按下, 频率设为2000Hz(占空比保持不变)
        *_refresh = TRUE;   // 刷新波形使能
        
        LED3_TOGGLE();
        uint16_t  wordlength;
        strcpy((char*)U1_Tx_Buf, "喜欢永雏塔菲\n");
        wordlength = strlen((char*)U1_Tx_Buf);
        HAL_UART_Transmit(&huart1, U1_Tx_Buf, wordlength, 100);
    } 
    
    if(_key->key4 == KEY_JUST_PRESSED)   
    { 
        *_freq    = 5000;   // D、K4按下, 频率设为5000Hz(占空比保持不变)
        *_refresh = TRUE;   // 刷新波形使能
        
        LED4_TOGGLE();
        uint16_t  wordlength;
        strcpy((char*)U1_Tx_Buf, "喜欢lzy\n");
        wordlength = strlen((char*)U1_Tx_Buf);
        HAL_UART_Transmit(&huart1, U1_Tx_Buf, wordlength, 100);
    } 
}

//void  Key_Process(struKEY *_key, int *_freq, double *_duty, BOOL *_refresh)
//{   
//    if(_key->key1 == KEY_JUST_PRESSED)   
//    { 
//        *_duty    = 0.3;
//        *_refresh = TRUE;
//    }    
//        
//    if(_key->key2 == KEY_JUST_PRESSED) 
//    {        
//        *_duty = 0.7;
//        *_refresh = TRUE;
//    }
//        
//    if(_key->key3 == KEY_JUST_PRESSED)   
//    { 
//        LED3_TOGGLE();
//    } 
//    
//    if(_key->key4 == KEY_JUST_PRESSED)   
//    { 
//        LED4_TOGGLE();
//    } 
//}

/************************************************************************
// 函 数 名:      void  Key_Process(struKeyStatus *key)
// 函数功能描述:  按键的处理                
// 参数描述:      struKeysStatus  *key:  4个按键当前的状态
// 返 回 值:      无
// 返回值描述:    无
// 全局变量:      无
// 静态变量:      无
// 函数状态：     完成测试
// 函数层次：     应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
************************************************************************/
//void  Key_Process(struKEY *_key, struPulse *_pulse)
//{   
//    if(_key->key1 == KEY_JUST_PRESSED)   
//    { 
//        _pulse->freq         = 50;     // 频率赋值=50Hz
//        _pulse->pulse_out_en = ENABLE; // 脉冲输出并显示使能
//    } 
//        
//    if(_key->key2 == KEY_JUST_PRESSED) 
//    {        
//        _pulse->freq         = 500;    // 频率赋值=500Hz
//        _pulse->pulse_out_en = ENABLE; // 脉冲输出并显示使能
//    }
//        
//    if(_key->key3 == KEY_JUST_PRESSED)   
//    { 
//        _pulse->freq         = 5000;   // 频率赋值=5000Hz
//        _pulse->pulse_out_en = ENABLE; // 脉冲输出并显示使能
//    } 
//    
//    if(_key->key4 == KEY_JUST_PRESSED)   
//    { 
//        _pulse->freq         = 50000;  // 频率赋值=50000Hz
//        _pulse->pulse_out_en = ENABLE; // 脉冲输出并显示使能
//    } 
//}

//void  Key_Process(struKEY *_key, int *_freq, FunctionalState *_freq_flag)
//{   
//    if(_key->key1 == KEY_JUST_PRESSED)   
//    { 
//        *_freq = *_freq + 100;
//        if(*_freq > FREQmax)
//        {
//            *_freq = FREQmax;
//        }
//        *_freq_flag = ENABLE;
//    } 
//        
//    if(_key->key2 == KEY_JUST_PRESSED) 
//    {        
//        *_freq = *_freq + 1000;
//        if(*_freq > FREQmax)
//        {
//            *_freq = FREQmax;
//        }
//        *_freq_flag = ENABLE; 
//    }
//        
//    if(_key->key3 == KEY_JUST_PRESSED)   
//    { 
//        *_freq = *_freq - 100;
//        if(*_freq < FREQmin)
//        {
//            *_freq = FREQmin;
//        }
//        *_freq_flag = ENABLE;
//    } 
//    
//    if(_key->key4 == KEY_JUST_PRESSED)   
//    { 
//        *_freq = *_freq - 1000;
//        if(*_freq < FREQmin)
//        {
//            *_freq = FREQmin;
//        }
//        *_freq_flag = ENABLE;
//    } 
//}

