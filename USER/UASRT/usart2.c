#include "usart2.h"
#include "string.h"
#include "stdlib.h"   
#include "common.h"
#include "Alarm.h"
#include "lcd.h"

u8 receive2_str[USART2_REC_NUM];     //���ջ�������,���USART_REC_LEN���ֽ� 
u8 uart2_byte_count=0;

u8 aRxBuffer2[RXBUFFERSIZE2]; //HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART2_Handler; //UART���


/****************************************************************************
* ��    ��: void HAL_UART_MspInit(UART_HandleTypeDef *huart)
* ��    �ܣ�UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
* ��ڲ�����huart:���ھ��
* ���ز�������
* ˵    �����˺����ᱻHAL_UART_Init()���� 
****************************************************************************/
//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//    
//    if(huart->Instance == USART2)  // �����USART2������USART2 MSP��ʼ��
//    {
//        __HAL_RCC_GPIOA_CLK_ENABLE();        // ʹ��GPIOAʱ��
//        __HAL_RCC_USART2_CLK_ENABLE();       // ʹ��USART2ʱ��
//    
//        GPIO_Initure.Pin = GPIO_PIN_2;       // PA2
//        GPIO_Initure.Mode = GPIO_MODE_AF_PP; // �����������
//        GPIO_Initure.Pull = GPIO_PULLUP;     // ����
//        GPIO_Initure.Speed = GPIO_SPEED_FAST;// ����
//        GPIO_Initure.Alternate = GPIO_AF7_USART2; // ����ΪUSART2
//        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA2

//        GPIO_Initure.Pin = GPIO_PIN_3;       // PA3
//        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA3
//        
//        HAL_NVIC_EnableIRQ(USART2_IRQn);        // ʹ��USART2�ж�ͨ��
//        HAL_NVIC_SetPriority(USART2_IRQn, 0x01, 0x02); // ��ռ���ȼ�1�������ȼ�3
//    }
//}



/****************************************************************************
* ��    ��: void uart1_init(u32 bound)
* ��    �ܣ�USART1��ʼ��
* ��ڲ�����bound��������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart2_init(u32 bound)
{   
  //UART ��ʼ������
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}


//����1����һ���ַ�
void uart2SendChar(u8 ch)
{      
	while((USART2->SR&0x40)==0);  
    USART2->DR = (u8) ch;      
}


/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str, u16 strlen)
* ��    �ܣ�����1����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
            strlen���ַ�������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart2SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart2SendChar(*(str + k)); k++; }   //ѭ������,ֱ���������   
    while (k < strlen); 
} 


//����1�жϷ������
void USART2_IRQHandler(void)  
{
	u32 timeout=0;
	
	
	HAL_UART_IRQHandler(&UART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0; 
	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
  } 
}

void usart2_disp()//����һ�½��յ�������
{
	LCD_DisplayString(30,70,16,receive2_str);
	
}






