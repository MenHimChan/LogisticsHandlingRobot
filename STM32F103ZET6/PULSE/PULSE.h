#ifndef PULSE_H
#define PULSE_H

#include "sys.h"


#define U16_MAX 65535

#define Dir 1.8														//步进角

#define M_Step 16													//细分

#define Max_Speed 6400

extern u16 dc;

extern u32 CCR1_Val;											//通道一频率
extern u32 CCR2_Val;											//通道二频率
extern u32 CCR3_Val;											//通道三频率
extern u32 CCR4_Val;											//通道四频率

typedef struct Motor_Go_Command{
	
		u16 Stp;															//脉冲数
		u16 Per1;															//电机1脉冲细分
		u16 Per2;															//电机2脉冲细分
		u16 Per3;															//电机3脉冲细分
		u16 Per4;															//电机4脉冲细分
	
		u8 Dire1;															//电机1方向
		u8 Dire2;															//电机2方向
		u8 Dire3;															//电机3方向
		u8 Dire4;															//电机4方向
		struct Motor_Go_Command* next;				//用于链表

}Motor_Go_Command;												//指令结构体

extern Motor_Go_Command* first;						//链表头

extern Motor_Go_Command* last;						//链表尾

extern u8 Command_Finished;								//指令执行完成



extern u16 ARRto;
extern u32 Num1;
extern u32 Num2;
extern u32 Num3;
extern u32 Num4;
extern u32 NowCommandCount;
extern u32 NowCommandPercent[4];


extern Motor_Go_Command* Now;
extern u8 direction_change;
extern u8 go_direction;









void TIM3_Init(u16 PulseNum, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4);													//从定时器初始化

void Pulse_Init(u16 Hertz, u16 DutyCycle);																														//主定时器初始化

void Motor_GPIO_config(void);																																								//IO口初始化

void PWM_Output(u16 Freq, u16 DutyCycle, u16 PulseNum, u16 per1, u16 per2, u16 per3, u16 per4);				//脉冲输出设置

void push_command(u16 PulseNum, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]);					//指令弹入链表

void pop_command(void);																																								//指令弹出链表

void push_command_front(u16 Stp, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]);					//指令弹入链表头

void Motor_Go_Step(u16 Stp, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);											//按脉冲数旋转电机

void Motor_Go_Angle(u16 Angle, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);										//按角度旋转电机

void Motor_Go_Circle(u16 Num, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);										//按圈数旋转电机

void Motor_Move_Up_Circle(u16 Num);																																		//按圈数向上移动
	
void Motor_Move_Down_Circle(u16 Num);																																	//按圈数向下移动
	
void Motor_Move_Left_Circle(u16 Num);																																	//按圈数向左移动
	
void Motor_Move_Right_Circle(u16 Num);																																//按圈数向右移动

void Motor_Move_Left_Angle(u16 Num);

void Motor_Move_Right_Angle(u16 Num);

u8 Execute_the_command_End(void);

void Motor_Move_Left_Rotate(u16 Num);

void Motor_Move_Right_Rotate(u16 Num);

void Motor_Move_Up(u16 Num);

void Motor_Move_Down(u16 Num);
	
void Motor_Move_Left(u16 Num);
	
void Motor_Move_Right(u16 Num);

void Motor_Move_Left_Up_Circle(u16 Num);

void Motor_Move_Left_Down_Circle(u16 Num);

void Motor_Move_Right_Up_Circle(u16 Num);

void Motor_Move_Right_Down_Circle(u16 Num);

void Change_Direction_While_Going(u8 dire);

void Clear_Command(void);

void Motor_stop(void);
#endif





