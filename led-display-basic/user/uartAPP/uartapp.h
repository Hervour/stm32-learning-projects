#ifndef __UARTAPP_H__
#define __UARTAPP_H__
#include "common.h"
#include "usart.h"
#include "dacapp.h"

//////////////////////////////////////////////////////////////////////////////////////////////
#define U1_RXD_SIZE  500
#define U1_TXD_SIZE  500
 
extern  uint8_t      U1_Rx_Buf[U1_RXD_SIZE]; 
extern  uint8_t      U1_Tx_Buf[U1_TXD_SIZE];

extern  ErrorStatus  U1_Rx_Success;
extern  uint16_t     U1_IDLE_Rxed_Num;

//////////////////////////////////////////////////////////////////////////////////////////////
// void Usart1_Process(uint8_t *u1rxd, uint16_t u1length, struDacParam *_dacparam);
void Usart1_Process(uint8_t *, uint16_t );
//////////////////////////////////////////////////////////////////////////////////////////////

#endif

