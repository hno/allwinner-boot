/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/
#ifndef  __BOOT_IMG__
#define  __BOOT_IMG__


#define    BOOT_OS_MELIS                1
#define    BOOT_OS_LINUX                2
#define    BOOT_OS_WINCE                3
#define    BOOT_OS_ANDROID              4


typedef struct _boot_img_cfg
{
    char  file_name[20];
    char  file_name_bk[20];
    __u32 base_addr;
    __u32 used_size;
    __u32 full_size;
}
boot_img_cfg_t;

typedef struct boot_img_t
{
    char               img_name[32];         //32个字节描述镜像文件名称，包含完全路径
    __u32              img_size;             //镜像最大长度
    __u32              img_full_size;        //镜像实际长度
    __u32              img_base;             //镜像存放的内存中的位置
}
boot_img_set_t;

typedef struct  boot_sys_img_t
{
    char               os_name[32];          //OS名称
    __s32              start_mode;           //启动模式，区分 0:按照镜像文件的方式启动 1:按照分区数据的方式启动
    boot_img_set_t     img_set[16];          //指向镜像
    boot_img_set_t     script_info;          //指向脚本信息
    __u32              img_count;            //当前OS所拥有的镜像个数
    char               logo_name[32];        //用于显示logo的路径
    __s32              logo_show;            //是否需要显示开机LOGO
    __u32              logo_address;         //开机logo地址
    __s32			   logo_off;             //进入系统前是否需要关闭LOGO
}
boot_sys_img_set_t;

typedef struct boot_global_info
{
    __s32              os_count;             //总共的OS个数
    char               os_name[8][32];       //OS类型, 0代表没有选择，1代表MELIS，2代表LINUX，3代表WINCE
    char               start_os_name[32];    //当前启动的OS的名称
    __s32              start_os_index;       //当前启动的OS的下标
    __s32              last_os_name;         //上次启动的OS
    __s32              user_set_timeout;     //如果是多系统引导，则在系统选择的延时时间
    __s32              display_device;       //显示在LCD，TV，HDMI上
    __s32              display_mode;         //显示模式
    pic_name_info_t    os_pic[8];            //OS
}
boot_global_info_t;


#endif   //__BOOT_IMG__

