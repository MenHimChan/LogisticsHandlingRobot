#ifndef __UART4__OPENMV__H
#define __UART4__OPENMV__H


#include "sys.h"
#include "led.h"


extern u8 uart4_data_flag;
extern u8 receive_data[8];

void UART4_OPENMV_Init(void);
void Send_OpenMV_WLCMD(void);
//void OpenMV_Send_Cmd(void);
//void OpenMV_Send_Cmd_Down(void);
//void OpenMV_Send_Cmd_Up(void);
#endif 



