/***********************************************************************************************************************
 @���Լ�¼��
	2022-09-16 7:50�����봮���ĳ�ʼ�����롣�������ж��еĵ�Ƴ����ܹ�����ִ�У������ж���ԭ���Ĺ����ļ�������  
	
	
	
	
	
	
	
	
	
	
	
***********************************************************************************************************************/
#include "UART4_OPENMV.h"
#include "delay.h"

u16 uart4_data_cnt = 0;
u8 uart4_data_read[100];
u8 uart4_data_flag = 0;
u8 receive_data[8] ={'1','2','3','1','2','3'};  // �������� 213+123\r\n  ������0~2���ϲ����� 3�ǡ�+�� 4~7���²�����

 void UART4_OPENMV_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //UART4 TX��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�˿�C��
		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //UART4 RX��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //�������룻
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�˿�C��

	USART_InitStructure.USART_BaudRate = 9600; //�����ʣ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ1λ��
	USART_InitStructure.USART_Parity = USART_Parity_No; //��У��λ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//�շ�ģʽ��

	USART_Init(UART4, &USART_InitStructure);					//���ô��ڲ�����
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //�жϺţ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ���
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //��Ӧ���ȼ���
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE); //ʹ�ܴ��ڣ�
}

void UART4_IRQHandler(void)
{
	u8 data;
//	LED0 = 0;
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE)==SET)
	{
		data = USART_ReceiveData(UART4);
		receive_data[uart4_data_cnt++] = data;
		
		if(data == 0x0A)
		{
			uart4_data_flag = 1;
			uart4_data_cnt = 0;
		}
	}
	USART_ClearITPendingBit(UART4,USART_IT_RXNE);	//��ձ�־λ
}


// �µ�ͨ��Э��涨ֻ����һ��'$'����OPENMV����ʶ��
void Send_OpenMV_WLCMD(void)
{
	int i;
	for(i = 0; i < 3; i++)											
	{
	USART_SendData(UART4,'$');
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!= SET);  		// �ȴ��������
	delay_ms(10);
	}
}

