#include "DHT11.h"


void DHT11_IO_OUT (void){ //��ʪ��ģ���������
	
	
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           
 
	GPIO_Initure.Pin=GPIO_PIN_6; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������    
    GPIO_Initure.Pull= GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
}

void DHT11_IO_IN (void){ //��ʪ��ģ�����뺯��
	
	
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           
	GPIO_Initure.Pin=GPIO_PIN_6; 
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //��������    
    GPIO_Initure.Pull= GPIO_NOPULL;          //���ϡ���
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	

}

void DHT11_RST (void){ 						//DHT11�˿ڸ�λ��������ʼ�źţ�IO���ͣ�
	
	DHT11_IO_OUT();							//�˿�Ϊ���
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	delay_us(30);
	//GPIO_ResetBits(DHT11_PORT,DHT11_IO); 	//ʹ����Ϊ�͵�ƽ
	//Delay_ms(20); 							//��������18ms						
	//GPIO_SetBits(DHT11_PORT,DHT11_IO); 		//ʹ����Ϊ�ߵ�ƽ							
	//Delay_us(30); 							//��������20~40us
}

u8 DHT11_Check(void){ 	//�ȴ�DHT11��Ӧ������1:δ��⵽DHT11������0:�ɹ���IO���գ�	   
    u8 retry=0;			//������ʱ����
    DHT11_IO_IN();		//IO������״̬	 
//GPIO�˿�����ʱ������Ϊ����������߸������룬��Ϊ����������裬����Ĭ��Ϊ�ߵ�ƽ
//���DHT11������������Ϊ�ߵ�ƽ���� retry С��100���� retry ��1������ʱ1΢�룬�ظ��������ֱ�� retry ���ڵ���100 ����DHT11�������������ɵ͵�ƽ����� retry ���ڵ���100����ʾ���ʧ�ܣ�����1�����򣬽� retry ����Ϊ0��
	while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 0) && retry<100)	//DHT11������40~80us
	{
		
		retry++;
        //Delay_us(1);
		delay_us(1);
    }
    if(retry>=100)return 1; 	
	else retry=0;
//���DHT11������������Ϊ�͵�ƽ���� retry С��100���� retry ��1������ʱ1΢�룬�ظ��������ֱ�� retry ���ڵ���100 ����DHT11�������������ɸߵ�ƽ����� retry ���ڵ���100����ʾ���ʧ�ܣ�����1�����򣬷���0����ʾ���ɹ���
    while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 1) && retry<100)  //DHT11���ͺ���ٴ�����40~80us
	{  
        retry++;
		delay_us(1);
        //Delay_us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

u8 DHT11_Init (void){	//DHT11��ʼ��
	//RCC_APB2PeriphClockCmd(DHT11_RCC,ENABLE);	//��ʼDHT11��ʱ��
	  __HAL_RCC_GPIOA_CLK_ENABLE(); 
	DHT11_RST();								//DHT11�˿ڸ�λ��������ʼ�ź�
	return DHT11_Check(); 						//�ȴ�DHT11��Ӧ
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;
    while((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)== 1) && retry < 100) //�ȴ���Ϊ�͵�ƽ
    {
        retry++;
        //Delay_us(1);
		delay_us(1);
    }
    retry = 0;
    while((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 0) && retry < 100) //�ȴ���ߵ�ƽ
    {
        retry++;
        //Delay_us(1);
		delay_us(1);
    }
    //Delay_us(40);//�ȴ�40us
	delay_us(40);
    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)== 1)       //�����жϸߵ͵�ƽ��������1��0
        return 1;
    else
        return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;					//���������,dat����1λ
        dat |= DHT11_Read_Bit();	//"|"��ʾ��λ�����
    }
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;
    DHT11_RST();						//DHT11�˿ڸ�λ��������ʼ�ź�
    if(DHT11_Check() == 0)				//�ȴ�DHT11��Ӧ��0Ϊ�ɹ���Ӧ
    {
        for(i = 0; i < 5; i++) 			//��ȡ40λ����
        {
            buf[i] = DHT11_Read_Byte();	//��������
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])	//����У��
        {
            *humi = buf[0];				//��ʪ��ֵ����ָ��humi
            *temp = buf[2];				//���¶�ֵ����ָ��temp
        }
    }
    else return 1;
    return 0;
}







