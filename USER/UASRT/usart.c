#include "usart.h"



/****************************************************************************
* ��    ��: void HAL_UART_MspInit(UART_HandleTypeDef *huart)
* ��    �ܣ�UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
* ��ڲ�����huart:���ھ��
* ���ز�������
* ˵    �����˺����ᱻHAL_UART_Init()���� 
****************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,0x02,0x02);			//��ռ���ȼ�1�������ȼ�3	
	}
	else if(huart->Instance == USART2)  // �����USART2������USART2 MSP��ʼ��
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();        // ʹ��GPIOAʱ��
        __HAL_RCC_USART2_CLK_ENABLE();       // ʹ��USART2ʱ��
    
        GPIO_Initure.Pin = GPIO_PIN_2;       // PA2
        GPIO_Initure.Mode = GPIO_MODE_AF_PP; // �����������
        GPIO_Initure.Pull = GPIO_PULLUP;     // ����
        GPIO_Initure.Speed = GPIO_SPEED_FAST;// ����
        GPIO_Initure.Alternate = GPIO_AF7_USART2; // ����ΪUSART2
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA2

        GPIO_Initure.Pin = GPIO_PIN_3;       // PA3
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA3
        
        HAL_NVIC_EnableIRQ(USART2_IRQn);        // ʹ��USART2�ж�ͨ��
        HAL_NVIC_SetPriority(USART2_IRQn, 0x02, 0x02); // ��ռ���ȼ�1�������ȼ�3
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //�ж�ִ��������  ��  �жϷ���������
{
	
	if(huart->Instance==USART1)//����Ǵ���1
	{
		
		receive_str[uart_byte_count]=aRxBuffer[0];
		uart_byte_count++;
	
	}
	else if(huart->Instance==USART2)//����Ǵ���2
	{
		receive2_str[uart2_byte_count]=aRxBuffer2[0];
		uart2_byte_count++;
	
	}

}