#include "include.h"

static  volatile  unsigned  timerl = 0;

//使用TIMER5记录时间，对相关硬件进行初始化操作

//启动一个定时器计时
unsigned    timer_start(void)
{
    __u32 reg_val;
    __u32 i;

	*(unsigned int *)(0x01c20C00 + 0x6C) = 0;
	*(unsigned int *)(0x01c20C00 + 0x70) = 0;
	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 4) & 0x01;
	}
	while(reg_val);
	for(i=0;i<50;i++);

	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 5) & 0x01;
	}
	while(reg_val);

	timerl = *(unsigned int *)(0x01c20C00 + 0x6C);

    return 0;
}

//使用AVS记录时间，并获取从start开始到stop之间的时间，单位ms
unsigned   timer_stop(void)
{
    __u32 reg_val;

	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 5) & 0x01;
	}
	while(reg_val);

	timerl = (*(unsigned int *)(0x01c20C00 + 0x6C)) - timerl;

	timerl/= 24000;

    return timerl;
}


