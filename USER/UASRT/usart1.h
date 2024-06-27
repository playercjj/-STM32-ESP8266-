#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 


/*usart1*/
#define USART1_REC_NUM  			100  	//�����������ֽ��� 200
extern u8 uart_byte_count;          //uart_byte_countҪС��USART_REC_LEN
extern u8 receive_str[USART1_REC_NUM];  

extern UART_HandleTypeDef UART1_Handler; //UART���
#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

void uart1_init(u32 bound);
void uart1SendChars(u8 *str, u16 strlen);
void usart_disp(void);//����һ�½��յ�������


#endif


