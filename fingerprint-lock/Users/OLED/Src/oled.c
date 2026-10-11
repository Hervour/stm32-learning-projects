#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "bmp.h"
#include "math.h"

#ifdef SOFT_I2C // 使用软件IIC
GPIO_IIC gpio_oled;
#endif

/// OLED控制用函数

static void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
static void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
static void OLED_Set_Pos(uint8_t x, uint8_t y);

/*******************函数实现*********************/

/**
 * @brief 清屏函数,清完屏,整个屏幕是黑色的
 */
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // 更新显示
}

/**
 * @brief 显示字符串(大字体4行,小字体8行,大小字体每行均15字符)
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param str 字符串指针
 * @param Char_Size 字体大小(8或16)
 */
void OLED_ShowString(uint8_t y, uint8_t x, const uint8_t *str, uint8_t Char_Size)
{
	uint8_t j = 0;
	while (str[j] != '\0')
	{
		OLED_ShowChar(x, y, str[j], Char_Size);
		x += 8;
		if (x > 120 || str[j] == '\n')
		{
			x = 0;
			if (Char_Size == 16)
				y += 2;
			else
				y += 1;
		}
		j++;
	}
}

/**
 * @brief 显示汉字
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param no 第几个汉字(需要自己设置oledfont.h)
 */
void OLED_ShowCHinese(uint8_t y, uint8_t x, uint8_t no)
{
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}

/**
 * @brief 显示图片(需要自己设置bmp.h)
 * @param x0 左上角x坐标
 * @param y0 左上角y坐标
 * @param x1 右下角x坐标
 * @param y1 右下角y坐标
 * @param BMP 图片数据数组,包含bmp.h后使用
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint32_t j = 0;
	uint8_t x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

/// @brief 开启OLED显示(已默认开启)
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

/**
 * @brief 显示数字
 * @param x x坐标(列)
 * @param y y坐标(行)
 * @param len 数字的位数
 * @param float_len 显示小数点后几位
 * @param size2 字体大小(8或16)
 * @note 如果是小数,最多只显示2位小数，len不要不算上小数位
 */
void OLED_ShowNum(uint8_t y, uint8_t x, float num, uint8_t len, uint8_t float_len, uint8_t size2)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	/// 判断是正数还是负数，如果是负数，显示负号
	if (num < 0)
	{
		OLED_ShowChar(x, y, '-', size2);
		num = -num;
		x += size2 / 2;
	}
	for (t = 0; t < len; t++)
	{
		temp = ((int)num / (int)pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x, y, ' ', size2);
				x += size2 / 2;
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x, y, temp + '0', size2);
		x += size2 / 2;
	}
	// 将整数后一位设为空，防止某些情况的影响（比如你先显示一个负数，再显示一个它的相反数，~~）
	OLED_ShowChar(x, y, ' ', size2);
	/// 如果是小数，显示小数点后两位
	if (num != (int)num)
	{
		OLED_ShowChar(x, y, '.', size2);
		x += size2 / 2;
		num = (int)((int)(num * pow(10, float_len)) % (int)pow(10, float_len));
		OLED_ShowNum(y, x, num, float_len, 0, size2);
	}
}

/**
 * @brief 初始化OLED屏(SSD1306)
 * @param oled Oled结构体指针
 */
void OLED_Init(Oled *oled)
{
#ifdef SOFT_I2C // 使用软件IIC
				/// 初始化OLED模拟IIC引脚
	gpio_oled.SCL.GPIO_Port = OLED_SCL_GPIO_Port;
	gpio_oled.SCL.GPIO_PIN = OLED_SCL_Pin;
	gpio_oled.SDA.GPIO_Port = OLED_SDA_GPIO_Port;
	gpio_oled.SDA.GPIO_PIN = OLED_SDA_Pin;
#endif
	/// 初始化Oled结构体(绑定函数)
	oled->OLED_Clear = OLED_Clear;
	oled->OLED_Display_Off = OLED_Display_Off;
	oled->OLED_Display_On = OLED_Display_On;
	oled->OLED_DrawBMP = OLED_DrawBMP;
	oled->OLED_ShowCHinese = OLED_ShowCHinese;
	oled->OLED_ShowString = OLED_ShowString;
	oled->OLED_ShowNum = OLED_ShowNum;
	/// IIC初始化OLED屏幕
	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //
	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

	OLED_Clear();
	OLED_ShowString(0, 0, "Success Connect With OLED!", 16);
	HAL_Delay(1000);
	OLED_Clear();
}

static void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
#ifdef SOFT_I2C // 使用软件IIC
	if (cmd)
		IIC_Mem_Write_Soft(&gpio_oled, OLED_REG_ADDR, OLED_REG_DATA, &dat, 1);
	else
		IIC_Mem_Write_Soft(&gpio_oled, OLED_REG_ADDR, OLED_REG_COMMAND, &dat, 1);
#else
	if (cmd)
		HAL_I2C_Mem_Write(&OLED_IIC, OLED_REG_ADDR, OLED_REG_DATA, I2C_MEMADD_SIZE_8BIT, &dat, 1, 10);
	else
		HAL_I2C_Mem_Write(&OLED_IIC, OLED_REG_ADDR, OLED_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, &dat, 1, 10);
#endif
}

// 坐标设置
static void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
static void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
	if (chr == '\n')
		return;
	uint8_t c = 0, i = 0;
	c = chr - ' '; // 得到偏移后的值
	if (x > Max_Column - 1 || chr == '\n')
	{
		x = 0;
		y = y + 2;
	}
	if (Char_Size == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
	}
}
