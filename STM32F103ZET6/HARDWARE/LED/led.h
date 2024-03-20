#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//LED驱动代码	 

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com	

//********************************************************************************							  
////////////////////////////////////////////////////////////////////////////////// 


#define LED0 PBout(9)// PB9
#define LED1 PEout(5)// PE5	

#define LED_D0_toggle   GPIO_WriteBit(GPIOB, GPIO_Pin_9,(BitAction)(1-GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)))  //LED显示状态翻转

#define LED_D1_toggle   GPIO_WriteBit(GPIOE, GPIO_Pin_5,(BitAction)(1-GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)))  //LED显示状态翻转


void LED_Init(void);//初始化
void test_stp_moto(void);
		 				    
#endif






































//********************************************************************************

//LED驱动代码	 

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com	

//********************************************************************************	












