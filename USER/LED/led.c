#include "led.h" 

/*********************************************************************************
************************�������� STM32F407ZGT6���İ�******************************
**********************************************************************************
* �ļ�����: led.c                                                                *
* �ļ�������LED��ʼ��                                                            *
* �������ڣ�2018.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����LED��ӦIO�ڳ�ʼ��                                                    * 
* �Ա����̣�https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

//LED��ӦIO��ʼ��
void LED_Init(void)
{    	 
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOGʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14; //PG13,14
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_SET);	//PG13��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);	//PG14��1��Ĭ�ϳ�ʼ�������
}






