#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART2_REC_NUM  			100  	//�����������ֽ��� 200
extern u8 uart2_byte_count;          //uart_byte_countҪС��USART_REC_LEN
extern u8 receive2_str[USART2_REC_NUM];  

extern UART_HandleTypeDef UART2_Handler; //UART���
#define RXBUFFERSIZE2   1 //�����С
extern u8 aRxBuffer2[RXBUFFERSIZE2];//HAL��USART����Buffer

void uart2_init(u32 bound);
void uart2SendChars(u8 *str, u16 strlen);
void usart2_disp(void);//����һ�½��յ�������
#endif


