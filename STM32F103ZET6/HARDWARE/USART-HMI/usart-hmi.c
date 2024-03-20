#include "usart-hmi.h"
#include "delay.h"

/*********************************************************
 @�������ƣ�uart3_init
 @para    : u32 bound ����Ҫ�Ĳ�����
 @retval  : NULL
 @addion  : ʹ�õ���PB10 TX PB11 RX
*********************************************************/
void uart3_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//ʱ��GPIOB��USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
}

/**************************************************************
 @funcName��HMISends
 @para    : char *buf1 �ַ����������ַ�������׵�ַ
 @retval  : NULL
 @function: ͨ������������һ���ַ�����������
 @addion  : �ڲ����USART_SendData(USART3,buf1[i])�Ĺ̼��⺯��
**************************************************************/
void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART3,buf1[i]);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		 	i++;
		}
	 else return ;
	}
}

/**************************************************************
 @funcName��HMI_Sendb3times
 @para    : char *buf1 �ַ����������ַ�������׵�ַ
 @retval  : NULL
 @function: ͨ������������һ���ַ�����������
 @addion  : �ڲ����USART_SendData(USART3,buf1[i])�Ĺ̼��⺯��
**************************************************************/
void HMI_Sendb3times(u8 k)		       //�ֽڷ��ͺ���
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART3,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else return ;
	 } 
}


/**************************************************************
 @funcName��HMISendstart
 @para    : NULL
 @retval  : NULL
 @function: ��ֹ�ϴ�Ҫ���͵����ݿ��ڶԱ��η������Ӱ��
 @addion  : �����ڳ�ʼ��������
**************************************************************/
void HMISendstart(void)
{
	 	delay_ms(200);
		HMI_Sendb3times(0xff);
		delay_ms(200);
}


// �������Դ�������printh ��ӡ��16������
void USART3_IRQHandler(void)   
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)   //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			Res = USART_ReceiveData(USART3);				// ��ȡ���յ�������
//				if(Res == 0x01) 						
//			{
//				Flag_inTemp_page = 1 ;
//				sprintf((char*)str_show_buff2,"Flag_inTemp_page = %d",Flag_inTemp_page);
//			}
//				else if(Res == 0x02) 					
//			{
//				Flag_inTemp_page = 0 ;		// ��ҳ�泬ʱ���򽫲���ҳ��ı�־λ������Ϊ0
//				sprintf((char*)str_show_buff2,"Flag_inTemp_page = %d",Flag_inTemp_page);
//			}
//				else if(Res == 0x03)		 	// Flag_iniris_page
//			{
//				Flag_inIris_page = 1;			// ���յ���������ת���������͵�0x03 ������ҳ��
//				sprintf((char*)str_show_buff4,"Flag_inIris_page = %d",Flag_inIris_page);
//			}
//		else if(Res==0x02) Mode_CorrectFlag=1;
//		else if(Res==0x03) Mode_MeasureFlag=0; 
//		else if(Res==0x10) MeasureRi_Flag=1;
//		else if(Res==0x11) MeasureRo_Flag=1;
//		else if(Res==0x12) MeasureAv_Flag=1;
			USART_ClearITPendingBit(USART3,USART_IT_RXNE);    // ����жϱ�־λ
		}
}








