#ifndef __JY61_H
#define __JY61_H

// 如果想读取GY-61P的数据的话，直接访问下面的结构体的值就好了


// 加速度结构体
struct SAcc
{
	short a[3];
	short T;
};

// 角速度结构体
struct SGyro
{
	short w[3];
	short T;
};

// 角度结构体
struct SAngle
{
	short Angle[3];
	short T;
};
 
								
#endif
