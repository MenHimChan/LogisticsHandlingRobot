#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//LED��������	 

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com	

//********************************************************************************							  
////////////////////////////////////////////////////////////////////////////////// 


#define LED0 PBout(9)// PB9
#define LED1 PEout(5)// PE5	

#define LED_D0_toggle   GPIO_WriteBit(GPIOB, GPIO_Pin_9,(BitAction)(1-GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)))  //LED��ʾ״̬��ת

#define LED_D1_toggle   GPIO_WriteBit(GPIOE, GPIO_Pin_5,(BitAction)(1-GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)))  //LED��ʾ״̬��ת


void LED_Init(void);//��ʼ��
void test_stp_moto(void);
		 				    
#endif






































//********************************************************************************

//LED��������	 

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com	

//********************************************************************************	












