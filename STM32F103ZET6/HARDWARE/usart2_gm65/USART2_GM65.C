/**************************************************************************************************
	@调试日志：
	2022-09-16 12：36：编写串口2发送给GM65扫码指令的函数，经测试模块能够收到。 
	2022-09-16 19：52：编写接收之后的函数 



**************************************************************************************************/


#include "USART2_GM65.h"												 //二维码模块的头文件
#include "LobotServoController.h"

u8 recv_ok = 0;       													 //接收完成标志
u8 uart_buf[32]={0};  													 //用于保存串口数据，0~6位保存着位置的数据
u8 uart_cnt=0;        													 //用于定位串口数据的位置
u8 Scan_CMD[9] = {0X7E,0X00,0X08,0X01,0X00,0X02,0X01,0XAB,0XCD};		 //扫码命令指令数组

// 串口2初始化
void usart2_init(unsigned int baud)
{
    GPIO_InitTypeDef GPIO_Init_Structure;                            //定义GPIO结构体
    USART_InitTypeDef USART_Init_Structure;                          //定义串口结构体
  	NVIC_InitTypeDef  NVIC_Init_Structure;														//定义中断结构体
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    RCC_APB2PeriphClockCmd(USART2_GPIO_CLK,  ENABLE);                 //开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);             //开启APB2总线复用时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,  ENABLE);           //开启USART2时钟
    
    //配置PA2 TX
    GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_AF_PP;                //复用推挽
    GPIO_Init_Structure.GPIO_Pin   = USART2_TX_GPIO_PIN;
    GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( USART2_GPIO_PORT, &GPIO_Init_Structure);
    
    //配置PA3 RX
    GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                //复用推挽
    GPIO_Init_Structure.GPIO_Pin   = USART2_RX_GPIO_PIN;
    GPIO_Init(USART2_GPIO_PORT, &GPIO_Init_Structure);
	
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
    USART_Init_Structure.USART_BaudRate = 9600;                                          //波特率设置为115200
    USART_Init_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;       //硬件流控制为无
    USART_Init_Structure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                       //模式设为收和发
    USART_Init_Structure.USART_Parity = USART_Parity_No;                                   //无校验位
    USART_Init_Structure.USART_StopBits = USART_StopBits_1;                                //一位停止位
    USART_Init_Structure.USART_WordLength = USART_WordLength_8b;                           //字长为8位   
    USART_Init(USART2, &USART_Init_Structure);   
    USART_Cmd(USART2, ENABLE);
		
	//中断结构体配置
	NVIC_Init_Structure.NVIC_IRQChannel 		=   USART2_IRQn;
	NVIC_Init_Structure.NVIC_IRQChannelCmd   	=   ENABLE;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority  =  1;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority         =  3;
	NVIC_Init(&NVIC_Init_Structure);
}


// 每次返回的数据都是7个16进制的
// 
// 串口2中断
void USART2_IRQHandler(void)      // 串口2中断服务程序
{
	uint8_t d;
	// 检测标志位
	if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
	{					
			//LED0 = 0;
			d = USART_ReceiveData(USART2);				// 接收数据
			uart_buf[uart_cnt++] = d;  					// 存放进入数组中
		
			// 判断到底是回调信息还是QRcode信息
			if(uart_cnt == 7)				
		{
			if(uart_buf[6] == 0x31 && uart_buf[5] == 0x33 && uart_buf[4] == 0x00) 			// 命令触发模式下的回调码: 02 00 00 01 00 33 31
			{
				recv_ok = 0;                // 接收到的是回调信息，接收未完成
				uart_cnt = 0;				// 索引清零重新开始第零位存放接收数据
			}
		}
			if(d == 0x0D) 				// GM65模块发完一组数据后会自动发送一个回车符，所以通过检测是否接受到回车来判断数据是否接收完成
		{
			recv_ok = 1;  //接收完成
			sprintf((char*)SendHMI_buf1,"page0.t2.txt=\"%.7s\"",uart_buf);
			HMISends((char*)SendHMI_buf1);
			HMI_Sendb3times(0XFF);
			USART_ClearFlag(USART3,USART_FLAG_TXE);         // 清空标志位
			uart_cnt = 0; 									// 将索引复位
//			runActionGroup(0,1);
//			LED0 = 0;										// 
		}
	}
//		USART_SendData(USART1, d);
//		USART_ClearFlag(USART1,USART_FLAG_TXE);         //清空标志位
//		USART_ClearFlag(USART3,USART_FLAG_TXE);         //清空标志位
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清空标志位
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清空标志位
}	
 


// 发送扫码指令 扫码指令连续发送了三次保证接收端一定能够收的到
void Send_Scan_CMD(void)
{
	u8 i,count;
	for(count = 0; count < 3; count++)
	{
		for(i = 0; i < 9; i++)
		{
			USART_SendData(USART2,Scan_CMD[i]);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  		// 等待发送完成
		}
	}
}



