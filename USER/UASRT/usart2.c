#include "usart2.h"
#include "string.h"
#include "stdlib.h"   
#include "common.h"
#include "Alarm.h"
#include "lcd.h"

u8 receive2_str[USART2_REC_NUM];     //接收缓存数组,最大USART_REC_LEN个字节 
u8 uart2_byte_count=0;

u8 aRxBuffer2[RXBUFFERSIZE2]; //HAL库使用的串口接收缓冲
UART_HandleTypeDef UART2_Handler; //UART句柄


/****************************************************************************
* 名    称: void HAL_UART_MspInit(UART_HandleTypeDef *huart)
* 功    能：UART底层初始化，时钟使能，引脚配置，中断配置
* 入口参数：huart:串口句柄
* 返回参数：无
* 说    明：此函数会被HAL_UART_Init()调用 
****************************************************************************/
//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//    
//    if(huart->Instance == USART2)  // 如果是USART2，进行USART2 MSP初始化
//    {
//        __HAL_RCC_GPIOA_CLK_ENABLE();        // 使能GPIOA时钟
//        __HAL_RCC_USART2_CLK_ENABLE();       // 使能USART2时钟
//    
//        GPIO_Initure.Pin = GPIO_PIN_2;       // PA2
//        GPIO_Initure.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
//        GPIO_Initure.Pull = GPIO_PULLUP;     // 上拉
//        GPIO_Initure.Speed = GPIO_SPEED_FAST;// 高速
//        GPIO_Initure.Alternate = GPIO_AF7_USART2; // 复用为USART2
//        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA2

//        GPIO_Initure.Pin = GPIO_PIN_3;       // PA3
//        HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA3
//        
//        HAL_NVIC_EnableIRQ(USART2_IRQn);        // 使能USART2中断通道
//        HAL_NVIC_SetPriority(USART2_IRQn, 0x01, 0x02); // 抢占优先级1，子优先级3
//    }
//}



/****************************************************************************
* 名    称: void uart1_init(u32 bound)
* 功    能：USART1初始化
* 入口参数：bound：波特率
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart2_init(u32 bound)
{   
  //UART 初始化设置
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}


//串口1发送一个字符
void uart2SendChar(u8 ch)
{      
	while((USART2->SR&0x40)==0);  
    USART2->DR = (u8) ch;      
}


/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u16 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart2SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart2SendChar(*(str + k)); k++; }   //循环发送,直到发送完毕   
    while (k < strlen); 
} 


//串口1中断服务程序
void USART2_IRQHandler(void)  
{
	u32 timeout=0;
	
	
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0; 
	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
  } 
}

void usart2_disp()//处理一下接收到的数据
{
	LCD_DisplayString(30,70,16,receive2_str);
	
}






