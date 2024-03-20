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



extern u8 uart_buf[32];				 // 0~6��������ɨ����Ϣ
unsigned char SendHMI_buf1[64];		 // ��Ҫ���͸����������ַ�������
unsigned char SendHMI_buf2[64];

// �����������
//u8 Servo_Catch_Up[3] = {CATCH_YL_LEFT,CATCH_YL_MID,CATCH_YL_RIGHT};		// ץȡ�ϲ����ϵĶ�����
u8 Index_holder[6] = {1,2,3,1,2,3};										// �洢
u8 colorOrder[4] = {0, 2, 3, 1};
int angle_now = 0;


// ���ڷ���GY-61P�����ǵĻص����ݣ��Ƕȣ�
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
		LED_Init();		  			  // ��ʼ����LED���ӵ�Ӳ���ӿ�
		uartInit(9600);        	  // ����1  �ö�������	
		usart2_init(9600);  		  // ����2  ����GM65ģ������
		uart3_init(115200);			  // ����3  ������
		Track_Gpio_Init();
		Led_Gpio_Init();
		UART4_OPENMV_Init();			// OPENMV
		UART5_Init(115200);				// ������
		delay_ms(500);
		JY61_Init(115200, 1);		  // ����5    ��ʼ��JY-61P
		delay_s(1);
		Angle = CALCULATE_ANGLE((float)(stcAngle.Angle[2]));
	
		if(KEY_Scan(1)){
			return 0;
		}
		
		runActionGroup(0x00, 1);			// ��ʼ��̬
	
		moveUp(1, Track_Dire_Right, Track_Pos_3, 0);
		//���߰��
		moveLeft(2, Track_Dire_Up, Track_Pos_3, 0);
		//��ʱ���꣨1��1��
		moveUpFast(1, Track_Dire_Up, Track_Pos_3, 1);
		Send_Scan_CMD();
		moveUp(1, Track_Dire_Up, Track_Pos_3, 1);
		//ǰ��һ���
		//Send_Scan_CMD();															//ɨ��
		while(!recv_ok){}															//ɨ�����
		//-------------------------------------------------------------------------------------
		
		/* �ߵ�ԭ���� */
		//-------------------------------------------------------------------------------------
		moveUpFast(3, Track_Dire_Right, Track_Pos_3, 0);				//����3��
		moveUp(1, Track_Dire_Right, Track_Pos_3, 0);				//����3��
		//��ʱ���꣨2��6��
		correctPos(-1, -1, -1, Track_Pos_3, 0);
		//У׼һ��
		//���ư��	
		moveRight(1, Track_Dire_Right, Track_Pos_3, 1);
			
		
		/* ���� and ��ʾ */ 
		// -------------------------------------------------------------------------------------
		
//		delay_s(1);							// ����ӳ�ʱ��
//		
//		runActionGroup(0x01, 1);			// ������̬
//		delay_s(4);							// ����ӳ�ʱ��
//		
//		Send_OpenMV_WLCMD();
//		while(!uart4_data_flag); 		// �����ճɹ���־λ������1����������
//		
//		// ����������ʾ ʶ�𵽵�������Ϣ
//		sprintf((char*)SendHMI_buf2,"page0.t4.txt=\"%.6s\"",receive_data);
//		HMISends((char*)SendHMI_buf2);
//		HMI_Sendb3times(0XFF);
		//---------------------------------------------------------------------------------------

		/* ץȡ�ϲ�����	*/  // �Ż����
		//---------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x08 + get_Order(i, 1), 1); //��ȡ����
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(5); // 5 3
			
			runActionGroup(0x05 + get_Order(i, 1), 1); //ת����
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(1);//  15 13   ���ʱ��
				
			runActionGroup(0x02, 1); //�������ϲ�
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
		}
		/* �ߵ��ּӹ��� */
		//------���İ汾---------------------------------------------------------------------------------
		// ��������
		moveLeft(3, Track_Dire_Up, Track_Pos_4, 0); 
		// ����һ��
		moveUp(1, Track_Dire_Up, Track_Pos_2, 1);
		//----------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x05 + get_Order(i, 1), 1); //ת����
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x04, 1);		// ��������ץȡ����
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0B + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// ���м�λ�÷��õ�ɫ����
			TIM5_Int_Init(2300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
		}
		
		
		//----------------------------------------------------------------------------------------
	
		/* �ּӹ���ɫ�� -> ��  */
		//----------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
			
			runActionGroup(0x05 + get_Order(i, 1), 1);		// ת��������
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0E + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// ץȡɫ����������
			TIM5_Int_Init(3100);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
				
			runActionGroup(0x02, 1);						// ���õ���������
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
		}
		
		/* �ߵ����Ʒ�� */
		//-------------------------------------------------------------------------------------------------
		moveDownFast(3, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Up, Track_Pos_3); 
		angle_now = 90;
		ReSet_State();
		moveUpFast(2, Track_Dire_Right, Track_Pos_5, 0);
		moveUp(1, Track_Dire_Right, Track_Pos_5, 0);
		//-------------------------------------------------------------------------------------------------
		
	
		/* �� -> ���Ʒ�� */
		//----------------------------------------------------------------------------------------------
		for(i = 1; i <= 3; i++){
		
			runActionGroup(0x05 + get_Order(i, 1), 1); //ת����
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
			
			runActionGroup(0x04, 1);		// ��������ץȡ����
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
			
			runActionGroup(0x11 + colorOrder[uart_buf[i - 1] - '0'] - 1, 1);	// ���м�λ�÷��õ�ɫ����
			TIM5_Int_Init(2600);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, -1, Track_Pos_5, 1);
			}
		}
		//----------------------------------------------------------------------------------------------
//		runActionGroup(0x02,1);						// �ص��м�λ��
		//delay_s(4);		
//		runActionGroup(0x00,1);
		//delay_s(3);
		/* �߻�ԭ���� */
		//----------------------------------------------------------------------------------------------
		moveDownFast(1, Track_Dire_Right, Track_Pos_1, 0);
		//moveRightRotate(2, Track_Dire_Right, Track_Pos_3);     // ��ת
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
		
		moveUp(1, Track_Dire_Down, Track_Pos_2, 1);															// ͣ��ԭ����
		
		
		/* ��ȡ�²����� */
		//----------------------------------------------------------------------------------------------

		for(i = 1; i <= 3; i++){
			runActionGroup(0x14 + get_Order(i, 0), 1); 	//��ȡ����
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
			
			runActionGroup(0x05 + get_Order(i, 0), 1); 	//ת����
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
			//delay_s(1);//  15 13   ���ʱ��
				
			runActionGroup(0x03, 1); 					//�������²�
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, -1, Track_Pos_3, -1, 1);
			}
		}  
		
		// --------------����-��ȡ���������ּӹ�����-------------------------
		//delay_s(5);		
		moveDown(1, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Down, Track_Pos_4);   // ��ת
		angle_now = 0;
		correctAngle(0);
		ReSet_State();
		// ��������
		moveLeft(3, Track_Dire_Up, Track_Pos_4, 0); 
		// ����2��
		moveUp(1, Track_Dire_Up, Track_Pos_2, 1);
        // --------------------------------------------------
		
		
		/* �ڶ��η��õ��ּӹ��� */
		for(i = 1; i <= 3; i++){
			runActionGroup(0x05 + get_Order(i, 0), 1); //ת����
			TIM5_Int_Init(300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x04, 1);		// ��������ץȡ����
			TIM5_Int_Init(3300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}
			
			runActionGroup(0x0B + colorOrder[uart_buf[i + 3] - '0'] - 1, 1);	// ���м�λ�÷��õ�ɫ����
			TIM5_Int_Init(2300);
			while(!count_over){
				correctAngle(1);
				correctPos(-1, Track_Pos_3, -1, -1, 1);
			}					// 10
		}
		while(1);
		//----------------------------------------------------------------------------------------
	
		/* �ڶ��� �ּӹ���ɫ�� -> ��  */
		//----------------------------------------------------------------------------------------
//		for(i = 1; i <= 3; i++){
//				
//				runActionGroup(0x0D, 1);		// �м�λ��
//				delay_s(10);						// ��������ط�����ʱ������
//			
//				runActionGroup(0x02 + get_Order(i, 0), 1);		// ת��������
//				delay_s(10);
//			
//				runActionGroup(0x0D + colorOrder[uart_buf[i + 3] - '0'], 1);	// ץȡɫ����������
//				delay_s(14);
//				
//				runActionGroup(0x11, 1);						// ���õ���������
//				delay_s(7);										// 10
//		}
//		runActionGroup(0x0D, 1);								// �м���̬׼���뿪
		//delay_s(2);
		//----------------------------------------------------------------------------------------------		

		
		/* ���İ汾---�ڶ��δӴּӹ����ߵ����Ʒ�� */
		//-------------------------------------------------------------------------------------------------
		moveDownFast(3, Track_Dire_Left, Track_Pos_2, 0);
		moveLeftRotate(1, Track_Dire_Up, Track_Pos_3); 
		ReSet_State();
		moveUp(3, Track_Dire_Right, Track_Pos_5, 0);
		moveDownFast(1, Track_Dire_Left, Track_Pos_2, 0);
		
		//-------------------------------------------------------------------------------------------------
		
		
		/* ��� */
		//-------------------------------------------------------------------------------------------------
		//delay_s(2);			
//		for(i = 1; i <= 3; i++){
//			
//				runActionGroup(0x0D, 1);			// �м�λ��
//				delay_s(5);						// 2 
//			
//				runActionGroup(0x02 + get_Order(i, 0), 1);			// ת��������
//				delay_s(5);							// 3
//				
//				runActionGroup(0x12,1);				// ���������ϼ�ȡ����(������צ��ͣ��������λ��)
//				delay_s(6);							// 10
//				
//				runActionGroup(0x16,1);				// צ�ӻص��м�λ��
//				delay_s(6);							// 10
//			
//				runActionGroup(0x12 + colorOrder[uart_buf[i + 3] - '0'], 1);	// צ�Ӵ��м�λ�÷��õ���Ӧɫ����
//				delay_s(6);						// 8
//				
//				runActionGroup(0x16, 1);			// צ�ӻص��м�λ��
//				delay_s(5);									// 3
//		}		
		//-------�����ܻ��յ�----------------------------------------------------------------
		
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
				ִ�ж�����(get_Order(i, 1));
		}
		for(i = 1; i <= 3; i++){
				ִ�ж�����(get_Order(i, 0));
		}
*/
//����ֵ��1-3
u8 get_Order(u8 Num, u8 up){
		u8 i;
		if(up){																										//�Ƿ����ϲ�
				for(i = 0; i < 3; i++){																				//�����ϲ���ɫ˳��
						if(receive_data[i] == uart_buf[Num - 1]){													//�����ɫ��ŵ��ڼ�ȡ���
								return i;																		//���ؼ�ȡλ��
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
				count + (Track_Get_Result(countint_dir, counting_pos) && !doubleCounting), 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
//				count, 																				//��������
//				countint_dir, counting_pos,						//�������ûҶȴ�����
//				0,
//				-1, -1,								//ʶ����ƫʹ�ô�����
//				-1, -1								//ʶ����ƫ���ô�����
//		);

//		while(!trace_finish());
		//Correct_Dire();
		Trace_Tim_Cmd(
				count + 1, 																//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
				0,
				-1																		//ʶ��ƫ�����ô�����
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
				count, 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
				0,
				-1, -1,								//ʶ����ƫʹ�ô�����
				-1, -1								//ʶ����ƫ���ô�����
		);

		while(!trace_finish());*/
		//Correct_Dire();
		Trace_Tim_Cmd(
				count + 1, 																				//��������
				countint_dir, counting_pos,						//�������ûҶȴ�����
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
//������213 ��ɫ˳��312

//��ȡ˳��321
