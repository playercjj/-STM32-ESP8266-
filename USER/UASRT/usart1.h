#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 


/*usart1*/
#define USART1_REC_NUM  			100  	//定义最大接收字节数 200
extern u8 uart_byte_count;          //uart_byte_count要小于USART_REC_LEN
extern u8 receive_str[USART1_REC_NUM];  

extern UART_HandleTypeDef UART1_Handler; //UART句柄
#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

void uart1_init(u32 bound);
void uart1SendChars(u8 *str, u16 strlen);
void usart_disp(void);//处理一下接收到的数据


#endif


