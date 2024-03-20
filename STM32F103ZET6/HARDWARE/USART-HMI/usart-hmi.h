#ifndef __USARTHMI_H
#define __USARTHMI_H		   
#include "sys.h"

//extern u8 Flag_go_measure ;      // 开始测温标志位
//extern u8 Flag_inTemp_page ;	 // 在测温页面的标志位，意思：只有在测温页面喊“我要测体温”才有用。
//extern u8 str_show_buff2[64] ;   // 为了让LCD屏幕能显示标志位状态而设定的字符缓存区
//extern u8 Flag_inIris_page;	 	 // 跳转进入虹膜识别页面的标志位
//extern u8 str_show_buff4[64];	 // 该标志位的字符串缓冲区



void uart3_init(u32 bound);		// HMI串口3初始化
void HMISends(char *buf1); 		// 发送一个字符串
void HMI_Sendb3times(u8 k);		// 连续三次发送一个字节 用于做字符串发送完毕后的结束标志
void HMISendstart(void);		// 发送一个起始信号，在主函数里面调用。

#endif






