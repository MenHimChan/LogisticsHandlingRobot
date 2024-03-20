#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//������������	  

// WKUP -- PA0      WKUP ���� --���º󣬸ߵ�ƽ  
// K0 -- PE4        K0   ���� --���º󣬵͵�ƽ  

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com			

//********************************************************************************

//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);  //ʹ��PA,PEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;          //K0 -- PE4 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);							//��ʼ��PE4

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;     // WKUP -- PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);         //��ʼ��PA0

}






//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!


static u8 key_up=1;//�������ɿ���־

u8 KEY_Scan(u8 mode)
{	 
		
	if(mode)key_up=1;  //֧������		 
	
	if(key_up&&(KEY0==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		
		key_up=0;
		
		if(KEY0==0)return KEY0_PRES;
		
		else if(WK_UP==1)return WKUP_PRES;
		
	}else if(KEY0==1&&WK_UP==0)key_up=1; 	
	
 	return 0;// �ް�������
}
























//********************************************************************************

//������������	  

// WKUP -- PA0      WKUP ���� --���º󣬸ߵ�ƽ  
// K0 -- PE4        K0   ���� --���º󣬵͵�ƽ  

//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com			

//********************************************************************************











