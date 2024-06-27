#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART2_REC_NUM  			100  	//定义最大接收字节数 200
extern u8 uart2_byte_count;          //uart_byte_count要小于USART_REC_LEN
extern u8 receive2_str[USART2_REC_NUM];  

extern UART_HandleTypeDef UART2_Handler; //UART句柄
#define RXBUFFERSIZE2   1 //缓存大小
extern u8 aRxBuffer2[RXBUFFERSIZE2];//HAL库USART接收Buffer

void uart2_init(u32 bound);
void uart2SendChars(u8 *str, u16 strlen);
void usart2_disp(void);//处理一下接收到的数据
#endif


