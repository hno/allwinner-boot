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

static __u32 sprite_led_hd;
static __u32 codec_led_hd;
static __u32 sprite_timer_hd;
static int   sprite_led_status;
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
static void sprite_timer_func(void *p)
{
	if(codec_led_hd)
	{
		codec_wink(sprite_led_status);
		sprite_led_status = (~sprite_led_status) & 0x01;
	}
	else if(sprite_led_hd)
	{
		wBoot_GPIO_Write_One_PIN_Value(sprite_led_hd, sprite_led_status, "sprite_gpio0");
		sprite_led_status = (~sprite_led_status) & 0x01;
	}

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
int sprite_led_init(void)
{
    user_gpio_set_t     gpio_init;
	int					ret;
	int					delay;

	sprite_timer_hd = wBoot_timer_request((void *)sprite_timer_func, 0);
	if(!sprite_timer_hd)
	{
		__inf("reuqest timer for led failed\n");

		return -1;
	}
	else
	{
		sprite_led_status = 1;
		ret = wBoot_script_parser_fetch("card_boot", "sprite_work_delay", (void *)&delay, 1);
		if((ret) || (!delay))
		{
			delay = 500;
		}
	}
	__inf("try gpio config\n");
	sprite_led_hd = NULL;
	memset(&gpio_init, 0, sizeof(user_gpio_set_t));
	ret = wBoot_script_parser_fetch("card_boot", "sprite_gpio0", (void *)&gpio_init, sizeof(user_gpio_set_t)>>2);
	if(!ret)
	{
		if(gpio_init.port)
		{
			sprite_led_hd = wBoot_GPIO_Request(&gpio_init, 1);
			if(!sprite_led_hd)
			{
				__inf("reuqest gpio for led failed\n");

				goto _led_err;
			}
			__inf("gpio start\n");
			wBoot_timer_start(sprite_timer_hd, delay, 1);	//初始1000ms翻转一次，重复显示

			return 0;
		}
	}
	__inf("try codec config\n");
	codec_init();
	codec_led_hd = codec_play();
	wBoot_timer_start(sprite_timer_hd, delay, 1);	//初始1000ms翻转一次，重复显示

	return 0;


_led_err:
	if(sprite_timer_hd)
	{
		wBoot_timer_release(sprite_timer_hd);
	}

	return -1;
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
int sprite_led_exit(int status)
{
	int ret;
	int delay;

	if(!sprite_timer_hd)
	{
		return -1;
	}
	wBoot_timer_stop(sprite_timer_hd);
	if(status < 0)
	{
		ret = wBoot_script_parser_fetch("card_boot", "sprite_err_delay", (void *)&delay, 1);
		if((ret) || (!delay))
		{
			delay = 100;
		}
		wBoot_timer_start(sprite_timer_hd, delay, 1);
	}

	return 0;
}


