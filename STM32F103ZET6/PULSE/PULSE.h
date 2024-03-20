#ifndef PULSE_H
#define PULSE_H

#include "sys.h"


#define U16_MAX 65535

#define Dir 1.8														//������

#define M_Step 16													//ϸ��

#define Max_Speed 6400

extern u16 dc;

extern u32 CCR1_Val;											//ͨ��һƵ��
extern u32 CCR2_Val;											//ͨ����Ƶ��
extern u32 CCR3_Val;											//ͨ����Ƶ��
extern u32 CCR4_Val;											//ͨ����Ƶ��

typedef struct Motor_Go_Command{
	
		u16 Stp;															//������
		u16 Per1;															//���1����ϸ��
		u16 Per2;															//���2����ϸ��
		u16 Per3;															//���3����ϸ��
		u16 Per4;															//���4����ϸ��
	
		u8 Dire1;															//���1����
		u8 Dire2;															//���2����
		u8 Dire3;															//���3����
		u8 Dire4;															//���4����
		struct Motor_Go_Command* next;				//��������

}Motor_Go_Command;												//ָ��ṹ��

extern Motor_Go_Command* first;						//����ͷ

extern Motor_Go_Command* last;						//����β

extern u8 Command_Finished;								//ָ��ִ�����



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









void TIM3_Init(u16 PulseNum, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4);													//�Ӷ�ʱ����ʼ��

void Pulse_Init(u16 Hertz, u16 DutyCycle);																														//����ʱ����ʼ��

void Motor_GPIO_config(void);																																								//IO�ڳ�ʼ��

void PWM_Output(u16 Freq, u16 DutyCycle, u16 PulseNum, u16 per1, u16 per2, u16 per3, u16 per4);				//�����������

void push_command(u16 PulseNum, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]);					//ָ�������

void pop_command(void);																																								//ָ�������

void push_command_front(u16 Stp, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]);					//ָ�������ͷ

void Motor_Go_Step(u16 Stp, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);											//����������ת���

void Motor_Go_Angle(u16 Angle, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);										//���Ƕ���ת���

void Motor_Go_Circle(u16 Num, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]);										//��Ȧ����ת���

void Motor_Move_Up_Circle(u16 Num);																																		//��Ȧ�������ƶ�
	
void Motor_Move_Down_Circle(u16 Num);																																	//��Ȧ�������ƶ�
	
void Motor_Move_Left_Circle(u16 Num);																																	//��Ȧ�������ƶ�
	
void Motor_Move_Right_Circle(u16 Num);																																//��Ȧ�������ƶ�

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





