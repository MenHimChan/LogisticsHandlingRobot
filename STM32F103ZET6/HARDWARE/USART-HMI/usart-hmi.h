#ifndef __USARTHMI_H
#define __USARTHMI_H		   
#include "sys.h"

//extern u8 Flag_go_measure ;      // ��ʼ���±�־λ
//extern u8 Flag_inTemp_page ;	 // �ڲ���ҳ��ı�־λ����˼��ֻ���ڲ���ҳ�溰����Ҫ�����¡������á�
//extern u8 str_show_buff2[64] ;   // Ϊ����LCD��Ļ����ʾ��־λ״̬���趨���ַ�������
//extern u8 Flag_inIris_page;	 	 // ��ת�����Ĥʶ��ҳ��ı�־λ
//extern u8 str_show_buff4[64];	 // �ñ�־λ���ַ���������



void uart3_init(u32 bound);		// HMI����3��ʼ��
void HMISends(char *buf1); 		// ����һ���ַ���
void HMI_Sendb3times(u8 k);		// �������η���һ���ֽ� �������ַ���������Ϻ�Ľ�����־
void HMISendstart(void);		// ����һ����ʼ�źţ���������������á�

#endif






