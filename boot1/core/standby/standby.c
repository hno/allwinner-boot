/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : standby.c
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
#include "standby.h"


static int eGon2_enter_standby(void);
static int eGon2_exit_standby(void);
static int eGon2_standby_detect(void);
static int eGon2_mod_enter_standby(void);
static int eGon2_mod_exit_standby(void);

static int standby_flag = 0;
static int status;

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
int eGon2_standby_mode(void)
{
	__u32 dcin_exist, battery_exist;
	__s32 key_status;
	status = -1;
	//检查是否有按键按下
	__debug("at the start of %s\n",__FUNCTION__);
	key_status = eGon2_power_get_key();
	if(key_status & 0x01)			//长按键的情况下，不管电源是否移除，直接进入系统
	{
		//退出所有的驱动模块
		if(standby_flag)
		{
			eGon2_mod_exit_standby();
		}
		return 3;
	}
	//检查外部电源是否存在
	dcin_exist = 100;
	battery_exist = 100;
	eGon2_power_get_dcin_battery_exist(&dcin_exist, &battery_exist);
	if(!dcin_exist)							//外部电源移除
	{
		//退出所有的驱动模块
		if(standby_flag)
		{
			eGon2_mod_exit_standby();
		}
		return 6;
	}
//	charge_status = eGon2_power_battery_charge_status();		//充电完成
//	if((charge_status > 0) && (battery_exist == 1))
//	{
//		//退出所有的驱动模块
//		if(standby_flag)
//		{
//			eGon2_mod_exit_standby();
//		}
//		return 7;
//	}
	if(key_status & 0x02)			//当外部电源存在，继续standby
	{
		//退出所有的驱动模块
		if(standby_flag)
		{
			eGon2_mod_exit_standby();
		}
		return 2;
	}
	//保存原来的SP
	eGon2_store_sp();
	//设置新的SP，存放在SRAM中
	eGon2_set_sp();

	if(!standby_flag)
	{
		eGon2_mod_enter_standby();
	}
	eGon2_enter_standby();
	//现在，clock运行在32k上
	do
	{
		//开始循环检查是否开始唤醒
		eGon2_halt();
		status = eGon2_standby_detect();
	}
	while(status <= 0);
	//发现需要唤醒，退出standby
	eGon2_exit_standby();
	//退出模块的standby
	if((status != 8) && (status != 9))
	{
		eGon2_mod_exit_standby();
		standby_flag = 0;
	}
	else
	{
		standby_flag = 1;
	}    
  
	eGon2_restore_sp();

	return status;
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
#define   DCDC2_STANDBY_VOL      (1250)
#define   DCDC3_STANDBY_VOL		 (1250)

static int eGon2_enter_standby(void)
{
	volatile int i;
//	__u8  power_int_status[5];
	//限制standby充电电流
	eGon2_config_charge_current(1);
	//设置VBUS充电不限流
//	eGon2_power_vbus_cur_limit();
//	//清除power的中断pending
//	eGon2_power_int_query(power_int_status);
	//处理中断
	//__debug("standby int init\n");
	standby_int_init();
	//处理clock
	standby_clock_store();

    #ifndef CONFIG_AW_FPGA_PLATFORM
	//处理dram，之后不允许再访问dram
	dram_power_save_process();
	//禁止drampll输出使能
	standby_clock_drampll_ouput(0);
    #endif
	//切换到24M
	standby_clock_to_source(24000000);
    //__debug("after standby_clock_to_source\n");
	//关闭所有pll输出
	standby_clock_plldisable();
   // __debug("after standby_clock_plldisable\n");
    
	//降低电源电压输出
	eGon2_power_set_dcdc2(DCDC2_STANDBY_VOL);
    //__debug("after standby_clock_plldisable\n");
	//eGon2_power_set_dcdc3(DCDC3_STANDBY_VOL);
	//使能电源中断，等待唤醒
	eGon2_power_int_enable();
	//切换分频比全为0
	standby_clock_divsetto0();
    //__debug("after standby_clock_divsetto0 fail\n");
	//切换apb1到32k
	standby_clock_apb1_to_source(32000);
	//切换时钟，关闭时钟
	for(i=0;i<2000;i++);
	standby_clock_to_source(32000);
	//关闭24M晶振
	standby_clock_24m_op(0);
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
static int eGon2_exit_standby(void)
{
	int i;

	//关闭电源中断
	eGon2_power_int_disable();
	//设置电压
	eGon2_power_set_dcdc2(-1);
//	eGon2_power_set_dcdc3(-1);
	//还原所有pll，原来打开的则打开，原来关闭的不处理
	
	standby_clock_restore();
	for(i=0;i<80000;i++);//0x100000
	//切换时钟到PLL1
	standby_clock_to_source(0);
    #ifndef CONFIG_AW_FPGA_PLATFORM
	//打开dram输出使能
	standby_clock_drampll_ouput(1);
	//激活dram
	standby_tmr_enable_watchdog();
	dram_power_up_process();
	standby_tmr_disable_watchdog();
    #endif
	//还原中断状态
	standby_int_exit();
	//还原充电电流
	eGon2_config_charge_current(0);
	//还原所有的驱动模块
	//eGon2_timer_delay(50);

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
static int eGon2_standby_detect(void)
{
	int   i;
	__u32 dcin_exist, battery_exist;
	__u8  power_int_status[5];

	//开启24M晶振
	standby_clock_24m_op(1);
	//切换apb1到24M
	standby_clock_apb1_to_source(24000000);
	//设置频率到24M
	standby_clock_to_source(24000000);
	standby_clock_divsetback();
	for(i=0;i<2000;i++);
	//检查中断触发
	eGon2_power_int_query(power_int_status);
	//清除中断控制器的pending
//	standby_int_query();
//	if(eGon2_key_get_status() == 1)			//普通ADC按键按下
//	{
//		return 1;
//	}
//	eGon2_power_vbus_unlimit();
	if(power_int_status[2] & 0x02)			//电源按键短按
	{
		return 2;
	}
	if(power_int_status[2] & 0x01)			//电源按键长按
	{
		return 3;
	}
	dcin_exist = 100;
	battery_exist = 100;
	eGon2_power_get_dcin_battery_exist(&dcin_exist, &battery_exist);
	if(!dcin_exist)							//外部电源移除
	{
		return 4;
	}
//	if((power_int_status[1] & 0x04) && (battery_exist==1))			//充电完成
//	{
//		return 5;
//	}
	if(power_int_status[0] & 0x08)			//usb火牛接入
	{
		return 8;
	}
	if(power_int_status[0] & 0x04)			//usb火牛移除
	{
		return 9;
	}

	//还原到32K
	standby_clock_divsetto0();
	standby_clock_apb1_to_source(32000);
	standby_clock_to_source(32000);
	standby_clock_24m_op(0);

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
static int eGon2_mod_enter_standby(void)
{
	int i;

	for(i=0;i<EMOD_COUNT_MAX;i++)
	{
		eGon2_driver_standby(i, EGON2_MOD_ENTER_STANDBY, 0);
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
static int eGon2_mod_exit_standby(void)
{
	int i;

	for(i=0;i<EMOD_COUNT_MAX;i++)
	{
		eGon2_driver_standby(i, EGON2_MOD_EXIT_STANDBY, 0);
	}

	return 0;
}




