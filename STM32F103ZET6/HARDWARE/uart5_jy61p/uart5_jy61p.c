#include "uart5_jy61p.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"



// JY-61串口指令集
// 使用过程中请勿修改
/////////////////////////////////////////////////////
char YAWCMD[3] = {0XFF,0XAA,0X52};
char ACCCMD[3] = {0XFF,0XAA,0X67};
char SLEEPCMD[3] = {0XFF,0XAA,0X60};
char UARTMODECMD[3] = {0XFF,0XAA,0X61};
char IICMODECMD[3] = {0XFF,0XAA,0X62};
int update = 0;
/////////////////////////////////////////////////////

//static unsigned char TxBuffer[256];			// 发送缓冲区
//static unsigned char TxCounter=0;			// 
//static unsigned char count=0; 

// TX-PC12 RX-PD2
void UART5_Init(u32 baud)
{
    USART_InitTypeDef USART_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能UART5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//GPIOC/D时钟
    //USART_DeInit(UART5);  //复位串口5
	
    //UART5_TX   PC12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC12
    //UART5_RX	  PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化PB11
 
    //UART 初始化设置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(UART5, &USART_InitStructure); //初始化串口
 
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART5, ENABLE);                    //使能串口
	
	//Uart5 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;		// 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
 
}

/*****************************************************
 @function :JY61_Send_Cmd(char cmd[],char cmd_len)
 @descript : JY61设置函数
 @para1	   : cmd[] 命令数组
 @para2    : cmd_len 命令长度，建议使用sizeof()传入
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
 @brief	   : JY_61 通过串口的初始化，内部调用了串口函数初始化函数
 @parament : baudrate 波特率设置；
			 comfirm_ACC_or_not 是否进行校准，校准需要放水平将近1s时间
*********************************************************************/
void JY61_Init(u32 baudrate,u8 comfirm_ACC_or_not)
{
	UART5_Init(baudrate);
	delay_ms(500);
	if(comfirm_ACC_or_not == 1)
	{
		{
			//printf("正在进行加速度校准\r\n");
			JY61_Send_Cmd(ACCCMD,sizeof(ACCCMD));//等待模块内部自动校准好，模块内部会自动计算需要一定的时间
			//printf("加速度校准完成\r\n");
			delay_ms(100);
			//printf("进行Z轴角度清零\r\n");
			JY61_Send_Cmd(YAWCMD,sizeof(YAWCMD));
			//printf("Z轴角度清零完成\r\n");
			delay_ms(100);
		}
			//printf("-----------------------------------\r\n");
	}

}


//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) 		//数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;					// 将八位数据放置到自己的结构体中
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);update = 1;break;				// 如果要得到角度信息的话一般运行的是这一段代码
		}
		ucRxCnt=0;																//清空缓存区
	}
}


// 测试无误
// 点灯时间来判断中断的时间有多少
void UART5_IRQHandler(void)
{
	LED1 = 0;
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{					
		CopeSerial2Data((unsigned char)UART5->DR);		//处理数据
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);	//清空标志位
	}
	LED1 = 1;
}



