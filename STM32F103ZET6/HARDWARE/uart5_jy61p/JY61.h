#ifndef __JY61_H
#define __JY61_H

// ������ȡGY-61P�����ݵĻ���ֱ�ӷ�������Ľṹ���ֵ�ͺ���


// ���ٶȽṹ��
struct SAcc
{
	short a[3];
	short T;
};

// ���ٶȽṹ��
struct SGyro
{
	short w[3];
	short T;
};

// �ǶȽṹ��
struct SAngle
{
	short Angle[3];
	short T;
};
 
								
#endif
