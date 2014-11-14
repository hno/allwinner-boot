/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#include "common_res.h"

#define   BAT_VOL_ACTIVE     (10)
//#define   FORCE_BOOT_STANDBY
#undef    FORCE_BOOT_STANDBY

volatile __u32  boot_standby_action = 0;
__u32  pic_layer_para;

static int show_battery_full(int *status);
static int shut_battery_full(void);
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
#if 0
__s32 check_file_to_fel(char *name)
{
    H_FILE  hfile = NULL;

    hfile = wBoot_fopen(name, "r");
    if(hfile)
    {
        wBoot_fclose(hfile);
        wBoot_rmfile(name);
        hfile = NULL;
    	hfile = wBoot_fopen(name, "r");
    	if(!hfile)
    	{
    		__inf("dest file is not exist\n");
    	}
    	else
    	{
    		wBoot_fclose(hfile);
    	}

    	return 0;
    }

    return -1;
}
#endif
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
*    说明    ：如果是量产完成，直接关机
*
*
************************************************************************************************************
*/
#if 0
__s32 check_natch_time(char *file_name, __u32 work_mode)
{
    H_FILE  hfile = NULL;
    __s32   ret = -1;

    hfile = wBoot_fopen(file_name, "r");
    if(hfile)
    {
        if(work_mode == WORK_MODE_PRODUCT)
        {
            ret = 0;
        }
        wBoot_fclose(hfile);
        wBoot_rmfile(file_name);
    }

    return ret;
}
#endif
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ： 0:正常启动
*			   -1:关机
*
*    说明    ：如果是量产完成，直接关机
*
*
************************************************************************************************************
*/
#if 0
__s32 check_power_status(void)
{
	if(power_set_limit())
	{
		usb_detect_enter();
	}

	return 0;
}
#else
__s32 check_power_status(void)
{
	__s32 status;
	__s32 power_start;

	//this is used to clear key pending
	wBoot_power_get_key();
	status = wBoot_power_get_level();
	if(status == BATTERY_RATIO_TOO_LOW_WITHOUT_DCIN)
	{
		__inf("battery low power with no dc or ac, should set power off\n");
		ShowPictureEx("c:\\os_show\\low_pwr.bmp", 0);
		wBoot_timer_delay(3000);

		return -1;
	}
	power_start = 0;
	//power_start的含义
	//0: 不允许插火牛直接开机，必须通过判断：满足以下条件可以直接开机：长按power按键，前次是系统状态，如果电池电量过低，则不允许开机
	//1: 任意状态下，允许插火牛直接开机，同时要求电池电量足够高
	//2: 不允许插火牛直接开机，必须通过判断：满足以下条件可以直接开机：长按power按键，前次是系统状态，不要求电池电量
	//3: 任意状态下，允许插火牛直接开机，不要求电池电量
	wBoot_script_parser_fetch("target", "power_start", &power_start, 1);
	__inf("power status = %d\n", status);
	__inf("power start cause = %d\n", power_start);
	if(status == BATTERY_RATIO_TOO_LOW_WITH_DCIN)//低电，同时带外部电源状态下
	{
		if(!(power_start & 0x02))	//需要判断当前电池电量，要求power_start的第1bit的值为0
		{							//此种情况下，直接关机
			__inf("battery low power with dc or ac, should charge longer\n");
			ShowPictureEx("c:\\os_show\\bempty.bmp", 0);
			wBoot_timer_delay(3000);

			return -1;
		}
		else
		{
			if(power_start == 3)	//不需要判断当前电池电量，如果为3，则进入系统，如果为0，则进行后续判断
			{
				return 0;
			}
		}
	}
	else							//电池电量足够情况下，或者没有电池
	{
		if(power_start & 0x01)		//如果第0bit的值为1，则进入系统
		{
			return 0;
		}
	}

	status = -1;
	status = wBoot_power_check_startup();
	__inf("startup status = %d\n", status);
#ifdef FORCE_BOOT_STANDBY
	status = 1;
#endif
	if(status <= 0)
	{
		return 0;
	}
	//if(1)
	{
		int   dcin, bat_exist, counter;
		__s32 bat_cal, this_bat_cal;
		__u32 bat_show_hd = NULL;
		int   i, j, key_status;
		int   bat_full_status = 0;
		int   bat_init = 0;

		//当前可以确定是火牛开机，但是是否开机还不确定，需要确认电池是否存在
		//当电池不存在即开机，电池存在则关机
		counter = 4;
		do
		{
			dcin = 0;
			bat_exist = 0;
			wBoot_power_get_dcin_battery_exist(&dcin, &bat_exist);
			if(bat_exist == -1)
			{
				if(!bat_init)
				{
					bat_show_hd = ShowBatteryCharge_init(0);
					bat_init = 1;
				}
				wBoot_timer_delay(500);
			}
			else
			{
				break;
			}
		}
		while(counter --);
#ifdef FORCE_BOOT_STANDBY
		bat_exist = 1;
#endif
		if(bat_exist <= 0)
		{
			__inf("no battery exist\n");
			if(bat_init)
			{
				ShowBatteryCharge_exit(bat_show_hd);
			}
			return 0;
		}
		this_bat_cal = wBoot_power_get_cal();
		__inf("base bat_cal = %d\n", this_bat_cal);
		if(this_bat_cal > 95)
		{
			this_bat_cal = 100;
		}
		//启动中断检测
		//if(status == 2)	//only vbus exist
		{
			usb_detect_enter();
			wBoot_timer_delay(BOOT_USB_DETECT_DELAY_TIME + 200);
			usb_detect_exit();
		}
		power_limit_detect_enter();

		if(this_bat_cal == 100)
		{
			if(bat_init)
			{
				ShowBatteryCharge_exit(bat_show_hd);
			}
			bat_show_hd = NULL;
			show_battery_full(&bat_full_status);
			for(i =0;i<12;i++)
			{
				if(boot_standby_action & 0x02)	//短按
				{
					boot_standby_action &= ~2;
					i = 0;
				}
				else if(boot_standby_action & 0x01) //长按
				{
					power_limit_detect_exit();

					return 0;
				}
				wBoot_timer_delay(250);
			}
		}
		else
		{
			int one_delay;

			if(!bat_init)
			{
				bat_show_hd = ShowBatteryCharge_init(0);
			}
			one_delay = 1000/(10 - (this_bat_cal/10));
			for(j=0;j<3;j++)
			{
				for(i=this_bat_cal;i<110;i+=10)
				{
					ShowBatteryCharge_rate(bat_show_hd, i);
					if(boot_standby_action & 0x08)		//插入USB接口电源
					{
						boot_standby_action &= ~8;
						j = 0;
					}
					else if(boot_standby_action & 0x02)	//短按
					{
						boot_standby_action &= ~2;
						j = 0;
					}
					else if(boot_standby_action & 0x01) //长按
					{
						ShowBatteryCharge_exit(bat_show_hd);

						power_limit_detect_exit();
						return 0;
					}
					else if(boot_standby_action & 0x10) //usb out
					{
						status = 10;
						boot_standby_action &= ~0x10;

						goto __start_case_status__;
					}
					wBoot_timer_delay(one_delay);
				}
			}
			ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
			for(j=0;j<4;j++)
			{
				if(boot_standby_action & 0x08)		//插入USB接口电源
				{
					boot_standby_action &= ~8;
					j = 0;
				}
				else if(boot_standby_action & 0x10) //usb out
				{
					status = 10;
					boot_standby_action &= ~0x10;

					goto __start_case_status__;
				}
				wBoot_timer_delay(250);
			}
		}
		__inf("extenal power low go high startup\n");
/******************************************************************
*
*	standby 返回值说明
*
*	   -1: 进入standby失败
*		1: 普通按键唤醒
*		2: 电源按键短按唤醒
*		3: 电源按键长按唤醒
*		4: 外部电源移除唤醒
*		5: 电池充电完成
*		6: 在唤醒状态下外部电源被移除
*		7: 在唤醒状态下充电完成
*
******************************************************************/
		do
		{
			__inf("enter standby\n");
			De_CloseLayer(board_res.layer_hd);
			power_limit_detect_exit();
			status = wBoot_standby();
			__inf("exit standby by %d\n", status);

			bat_cal = wBoot_power_get_cal();
			__inf("current bat_cal = %d\n", bat_cal);
			if(bat_cal > this_bat_cal)
			{
				this_bat_cal = bat_cal;
			}
__start_case_status__:
			__inf("status = %d\n", status);
			switch(status)
			{
				case 2:		//短按power按键导致唤醒
					//启动中断检测
					boot_standby_action = 0;
					power_limit_detect_enter();
					De_OpenLayer(board_res.layer_hd);
				case 1:
					if(this_bat_cal == 100)
					{
						if(bat_show_hd)
						{
							ShowBatteryCharge_exit(bat_show_hd);
							bat_show_hd = NULL;
						}
						show_battery_full(&bat_full_status);
						for(i=0;i<12;i++)
						{
							if(boot_standby_action & 0x08)		//插入USB接口电源
							{
								boot_standby_action &= ~8;
								i = 0;
							}
							else if(boot_standby_action & 0x02)	//短按
							{
								boot_standby_action &= ~2;
								i = 0;
							}
							else if(boot_standby_action & 0x01) //长按
							{
								ShowBatteryCharge_exit(bat_show_hd);
								power_limit_detect_exit();

								return 0;
							}
							else if(boot_standby_action & 0x10) //usb out
							{
								status = 10;
								boot_standby_action &= ~0x10;

								goto __start_case_status__;
							}

							wBoot_timer_delay(250);
						}
					}
					else
					{
						int one_delay;

						one_delay = 1000/(10 - (this_bat_cal/10));
						for(j=0;j<3;j++)
						{
							for(i=this_bat_cal;i<110;i+=10)
							{
								ShowBatteryCharge_rate(bat_show_hd, i);
								if(boot_standby_action & 0x08)		//插入USB接口电源
								{
									boot_standby_action &= ~8;
									j = 0;
								}
								if(boot_standby_action & 0x02)	//短按
								{
									boot_standby_action &= ~2;
									j = 0;
								}
								else if(boot_standby_action & 0x01) //长按
								{
									ShowBatteryCharge_exit(bat_show_hd);
									power_limit_detect_exit();

									return 0;
								}
								else if(boot_standby_action & 0x10) //usb out
								{
									status = 10;
									boot_standby_action &= ~0x10;

									goto __start_case_status__;
								}
								wBoot_timer_delay(one_delay);
							}
						}
						ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
						for(j=0;j<4;j++)
						{
							if(boot_standby_action & 0x08)		//插入USB接口电源
							{
								boot_standby_action &= ~8;
								j = 0;
							}
							else if(boot_standby_action & 0x10) //usb out
							{
								status = 10;
								boot_standby_action &= ~0x10;

								goto __start_case_status__;
							}
							wBoot_timer_delay(250);
						}
					}

					break;

				case 3:		//长按电源按键之后，关闭电池图标，进入系统
					ShowBatteryCharge_exit(bat_show_hd);

					return 0;

				case 4:		//当移除外部电源时候，重新显示当前电池图标后，3秒后关机
				case 5:		//当电池充电完成的时候，需要关机
					//启动中断检测
					boot_standby_action = 0;
					power_limit_detect_enter();

					De_OpenLayer(board_res.layer_hd);
					ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
				case 6:
				case 7:
					if((status != 4) && (status != 5))
					{
						De_OpenLayer(board_res.layer_hd);
						ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
					}
				case 10:
					ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
					wBoot_timer_delay(500);
					if(bat_show_hd)
					{
						int ret;

						do
						{
							if(!(boot_standby_action & 0x04))
							{
								ret = ShowBatteryCharge_degrade(bat_show_hd, 10);
							}
							else
							{
								status = 1;
								ShowBatteryCharge_reset(bat_show_hd);

								goto __start_case_status__;
							}
						}
						while(!ret);

						ShowBatteryCharge_exit(bat_show_hd);
					}
					else
					{
						shut_battery_full();
					}
					power_limit_detect_exit();

					return -1;

				break;
				case 8:		//standby过程中检测到vbus接入
				{
					usb_detect_enter();
					wBoot_timer_delay(BOOT_USB_DETECT_DELAY_TIME + 200);
					usb_detect_exit();
				}
				break;
				case 9:		//standby过程中检测到vbus移除，同时存在普通dc
				{
//					power_set_usbpc();
				}
				break;

				default:
					break;
			}
		}
		while(1);
	}
}
#endif

static int show_battery_full(int *status)
{
	if(!*status)
	{
		*status = 1;
		pic_layer_para = ShowPictureEx("c:\\os_show\\bat10.bmp", 0);
	}
	else
	{
		De_OpenLayer(board_res.layer_hd);
	}

	return 0;
}

static int shut_battery_full(void)
{
	int  alpha_step, i;
	int  aplha, delay_time;
	display_layer_info_t *layer_para;

	if(!pic_layer_para)
    {
        return -1;
    }
    layer_para = (display_layer_info_t *)pic_layer_para;
	alpha_step = 5;
	delay_time = 50;
	aplha = layer_para->alpha_val;

	for(i=0xff;i>0;i -= alpha_step)
	{
		layer_para->alpha_en = 1;
		aplha -= alpha_step;
		if(aplha > 0)
		{
			De_SetLayerPara(board_res.layer_hd, layer_para);
			wBoot_timer_delay(delay_time);
			layer_para->alpha_val = aplha;
		}
		else
		{
			break;
		}
	}

	return 0;
}

