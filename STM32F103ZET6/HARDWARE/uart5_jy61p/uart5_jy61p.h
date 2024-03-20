#ifndef __JY_61__
#define __JY_61__

#include "sys.h"
#include "led.h"
#include "JY61.h"  // 内置串口命令和结构体定义



#define CALCULATE_ANGLE(ANGLE) 	(ANGLE/32768*180)		// 用于计算出回调的角度值


// 在main中定义出来的各个角度存放变量
extern struct SAcc 		stcAcc;
extern struct SGyro 	stcGyro;
extern struct SAngle 	stcAngle;
extern int update;

void UART5_Init(u32 baud);
void JY61_Init(u32 baudrate,u8 comfirm_ACC_or_not);
void JY61_Send_Cmd(char cmd[],char cmd_len);
void CopeSerial2Data(unsigned char ucData);    	  // CopeSerialData为串口5中断调用函数，串口每收到一个数据，调用一次这个函数。













#endif
