#ifndef __IIC_SOFT_H
#define __IIC_SOFT_H

#include "main.h"

#define SOFT_I2C // 使用软件IIC

/// @brief IIC引脚结构体
typedef struct
{
    struct
    {
        uint16_t GPIO_PIN;
        GPIO_TypeDef *GPIO_Port;
    } SCL, SDA;

} GPIO_IIC;

/**
 * @brief 使用软件IIC向从机通讯地址的指定寄存器写入数据(单位：字节)
 * @param gpio IIC引脚结构体指针
 * @param DevAddress 从机通讯地址
 * @param MemAddress 寄存器地址
 * @param pData 写入的数据
 * @param Size 写入数据的长度
 * @retval HAL status
 */
HAL_StatusTypeDef IIC_Mem_Write_Soft(GPIO_IIC *gpio, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

/**
 * @brief 使用软件IIC从从机通讯地址的指定寄存器中读入数据(单位：字节)
 * @param gpio IIC引脚结构体指针
 * @param DevAddress 从机通讯地址
 * @param MemAddress 寄存器地址
 * @param pData 存放读取到的数据
 * @param Size 读取的长度
 * @retval HAL status
 */
HAL_StatusTypeDef IIC_Mem_Read_Soft(GPIO_IIC *gpio, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

#endif
