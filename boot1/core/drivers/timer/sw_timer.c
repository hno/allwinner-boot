/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON driver sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : timer.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "include.h"
#include "sw_timer.h"

sw_timer_mag_t  timer_port[2] = {
                                    {0, 0, 0, 0, (struct sw_timer *)(CFG_SW_TIMER_CTL0)},
                                    {0, 0, 0, 0, (struct sw_timer *)(CFG_SW_TIMER_CTL1)}
                                };

usr_func    timer_int_func[2];

static __s32  timer_default_func(void *p_arg)             //TP的默认回调函数
{
    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：delay_time: 设置timer定时时间
*
*              auto_restart : timer延时模式，0：delay一次之后自动停止。非0: delay之后自动重新delay
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 eGon2_timer_request(void *func_addr, void *p_arg)
{
    __u32 reg_val, i;

    for(i = 0;i < CFG_SW_TIMER_MAX; i++)
    {
        if(!timer_port[i].used)
        {
            timer_port[i].used = 1;
            CFG_SW_TIMER_INT_CTRL &= ~(1 << i);     //关闭中断

            break;
        }
    }
    if(i >= CFG_SW_TIMER_MAX)
    {
        return 0;
    }
#ifndef CONFIG_SUN6I_FPGA
	reg_val =   (0 << 0)  |            // 不启动TIMER
				(1 << 1)  |            // 使用单次模式
				(1 << 2)  |            // 使用高频晶振24M
				(5 << 4)  |            // 除频系统32，保证当设置时间是1的时候，触发延时1ms
				(1 << 7);
#else
	reg_val =   (0 << 0)  |            // 不启动TIMER
				(1 << 1)  |            // 使用单次模式
				(0 << 2)  |            // 使用高频晶振24M
				(0 << 4)  |            //
				(1 << 7);
#endif

	timer_port[i].index				 = i;
    timer_port[i].arg                = p_arg;
    timer_port[i].timer_ctl->control = reg_val;
    if(func_addr)
    {
        timer_int_func[i] = (usr_func)func_addr;
    }
    else
    {
        timer_int_func[i] = timer_default_func;
    }

    return (__u32)&timer_port[i];
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
//__s32 eGon2_timer_start(__u32 hd, __s32 delay_time, __s32 auto_restart)
//{
//    __u32             reg_val;
//    sw_timer_mag_t   *tmp = (sw_timer_mag_t *)hd;
//
//    if((hd != (__u32)&timer_port[0]) && (hd != (__u32)&timer_port[1]))
//    {
//        return -1;
//    }
//
//    if(!tmp->used)
//    {
//        return -1;
//    }
//
//    reg_val =  (0  << 0)  |            // 不启动TIMER
//               (0  << 1)  |            // 使用单次模式
//               (1  << 2)  |            // 使用高频晶振24M
//               (5  << 4);              // 除频系统32，保证当设置时间是1的时候，触发延时1ms
//    reg_val |= (0  << 0)  |            // 暂时没有start timer
//               (1  << 1);              // 自动更新初始值用于计时
//
//    tmp->timer_ctl->control = reg_val; //首先把初始值写到寄存器，但是不启动
//    tmp->restart = auto_restart;
//    if(!auto_restart)                           //如果不需要自动重新运行
//    {
//        reg_val |= 1 << 7;                      //设置single mode,one shot mode
//    }
//    else
//    {
//        reg_val &= ~(1 << 7);                   //interval mode
//    }
//
//    if(delay_time >= SW_TIMER_MAX_TICK)
//    {
//        delay_time = SW_TIMER_MAX_TICK;
//    }
//
//    tmp->timer_ctl->init_val = delay_time * (24000 / 32);   //确保用户输入的数值1就可以代表1ms
//    tmp->timer_ctl->control = reg_val;
//
//    CFG_SW_TIMER_INT_CTRL |= (1 << tmp->index);    //开启中断
//    //这里开启中断
//    if(tmp->index == 0)
//    {
//        eGon2_EnableInt(GIC_SRC_TIMER0);
//    }
//    else
//    {
//        eGon2_EnableInt(GIC_SRC_TIMER1);
//    }
//    tmp->timer_ctl->control |= 1;     //启动timer
//
//    return 0;
//}
__s32 eGon2_timer_start(__u32 hd, __s32 delay_time, __s32 auto_restart)
{
    __u32             reg_val;
    sw_timer_mag_t   *tmp = (sw_timer_mag_t *)hd;

    if((hd != (__u32)&timer_port[0]) && (hd != (__u32)&timer_port[1]))
    {
        return -1;
    }

    if(!tmp->used)
    {
        return -1;
    }

#ifndef CONFIG_SUN6I_FPGA
	reg_val =   (0 << 0)  |            // 不启动TIMER
				(1 << 1)  |            // 使用单次模式
				(1 << 2)  |            // 使用高频晶振24M
				(5 << 4);              // 除频系统32，保证当设置时间是1的时候，触发延时1ms
#else
	reg_val =   (0 << 0)  |            // 不启动TIMER
				(1 << 1)  |            // 使用单次模式
				(0 << 2)  |            // 使用高频晶振24M
				(0 << 4);              //
#endif

    tmp->timer_ctl->control = reg_val; //首先把初始值写到寄存器，但是不启动
    tmp->restart = auto_restart;
    if(!auto_restart)                           //如果不需要自动重新运行
    {
        reg_val |= 1 << 7;                      //设置single mode,one shot mode
    }
    else
    {
        reg_val &= ~(1 << 7);                   //interval mode
    }

    if(delay_time >= SW_TIMER_MAX_TICK)
    {
        delay_time = SW_TIMER_MAX_TICK;
    }

#ifndef CONFIG_SUN6I_FPGA
	tmp->timer_ctl->init_val = delay_time * (24000 / 32);
#else
	tmp->timer_ctl->init_val = delay_time * 1000/32;
#endif
    tmp->timer_ctl->control = reg_val;

    CFG_SW_TIMER_INT_CTRL |= (1 << tmp->index);    //开启中断
    //这里开启中断
    if(tmp->index == 0)
    {
        eGon2_EnableInt(AW_IRQ_TIMER0);
    }
    else
    {
        eGon2_EnableInt(AW_IRQ_TIMER1);
    }
    tmp->timer_ctl->control |= 1;     //启动timer

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 eGon2_timer_stop(__u32 hd)
{
    sw_timer_mag_t   *tmp = (sw_timer_mag_t *)hd;

    if((hd != (__u32)&timer_port[0]) && (hd != (__u32)&timer_port[1]))
    {
        return -1;
    }

    if(!tmp->used)
    {
        return -1;
    }
    tmp->timer_ctl->control = 0;  //停止timer
    //这里关闭中断
    if(tmp->index == 0)
    {
        CFG_SW_TIMER_INT_STATS |= 0x01;
        eGon2_DisableInt(AW_IRQ_TIMER0);
    }
    else
    {
        CFG_SW_TIMER_INT_STATS |= 0x02;
        eGon2_DisableInt(AW_IRQ_TIMER1);
    }
    CFG_SW_TIMER_INT_CTRL &= ~(1 << tmp->index);    //关闭中断

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：delay_time: 设置timer定时时间
*
*              auto_restart : timer延时模式，0：delay一次之后自动停止。非0: delay之后自动重新delay
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 eGon2_timer_release(__u32 hd)
{
    sw_timer_mag_t   *tmp = (sw_timer_mag_t *)hd;

    if((hd != (__u32)&timer_port[0]) && (hd != (__u32)&timer_port[1]))
    {
        return -1;
    }

    if(!tmp->used)
    {
        return -1;
    }
    tmp->timer_ctl->control = 0;                    //停止计时
    CFG_SW_TIMER_INT_CTRL &= ~(1 << tmp->index);    //关闭中断
    if(tmp->index == 0)
    {
        eGon2_DisableInt(AW_IRQ_TIMER0);
    }
    else
    {
        eGon2_DisableInt(AW_IRQ_TIMER1);
    }

    tmp->used = 0;
    tmp->arg  = 0;

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：delay_time: 设置timer定时时间
*
*              auto_restart : timer延时模式，0：delay一次之后自动停止。非0: delay之后自动重新delay
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 timer0_int_func(void *arg)
{
    if(!(CFG_SW_TIMER_INT_STATS & 0x01))
    {
        return 0;
    }
    CFG_SW_TIMER_INT_STATS |= 0x01;
    eGon2_DisableInt(AW_IRQ_TIMER0);

    timer_int_func[0](timer_port[0].arg);
    if(timer_port[0].restart)
    {
        eGon2_EnableInt(AW_IRQ_TIMER0);
    }

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：delay_time: 设置timer定时时间
*
*              auto_restart : timer延时模式，0：delay一次之后自动停止。非0: delay之后自动重新delay
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 timer1_int_func(void *arg)
{
    if(!(CFG_SW_TIMER_INT_STATS & 0x02))
    {
        return 0;
    }
    CFG_SW_TIMER_INT_STATS |= 0x02;
    eGon2_DisableInt(AW_IRQ_TIMER1);
    timer_int_func[1](timer_port[1].arg);
    if(timer_port[1].restart)
    {
        eGon2_EnableInt(AW_IRQ_TIMER1);
    }

    return 0;
}

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void eGon2_watchdog_disable(void)
{
	CFG_SW_WATCHDOG_CTRL = 0;

	return;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void eGon2_watchdog_enable(void)
{
	CFG_SW_WATCHDOG_CFG  = 1;
	CFG_SW_WATCHDOG_CTRL = 1;

	return;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void eGon2_timer_init(void)
{
//	*(volatile unsigned int *)(0x01c20000 + 0x144) |= (1U << 31);
//	*(volatile unsigned int *)(0x01c20C00 + 0x80 )  = 1;
//	*(volatile unsigned int *)(0x01c20C00 + 0x8C )  = 0x0C;
//	*(volatile unsigned int *)(0x01c20C00 + 0x84 )  = 0;
	CFG_SW_TIMER_INT_STATS |= 0x03;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void eGon2_timer_exit(void)
{
//	*(volatile unsigned int *)(0x01c20C00 + 0x84 )  = 0;
//	*(volatile unsigned int *)(0x01c20C00 + 0x8C )  = 0x05DB05DB;
//	*(volatile unsigned int *)(0x01c20C00 + 0x80 )  = 0;
//	*(volatile unsigned int *)(0x01c20000 + 0x144) &= ~(1U << 31);

}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void eGon2_timer_delay(__u32 ms)
{
	__u32 t1, t2;

	t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	t2 = t1 + ms;
	do
	{
		t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	}
	while(t2 >= t1);

	return ;
}







