/*
*********************************************************************************************************
*											        eBase
*
*
*
*						        (c) Copyright 2006-2010, AW China
*											All	Rights Reserved
*
* File    	: 	egon_drv_common_inc.h
* Date	:	2010-09-25
* By      	: 	holigun
* Version 	: 	V1.00
*********************************************************************************************************
*/
#ifndef	_WBOOT_POWER_H_
#define	_WBOOT_POWER_H_

#define   POWER_KEY_EXIST                                1
#define   POWER_KEY_NOTEXIST                             0

#define   BATTERY_RATIO_DEFAULT                          0
#define   BATTERY_RATIO_TOO_LOW_WITHOUT_DCIN             1
#define   BATTERY_RATIO_TOO_LOW_WITH_DCIN                2
#define   BATTERY_RATIO_ENOUGH                           3

#define   AXP_POWER_ON_BY_POWER_KEY       				 0
#define   AXP_POWER_ON_BY_POWER_TRIGGER   				 1

typedef enum
{
    WBOOT_POWER_DCIN_BAT_STATUS,                         //获取power相关状态，是否存在dcin，是否存在电池连接, pbuffer = {power_dcin_bat_status_t}
    WBOOT_POWER_GET_VOL        ,                         //获取电池电压, pbuffer = vol (只获取DCDC2)
    WBOOT_POWER_OFF            ,                         //关机，无参数
    WBOOT_POWER_GET_KEY        ,                         //获取POWER按键信息，是否存在按键状态, 有按键返回1，没有返回0
    WBOOT_POWER_STARTUP_CHECK  ,                         //检查是否需要开机，当插入火牛的时候不开机，当按下开机按键开机, 无参数
    WBOOT_POWER_SW1_CONTROL                              //控制SW1的开关状态, aux = {wboot_power_sw1_status}

}
wboot_power_cmd_set_t;

typedef struct __power_dcin_bat_status_t
{
    __u32 dcin_exist;                                    //判断外部电源是否存在
    __u32 bat_exist;                                     //判断电池是否存在
}
power_dcin_bat_status_t;

typedef enum
{
    WBOOT_POWER_SW1_OPEN            ,                          //控制SW1的状态为打开
    WBOOT_POWER_SW1_CLOSE           ,                          //控制SW1的状态为关闭
    WBOOT_POWER_SW1_NOCHANGE                                   //控制SW1的状态为不变
}
wboot_power_sw1_status;

#endif	//_WBOOT_POWER_H_

/* end of _WBOOT_POWER_H_ */

