#include "uartapp.h"
#include "gui.h"
////////////////////////////////////////////////
// printf 重新定向到串口1
int fputc(int ch, FILE *f){
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart1, temp, 1, 0xffff);
    return ch;  
}

int fgetc(FILE * f)
{   
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1,&ch, 1, 0xffff);
    return  ch; 
}   

////////////////////////////////////////////////
// 串口1接收的参数
uint8_t     U1_Rx_Buf[U1_RXD_SIZE]; // 多字节接收时，数据保存缓存区
uint8_t     U1_Tx_Buf[U1_TXD_SIZE]; // 发送缓存区 
uint16_t    U1_IDLE_Rxed_Num;       // 串口1触发空闲中断时，接收到的字节数
ErrorStatus U1_Rx_Success;          // 串口1成功接收一帧数据标记位

////////////////////////////////////////////////
// 所有串口接收的中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        U1_Rx_Success = SUCCESS; // 串口1成功接收一帧数据
    }
}

////////////////////////////////////////////////
// 所有串口接收的空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance  == USART1) // 串口1触发的空闲中断回调
    {
        U1_Rx_Success    = SUCCESS; // 表示接收数据成功标记位
        U1_IDLE_Rxed_Num = Size;    // 获得一帧数据的长度
    }
}

void Usart1_Process(uint8_t *u1rxd, uint16_t u1length)
{
    memset(U1_Tx_Buf, 0, U1_TXD_SIZE);  // 发送缓冲区清0
    memcpy(U1_Tx_Buf, u1rxd, u1length); // 将接收缓冲区内的内容复制到发送缓冲区
    HAL_UART_Transmit_DMA(&huart1, U1_Tx_Buf, u1length);  // 串口1发送数据
    
    Gui_Draw_Asc24_Hzk24(50,400, WHITE_4_4, BLACK, U1_Tx_Buf);
}

//void Usart1_Process(uint8_t *u1rxd, uint16_t u1length, struDacParam *_dacparam)
//{
//    memset(U1_Tx_Buf, 0, U1_TXD_SIZE);  // 发送缓冲区清0
//    memcpy(U1_Tx_Buf, u1rxd, u1length); // 将接收缓冲区内的内容复制到发送缓冲区
//    strcat((char*)U1_Tx_Buf, "  ");
//    u1length = strlen((char*)U1_Tx_Buf);
//    HAL_UART_Transmit_DMA(&huart1, U1_Tx_Buf, u1length);  // 串口1发送数据
//    
//    if( (u1rxd[0]=='<') && (u1rxd[2]=='>') ) // 判断格式是否正确
//    {
//        if(u1rxd[1] == '1')
//        {
//            _dacparam->DAC_Type        = DAC_TYPE_SIN; // DAC输出信号类型为正弦波
//            _dacparam->DAC_Freq        = 100;          // Hz
//            _dacparam->DAC_Vpp         = 3.0;          // V
//            _dacparam->DAC_Refresh     = TRUE;         // 允许刷新DAC输出新的信号 
//        }
//        else if(u1rxd[1] == '2')
//        {
//            _dacparam->DAC_Type        = DAC_TYPE_SIN; // DAC输出信号类型为正弦波
//            _dacparam->DAC_Freq        = 200;          // Hz
//            _dacparam->DAC_Vpp         = 2.0;          // V
//            _dacparam->DAC_Refresh     = TRUE;         // 允许刷新DAC输出新的信号 
//        }
//        else if(u1rxd[1] == '3')
//        {
//            _dacparam->DAC_Type        = DAC_TYPE_SQU; // DAC输出信号类型为正弦波
//            _dacparam->DAC_Freq        = 300;          // Hz
//            _dacparam->DAC_Vpp         = 1.5;          // V
//            _dacparam->DAC_Refresh     = TRUE;         // 允许刷新DAC输出新的信号 
//        }
//        else if(u1rxd[1] == '4')
//        {
//            _dacparam->DAC_Type        = DAC_TYPE_SQU; // DAC输出信号类型为方波
//            _dacparam->DAC_Freq        = 400;          // Hz
//            _dacparam->DAC_Vpp         = 1.0;          // V
//            _dacparam->DAC_Refresh     = TRUE;         // 允许刷新DAC输出新的信号 
//        }
//    }
//}

