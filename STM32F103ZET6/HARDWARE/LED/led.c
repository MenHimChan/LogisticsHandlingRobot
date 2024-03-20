#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//LED驱动代码	 

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com	

//LED0 --  PB9
//LED1 --  PE5


//********************************************************************************								  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB9和PE5为输出口.并使能这两个口的时钟		

//LED IO初始化


void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 			 //LED0-->PB.9 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);									 //根据设定参数初始化PB.9
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						             //PB.9 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 		//LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 			//推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 						//PE.5 输出高 
}




void test_stp_moto(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 			 //LED0-->PB.9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //IO口速度为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);									 //根据设定参数初始化PB.9
//	GPIO_SetBits(GPIOB,GPIO_Pin_9);					


}






























//********************************************************************************

//LED驱动代码	 

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com	

//********************************************************************************	
 
