#include "stm32f10x.h"
#include "PULSE.h"
#include "delay.h"

#include "sys.h"
#include "uart.h"
#include "led.h"

#include "USART2_GM65.h" 								 
#include "USART-hmi.h"
#include "LobotServoController.h"

#include "Track.h"
#include "uart5_jy61p.h"

#include "UART4_OPENMV.h"

#include "timer.h"
#include "key.h"



extern u8 uart_buf[32];				 // 0~6索引存在扫码信息
unsigned char SendHMI_buf1[64];		 // 将要发送给串口屏的字符缓冲区
unsigned char SendHMI_buf2[64];

// 舵机控制数组
//u8 Servo_Catch_Up[3] = {CATCH_YL_LEFT,CATCH_YL_MID,CATCH_YL_RIGHT};		// 抓取上层物料的动作组
u8 Index_holder[6] = {1,2,3,1,2,3};										// 存储
u8 colorOrder[4] = {0, 2, 3, 1};
int angle_now = 0;


// 用于访问GY-61P陀螺仪的回调数据（角度）
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 		stcAngle;
///////////////////////////////////////////////////////////////////////////////////////////////

float Angle = 0;
u8 get_Order(u8 Num, u8 up);
void moveUp(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveDown(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveLeft(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveRight(int count, int countint_dir, int counting_pos, int doubleCounting);

void moveUpFast(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveDownFast(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveLeftFast(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveRightFast(int count, int countint_dir, int counting_pos, int doubleCounting);

void moveLeftRotate(int count, int countint_dir, int counting_pos);
void moveRightRotate(int count, int countint_dir, int counting_pos);

void moveLeftUp(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveLeftDown(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveRightUp(int count, int countint_dir, int counting_pos, int doubleCounting);
void moveRightDown(int count, int countint_dir, int counting_pos, int doubleCounting);


void moveAngleLeft90(void);
void moveAngleRight90(void);
void Correct_Dire(void);
void correctAngle(u8 countout);
void correctPos(int posUp, int posDown, int PosLeft, int PosRight, u8 countout);

void ReSet_State(void){

	dc = 50;
	CCR1_Val = 0;
	CCR2_Val = 0;
	CCR3_Val = 0;
	CCR4_Val = 0;
	ARRto = 0;
	Num1 = 0;
	Num2 = 0;
	Num3 = 0;
	Num4 = 0;
	NowCommandCount = 0;
	NowCommandPercent[0] = 1;
	NowCommandPercent[1] = 1;
	NowCommandPercent[2] = 1;
	NowCommandPercent[3] = 1;

	first = NULL;
	last = NULL;

	Now = NULL;

	Command_Finished = 1;
	direction_change = 0;
	go_direction = 0;
	
	count_flag = 0;
	point_count = 0;
	begin_track = 0;
	aim_count = 0;
	track_stop_dire_for_tim = 0;
	track_stop_pos_for_tim = 0;
	track_use_dire = -1;
	track_twoside = 0;

}

int main(void){
		int i;
		delay_init();
		Motor_GPIO_config();
		LED_Init();		  			  // 初始化与LED连接的硬件接口
		uartInit(9600);        	  // 串口1  幻尔驱动板	
		usart2_init(9600);  		  // 串口2  接收GM65模块数据
		uart3_init(115200);			  // 串口3  串口屏
		Track_Gpio_Init();
		Led_Gpio_Init();
		UART4_OPENMV_Init();			// OPENMV
		UART5_Init(115200);				// 陀螺仪
		delay_ms(500);
		JY61_Init(115200, 1);		  // 串口5    初始化JY-61P
		delay_s(1);
		Angle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
	
		if(KEY_Scan(1)){
			return 0;
		}
		
		runActionGroup(0x00, 1);			// 起始姿态
	
		moveUp(1, Track_Dire_Right, Track_Pos_3, 0);
		//左走半格
		moveLeft(2, Track_Dire_Up, Track_Pos_3, 0);
		//此时坐标（1，1）
		moveUpFast(1, Track_Dire_Up, Track_Pos_3, 1);
		Send_Scan_CMD();
		moveUp(1, Track_Dire_Up, Track_Pos_3, 1);
		//前走一格半
		//Send_Scan_CMD();															//扫码
		while(!recv_ok){}															//扫码完成
		//-------------------------------------------------------------------------------------
		
		/* 走到原料区 */
		//-------------------------------------------------------------------------------------
		moveUpFast(3, Track_Dire_Right, Track_Pos_3, 0);				//上移3格
		moveUp(1, Track_Dire_Right, Track_Pos_3, 0);				//上移3格
		//此时坐标（2，6）
		correctPos(-1, -1, -1, Track_Pos_3, 0);
		//校准一下
		//右移半格	
		moveRight(1, Track_Dire_Right, Track_Pos_3, 1);
			
		
		/* 拍照 and 显示 */ 
		// -------------------------------------------------------------------------------------
		
//		delay_s(1);							// 最佳延迟时间
//		
//		runActionGroup(0x01, 1);			// 拍照姿态
//		delay_s(4);							// 最佳延迟时间
//		
//		Send_OpenMV_WLCMD();
//		while(!uart4_data_flag); 		// 当接收成功标志位不等于1，卡死程序
//		
//		// 串口屏上显示 识别到的物料信息
//		sprintf((char*)SendHMI_buf2,"page0.t4.txt=\"%.6s\"",receive_data);
//		HMISends((char*)SendHMI_buf2);
//		HMI_Sendb3times(0XFF);
		//---------------------------------------------------------------------------------------

		/* 抓取上层物料	*/  // 优化完毕
		//---------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x08 + get_Order(i, 1), 1); //夹取物料
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(5); // 5 3
			
			runActionGroup(0x05 + get_Order(i, 1), 1); //转盘子
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(1);//  15 13   最佳时间
				
			runActionGroup(0x02, 1); //放盘子上层
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
		}
		/* 走到粗加工区 */
		//------严文版本---------------------------------------------------------------------------------
		// 左移三格
		moveLeft(3, Track_Dire_Up, Track_Pos_4, 0); 
		// 上移一格
		moveUp(1, Track_Dire_Up, Track_Pos_2, 1);
		//----------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x05 + get_Order(i, 1), 1); //转盘子
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x04, 1);		// 从物料盘抓取物料
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0B + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// 从中间位置放置到色环上
			TIM5_Int_Init(2300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
		}
		
		
		//----------------------------------------------------------------------------------------
	
		/* 粗加工区色环 -> 盘  */
		//----------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x05 + get_Order(i, 1), 1);		// 转动物料盘
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0E + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// 抓取色环区的物料
			TIM5_Int_Init(3100);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
				
			runActionGroup(0x02, 1);						// 放置到物料盘上
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
		}
		
		/* 走到半成品区 */
		//-------------------------------------------------------------------------------------------------
		moveDownFast(3, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Up, Track_Pos_3); 
		angle_now = 90;
		ReSet_State();
		moveUpFast(2, Track_Dire_Right, Track_Pos_5, 0);
		moveUp(1, Track_Dire_Right, Track_Pos_5, 0);
		//-------------------------------------------------------------------------------------------------
		
	
		/* 盘 -> 半成品区 */
		//----------------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
		
			runActionGroup(0x05 + get_Order(i, 1), 1); //转盘子
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
			
			runActionGroup(0x04, 1);		// 从物料盘抓取物料
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
			
			runActionGroup(0x11 + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// 从中间位置放置到色环上
			TIM5_Int_Init(2600);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
		}
		//----------------------------------------------------------------------------------------------
//		runActionGroup(0x02,1);						// 回到中间位置
		//delay_s(4);		
//		runActionGroup(0x00,1);
		//delay_s(3);
		/* 走回原料区 */
		//----------------------------------------------------------------------------------------------
		moveDownFast(1, Track_Dire_Right, Track_Pos_1, 0);
		//moveRightRotate(2, Track_Dire_Right, Track_Pos_3);     // 旋转
		angle_now = 270;
		moveLeftRotate(2, Track_Dire_Up, Track_Pos_3); 
		//correctAngle(0);
		correctPos(-1, -1, Track_Pos_3, -1, 0);
		if(
				!Track_Get_Result(Track_Dire_Left, Track_Pos_1) &&
				!Track_Get_Result(Track_Dire_Left, Track_Pos_2) &&
				!Track_Get_Result(Track_Dire_Left, Track_Pos_3) &&
				!Track_Get_Result(Track_Dire_Left, Track_Pos_4) &&
				!Track_Get_Result(Track_Dire_Left, Track_Pos_5)
		){
				moveDown(1, Track_Dire_Left, Track_Pos_3, 0);
		}
		
		//delay_s(2);	
		ReSet_State();
		angle_now = 270;
		
		moveLeft(2, Track_Dire_Up, Track_Pos_3, 0);
		
		moveUpFast(4, Track_Dire_Left, Track_Pos_4, 0);
		
		moveUp(1, Track_Dire_Down, Track_Pos_2, 1);															// 停在原料区
		
		
		/* 夹取下层物料 */
		//----------------------------------------------------------------------------------------------

		for(i = 1; i <= 3; i++){
			runActionGroup(0x14 + get_Order(i, 0), 1); 	//夹取物料
			TIM5_Int_Init(3500);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			TIM5_Int_Init(4000);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			
			runActionGroup(0x05 + get_Order(i, 0), 1); 	//转盘子
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(1);//  15 13   最佳时间
				
			runActionGroup(0x03, 1); 					//放盘子下层
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
		}  
		
		// --------------严文-夹取完物料往粗加工区走-------------------------
		//delay_s(5);		
		moveDown(1, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Down, Track_Pos_4);   // 左转
		angle_now = 0;
		correctAngle(0);
		ReSet_State();
		// 左移三格
		moveLeft(3, Track_Dire_Up, Track_Pos_4, 0); 
		// 上移2格
		moveUp(1, Track_Dire_Up, Track_Pos_2, 1);
        // --------------------------------------------------
		
		
		/* 第二次放置到粗加工区 */
		for(i = 1; i <= 3; i++){
			runActionGroup(0x05 + get_Order(i, 0), 1); //转盘子
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x04, 1);		// 从物料盘抓取物料
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0B + colorOrder[uart_buf[i + 3] - '0'] - 1, 1);	// 从中间位置放置到色环上
			TIM5_Int_Init(2300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}					// 10
		}
		while(1);
		//----------------------------------------------------------------------------------------
	
		/* 第二次 粗加工区色环 -> 盘  */
		//----------------------------------------------------------------------------------------
//		for(i = 1; i <= 3; i++){
//				
//				runActionGroup(0x0D, 1);		// 中间位置
//				delay_s(10);						// 怕是这个地方的延时有问题
//			
//				runActionGroup(0x02 + get_Order(i, 0), 1);		// 转动物料盘
//				delay_s(10);
//			
//				runActionGroup(0x0D + colorOrder[uart_buf[i + 3] - '0'], 1);	// 抓取色环区的物料
//				delay_s(14);
//				
//				runActionGroup(0x11, 1);						// 放置到物料盘上
//				delay_s(7);										// 10
//		}
//		runActionGroup(0x0D, 1);								// 中间姿态准备离开
		//delay_s(2);
		//----------------------------------------------------------------------------------------------		

		
		/* 严文版本---第二次从粗加工区走到半成品区 */
		//-------------------------------------------------------------------------------------------------
		moveDownFast(3, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Up, Track_Pos_3); 
		ReSet_State();
		moveUp(3, Track_Dire_Right, Track_Pos_5, 0);
		moveDownFast(1, Track_Dire_Left, Track_Pos_2, 0);
		
		//-------------------------------------------------------------------------------------------------
		
		
		/* 码垛 */
		//-------------------------------------------------------------------------------------------------
		//delay_s(2);			
//		for(i = 1; i <= 3; i++){
//			
//				runActionGroup(0x0D, 1);			// 中间位置
//				delay_s(5);						// 2 
//			
//				runActionGroup(0x02 + get_Order(i, 0), 1);			// 转动物料盘
//				delay_s(5);							// 3
//				
//				runActionGroup(0x12,1);				// 从物料盘上夹取物料(结束后爪子停在物料盘位置)
//				delay_s(6);							// 10
//				
//				runActionGroup(0x16,1);				// 爪子回到中间位置
//				delay_s(6);							// 10
//			
//				runActionGroup(0x12 + colorOrder[uart_buf[i + 3] - '0'], 1);	// 爪子从中间位置放置到对应色环上
//				delay_s(6);						// 8
//				
//				runActionGroup(0x16, 1);			// 爪子回到中间位置
//				delay_s(5);									// 3
//		}		
		//-------赋祥跑回终点----------------------------------------------------------------
		
	//while(1);
		moveDownFast(1, Track_Dire_Left, Track_Pos_2, 0); 
		moveLeftRotate(1, Track_Dire_Up, Track_Pos_3);
		ReSet_State();
		moveUpFast(2, Track_Dire_Right, Track_Pos_2, 0);
		moveRight(3, Track_Dire_Right, Track_Pos_2, 1);
		moveUpFast(2, Track_Dire_Down, Track_Pos_2, 1);
		PWM_Output(1800, 50, 600, 1, 1, 1, 1);
		while(1);
}

/*
		for(i = 1; i <= 3; i++){
				执行动作组(get_Order(i, 1));
		}
		for(i = 1; i <= 3; i++){
				执行动作组(get_Order(i, 0));
		}
*/
//返回值在1-3
u8 get_Order(u8 Num, u8 up){
		u8 i;
		if(up){																										//是否是上层
				for(i = 0; i < 3; i++){																				//遍历上层颜色顺序
						if(receive_data[i] == uart_buf[Num - 1]){													//如果颜色序号等于夹取序号
								return i;																		//返回夹取位置
						}
				}
		}else{
				for(i = 3; i < 6; i++){
						if(receive_data[i] == uart_buf[Num + 3]){
								return i - 3;
						}
				}
		}
		return 0;
}


void moveUp(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 1; i <= 5; i++){
				Motor_Move_Up_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Up
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveDown(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Down_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Down
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveLeft(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Left_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Left
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveRight(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Right_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Right
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveLeftUp(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Left_Up_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				-1
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveRightUp(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Right_Up_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				-1
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveLeftDown(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Right_Up_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				-1
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveRightDown(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Right_Up_Circle(1);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				-1
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveUpFast(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 1; i <= 5; i++){
				Motor_Move_Up_Circle(2);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Up
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveDownFast(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Down_Circle(2);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Down
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveLeftFast(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Left_Circle(2);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Left
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveRightFast(int count, int countint_dir, int counting_pos, int doubleCounting){
		int i;
		for(i = 0; i < 5; i++){
				Motor_Move_Right_Circle(2);
		}
		Trace_Tim_Cmd(
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				doubleCounting,
				Track_Dire_Right
		);

		while(!trace_finish());
		//delay_s(1);
		//Correct_Dire();
}

void moveLeftRotate(int count, int countint_dir, int counting_pos){
//		int i;
//		for(i = 0; i < 10; i++){
//				Motor_Move_Left_Rotate(1);
//		}
//		Trace_Tim_Cmd(
//				count, 																				//计数次数
//				countint_dir, counting_pos,						//计数所用灰度传感器
//				0,
//				-1, -1,								//识别左偏使用传感器
//				-1, -1								//识别右偏所用传感器
//		);

//		while(!trace_finish());
		//Correct_Dire();
		Trace_Tim_Cmd(
				count + 1, 																//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				0,
				-1																		//识别偏移所用传感器
		);
		do{
			Motor_Move_Left_Rotate(1);
			while(!Command_Finished){
				if(trace_finish()){
					break;	
				}
			}
		}while(!trace_finish());
		Motor_stop();
		angle_now += 90;
		if(angle_now > 180){
			angle_now -= 360;
		}
}

void moveRightRotate(int count, int countint_dir, int counting_pos){
		/*int i;
		for(i = 0; i < 10; i++){
				Motor_Move_Right_Rotate(1);
		}
		Trace_Tim_Cmd(
				count, 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				0,
				-1, -1,								//识别左偏使用传感器
				-1, -1								//识别右偏所用传感器
		);

		while(!trace_finish());*/
		//Correct_Dire();
		Trace_Tim_Cmd(
				count + 1, 																				//计数次数
				countint_dir, counting_pos,						//计数所用灰度传感器
				0,
				-1
		);
		do{
			Motor_Move_Right_Rotate(1);
			while(!Command_Finished){
				if(trace_finish()){
					break;
				}
			}
		}while(!trace_finish());
		Motor_stop();
		angle_now -= 90;
		if(angle_now <= -180){
			angle_now += 360;
		}
}

void moveAngleLeft90(){
		int i;
		
//		LED0 = 0;		//LED0 = 0;
		for(i = 0; i < 10; i++){
				Motor_Move_Left_Angle(180);
		}
		angle_now = 1;
		while(1){
				while(!Command_Finished){
						if((int)(CALCULATE_ANGLE((float)(stcAngle.Angle[2])) - Angle + 360) % 360 >= 90 && angle_now == 1){
								Motor_stop();
								break;
						}
						if((int)(CALCULATE_ANGLE((float)(stcAngle.Angle[2])) - Angle + 360) % 360 <= 90 && angle_now == 2){
								Motor_stop();
								break;
						}
				}
				delay_ms(30);
				
				if((int)(CALCULATE_ANGLE((float)(stcAngle.Angle[2])) - Angle + 360) % 360 == 90)break;
				
				if((int)(CALCULATE_ANGLE((float)(stcAngle.Angle[2])) - Angle + 360) % 360 > 90){
						for(i = 0; i < 10; i++){
								Motor_Move_Right_Angle(5);
						}
						angle_now = 2;
				}else{
						for(i = 0; i < 10; i++){
								Motor_Move_Left_Angle(5);
						}
						angle_now = 1;
				}
		}
//		LED0 = 1;
		angle_now = 0;
		Angle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
}

void moveAngleRight90(){
		int i;
		
//		LED0 = 0;		//LED0 = 0;
		for(i = 0; i < 10; i++){
				Motor_Move_Right_Angle(180);
		}
		angle_now = 2;
		while(1){
				while(!Command_Finished){
						if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 >= 90 && angle_now == 2){
								Motor_stop();
								break;
						}
						if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 <= 90 && angle_now == 1){
								Motor_stop();
								break;
						}
				}
				delay_ms(30);
				
				if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 == 90)break;
				
				if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 > 90){
						for(i = 0; i < 10; i++){
								Motor_Move_Left_Angle(5);
						}
						angle_now = 1;
				}else{
						for(i = 0; i < 10; i++){
								Motor_Move_Right_Angle(5);
						}
						angle_now = 2;
				}
		}
//		LED0 = 1;
		angle_now = 0;
		Angle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
}

void Correct_Dire(){
		int i;
		
//		LED0 = 0;		//LED0 = 0;
		if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 < 0){
				for(i = 0; i < 10; i++){
						Motor_Move_Left_Angle(5);
				}
				angle_now = 2;
		}else
		if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 > 0){
				for(i = 0; i < 10; i++){
						Motor_Move_Right_Angle(5);
				}
				angle_now = 1;
		}
		
		while(1){
				while(!Command_Finished){
						if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 >= 0 && angle_now == 2){
								Motor_stop();
								break;
						}
						if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 <= 0 && angle_now == 1){
								Motor_stop();
								break;
						}
				}
				delay_ms(30);
				
				if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 == 0)break;
				
				if((int)(Angle - CALCULATE_ANGLE((float)(stcAngle.Angle[2])) + 360) % 360 > 0){
						for(i = 0; i < 10; i++){
								Motor_Move_Right_Angle(5);
						}
						angle_now = 1;
				}else{
						for(i = 0; i < 10; i++){
								Motor_Move_Left_Angle(5);
						}
						angle_now = 2;
				}
		}
//		LED0 = 1;
		angle_now = 0;
		Angle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
}

void correctAngle(u8 countout){
	
	float tangle;
	int dangle;
	
	do{
		if(countout && count_over){
			break;
		}
		
		update = 0;
		
		while(update == 0);
		
		tangle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
		
		dangle = (int)((tangle - Angle) - angle_now + 360) % 360;
		
		printf("%d\n", dangle);
		
		if(dangle > 180){
			
			dangle -= 360;
			
		}
		
		if(dangle > 0){
			Motor_Move_Right_Angle(dangle * 2);
		}else
		if(dangle < 0){
			Motor_Move_Left_Angle(-dangle * 2);
		}
		while(Command_Finished != 1){
			if(countout && count_over){
				Motor_stop();
				break;
			}
		}
	}while((int)((tangle - Angle) - angle_now + 360) % 360 != 0);

}
	
void correctPos(int posUp, int posDown, int posLeft, int posRight, u8 countout){
	int i, j, minPos, maxPos, carryed;
	int pos4[4];
	void(* moveForPos[4][2])(unsigned short);
	pos4[Track_Dire_Up] = posUp;
	pos4[Track_Dire_Down] = posDown;
	pos4[Track_Dire_Left] = posLeft;
	pos4[Track_Dire_Right] = posRight; 
	moveForPos[Track_Dire_Up][0] = Motor_Move_Left;
	moveForPos[Track_Dire_Up][1] = Motor_Move_Right;
	moveForPos[Track_Dire_Down][0] = Motor_Move_Right;
	moveForPos[Track_Dire_Down][1] = Motor_Move_Left;
	moveForPos[Track_Dire_Left][0] = Motor_Move_Down;
	moveForPos[Track_Dire_Left][1] = Motor_Move_Up;
	moveForPos[Track_Dire_Right][0] = Motor_Move_Up;
	moveForPos[Track_Dire_Right][1] = Motor_Move_Down;
	for(j = Track_Dire_Up; j <= Track_Dire_Right; j++){
		if(pos4[j] == -1)continue;
		minPos = Track_Pos_5 + 1;
		maxPos = Track_Pos_1 - 1;
		carryed = 0;
		for(i = Track_Pos_1; i <= Track_Pos_5; i++){
			if(Track_Get_Result(j, i)){
				if(i < minPos){
					minPos = i;
				}
				if(i > maxPos){
					maxPos = i;
				}
			}
		}
		if(minPos < pos4[j]){
			while(Track_Get_Result(j, minPos)){
				moveForPos[j][0](60);
				while(!Command_Finished){
					if(!Track_Get_Result(j, minPos) || (countout && count_over)){
						break;
					}
				}
				if((countout && count_over)){
					break;
				}
			}
			carryed = 1;
		}
		
		if(maxPos > pos4[j]){
			while(Track_Get_Result(j, maxPos) || (countout && count_over)){
				moveForPos[j][1](60);
				while(!Command_Finished){
					if(!Track_Get_Result(j, maxPos) || (countout && count_over)){
						break;
					}
				}
				if((countout && count_over)){
					break;
				}
			}
			carryed = 1;
		}
		if(carryed){
			j--;
		}
		if((countout && count_over)){
			break;
		}
	}
	ReSet_State();
}
//任务码213 颜色顺序312

//夹取顺序321
