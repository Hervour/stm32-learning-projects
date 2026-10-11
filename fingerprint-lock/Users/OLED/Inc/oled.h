/**
 * @file oled.h
 * @brief OLED模块头文件
 * @note    使用说明：
 *          1.创建一个Oled结构体
 *          2.使用OLED_Init()函数初始化OLED屏幕
 *          3.调用结构体里的对外接口函数
 *  ！！！！！！重要！！！！！！
 *          4.若使用硬件IIC，请修改第25行iic句柄名称
 *           若使用软件IIC，请在cubemx上将对应的SCL引脚名称改为OLED_SCL，将SDA引脚改为OLED_SDA
 *
 */
#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"
#include "iic_soft.h"

#ifdef SOFT_I2C

#include "iic_soft.h"

#else

#include "i2c.h"
#define OLED_IIC hi2c1 // oled屏幕所用iic句柄

#endif

#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_REG_ADDR (0x3C << 1) // SSD1306从机地址位
#define OLED_REG_COMMAND 0x00     // 命令寄存器地址
#define OLED_REG_DATA 0x40        // 数据寄存器地址

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

/// @brief OLED功能函数封装结构体，方便使用
typedef struct
{
    /// @brief 开启OLED显示(已默认开启)
    void (*OLED_Display_On)();

    /// @brief 关闭OLED显示
    void (*OLED_Display_Off)();

    /// @brief 清屏,使整个屏幕变黑
    void (*OLED_Clear)(void);

    /**
     * @brief 显示字符串(大字体4行,小字体8行,大小字体每行均15字符)
     * @param x x坐标(行)
     * @param y y坐标(列)
     * @param str 字符串指针
     * @param Char_Size 字体大小(8或16)
     */
    void (*OLED_ShowString)(uint8_t x, uint8_t y, const uint8_t *str, uint8_t Char_Size);

    /**
     * @brief 显示汉字
     * @param x x坐标(行)
     * @param y y坐标(列)
     * @param no 第几个汉字(需要自己设置oledfont.h)
     */
    void (*OLED_ShowCHinese)(uint8_t x, uint8_t y, uint8_t no);

    /**
     * @brief 显示图片(需要自己设置bmp.h)
     * @param x0 左上角x坐标
     * @param y0 左上角y坐标
     * @param x1 右下角x坐标
     * @param y1 右下角y坐标
     * @param BMP 图片数据数组,包含bmp.h后使用
     */
    void (*OLED_DrawBMP)(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

    /**
     * @brief 显示数字
     * @param x x坐标(列)
     * @param y y坐标(行)
     * @param len 数字的位数
     * @param float_len 显示小数点后几位
     * @param size2 字体大小(8或16)
     * @note 如果是小数,最多只显示2位小数，len不要不算上小数位
     */
    void (*OLED_ShowNum)(uint8_t y, uint8_t x, float num, uint8_t len, uint8_t float_len, uint8_t size2);
} Oled;

/**
 * @brief 初始化OLED屏(SSD1306)
 * @param oled Oled结构体指针
 */
void OLED_Init(Oled *oled);

/**
 * @brief 清屏函数,清完屏,整个屏幕是黑色的
 */
void OLED_Clear(void);

/**
 * @brief 显示字符串(大字体4行,小字体8行,大小字体每行均15字符)
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param str 字符串指针
 * @param Char_Size 字体大小(8或16)
 */
void OLED_ShowString(uint8_t y, uint8_t x, const uint8_t *str, uint8_t Char_Size);

/**
 * @brief 显示汉字
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param no 第几个汉字(需要自己设置oledfont.h)
 */
void OLED_ShowCHinese(uint8_t y, uint8_t x, uint8_t no);

/**
 * @brief 显示图片(需要自己设置bmp.h)
 * @param x0 左上角x坐标
 * @param y0 左上角y坐标
 * @param x1 右下角x坐标
 * @param y1 右下角y坐标
 * @param BMP 图片数据数组,包含bmp.h后使用
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

/// @brief 开启OLED显示(已默认开启)
void OLED_Display_On(void);

// 关闭OLED显示
void OLED_Display_Off(void);

/**
 * @brief 显示数字
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param len 数字的位数
 * @param float_len 显示小数点后几位
 * @param size2 字体大小(8或16)
 * @note 如果是小数,最多只显示2位小数，len不要不算上小数位
 */
void OLED_ShowNum(uint8_t y, uint8_t x, float num, uint8_t len, uint8_t float_len, uint8_t size2);

#endif
