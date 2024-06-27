#include "usart1.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "common.h"
#include "Alarm.h"


u8 receive_str[USART1_REC_NUM];     //���ջ�������,���USART_REC_LEN���ֽ� 
u8 uart_byte_count=0;

u8 aRxBuffer[RXBUFFERSIZE]; //HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���



/****************************************************************************
* ��    ��: void uart1_init(u32 bound)
* ��    �ܣ�USART1��ʼ��
* ��ڲ�����bound��������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart1_init(u32 bound)
{   
  //UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}


/*��printf()֮��ĺ�����ʹ���˰�����ģʽ��ʹ�ñ�׼��ᵼ�³����޷�
	����,�����ǽ������:ʹ��΢��,��Ϊʹ��΢��Ļ�,����ʹ�ð�����ģʽ. 
	���ڹ������Եġ�Target��-����Code Generation���й�ѡ��Use MicroLIB����
	���Ժ�Ϳ���ʹ��printf��sprintf������*/ 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}


//����1����һ���ַ�
void uart1SendChar(u8 ch)
{      
	while((USART1->SR&0x40)==0);  
    USART1->DR = (u8) ch;      
}


/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str, u16 strlen)
* ��    �ܣ�����1����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
            strlen���ַ�������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart1SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart1SendChar(*(str + k)); k++; }   //ѭ������,ֱ���������   
    while (k < strlen); 
} 


//����1�жϷ������
void USART1_IRQHandler(void)  
{
	u32 timeout=0;
	
	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0; 
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
  } 
}

void usart_disp()//����һ�½��յ�������
{
	//Set Alarm 12-00-00
	if(uart_byte_count==18&&receive_str[12]=='-'&&receive_str[15]=='-')
	{
		Alarm_set(receive_str);
	}
	else
	{
		printf("error input");

	}
}

/*usart2*/	




