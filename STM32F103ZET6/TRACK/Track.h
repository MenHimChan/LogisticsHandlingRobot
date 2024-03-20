#ifndef TRACK_H
#define TRACK_H
#include "sys.h"

#define Track_Dire_Up 0												//上边的模块
#define Track_Dire_Down 1											//下边的模块
#define Track_Dire_Left 2											//左边的模块
#define Track_Dire_Right 3										//右边的模块

#define Track_Pos_1 1											
#define Track_Pos_2 2											//左边
#define Track_Pos_3 3											
#define Track_Pos_4 4											//右边
#define Track_Pos_5 5


extern int count_flag;
extern int point_count;
extern int begin_track;
extern int aim_count;
extern int track_stop_dire_for_tim;
extern int track_stop_pos_for_tim;
extern int track_use_dire;
extern int track_twoside;

void Track_Gpio_Init(void);

u8 Track_Get_Result(int Dire, int Pos);

void Led_Gpio_Init(void);

void Trace_Tim_Cmd(int count_num, int stop_dire, int stop_pos, int twoside, int track_dir);

int trace_finish(void);

#endif



