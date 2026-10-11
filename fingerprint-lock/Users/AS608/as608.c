#include "as608.h"
#include <string.h>
#include "stdio.h"
#include "oled.h"

uint32_t AS608Addr = 0XFFFFFFFF; // 设备地址
static uint8_t as608_buf[30];	 // 串口数据缓冲区
static SysPara AS608Para;		 // 模块基本参数

static void MYUSART_SendData(uint8_t data);
static void SendHead(void);
static void SendAddr(void);
static void SendFlag(uint8_t flag);
static void SendLength(int length);
static void Sendcmd(uint8_t cmd);
static void SendCheck(uint16_t check);
static uint8_t *JudgeStr(uint16_t waittime);
static uint8_t PS_HandShake(uint32_t *PS_Addr);
static uint8_t PS_GetImage(void);
static uint8_t PS_GenChar(uint8_t BufferID);
static uint8_t PS_Match(void);
static uint8_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p);
static uint8_t PS_RegModel(void);
static uint8_t PS_StoreChar(uint8_t BufferID, uint16_t PageID);
static uint8_t PS_DeletChar(uint16_t PageID, uint16_t N);
static uint8_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p);

/**
 * @brief 通过串口发送一个字节
 * @param data 发送的数据
 */
static void MYUSART_SendData(uint8_t data)
{
	HAL_UART_Transmit(&AS608_UART, &data, 1, 100);
}

/**
 * @brief 初始化AS608指纹模块
 * @param as608 AS608指纹模块结构体指针
 */
void AS608_Init(AS608 *as608)
{
	/// 绑定函数指针
	as608->Add_FR = Add_FR;
	as608->Del_FR = Del_FR;
	as608->EnsureMessage = EnsureMessage;
	as608->Press_FR = Press_FR;
	as608->PS_Empty = PS_Empty;
	as608->PS_ReadNotepad = PS_ReadNotepad;
	as608->PS_ReadSysPara = PS_ReadSysPara;
	as608->PS_SetAddr = PS_SetAddr;
	as608->PS_ValidTempleteNum = PS_ValidTempleteNum;
	as608->PS_WriteNotepad = PS_WriteNotepad;
	as608->PS_WriteReg = PS_WriteReg;
	as608->ShowErrMessage = ShowErrMessage;
	as608->AS608Para_ = &AS608Para;
	/// 与指纹模块握手，建立联系
	OLED_Clear();
	while (PS_HandShake(&AS608Addr))
	{
		OLED_ShowString(0, 0, "Wait To Connect With As608!", 16);
		HAL_Delay(200);
	}
	OLED_Clear();
	OLED_ShowString(0, 0, "Success Connect With As608!", 16);
	HAL_Delay(1000);
	OLED_Clear();
	/// 读取模块基本参数
	PS_ReadSysPara(as608->AS608Para_);
}

/**
 * @brief 清空指纹库
 * @param 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_Empty(void)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01 + 0x03 + 0x0D;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 写模块寄存器，可修改模块的配置
 * @param RegNum 寄存器序号:4 or 5 or 6
 * @param DATA 写入的数据
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_WriteReg(uint8_t RegNum, uint8_t DATA)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 读取模块的基本参数（波特率，包大小等)
 * @param p 模块基本参数结构体指针
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_ReadSysPara(SysPara *p)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01 + 0x03 + 0x0F;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
	{
		ensure = data[9];
		p->PS_max = (data[14] << 8) + data[15];
		p->PS_level = data[17];
		p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 设置模块地址
 * @param PS_addr 新的模块地址
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_SetAddr(uint32_t PS_addr)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr >> 24);
	MYUSART_SendData(PS_addr >> 16);
	MYUSART_SendData(PS_addr >> 8);
	MYUSART_SendData(PS_addr);
	temp = 0x01 + 0x07 + 0x15 + (uint8_t)(PS_addr >> 24) + (uint8_t)(PS_addr >> 16) + (uint8_t)(PS_addr >> 8) + (uint8_t)PS_addr;
	SendCheck(temp);
	AS608Addr = PS_addr; // 发送完指令，更换地址
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	AS608Addr = PS_addr;
	// if (ensure == 0x00)
	// 	// printf("\r\n设置地址成功！");
	// 	else
	// 		// printf("\r\n%s", EnsureMessage(ensure));
	return ensure;
}

/**
 * @brief 模块内部为用户开辟了512bytes的FLASH空间用于存用户记事本,该记事本逻辑上被分成 16 个页,*        使用此函数写入数据
 * @param NotePageNum 要写入第几页FLASH
 * @param Byte32 写入的数据首地址，最多32bytes
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_WriteNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
	uint16_t temp;
	uint8_t ensure, i;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for (i = 0; i < 32; i++)
	{
		MYUSART_SendData(Byte32[i]);
		temp += Byte32[i];
	}
	temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 模块内部为用户开辟了512bytes的FLASH空间用于存用户记事本,该记事本逻辑上被分成 16 个页,*        使用此函数从中读取数据
 * @param NotePageNum 要读取第几页FLASH
 * @param Byte32 读出的数据，共32个字节
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_ReadNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
	uint16_t temp;
	uint8_t ensure, i;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01 + 0x04 + 0x19 + NotePageNum;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
	{
		ensure = data[9];
		for (i = 0; i < 32; i++)
		{
			Byte32[i] = data[10 + i];
		}
	}
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 读已有指纹个数
 * @param ValidN 已存储指纹的个数
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01 + 0x03 + 0x1d;
	SendCheck(temp);
	data = JudgeStr(100);
	if (data)
	{
		ensure = data[9];
		*ValidN = (data[10] << 8) + data[11];
	}
	else
		ensure = 0xff;

	return ensure;
}

/**
 * @brief 确认码错误信息解析
 * @param ensure 确认码
 * @retval 确认码对应的错误信息
 */
const char *EnsureMessage(uint8_t ensure)
{
	const char *p;
	switch (ensure)
	{
	case 0x00:
		p = "OK";
		break;
	case 0x01:
		// p = "数据包接收错误";
		p = "Packet reception error";
		break;
	case 0x02:
		// p = "传感器上没有手指";
		p = "There are no fingers on the sensor";
		break;
	case 0x03:
		// p = "录入指纹图像失败";
		p = "Failed to input fingerprint image";
		break;
	case 0x04:
		// p = "指纹图像太干、太淡而生不成特征";
		p = "Fingerprint images are too dry or too light to form features";
		break;
	case 0x05:
		// p = "指纹图像太湿、太糊而生不成特征";
		p = "Fingerprint images are too wet or too blurry to form features";
		break;
	case 0x06:
		// p = "指纹图像太乱而生不成特征";
		p = "Fingerprint images are too messy to produce features";
		break;
	case 0x07:
		// p = "指纹图像正常，但特征点太少（或面积太小）而生不成特征";
		p = "The fingerprint image is normal, but the feature points are too few (or the area is too small) to generate features";
		break;
	case 0x08:
		// p = "指纹不匹配";
		p = "Fingerprint mismatch";
		break;
	case 0x09:
		// p = "没搜索到指纹";
		p = "No fingerprints found";
		break;
	case 0x0a:
		// p = "特征合并失败";
		p = "Feature merge failed";
		break;
	case 0x0b:
		// p = "访问指纹库时地址序号超出指纹库范围";
		p = "When accessing the fingerprint database, the address number exceeds the range of the fingerprint database";
		break;
	case 0x10:
		// p = "删除模板失败";
		p = "Delete template failed";
		break;
	case 0x11:
		// p = "清空指纹库失败";
		p = "Clearing fingerprint library failed";
		break;
	case 0x15:
		// p = "缓冲区内没有有效原始图而生不成图像";
		p = "Unable to generate an image without a valid original image in the buffer zone";
		break;
	case 0x18:
		// p = "读写 FLASH 出错";
		p = "Error reading and writing FLASH";
		break;
	case 0x19:
		// p = "未定义错误";
		p = "Undefined error";
		break;
	case 0x1a:
		// p = "无效寄存器号";
		p = "Invalid register number";
		break;
	case 0x1b:
		// p = "寄存器设定内容错误";
		p = "Register setting content error";
		break;
	case 0x1c:
		// p = "记事本页码指定错误";
		p = "Notepad page number specified incorrectly";
		break;
	case 0x1f:
		// p = "指纹库满";
		p = "Fingerprint database full";
		break;
	case 0x20:
		// p = "地址错误";
		p = "Address Error";
		break;
	default:
		// p = "模块返回确认码有误";
		p = "Module return confirmation code error";
		break;
	}
	return p;
}

/**
 * @brief 将确认码对应的错误信息从OLED上显示出来
 * @param ensure 确认码
 */
void ShowErrMessage(uint8_t ensure)
{
	OLED_Clear();
	OLED_ShowString(1, 0, (const uint8_t *)EnsureMessage(ensure), 8);
	HAL_Delay(1000);
}

/**
 * @brief 添加指纹
 * @param ID 指纹库位置号,范围：0~299
 */
void Add_FR(uint16_t ID)
{
	uint8_t processnum = 0, ensure;
	uint16_t k = 4000;
	uint16_t waittime = k;
	while (waittime--)
	{
		switch (processnum)
		{
		case 0:
			// OLED_Clear();
			OLED_ShowString(1, 0, "Pleas touch finger!", 16); // 第一次输入指纹
			ensure = PS_GetImage();
			if (ensure == 0x00)
			{
				ensure = PS_GenChar(CharBuffer1); // 生成特征
				if (ensure == 0x00)
				{
					OLED_Clear();
					OLED_ShowString(1, 0, "Fingerprint correct!", 16); // 指纹正确
					waittime = k;
					processnum = 1; // 跳到第二步
					HAL_Delay(500);
				}
				else
					ShowErrMessage(ensure);
			}
			break;

		case 1:
			OLED_Clear();
			OLED_ShowString(1, 0, "Pleas touch finger again!", 16); // 再按一次手指
			ensure = PS_GetImage();
			if (ensure == 0x00)
			{
				ensure = PS_GenChar(CharBuffer2); // 生成特征
				if (ensure == 0x00)
				{
					OLED_Clear();
					OLED_ShowString(1, 0, "Fingerprint correct!", 16); // 指纹正确
					waittime = k;
					processnum = 2; // 跳到第三步
					HAL_Delay(200);
				}
			}
			break;

		case 2:
			OLED_Clear();
			OLED_ShowString(1, 0, "Compare twice fingerprint!", 16); // 对比两次指纹
			ensure = PS_Match();
			if (ensure == 0x00)
			{
				OLED_Clear();
				OLED_ShowString(1, 0, "Twice fingerprint are same!", 16); // 两次指纹是一样的
				processnum = 3;											  // 跳到第四步
				HAL_Delay(200);
			}
			else
			{
				ShowErrMessage(ensure);
				OLED_Clear();
				OLED_ShowString(1, 0, "Compare fail,pleas touch again!", 16); // 对比失败，请重新按手指
				HAL_Delay(500);
				waittime = k;
				processnum = 0; // 跳回第一步
			}
			break;

		case 3:
			OLED_Clear();
			OLED_ShowString(1, 0, "Generate fingerprint template!", 16); // 产生一个指纹模板指
			ensure = PS_RegModel();
			if (ensure == 0x00)
			{
				OLED_Clear();
				OLED_ShowString(1, 0, "Generate fingerprint success!", 16); // 生成指纹模板成功
				processnum = 4;												// 跳到第五步
				HAL_Delay(200);
			}
			else
			{
				processnum = 0;
				ShowErrMessage(ensure);
			}
			break;

		case 4:
			ensure = PS_StoreChar(CharBuffer2, ID); // 储存模板
			if (ensure == 0x00)
			{
				uint16_t ValidN;
				char str[50];
				OLED_Clear();
				PS_ValidTempleteNum(&ValidN); // 读库指纹个数
				sprintf(str, "Add fingerprint success!!!\n现有指纹个数:%d\n剩余指纹个数:%d", ValidN, AS608Para.PS_max - ValidN);
				OLED_ShowString(2, 0, (const uint8_t *)str, 8);
				HAL_Delay(1500); // 延时后清除显示
				OLED_Clear();
				return;
			}
			else
			{
				processnum = 0;
				ShowErrMessage(ensure);
			}
			break;
		}
	}
	/// 超时后自动退出
	OLED_Clear();
	OLED_ShowString(3, 0, "Overtime!!!!!", 16);
	HAL_Delay(1500); // 延时后清除显示
	OLED_Clear();
}

/**
 * @brief 刷指纹,搜索结果存放于参数中
 * @param result 存放搜索结果的结构体
 * @retval 确认码： 0x00:OK  0x01:表示收包有错   0x09:表示没搜索到该指纹
 */
uint8_t Press_FR(SearchResult *result)
{
	uint16_t waittime = 8;
	uint8_t ensure;
	char str[150];

	OLED_Clear();
	OLED_ShowString(1, 0, "Pleas touch finger!", 16);

	while (waittime--)
	{
		ensure = PS_GetImage();
		if (ensure == 0x00) // 获取图像成功
		{
			ensure = PS_GenChar(CharBuffer1);
			if (ensure == 0x00) // 生成特征成功
			{
				ensure = PS_HighSpeedSearch(CharBuffer1, 0, 300, result);
				if (ensure == 0x00) // 搜索成功
				{
					OLED_Clear();
					// 显示指纹所在指纹库的位置号和匹配程度
					sprintf(str, "Search fingerprint success!\nMatch ID:%d\nMatch score:%d", result->pageID, result->mathscore);
					OLED_ShowString(0, 0, (const uint8_t *)str, 8); // 搜索指纹成功
					HAL_Delay(1500);								// 延时后清屏
					OLED_Clear();
				}
				else
				{
					ShowErrMessage(ensure);
					HAL_Delay(1000);
					OLED_Clear();
				}
				return ensure;
			}
			else
				ShowErrMessage(ensure);
		}
		HAL_Delay(0);
	}
	OLED_Clear();
	OLED_ShowString(1, 0, "Overtime!!!", 16);
	return ensure;
}

/**
 * @brief 删除指纹库中的N个指纹数据
 * @param PageID 从此处开始删除指纹
 * @param N 删除N个指纹
 */
void Del_FR(uint16_t PageID, uint16_t N)
{
	uint8_t ensure;

	ensure = PS_DeletChar(PageID, N); // 删除指纹
	if (ensure == 0)
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "Delete fingerprint success!!!", 16); // 删除指纹成功
		HAL_Delay(1000);											// 延时后清除显示
		OLED_Clear();
		/// 显示库指纹个数
		char str[50];
		uint16_t ValidN;
		PS_ValidTempleteNum(&ValidN); // 读库指纹个数
		sprintf(str, "现有指纹个数:%d\n剩余指纹个数:%d", ValidN, AS608Para.PS_max - ValidN);
		OLED_ShowString(2, 0, (const uint8_t *)str, 16);
		HAL_Delay(1500); // 延时后清除显示
		OLED_Clear();
	}
	else
		ShowErrMessage(ensure);
}

/**
 * @brief 发送包头
 */
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}

/**
 * @brief 发送地址
 */
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr >> 24);
	MYUSART_SendData(AS608Addr >> 16);
	MYUSART_SendData(AS608Addr >> 8);
	MYUSART_SendData(AS608Addr);
}

/**
 * @brief 发送包标识
 */
static void SendFlag(uint8_t flag)
{
	MYUSART_SendData(flag);
}

/**
 * @brief 发送包长度
 */
static void SendLength(int length)
{
	MYUSART_SendData(length >> 8);
	MYUSART_SendData(length);
}

/**
 * @brief 发送指令码
 */
static void Sendcmd(uint8_t cmd)
{
	MYUSART_SendData(cmd);
}

/**
 * @brief 发送校验和
 */
static void SendCheck(uint16_t check)
{
	MYUSART_SendData(check >> 8);
	MYUSART_SendData(check);
}

/**
 * @brief 判断中断接收的数组有没有应答包
 * @param waittime 等待中断接收数据的时间（单位1ms）
 * @retval 应答包首地址
 */
static uint8_t *JudgeStr(uint16_t waittime)
{
	uint8_t *data;
	uint8_t str[7];
	str[0] = 0xef;
	str[1] = 0x01;
	str[2] = AS608Addr >> 24;
	str[3] = AS608Addr >> 16;
	str[4] = AS608Addr >> 8;
	str[5] = AS608Addr;
	str[6] = 0x07;
	memset(as608_buf, 0, sizeof(as608_buf));
	HAL_UART_Receive(&AS608_UART, as608_buf, sizeof(as608_buf), waittime);
	data = (uint8_t *)strstr((const char *)as608_buf, (const char *)str);
	if (data)
		return (uint8_t *)data;
	return 0;
}

/**
 * @brief 与AS608握手，握手成功后才能正常使用指纹模块的功能
 * @param PS_Addr 握手成功后，将模块地址存于该变量中
 * @retval 0:握手成功
 * 		   1:握手失败
 */
static uint8_t PS_HandShake(uint32_t *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);
	HAL_UART_Receive(&AS608_UART, as608_buf, sizeof(as608_buf), 200);
	if ( // 判断是不是模块返回的应答包
		as608_buf[0] == 0XEF && as608_buf[1] == 0X01 && as608_buf[6] == 0X07)
	{
		*PS_Addr = (as608_buf[2] << 24) + (as608_buf[3] << 16) + (as608_buf[4] << 8) + (as608_buf[5]);
		return 0;
	}
	PS_ReadSysPara(&AS608Para);
	return 1;
}

/**
 * @brief 探测手指，探测到后录入指纹图像存于ImageBuffer。
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_GetImage(void)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
	temp = 0x01 + 0x03 + 0x01;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 生成特征，将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2
 * @param BufferID CharBuffer1 or CharBuffer2
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_GenChar(uint8_t BufferID)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01 + 0x04 + 0x02 + BufferID;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 精确比对CharBuffer1 与CharBuffer2 中的特征文件
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_Match(void)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01 + 0x03 + 0x03;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库
 * @param BufferID CharBuffer1 or CharBuffer2
 * @param StartPage 从此处指纹库位置开始搜索
 * @param PageNum 需在指纹库中搜索多少处
 * @param p 存放搜索结果的结构体，分别为指纹对应的ID及匹配程度(匹配得分)
 */
static uint8_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x04 + BufferID + (StartPage >> 8) + (uint8_t)StartPage + (PageNum >> 8) + (uint8_t)PageNum;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer2中
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_RegModel(void)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01 + 0x03 + 0x05;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置
 * @param BufferID charBuffer1 or charBuffer1
 * @param PageID 在指纹库中存放的位置
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_StoreChar(uint8_t BufferID, uint16_t PageID)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	temp = 0x01 + 0x06 + 0x06 + BufferID + (PageID >> 8) + (uint8_t)PageID;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 删除指纹库中指定位置开始的N个指纹
 * @param PageID 从此处开始删除指纹
 * @param N 删除N个指纹
 * @retval 确认码： 0x00:OK  其它:见用户手册
 */
static uint8_t PS_DeletChar(uint16_t PageID, uint16_t N)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N >> 8);
	MYUSART_SendData(N);
	temp = 0x01 + 0x07 + 0x0C + (PageID >> 8) + (uint8_t)PageID + (N >> 8) + (uint8_t)N;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}

/**
 * @brief 以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
 * @param BufferID CharBuffer1 or CharBuffer2
 * @param StartPage 从此处指纹库位置开始搜索
 * @param PageNum 需在指纹库中搜索多少处
 * @param p 存放搜索结果的结构体
 * @retval 确认码： 0x00:OK  0x01:表示收包有错   0x09:表示没搜索到该指纹
 */
static uint8_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
	uint16_t temp;
	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x1b + BufferID + (StartPage >> 8) + (uint8_t)StartPage + (PageNum >> 8) + (uint8_t)PageNum;
	SendCheck(temp);
	data = JudgeStr(500);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}
