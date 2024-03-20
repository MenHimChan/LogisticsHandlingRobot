#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//LED��������	 

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com	

//LED0 --  PB9
//LED1 --  PE5


//********************************************************************************								  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB9��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		

//LED IO��ʼ��


void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 			 //LED0-->PB.9 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);									 //�����趨������ʼ��PB.9
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						             //PB.9 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 		//LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 			//������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 						//PE.5 ����� 
}




void test_stp_moto(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 			 //LED0-->PB.9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);									 //�����趨������ʼ��PB.9
//	GPIO_SetBits(GPIOB,GPIO_Pin_9);					


}






























//********************************************************************************

//LED��������	 

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com	

//********************************************************************************	
 
