#include "uart5_jy61p.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"



// JY-61����ָ�
// ʹ�ù����������޸�
/////////////////////////////////////////////////////
char YAWCMD[3] = {0XFF,0XAA,0X52};
char ACCCMD[3] = {0XFF,0XAA,0X67};
char SLEEPCMD[3] = {0XFF,0XAA,0X60};
char UARTMODECMD[3] = {0XFF,0XAA,0X61};
char IICMODECMD[3] = {0XFF,0XAA,0X62};
int update = 0;
/////////////////////////////////////////////////////

//static unsigned char TxBuffer[256];			// ���ͻ�����
//static unsigned char TxCounter=0;			// 
//static unsigned char count=0; 

// TX-PC12 RX-PD2
void UART5_Init(u32 baud)
{
    USART_InitTypeDef USART_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��UART5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//GPIOC/Dʱ��
    //USART_DeInit(UART5);  //��λ����5
	
    //UART5_TX   PC12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC12
    //UART5_RX	  PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);  //��ʼ��PB11
 
    //UART ��ʼ������
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(UART5, &USART_InitStructure); //��ʼ������
 
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���
	
	//Uart5 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;		// �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
 
}

/*****************************************************
 @function :JY61_Send_Cmd(char cmd[],char cmd_len)
 @descript : JY61���ú���
 @para1	   : cmd[] ��������
 @para2    : cmd_len ����ȣ�����ʹ��sizeof()����
*****************************************************/
void JY61_Send_Cmd(char cmd[],char cmd_len)
{
	char i=0;
	for(i=0;i<cmd_len;i++)
	{
		USART_SendData(UART5,cmd[i]);
		while(USART_GetFlagStatus(UART5, USART_IT_TC) == RESET);
	}
}

/*********************************************************************
 @function : JY61_Init(u32 baudrate,u8 comfirm_ACC_or_not)
 @brief	   : JY_61 ͨ�����ڵĳ�ʼ�����ڲ������˴��ں�����ʼ������
 @parament : baudrate ���������ã�
			 comfirm_ACC_or_not �Ƿ����У׼��У׼��Ҫ��ˮƽ����1sʱ��
*********************************************************************/
void JY61_Init(u32 baudrate,u8 comfirm_ACC_or_not)
{
	UART5_Init(baudrate);
	delay_ms(500);
	if(comfirm_ACC_or_not == 1)
	{
		{
			//printf("���ڽ��м��ٶ�У׼\r\n");
			JY61_Send_Cmd(ACCCMD,sizeof(ACCCMD));//�ȴ�ģ���ڲ��Զ�У׼�ã�ģ���ڲ����Զ�������Ҫһ����ʱ��
			//printf("���ٶ�У׼���\r\n");
			delay_ms(100);
			//printf("����Z��Ƕ�����\r\n");
			JY61_Send_Cmd(YAWCMD,sizeof(YAWCMD));
			//printf("Z��Ƕ��������\r\n");
			delay_ms(100);
		}
			//printf("-----------------------------------\r\n");
	}

}


//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) 		//����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;					// ����λ���ݷ��õ��Լ��Ľṹ����
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);update = 1;break;				// ���Ҫ�õ��Ƕ���Ϣ�Ļ�һ�����е�����һ�δ���
		}
		ucRxCnt=0;																//��ջ�����
	}
}


// ��������
// ���ʱ�����ж��жϵ�ʱ���ж���
void UART5_IRQHandler(void)
{
	LED1 = 0;
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{					
		CopeSerial2Data((unsigned char)UART5->DR);		//��������
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);	//��ձ�־λ
	}
	LED1 = 1;
}



