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
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);    	//使能GPIOD时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);    	//使能GPIOE时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);    	//使能GPIOG时钟
	
	
		//-----------------------------------------Track1-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                      	//PE7
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                      	//PE8
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PE9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                      	//PE10
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                      	//PE11
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
		}
		
		//-----------------------------------------Track2-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                      //PE12
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                                      //PE13
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;                                      //PE14
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                                      //PE15
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                      	//PG7
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
		}
		
		//-----------------------------------------Track3-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                      	//PG2
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                      	//PG3
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                      	//PG4
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                      	//PG5
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                      	//PG6
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);																					//初始化GPIO口
		}
		
		//-----------------------------------------Track4-----------------------------------------
		{
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                      	//PD8
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PD9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                      //PD10
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//初始化GPIO口
					
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                                      //PD11
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//初始化GPIO口
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                      //PD12
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			                        			//下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
				GPIO_Init(GPIOD, &GPIO_InitStructure);																					//初始化GPIO口
		}


}

void Led_Gpio_Init(void){

		GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);    	//使能GPIOD时钟
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      	//PD0
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        	//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);																					//初始化GPIO口
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                      	//PD1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			                        	//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              	//指定输入速率最大为50MHz
		GPIO_Init(GPIOE, &GPIO_InitStructure);																					//初始化GPIO口

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

	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseStructure;							//定时器初始化结构体
    NVIC_InitTypeDef	        NVIC_InitStructure;								//中断初始化结构体
	u16 ARR; 																	//自动重装值
	u16 PSC = 1;																//预分频系数
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 						//时钟使能
		
	//根据PWM脉冲频率设定定时器时钟的预分频系数
    while(SystemCoreClock / PSC / 150 > U16_MAX){
			
        PSC++;																	//当自动重装值大于65535时，预分频系数加1
			
    }
		
	ARR = SystemCoreClock / PSC / 150;															//计算自动重装值
    
    TIM_TimeBaseStructure.TIM_Period = ARR;                  			//自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = PSC;                    //分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);								//初始化tim4定时器
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);										//清除更新中断标志位
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);										//使能更新中断4
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //中断组别：3
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;								//定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;      
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void Trace_Tim_Cmd(int count_num, int stop_dire, int stop_pos, int twoside, int track_dir){
		
		count_flag = 0;																								//初始化
		point_count = 0;
		begin_track = 1;
		aim_count = count_num;
		track_stop_dire_for_tim = stop_dire;													//参数存全局
		track_stop_pos_for_tim = stop_pos;
		track_use_dire = track_dir;
		track_twoside = twoside;
	
		TIM4_Init_for_Trace();																				//TIM4初始化

		TIM_Cmd(TIM4, ENABLE);																				//使能从定时器

}

int trace_finish(void){

		return !begin_track;

}

void TIM4_IRQHandler(void){
		int dx;																									//delta x
		int tx;
		int ax;
		
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){			//更新中断
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					//清除标志位
				
				dx = (
							Track_Get_Result(track_use_dire, Track_Pos_1) || Track_Get_Result(track_use_dire, Track_Pos_2) 
							)
						- ( 
							Track_Get_Result(track_use_dire, Track_Pos_4) || Track_Get_Result(track_use_dire, Track_Pos_5) 
							);
				if(dx > 0 && Track_Get_Result(track_use_dire, Track_Pos_1))dx = 2;
				if(dx < 0 && Track_Get_Result(track_use_dire, Track_Pos_4))dx = -2;
				tx = (Track_Get_Result(track_stop_dire_for_tim, Track_Pos_1) - Track_Get_Result(track_stop_dire_for_tim, Track_Pos_5) || (!Track_Get_Result(track_stop_dire_for_tim, Track_Pos_1) || !Track_Get_Result(track_stop_dire_for_tim, Track_Pos_5)));
																														//获取偏移方向
				
				if(Track_Get_Result(track_stop_dire_for_tim, track_stop_pos_for_tim) && (!track_twoside || tx == 0)){
						if(count_flag == 0){														//循迹计数
								point_count++;															//计数
								count_flag = 1;															//使不可计数
								if(point_count >= aim_count){								//计数值大于目标值
										Motor_stop();														//停止电机
										begin_track = 0;												//标志位
										TIM_Cmd(TIM4, DISABLE);									//禁用定时器
								}
						}
				}else{
						count_flag = 0;																	//使可计数
				
						if(dx == 0){
								Change_Direction_While_Going(0);						//直走
						}
						if(dx == -1){
								Change_Direction_While_Going(1);						//往左偏
						}
						if(dx == 1){
								Change_Direction_While_Going(2);						//往右偏
						}
						
						if(dx == -2){
								Change_Direction_While_Going(3);						//往左偏
						}
						if(dx == 2){
								Change_Direction_While_Going(4);						//往右偏
						}
				}
		}
	
}




