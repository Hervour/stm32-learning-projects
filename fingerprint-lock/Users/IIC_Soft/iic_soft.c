#include "iic_soft.h"

/******************引脚电平设置宏定义*********************/

#define IIC_SCL(x) HAL_GPIO_WritePin(gpio->SCL.GPIO_Port, gpio->SCL.GPIO_PIN, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)

#define IIC_SDA(x) HAL_GPIO_WritePin(gpio->SDA.GPIO_Port, gpio->SDA.GPIO_PIN, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)

#define IIC_READ_SDA() HAL_GPIO_ReadPin(gpio->SDA.GPIO_Port, gpio->SDA.GPIO_PIN)

/*******************IIC控制函数******************/

static void HAL_Delay_us(uint32_t us);
static void IIC_Start(GPIO_IIC *gpio);
static void IIC_Stop(GPIO_IIC *gpio);
uint8_t IIC_Wait_Ack(GPIO_IIC *gpio);
static void IIC_Ack(GPIO_IIC *gpio);
static void IIC_NAck(GPIO_IIC *gpio);
static void IIC_Send_Byte(GPIO_IIC *gpio, uint8_t data);
static uint8_t IIC_Read_Byte(GPIO_IIC *gpio, uint8_t ack);

/********************函数实现************************/

/**
 * @brief 使用软件IIC向从机通讯地址的指定寄存器写入数据(单位：字节)
 * @param gpio IIC引脚结构体指针
 * @param DevAddress 从机通讯地址
 * @param MemAddress 寄存器地址
 * @param pData 写入的数据
 * @param Size 写入数据的长度
 * @retval HAL status
 */
HAL_StatusTypeDef IIC_Mem_Write_Soft(GPIO_IIC *gpio, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t i;

    IIC_Start(gpio);
    IIC_Send_Byte(gpio, DevAddress | 0);
    if (IIC_Wait_Ack(gpio) == 1)
    {
        IIC_Stop(gpio);
        return HAL_ERROR;
    }
    IIC_Send_Byte(gpio, MemAddress);
    if (IIC_Wait_Ack(gpio) == 1)
    {
        IIC_Stop(gpio);
        return HAL_ERROR;
    }
    for (i = 0; i < Size; i++)
    {
        IIC_Send_Byte(gpio, pData[i]);
        if (IIC_Wait_Ack(gpio) == 1)
        {
            IIC_Stop(gpio);
            return HAL_ERROR;
        }
    }
    IIC_Stop(gpio);
    return HAL_OK;
}

/**
 * @brief 使用软件IIC从从机通讯地址的指定寄存器中读入数据(单位：字节)
 * @param gpio IIC引脚结构体指针
 * @param DevAddress 从机通讯地址
 * @param MemAddress 寄存器地址
 * @param pData 存放读取到的数据
 * @param Size 读取的长度
 * @retval HAL status
 */
HAL_StatusTypeDef IIC_Mem_Read_Soft(GPIO_IIC *gpio, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    IIC_Start(gpio);
    IIC_Send_Byte(gpio, DevAddress | 0);
    if (IIC_Wait_Ack(gpio) == 1)
    {
        IIC_Stop(gpio);
        return HAL_ERROR;
    }
    IIC_Send_Byte(gpio, MemAddress);
    if (IIC_Wait_Ack(gpio) == 1)
    {
        IIC_Stop(gpio);
        return HAL_ERROR;
    }
    IIC_Start(gpio);
    IIC_Send_Byte(gpio, DevAddress | 1);
    if (IIC_Wait_Ack(gpio) == 1)
    {
        IIC_Stop(gpio);
        return HAL_ERROR;
    }
    while (Size)
    {
        *pData = IIC_Read_Byte(gpio, (Size > 1) ? 1 : 0);
        Size--;
        pData++;
    }
    IIC_Stop(gpio);
    return HAL_OK;
}

/**
 * @brief HAL微妙延时函数
 * @param us 要延时的微妙
 */
static void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(SystemCoreClock / (1000000U / uwTickFreq));
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
}

/**
 * @brief 产生IIC起始信号
 * @param gpio IIC引脚结构体指针
 */
static void IIC_Start(GPIO_IIC *gpio)
{
    IIC_SDA(1);
    IIC_SCL(1);
    HAL_Delay_us(2);
    IIC_SDA(0);
    HAL_Delay_us(2);
    IIC_SCL(0);
    HAL_Delay_us(2);
}

/**
 * @brief 产生IIC停止信号
 * @param gpio IIC引脚结构体指针
 */
static void IIC_Stop(GPIO_IIC *gpio)
{
    IIC_SDA(0);
    HAL_Delay_us(2);
    IIC_SCL(1);
    HAL_Delay_us(2);
    IIC_SDA(1);
    HAL_Delay_us(2);
}

/**
 * @brief 等待IIC应答信号
 * @param gpio IIC引脚结构体指针
 * @retval      0: 应答信号接收成功
 *              1: 应答信号接收失败
 */
uint8_t IIC_Wait_Ack(GPIO_IIC *gpio)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);
    HAL_Delay_us(2);
    IIC_SCL(1);
    HAL_Delay_us(2);

    while (IIC_READ_SDA())
    {
        waittime++;

        if (waittime > 250)
        {
            IIC_Stop(gpio);
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);
    HAL_Delay_us(2);

    return rack;
}

/**
 * @brief 产生ACK应答信号
 * @param gpio IIC引脚结构体指针
 */
static void IIC_Ack(GPIO_IIC *gpio)
{
    IIC_SDA(0);
    HAL_Delay_us(2);
    IIC_SCL(1);
    HAL_Delay_us(2);
    IIC_SCL(0);
    HAL_Delay_us(2);
    IIC_SDA(1);
    HAL_Delay_us(2);
}

/**
 * @brief 不产生ACK应答信号
 * @param gpio IIC引脚结构体指针
 */
static void IIC_NAck(GPIO_IIC *gpio)
{
    IIC_SDA(1);
    HAL_Delay_us(2);
    IIC_SCL(1);
    HAL_Delay_us(2);
    IIC_SCL(0);
    HAL_Delay_us(2);
}

/**
 * @brief IIC发送一个字节
 * @param gpio IIC引脚结构体指针
 * @param data 要发送的字节
 */
static void IIC_Send_Byte(GPIO_IIC *gpio, uint8_t data)
{
    uint8_t t;

    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);
        HAL_Delay_us(2);
        IIC_SCL(1);
        HAL_Delay_us(2);
        IIC_SCL(0);
        data <<= 1;
    }
    IIC_SDA(1);
}

/**
 * @brief IIC接收一个字节
 * @param gpio IIC引脚结构体指针
 * @param ack ack=1时，产生ACK应答，否则不产生ACK应答
 * @retval 接收到的数据
 */
static uint8_t IIC_Read_Byte(GPIO_IIC *gpio, uint8_t ack)
{
    uint8_t i;
    uint8_t data = 0;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        IIC_SCL(1);
        HAL_Delay_us(2);

        if (IIC_READ_SDA())
        {
            data++;
        }

        IIC_SCL(0);
        HAL_Delay_us(2);
    }

    if (ack == 0)
        IIC_NAck(gpio);
    else
        IIC_Ack(gpio);

    return data;
}
