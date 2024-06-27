#include "usart.h"



/****************************************************************************
* 名    称: void HAL_UART_MspInit(UART_HandleTypeDef *huart)
* 功    能：UART底层初始化，时钟使能，引脚配置，中断配置
* 入口参数：huart:串口句柄
* 返回参数：无
* 说    明：此函数会被HAL_UART_Init()调用 
****************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,0x02,0x02);			//抢占优先级1，子优先级3	
	}
	else if(huart->Instance == USART2)  // 如果是USART2，进行USART2 MSP初始化
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();        // 使能GPIOA时钟
        __HAL_RCC_USART2_CLK_ENABLE();       // 使能USART2时钟
    
        GPIO_Initure.Pin = GPIO_PIN_2;       // PA2
        GPIO_Initure.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_FAST;// 高速
        GPIO_Initure.Alternate = GPIO_AF7_USART2; // 复用为USART2
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA2

        GPIO_Initure.Pin = GPIO_PIN_3;       // PA3
        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA3
        
        HAL_NVIC_EnableIRQ(USART2_IRQn);        // 使能USART2中断通道
        HAL_NVIC_SetPriority(USART2_IRQn, 0x02, 0x02); // 抢占优先级1，子优先级3
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //中断执行任务函数  被  中断服务函数调用
{
	
	if(huart->Instance==USART1)//如果是串口1
	{
		
		receive_str[uart_byte_count]=aRxBuffer[0];
		uart_byte_count++;
	
	}
	else if(huart->Instance==USART2)//如果是串口2
	{
		receive2_str[uart2_byte_count]=aRxBuffer2[0];
		uart2_byte_count++;
	
	}

}