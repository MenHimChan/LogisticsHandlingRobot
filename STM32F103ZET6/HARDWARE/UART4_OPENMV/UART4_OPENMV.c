/***********************************************************************************************************************
 @调试记录：
	2022-09-16 7:50：加入串口四初始化代码。经测试中断中的点灯程序能够正常执行，初步判断是原来的工程文件有问题  
	
	
	
	
	
	
	
	
	
	
	
***********************************************************************************************************************/
#include "UART4_OPENMV.h"
#include "delay.h"

u16 uart4_data_cnt = 0;
u8 uart4_data_read[100];
u8 uart4_data_flag = 0;
u8 receive_data[8] ={'1','2','3','1','2','3'};  // 接收样例 213+123\r\n  索引：0~2是上层物料 3是“+” 4~7是下层物料

 void UART4_OPENMV_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //UART4 TX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出；
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口C；
		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //UART4 RX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入；
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口C；

	USART_InitStructure.USART_BaudRate = 9600; //波特率；
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位；
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位；
	USART_InitStructure.USART_Parity = USART_Parity_No; //无校验位；
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//收发模式；

	USART_Init(UART4, &USART_InitStructure);					//配置串口参数；
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //中断号；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级；
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级；
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE); //使能串口；
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
	USART_ClearITPendingBit(UART4,USART_IT_RXNE);	//清空标志位
}


// 新的通信协议规定只发送一个'$'就让OPENMV开启识别
void Send_OpenMV_WLCMD(void)
{
	int i;
	for(i = 0; i < 3; i++)											
	{
	USART_SendData(UART4,'$');
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!= SET);  		// 等待发送完成
	delay_ms(10);
	}
}

