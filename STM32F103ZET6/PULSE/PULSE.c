#include "PULSE.h"
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "stdlib.h"
#include "track.h"

u16 dc = 50;
u32 CCR1_Val = 0;
u32 CCR2_Val = 0;
u32 CCR3_Val = 0;
u32 CCR4_Val = 0;
u16 ARRto = 0;
u32 Num1 = 0;
u32 Num2 = 0;
u32 Num3 = 0;
u32 Num4 = 0;
u32 NowCommandCount = 0;
u32 NowCommandPercent[4] = {1, 1, 1, 1};

Motor_Go_Command* first = NULL;
Motor_Go_Command* last = NULL;

Motor_Go_Command* Now = NULL;

u8 Command_Finished = 1;
u8 direction_change = 0;
u8 go_direction = 0;
u16 now_Freq = 0;

void Motor_GPIO_config(void){
	
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);     	//ʹ��TIM3 | TIM2ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);    	//ʹ��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);    	//ʹ��GPIOBʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                      	//TIM3-CH1: PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);																					//��ʼ��GPIO��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                                      	//TIM3-CH1: PA7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);																					//��ʼ��GPIO��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                      	//TIM3-CH1: PB0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                      	//TIM3-CH1: PB1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
		
		
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                      	//TIM3-CH1: PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                                      	//TIM3-CH1: PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                      	//TIM3-CH1: PA4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                      	//TIM3-CH1: PA5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIO��
}

//A6 A7 B0 B1
void Pulse_Init(u16 Hertz, u16 DutyCycle){
		
		u16 ARR; 																											//�Զ���װֵ
		u16 PSC = 1;																									//Ԥ��Ƶϵ��
	
		TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;							//��ʱ����ʼ���ṹ��
		
		//����PWM����Ƶ���趨��ʱ��ʱ�ӵ�Ԥ��Ƶϵ��
    while(SystemCoreClock / PSC / Hertz > U16_MAX){
			
        PSC++;																										//���Զ���װֵ����65535ʱ��Ԥ��Ƶϵ����1
			
    }
		
		ARR = SystemCoreClock / PSC / Hertz;											//�����Զ���װֵ
		
		//��ʱ����ʼ��
		ARRto = ARR - 1;
		TIM_TimeBaseStructure.TIM_Period = (ARR - 1);       					//���Զ���װֵ��Ϊ����
		TIM_TimeBaseStructure.TIM_Prescaler = (PSC - 1);    					//����Ԥ��Ƶϵ��
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//ʹ����Ӧ����Ƶ�ʣ�����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);               //��ʼ��TIM3
		
		TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable); //��ʱ��Ϊ����ʱ��
		TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);        //ѡ�񴥷���ʽ��ʹ�ø����¼���Ϊ�������
}


void TIM3_Init(u16 PulseNum, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4){

		TIM_TimeBaseInitTypeDef	  TIM_TimeBaseStructure;							//��ʱ����ʼ���ṹ��
		TIM_OCInitTypeDef		    	TIM_OCInitStructure;               	//��ʱ������ṹ��
    NVIC_InitTypeDef	        NVIC_InitStructure;									//�жϳ�ʼ���ṹ��
    
    TIM_TimeBaseStructure.TIM_Period = PulseNum;                  //���������˴����ж�ֹͣ����
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);								//��ʼ��tim3��ʱ��
	
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;						  //���ù���ģʽ��pwm
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //������ԣ��͵�ƽ��Ч
		
		CCR1_Val = Multi1;
		Num1 = 1;
		TIM_OCInitStructure.TIM_Pulse = Multi1;												//���ô�װ�벶��Ĵ���������ֵ
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);                     	//��ʼ�� TIM3-OC1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);            		
		
		CCR2_Val = Multi2;
		Num2 = 1;
		TIM_OCInitStructure.TIM_Pulse = Multi2;												//���ô�װ�벶��Ĵ���������ֵ
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);                     	//��ʼ�� TIM3-OC2
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);            		
		
		
		CCR3_Val = Multi3;
		Num3 = 1;
		TIM_OCInitStructure.TIM_Pulse = Multi3;												//���ô�װ�벶��Ĵ���������ֵ
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);                     	//��ʼ�� TIM3-OC3
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);            		
		
		CCR4_Val = Multi4;
		Num4 = 1;
		TIM_OCInitStructure.TIM_Pulse = Multi4;												//���ô�װ�벶��Ĵ���������ֵ
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);                     	//��ʼ�� TIM3-OC4
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);            		
		
 
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);                   	// TIM3-����TIM2-�� ITR2
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);          	// TIM4��ģʽ��ѡ�񴥷�����TRGI����������Ϊ�������ṩʱ��
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);                  	// ����TIM2�����ж�
		
		TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);               //�ж����3
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;								//��ʱ��3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;      
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void PWM_Output(u16 Freq, u16 DutyCycle, u16 PulseNum, u16 per1, u16 per2, u16 per3, u16 per4){
		now_Freq = Freq;
		NowCommandCount = PulseNum;
		TIM3_Init(PulseNum, per1, per2, per3, per4);									//�Ӷ�ʱ����ʼ��
		TIM_Cmd(TIM3, ENABLE);																				//ʹ�ܴӶ�ʱ��
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//��������жϱ�־λ
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);										//ʹ�ܸ����ж�
		Pulse_Init(Freq, DutyCycle);																	//����ʱ����ʼ��
		TIM_Cmd(TIM2, ENABLE);																				//ʹ������ʱ��
}

void Change_Direction_While_Going(u8 dire){
		if(direction_change != dire){
				if(dire == 0 && direction_change != 0){
						direction_change = dire;
						TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
						TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
						TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
						TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
						PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 1);
				}
				direction_change = dire;
				if(go_direction == 1){
						if(dire == 1){
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 2, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 2){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 2);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 3){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 4, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 4){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 4);
								//Motor_Move_Up_Circle(1);
						}
				}else
				if(go_direction == 2){
						if(dire == 1){
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 2);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 2){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 2, 1, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 3){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 4);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 4){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 4, 1, 1, 1);
								//Motor_Move_Up_Circle(1);
						}
				}else
				if(go_direction == 3){
						if(dire == 1){
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 2, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 2){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 2, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 3){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 4, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 4){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 1, 4, 1);
								//Motor_Move_Up_Circle(1);
						}
				}else
				if(go_direction == 4){
						if(dire == 1){
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 2, 1, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 2){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 2, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 3){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 4, 1, 1, 1);
								//Motor_Move_Up_Circle(1);
						}else
						if(dire == 4){
						
								TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
								TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
								TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
								TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
								PWM_Output(now_Freq, 50, now_Freq, 1, 4, 1, 1);
								//Motor_Move_Up_Circle(1);
						}
				}
		}

}

void push_command(u16 Stp, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]){

		Motor_Go_Command* tmp;																				//��ʱָ��
	
		tmp = (Motor_Go_Command*)malloc(sizeof(Motor_Go_Command));		//����ռ�
		
		tmp->Stp = Stp;																								//����ָ������
		
		tmp->Per1 = Multi1;
		
		tmp->Per2 = Multi2;
		
		tmp->Per3 = Multi3;
		
		tmp->Per4 = Multi4;
		
		tmp->Dire1 = Dire[0];
		
		tmp->Dire2 = Dire[1];
		
		tmp->Dire3 = Dire[2];
		
		tmp->Dire4 = Dire[3];
	
		if(first == NULL){																						//��ָ����������
				first = tmp;
				last = tmp;
		}else{
				last->next = tmp;
				last = tmp;
		}
	
}

void push_command_front(u16 Stp, u16 Multi1, u16 Multi2, u16 Multi3, u16 Multi4, u8 Dire[4]){
	
		//��ʱ����

		Motor_Go_Command* tmp, *tmpfirst;																				//��ʱָ��
	
		tmp = (Motor_Go_Command*)malloc(sizeof(Motor_Go_Command));		//����ռ�
	
		tmpfirst = (Motor_Go_Command*)malloc(sizeof(Motor_Go_Command));		//����ռ�
		
		tmp->Stp = Stp;																								//����ָ������
		
		tmp->Per1 = Multi1;
		
		tmp->Per2 = Multi2;
		
		tmp->Per3 = Multi3;
		
		tmp->Per4 = Multi4;
		
		tmp->Dire1 = Dire[0];
		
		tmp->Dire2 = Dire[1];
		
		tmp->Dire3 = Dire[2];
		
		tmp->Dire4 = Dire[3];
		
		tmpfirst->Stp = first->Stp;
		
		tmpfirst->Per1 = first->Per1;
		
		tmpfirst->Per2 = first->Per2;
		
		tmpfirst->Per3 = first->Per3;
		
		tmpfirst->Per4 = first->Per4;
		
		tmpfirst->Dire1 = first->Dire1;
		
		tmpfirst->Dire2 = first->Dire2;
		
		tmpfirst->Dire3 = first->Dire3;
		
		tmpfirst->Dire4 = first->Dire4;
		
		tmpfirst->next = first->next;
		
		tmp->next = tmpfirst;
		
		free(first);
		
		first = tmp;
	
}

void pop_command(){

		Motor_Go_Command* tmp;																				//��ʱ����
	
		if(first == NULL)return;																			//�����в�û��ָ��
		
		tmp = first;																									//��ʱָ�������ȡ����ͷ��ָ��
		
		first = first->next;																					//����ͷָ����һ��ָ��
		
		free(tmp);																										//�ͷ�ԭ����ͷռ���ڴ�
	
}

void Motor_Go_Step(u16 Stp, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]){
		
		if(Command_Finished == 1){																		//ָ��ִ�н���
			
				//��������
				if(Dire[0] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
				}
				
				if(Dire[1] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_1);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_1);
				}
				
				if(Dire[2] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
				}
				
				if(Dire[3] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
				}
				
				PWM_Output(Stp, 50, Stp, Per1, Per2, Per3, Per4);					//һ���ӷ�stp������
				
				Command_Finished = 0;																			//����δ����ָ��״̬
				
		}else{																												//ָ������ִ��
		
				push_command(Stp, Per1, Per2, Per3, Per4, Dire);								//��ָ���������
		
		}
	
}

void Motor_Go_Angle(u16 Angle, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]){
	
		float stp = Angle / Dir * M_Step;															//������ת�Ƕ����õ�������

		if(Command_Finished == 1){																		//ͬstep
			
				if(Dire[0] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
				}
				
				if(Dire[1] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_1);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_1);
				}
				
				if(Dire[2] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
				}
				
				if(Dire[3] == 0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				}else{
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
				}
				
				PWM_Output(stp, 50, stp, Per1, Per2, Per3, Per4);
				
				Command_Finished = 0;
				
		}else{
		
				push_command(stp, Per1, Per2, Per3, Per4, Dire);
		
		}

}

void Motor_Go_Circle(u16 Num, u16 Per1, u16 Per2, u16 Per3, u16 Per4, u8 Dire[4]){

		//Motor_Go_Angle(Num * 360, Per1, Per2, Per3, Per4, Dire);						//
		
		Motor_Go_Angle(Num * 360, Per1, Per2, Per3, Per4, Dire);

}

void Motor_Move_Up_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 0;
		
		go_direction = 1;
		
		Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Down_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 1;
		go_direction = 4;	
	
		Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Left_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 0;
		motordir[2] = 1;
		motordir[3] = 1;
		go_direction = 2;
		
		Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Right_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 1;
		motordir[2] = 0;
		motordir[3] = 0;
	
		go_direction = 3;
		
		Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Left_Up_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 0;
		
		go_direction = 11;
		
		Motor_Go_Circle(Num, 1, 65535, 1, 65535, motordir);

}

void Motor_Move_Right_Up_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 0;
		
		go_direction = 12;
		
		Motor_Go_Circle(Num, 65535, 1, 65535, 1, motordir);

}
void Motor_Move_Left_Down_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 1;
		
		go_direction = 11;
		
		Motor_Go_Circle(Num, 1, 65535, 1, 65535, motordir);

}

void Motor_Move_Right_Down_Circle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 1;
		
		go_direction = 12;
		
		Motor_Go_Circle(Num, 65535, 1, 65535, 1, motordir);

}

void Motor_Move_Up(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 0;
		
		go_direction = 1;
		
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Down(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 1;
		
		go_direction = 4;
		
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Left(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 0;
		motordir[2] = 1;
		motordir[3] = 1;
		
		go_direction = 2;
		
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Right(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 1;
		motordir[2] = 0;
		motordir[3] = 0;
		
		go_direction = 3;
		
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}

void Motor_Move_Left_Rotate(u16 Num){

		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 1;
	
		go_direction = 5;
		
		Motor_Go_Angle(Num * 120, 1, 1, 1, 1, motordir);

}

void Motor_Move_Right_Rotate(u16 Num){

		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 0;
	
		go_direction = 6;
		
		Motor_Go_Angle(Num * 120, 1, 1, 1, 1, motordir);

}

void Motor_Move_Left_Angle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 1;
		motordir[1] = 1;
		motordir[2] = 1;
		motordir[3] = 1;
	
		go_direction = 5;
		
		//Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}
void Motor_Move_Right_Angle(u16 Num){
		
		u8 motordir[4];
	
		motordir[0] = 0;
		motordir[1] = 0;
		motordir[2] = 0;
		motordir[3] = 0;
	
		go_direction = 6;
		
		//Motor_Go_Circle(Num, 1, 1, 1, 1, motordir);
		Motor_Go_Angle(Num, 1, 1, 1, 1, motordir);

}

u8 Execute_the_command_End(){

		return Command_Finished;

}

Motor_Go_Command* Get_NowCommand_Information(){
		
		Now->Stp = NowCommandCount;											//ʣ��������
		Now->Per1 = NowCommandPercent[0];								//��¼����
		Now->Per2 = NowCommandPercent[1];
		Now->Per3 = NowCommandPercent[2];
		Now->Per4 = NowCommandPercent[3];
		//Now->Dire1 = GPIO_ReadOutputDataBit();								//��¼����
		Now->Dire2 = NowCommandPercent[1];
		Now->Dire3 = NowCommandPercent[2];
		Now->Dire4 = NowCommandPercent[3];
		return Now;

}

void Clear_Command(void){
		while(first != NULL){
				pop_command();
		}
		Command_Finished = 1;
}
void Motor_stop(void){

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
		TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
		TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
		Clear_Command();

}
void TIM3_IRQHandler(void){
		
		NowCommandCount--;
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
			
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);										//����жϱ�־λ
				TIM_Cmd(TIM2, DISABLE);																				//�رն�ʱ��3
				TIM_Cmd(TIM3, DISABLE);																				//�رն�ʱ��2
				TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);										//�رո����ж�
				//PWM_Output(200, 50, 200, 1, 2, 3, 4);
				if(go_direction == 1){
						if(direction_change != 0){
								if(direction_change == 1){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 2, 1);
								}else
								if(direction_change == 2){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 2);
								}else
								if(direction_change == 3){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 4, 1);
								}else
								if(direction_change == 4){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 4);
								}
						}
				}else
				if(go_direction == 2){
						if(direction_change != 0){
								if(direction_change == 1){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 2);
								}else
								if(direction_change == 2){
										PWM_Output(now_Freq, 50, now_Freq, 2, 1, 1, 1);
								}else
								if(direction_change == 3){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 1, 4);
								}else
								if(direction_change == 4){
										PWM_Output(now_Freq, 50, now_Freq, 4, 1, 1, 1);
								}
						}
				}else
				if(go_direction == 3){
						if(direction_change != 0){
								if(direction_change == 1){
										PWM_Output(now_Freq, 50, now_Freq, 1, 2, 1, 1);
								}else
								if(direction_change == 2){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 2, 1);
								}else
								if(direction_change == 3){
										PWM_Output(now_Freq, 50, now_Freq, 1, 4, 1, 1);
								}else
								if(direction_change == 4){
										PWM_Output(now_Freq, 50, now_Freq, 1, 1, 4, 1);
								}
						}
				}else
				if(go_direction == 4){
						if(direction_change != 0){
								if(direction_change == 1){
										PWM_Output(now_Freq, 50, now_Freq, 2, 1, 1, 1);
								}else
								if(direction_change == 2){
										PWM_Output(now_Freq, 50, now_Freq, 1, 2, 1, 1);
								}else
								if(direction_change == 3){
										PWM_Output(now_Freq, 50, now_Freq, 4, 1, 1, 1);
								}else
								if(direction_change == 4){
										PWM_Output(now_Freq, 50, now_Freq, 1, 4, 1, 1);
								}
						}
				}
				if(first != NULL){																						//�����д���ָ��
						
						if(first->Dire1 == 0){
								GPIO_ResetBits(GPIOA, GPIO_Pin_0);
						}else{
								GPIO_SetBits(GPIOA, GPIO_Pin_0);
						}
						
						if(first->Dire2 == 0){
								GPIO_ResetBits(GPIOA, GPIO_Pin_1);
						}else{
								GPIO_SetBits(GPIOA, GPIO_Pin_1);
						}
						
						if(first->Dire3 == 0){
								GPIO_ResetBits(GPIOA, GPIO_Pin_4);
						}else{
								GPIO_SetBits(GPIOA, GPIO_Pin_4);
						}
						
						if(first->Dire4 == 0){
								GPIO_ResetBits(GPIOA, GPIO_Pin_5);
						}else{
								GPIO_SetBits(GPIOA, GPIO_Pin_5);
						}
						
						PWM_Output(
								first->Stp, 50,
								first->Stp, 
								first->Per1,
								first->Per2,
								first->Per3,
								first->Per4
						);																												//������������
					
						pop_command();																						//����������ָ��
				
				}else{
						
						Command_Finished = 1;																			//������û��ָ���ô����ָ�����״̬
				
				}
		}
		
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){										//ͨ��һ�����ж�
			
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);											//�����־λ
				
        TIM_SetCompare1(TIM3, CCR1_Val * (Num1 + 1));									//���㷢����һ������ıȽ�ֵ
				
				Num1++;																												//����
			
    }
		
    if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET){										//ͬͨ��һ
			
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
				
        TIM_SetCompare2(TIM3, CCR2_Val * (Num2 + 1));
			
				Num2++;
			
    }
		
		if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET){										//ͬͨ��һ
			
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
				
        TIM_SetCompare3(TIM3, CCR3_Val * (Num3 + 1));
			
				Num3++;
			
    }
		
		if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET){										//ͬͨ��һ
			
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
				
        TIM_SetCompare4(TIM3, CCR4_Val * (Num4 + 1));
			
				Num4++;
			
    }

}


