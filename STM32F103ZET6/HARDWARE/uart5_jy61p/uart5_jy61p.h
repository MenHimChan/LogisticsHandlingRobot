#ifndef __JY_61__
#define __JY_61__

#include "sys.h"
#include "led.h"
#include "JY61.h"  // ���ô�������ͽṹ�嶨��



#define CALCULATE_ANGLE(ANGLE) 	(ANGLE/32768*180)		// ���ڼ�����ص��ĽǶ�ֵ


// ��main�ж�������ĸ����Ƕȴ�ű���
extern struct SAcc 		stcAcc;
extern struct SGyro 	stcGyro;
extern struct SAngle 	stcAngle;
extern int update;

void UART5_Init(u32 baud);
void JY61_Init(u32 baudrate,u8 comfirm_ACC_or_not);
void JY61_Send_Cmd(char cmd[],char cmd_len);
void CopeSerial2Data(unsigned char ucData);    	  // CopeSerialDataΪ����5�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������













#endif
