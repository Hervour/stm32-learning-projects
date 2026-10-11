#ifndef __AS608_H
#define __AS608_H
#include "main.h"
#include "usart.h"

#define AS608_UART huart2 /// 指纹模块串口句柄，换串口后记得改这里

#define CharBuffer1 0x01
#define CharBuffer2 0x02

extern uint32_t AS608Addr; // 设备地址

/// @brief 刷指纹后模块返回的数据结构体
typedef struct
{
	uint16_t pageID;	// 指纹ID
	uint16_t mathscore; // 匹配得分
} SearchResult;

/// @brief 模块基本参数结构体
typedef struct
{
	uint16_t PS_max;  // 指纹最大容量
	uint8_t PS_level; // 安全等级
	uint32_t PS_addr;
	uint8_t PS_size; // 通讯数据包大小
	uint8_t PS_N;	 // 波特率基数N
} SysPara;

/// @brief AS608指纹模块功能函数封装结构体，方便使用
typedef struct
{
	SearchResult result_; // 刷指纹后模块返回的数据
	SysPara *AS608Para_;  // 模块基本参数

	/**
	 * @brief 添加指纹
	 * @param ID 指纹库位置号,范围：0~299
	 */
	void (*Add_FR)(uint16_t ID);

	/**
	 * @brief 删除指纹库中的N个指纹数据
	 * @param PageID 从此处开始删除指纹
	 * @param N 删除N个指纹
	 */
	void (*Del_FR)(uint16_t PageID, uint16_t N);

	/**
	 * @brief 确认码错误信息解析
	 * @param ensure 确认码
	 * @retval 确认码对应的错误信息
	 */
	const char *(*EnsureMessage)(uint8_t ensure);

	/**
	 * @brief 刷指纹,搜索结果存放于参数中
	 * @param result 存放搜索结果的结构体
	 * @retval 确认码： 0x00:OK  0x01:表示收包有错   0x09:表示没搜索到该指纹
	 */
	uint8_t (*Press_FR)(SearchResult *result);

	/**
	 * @brief 清空指纹库
	 * @param 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_Empty)(void);

	/**
	 * @brief 模块内部为用户开辟了512bytes的FLASH空间用于存用户记事本,该记事本逻辑上被分成 16 个页,*        使用此函数从中读取数据
	 * @param NotePageNum 要读取第几页FLASH
	 * @param Byte32 读出的数据，共32个字节
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_ReadNotepad)(uint8_t NotePageNum, uint8_t *note);

	/**
	 * @brief 读取模块的基本参数（波特率，包大小等)
	 * @param p 模块基本参数结构体指针
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_ReadSysPara)(SysPara *p);

	/**
	 * @brief 设置模块地址
	 * @param PS_addr 新的模块地址
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_SetAddr)(uint32_t addr);

	/**
	 * @brief 读已有指纹个数
	 * @param ValidN 已存储指纹的个数
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_ValidTempleteNum)(uint16_t *ValidN);

	/**
	 * @brief 模块内部为用户开辟了512bytes的FLASH空间用于存用户记事本,该记事本逻辑上被分成 16 个页,*        使用此函数写入数据
	 * @param NotePageNum 要写入第几页FLASH
	 * @param Byte32 写入的数据首地址，最多32bytes
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_WriteNotepad)(uint8_t NotePageNum, uint8_t *content);

	/**
	 * @brief 写模块寄存器，可修改模块的配置
	 * @param RegNum 寄存器序号:4 or 5 or 6
	 * @param DATA 写入的数据
	 * @retval 确认码： 0x00:OK  其它:见用户手册
	 */
	uint8_t (*PS_WriteReg)(uint8_t RegNum, uint8_t DATA);

	/**
	 * @brief 将确认码对应的错误信息从OLED上显示出来
	 * @param ensure 确认码
	 */
	void (*ShowErrMessage)(uint8_t ensure);

} AS608;

/**
 * @brief 初始化AS608指纹模块
 * @param as608 AS608指纹模块结构体指针
 */
void AS608_Init(AS608 *as608);

void Add_FR(uint16_t ID);
void Del_FR(uint16_t PageID, uint16_t N);
const char *EnsureMessage(uint8_t ensure);
uint8_t Press_FR(SearchResult *result);
uint8_t PS_Empty(void);
uint8_t PS_ReadNotepad(uint8_t NotePageNum, uint8_t *note);
uint8_t PS_ReadSysPara(SysPara *p);
uint8_t PS_SetAddr(uint32_t addr);
uint8_t PS_ValidTempleteNum(uint16_t *ValidN);
uint8_t PS_WriteNotepad(uint8_t NotePageNum, uint8_t *content);
uint8_t PS_WriteReg(uint8_t RegNum, uint8_t DATA);
void ShowErrMessage(uint8_t ensure);

#endif
