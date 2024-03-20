
#ifndef __KEY_H
#define __KEY_H	 



#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//按键驱动代码	  

// WKUP -- PA0      WKUP 按键 --按下后，高电平  
// K0 -- PE4        K0   按键 --按下后，低电平  

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com			

//********************************************************************************						  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define WK_UP PAin(0)	  //PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0

#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键(WK_UP) 

 
 

#define KEY0_PRES 	1	//KEY0 按下
#define WKUP_PRES   2	//WKUP 按下(即WKUP/KEY_UP)


void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    

#endif













//********************************************************************************

//按键驱动代码	  

// WKUP -- PA0      WKUP 按键 --按下后，高电平  
// K0 -- PE4        K0   按键 --按下后，低电平  

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com			

//********************************************************************************









