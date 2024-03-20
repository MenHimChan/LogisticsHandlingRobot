#include "stm32f10x.h"
#include "string.h"
#include "uart.h"
#include "bool.h"
#include "led.h"
#include "LobotServoController.h"

u8 UART_RX_BUF[16];
bool isUartRxCompleted = false;

int fputc(int ch, FILE *f)
{
        if(ch =='\n')  
        {  
            USART_SendData(USART1, 0x0D);  
            while (!(USART1->SR & USART_FLAG_TXE)); 
            USART_SendData(USART1, 0x0A);  
            while (!(USART1->SR & USART_FLAG_TXE));  
        }  
        else  
        {  
            USART_SendData(USART1, (unsigned char)ch);  
            while (!(USART1->SR & USART_FLAG_TXE));   
        }  
	return ch;
}

void uartNVICInit(void) {
		//Usart1 NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void uartInit(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	//USART1_TX   PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1_RX	  PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART 配置
	USART_InitStructure.USART_BaudRate = bound;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送接收

	USART_Init(USART1, &USART_InitStructure);
	uartNVICInit();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) {
		while ((USART1->SR & 0x40) == 0);
		USART_SendData(USART1,*buf++);
	}
}

extern uint8_t LobotRxBuf[16];

void USART1_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //判断接收中断
		LED0 = 0;
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//取出接收寄存器数据
		if (!isGotFrameHeader) {  //判断帧头
			if (Res == FRAME_HEADER) {
				frameHeaderCount++;
				if (frameHeaderCount == 2) {
					frameHeaderCount = 0;
					isGotFrameHeader = true;
					dataCount = 1;
				}
			} else {
				isGotFrameHeader = false;
				dataCount = 0;
				frameHeaderCount = 0;
			}
		}
		if (isGotFrameHeader) { //接收接收数据部分
			UART_RX_BUF[dataCount] = Res;
			if (dataCount == 2) {
				dataLength = UART_RX_BUF[dataCount];
				if (dataLength < 2 || dataLength > 8) {
					dataLength = 2;
					isGotFrameHeader = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 2) {
				if (isUartRxCompleted == false) {
					isUartRxCompleted = true;
					memcpy(LobotRxBuf, UART_RX_BUF, dataCount);
				}
				isGotFrameHeader = false;
			}
		}
	}
}
