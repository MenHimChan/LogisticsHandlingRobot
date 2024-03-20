#include "timer.h"

//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************

//定时器 驱动代码	

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com			

//********************************************************************************								  
//////////////////////////////////////////////////////////////////////////////////   	 

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器5!
u8 count_over = 1;
void TIM5_Int_Init(u16 ms)
{
	u16 arr;
	u16 psc;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	
	psc = SystemCoreClock * 0.0001;
	arr = ms * 10;
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM6中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);               //中断组别：3
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	count_over = 0;
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					 
}





//********************************************************************************

//定时器3中断服务程序

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com			

//********************************************************************************	



void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志
			count_over = 1;
		}
}


























//********************************************************************************

//定时器 驱动代码	

//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com			

//********************************************************************************	








