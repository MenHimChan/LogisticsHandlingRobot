#ifndef __GM65__H
#define __GM65__H
#include "sys.h"
#include "led.h"			// 用于中断点灯查看是否进入
#include "USART-hmi.h"		// 在中断程序中发送任务码到串口屏


//串口2 
#define USART2_GPIO_PORT      GPIOA
#define USART2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define USART2_TX_GPIO_PIN    GPIO_Pin_2
#define USART2_RX_GPIO_PIN    GPIO_Pin_3


extern u8 recv_ok;
extern unsigned char SendHMI_buf1[64];			// 外部声明



void usart2_init(unsigned int baud);			
void Send_Scan_CMD(void);


#endif 

