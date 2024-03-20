#include "Track.h"
#include "Pulse.h"
int count_flag = 0;
int point_count = 0;
int begin_track = 0;
int aim_count = 0;
int track_stop_dire_for_tim = 0;
int track_stop_pos_for_tim = 0;
int track_use_dire = -1;
int track_twoside = 0;

void Track_Gpio_Init(void){

		GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);    	//ʹ��GPIODʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);    	//ʹ��GPIOEʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);    	//ʹ��GPIOGʱ��
	
	
		//-----------------------------------------Track1-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                      	//PE7
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                      	//PE8
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PE9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                      	//PE10
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                      	//PE11
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
		}
		
		//-----------------------------------------Track2-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                      //PE12
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                                      //PE13
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;                                      //PE14
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                                      //PE15
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                      	//PG7
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
		}
		
		//-----------------------------------------Track3-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                      	//PG2
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                      	//PG3
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                      	//PG4
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                      	//PG5
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                      	//PG6
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//��ʼ��GPIO��
		}
		
		//-----------------------------------------Track4-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                      	//PD8
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PD9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                      //PD10
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//��ʼ��GPIO��
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                      //PD11
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//��ʼ��GPIO��
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                      //PD12
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//��������
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//��ʼ��GPIO��
		}


}

void Led_Gpio_Init(void){

		GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);    	//ʹ��GPIODʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PD0
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        	//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);																					//��ʼ��GPIO��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                      	//PD1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        	//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//ָ�������������Ϊ50MHz
		GPIO_Init(GPIOE, &GPIO_InitStructure);																					//��ʼ��GPIO��

}


u8 Track_Get_Result(int Dire, int Pos){

		if(Dire == Track_Dire_Up){
			
				if(Pos == Track_Pos_1){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
				
				}else
		
				if(Pos == Track_Pos_2){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
				
				}else
				
				if(Pos == Track_Pos_3){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
				
				}else
				
				if(Pos == Track_Pos_4){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10);
				
				}
				
				if(Pos == Track_Pos_5){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11);
				
				}
		
		}else
		
		if(Dire == Track_Dire_Down){
				
				if(Pos == Track_Pos_1){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);
				
				}else
		
				if(Pos == Track_Pos_2){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);
				
				}else
				
				if(Pos == Track_Pos_3){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);
				
				}else
				
				if(Pos == Track_Pos_4){
				
						return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15);
				
				}
				
				if(Pos == Track_Pos_5){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7);
				
				}
		
		}else
		
		if(Dire == Track_Dire_Left){
				
				if(Pos == Track_Pos_1){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_2);
				
				}else
		
				if(Pos == Track_Pos_2){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3);
				
				}else
				
				if(Pos == Track_Pos_3){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_4);
				
				}else
				
				if(Pos == Track_Pos_4){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_5);
				
				}
				
				if(Pos == Track_Pos_5){
				
						return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6);
				
				}
		
		}else
		
		if(Dire == Track_Dire_Right){
				
				if(Pos == Track_Pos_1){
				
						return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
				
				}else
				
				if(Pos == Track_Pos_2){
				
						return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
				
				}else
				
				if(Pos == Track_Pos_3){
				
						return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
				
				}else
				
				if(Pos == Track_Pos_4){
				
						return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
				
				}else
					
				if(Pos == Track_Pos_5){
					
						return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12);
		
				}
	}
		
		return 0;

}

void TIM4_Init_for_Trace(void){

	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseStructure;							//��ʱ����ʼ���ṹ��
    NVIC_InitTypeDef	        NVIC_InitStructure;								//�жϳ�ʼ���ṹ��
	u16 ARR; 																	//�Զ���װֵ
	u16 PSC = 1;																//Ԥ��Ƶϵ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 						//ʱ��ʹ��
		
	//����PWM����Ƶ���趨��ʱ��ʱ�ӵ�Ԥ��Ƶϵ��
    while(SystemCoreClock / PSC / 150 > U16_MAX){
			
        PSC++;																	//���Զ���װֵ����65535ʱ��Ԥ��Ƶϵ����1
			
    }
		
	ARR = SystemCoreClock / PSC / 150;															//�����Զ���װֵ
    
    TIM_TimeBaseStructure.TIM_Period = ARR;                  			//�Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = PSC;                    //��Ƶϵ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);								//��ʼ��tim4��ʱ��
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);										//��������жϱ�־λ
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);										//ʹ�ܸ����ж�4
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�ж����3
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;								//��ʱ��3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;      
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void Trace_Tim_Cmd(int count_num, int stop_dire, int stop_pos, int twoside, int track_dir){
		
		count_flag = 0;																								//��ʼ��
		point_count = 0;
		begin_track = 1;
		aim_count = count_num;
		track_stop_dire_for_tim = stop_dire;													//������ȫ��
		track_stop_pos_for_tim = stop_pos;
		track_use_dire = track_dir;
		track_twoside = twoside;
	
		TIM4_Init_for_Trace();																				//TIM4��ʼ��

		TIM_Cmd(TIM4, ENABLE);																				//ʹ�ܴӶ�ʱ��

}

int trace_finish(void){

		return !begin_track;

}

void TIM4_IRQHandler(void){
		int dx;																									//delta x
		int tx;
		int ax;
		
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){			//�����ж�
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					//�����־λ
				
				dx = (
							Track_Get_Result(track_use_dire, Track_Pos_1) || Track_Get_Result(track_use_dire, Track_Pos_2) 
							)
						- ( 
							Track_Get_Result(track_use_dire, Track_Pos_4) || Track_Get_Result(track_use_dire, Track_Pos_5) 
							);
				if(dx > 0 && Track_Get_Result(track_use_dire, Track_Pos_1))dx = 2;
				if(dx < 0 && Track_Get_Result(track_use_dire, Track_Pos_4))dx = -2;
				tx = (Track_Get_Result(track_stop_dire_for_tim, Track_Pos_1) - Track_Get_Result(track_stop_dire_for_tim, Track_Pos_5) || (!Track_Get_Result(track_stop_dire_for_tim, Track_Pos_1) || !Track_Get_Result(track_stop_dire_for_tim, Track_Pos_5)));
																														//��ȡƫ�Ʒ���
				
				if(Track_Get_Result(track_stop_dire_for_tim, track_stop_pos_for_tim) && (!track_twoside || tx == 0)){
						if(count_flag == 0){														//ѭ������
								point_count++;															//����
								count_flag = 1;															//ʹ���ɼ���
								if(point_count >= aim_count){								//����ֵ����Ŀ��ֵ
										Motor_stop();														//ֹͣ���
										begin_track = 0;												//��־λ
										TIM_Cmd(TIM4, DISABLE);									//���ö�ʱ��
								}
						}
				}else{
						count_flag = 0;																	//ʹ�ɼ���
				
						if(dx == 0){
								Change_Direction_While_Going(0);						//ֱ��
						}
						if(dx == -1){
								Change_Direction_While_Going(1);						//����ƫ
						}
						if(dx == 1){
								Change_Direction_While_Going(2);						//����ƫ
						}
						
						if(dx == -2){
								Change_Direction_While_Going(3);						//����ƫ
						}
						if(dx == 2){
								Change_Direction_While_Going(4);						//����ƫ
						}
				}
		}
	
}




