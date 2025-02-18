#include "DHT11.h"


void DHT11_IO_OUT (void){ //温湿度模块输出函数
	
	
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           
 
	GPIO_Initure.Pin=GPIO_PIN_6; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出    
    GPIO_Initure.Pull= GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
}

void DHT11_IO_IN (void){ //温湿度模块输入函数
	
	
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           
	GPIO_Initure.Pin=GPIO_PIN_6; 
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //浮空输入    
    GPIO_Initure.Pull= GPIO_NOPULL;          //无上、下
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	

}

void DHT11_RST (void){ 						//DHT11端口复位，发出起始信号（IO发送）
	
	DHT11_IO_OUT();							//端口为输出
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	delay_us(30);
	//GPIO_ResetBits(DHT11_PORT,DHT11_IO); 	//使总线为低电平
	//Delay_ms(20); 							//拉低至少18ms						
	//GPIO_SetBits(DHT11_PORT,DHT11_IO); 		//使总线为高电平							
	//Delay_us(30); 							//主机拉高20~40us
}

u8 DHT11_Check(void){ 	//等待DHT11回应，返回1:未检测到DHT11，返回0:成功（IO接收）	   
    u8 retry=0;			//定义临时变量
    DHT11_IO_IN();		//IO到输入状态	 
//GPIO端口输入时，配置为上拉输入或者浮空输入，因为外接上拉电阻，所以默认为高电平
//如果DHT11的数据线输入为高电平，且 retry 小于100，则将 retry 加1，并延时1微秒，重复这个过程直到 retry 大于等于100 或者DHT11的数据线输入变成低电平。如果 retry 大于等于100，表示检测失败，返回1；否则，将 retry 重置为0。
	while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 0) && retry<100)	//DHT11会拉低40~80us
	{
		
		retry++;
        //Delay_us(1);
		delay_us(1);
    }
    if(retry>=100)return 1; 	
	else retry=0;
//如果DHT11的数据线输入为低电平，且 retry 小于100，则将 retry 加1，并延时1微秒，重复这个过程直到 retry 大于等于100 或者DHT11的数据线输入变成高电平。如果 retry 大于等于100，表示检测失败，返回1；否则，返回0，表示检测成功。
    while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 1) && retry<100)  //DHT11拉低后会再次拉高40~80us
	{  
        retry++;
		delay_us(1);
        //Delay_us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

u8 DHT11_Init (void){	//DHT11初始化
	//RCC_APB2PeriphClockCmd(DHT11_RCC,ENABLE);	//开始DHT11的时钟
	  __HAL_RCC_GPIOA_CLK_ENABLE(); 
	DHT11_RST();								//DHT11端口复位，发出起始信号
	return DHT11_Check(); 						//等待DHT11回应
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;
    while((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)== 1) && retry < 100) //等待变为低电平
    {
        retry++;
        //Delay_us(1);
		delay_us(1);
    }
    retry = 0;
    while((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 0) && retry < 100) //等待变高电平
    {
        retry++;
        //Delay_us(1);
		delay_us(1);
    }
    //Delay_us(40);//等待40us
	delay_us(40);
    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)== 1)       //用于判断高低电平，即数据1或0
        return 1;
    else
        return 0;
}

//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;					//左移运算符,dat左移1位
        dat |= DHT11_Read_Bit();	//"|"表示按位或等于
    }
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;
    DHT11_RST();						//DHT11端口复位，发出起始信号
    if(DHT11_Check() == 0)				//等待DHT11回应，0为成功回应
    {
        for(i = 0; i < 5; i++) 			//读取40位数据
        {
            buf[i] = DHT11_Read_Byte();	//读出数据
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])	//数据校验
        {
            *humi = buf[0];				//将湿度值放入指针humi
            *temp = buf[2];				//将温度值放入指针temp
        }
    }
    else return 1;
    return 0;
}







