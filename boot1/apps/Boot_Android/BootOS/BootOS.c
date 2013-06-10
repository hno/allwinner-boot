/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: BootOS.c
*
* Author 		:
*
* Description 	: 操作系统引导
*
*
*************************************************************************************
*/
#include  "include.h"
#include  "boot_img.h"
#include  "BootOS_i.h"
#include  "common.h"
#include  "setup.h"
#define    ANDROID_FASTBOOT_MODE		1
#define    ANDROID_RECOVERY_MODE		2
#define    ANDROID_AUTOMODIFY_MODE      3

extern  boot1_private_head_t  boot1_priv_para;

extern  int boot_recovery_status;

#ifdef CONFIG_LOAD_BOOTIMG
//********************************************************************************************************

 /* Android bootimage file format */
#define FASTBOOT_BOOT_MAGIC "ANDROID!"
#define FASTBOOT_BOOT_MAGIC_SIZE 8
#define FASTBOOT_BOOT_NAME_SIZE 16
#define FASTBOOT_BOOT_ARGS_SIZE 512
#define CFG_FASTBOOT_MKBOOTIMAGE_PAGE_SIZE 2048    

struct fastboot_boot_img_hdr {
    unsigned char magic[FASTBOOT_BOOT_MAGIC_SIZE];

    unsigned kernel_size;  /* size in bytes */
    unsigned kernel_addr;  /* physical load addr */

    unsigned ramdisk_size; /* size in bytes */
    unsigned ramdisk_addr; /* physical load addr */

    unsigned second_size;  /* size in bytes */
    unsigned second_addr;  /* physical load addr */

    unsigned tags_addr;    /* physical addr for kernel tags */
    unsigned page_size;    /* flash page size we assume */
    unsigned unused[2];    /* future expansion: should be 0 */

    unsigned char name[FASTBOOT_BOOT_NAME_SIZE]; /* asciiz product name */

    unsigned char cmdline[FASTBOOT_BOOT_ARGS_SIZE];

    unsigned id[8]; /* timestamp / checksum / sha1 / etc */
};


/* borrowed from linux/include/linux/kernel.h */

#ifndef ALIGN
#define ALIGN(x, a)		__ALIGN_MASK(x, (typeof(x))(a)-1)
#define __ALIGN_MASK(x, mask)	(((x)+(mask))&~(mask))
#endif


int read_boot_img(__u32 boot_part_start, __u32 addr)
{

    unsigned kaddr, raddr,ksize_aligned,rsize_aligned,kstart_block,rstart_block;
    wBoot_block_read(boot_part_start,CFG_FASTBOOT_MKBOOTIMAGE_PAGE_SIZE>>9,(void *)addr);
    
	struct fastboot_boot_img_hdr *fb_hdr = (struct fastboot_boot_img_hdr *)addr;

    __debug("---------------------\n");
    __debug("page_size  : %x \n", fb_hdr->page_size);
    __debug("kernel size: %x \n", fb_hdr->kernel_size);
    __debug("kernel addr: %x \n", fb_hdr->kernel_addr);

    __debug("ramdisk size: %x \n", fb_hdr->ramdisk_size);
    __debug("ramdisk addr: %x \n", fb_hdr->ramdisk_addr);

    __debug("second size: %x \n", fb_hdr->second_size);
    __debug("second addr: %x \n", fb_hdr->second_addr);

    __debug("name: %s\n", fb_hdr->name);
    __debug("cmdline: %s\n", fb_hdr->cmdline);


    if (memcmp(fb_hdr->magic, FASTBOOT_BOOT_MAGIC, 8)) {
        __inf("boot1: bad boot image magic, maybe not a boot.img?\n");
        return -1;
    }

    kaddr = fb_hdr->kernel_addr;
    ksize_aligned = ALIGN(fb_hdr->kernel_size, fb_hdr->page_size);
    
    __debug(" ksize_aligned: %x \n",ksize_aligned);
    raddr = fb_hdr->ramdisk_addr;
    rsize_aligned = ALIGN(fb_hdr->ramdisk_size, fb_hdr->page_size);
    __debug(" rsize_aligned: %x \n",rsize_aligned);
    kstart_block = boot_part_start + ((fb_hdr->page_size)>>9);

    rstart_block = kstart_block+(ksize_aligned>>9);
    //read kernel from boot partition
    wBoot_block_read(kstart_block,ksize_aligned>>9,(void*)kaddr);
                           
    //read ramdisk from boot partition
    wBoot_block_read(rstart_block,rsize_aligned>>9,(void*)raddr);

    //mark that we have copy the kernel and ramdisk to the right memory location,
    //and avoid memmove operation in uboot.
    fb_hdr->unused[0] = 0x55;
    fb_hdr->unused[1] = 0xaa;
    return 0;
               


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
*    说明    ：
*
*
************************************************************************************************************
*/
static __s32 check_key_to_recovery(void)
{
    __s32 ret1, ret2;
	__s32 key_high, key_low;
	__s32 keyvalue;

	if(boot_recovery_status == 1)
	{
        #ifdef CONFIG_AW_HOMELET_PRODUCT
        return ANDROID_AUTOMODIFY_MODE;
        #else
		return ANDROID_RECOVERY_MODE;
        #endif
	}
	keyvalue = boot1_priv_para.uart_port;
	__inf("key value = %d\n", keyvalue);

    ret1 = wBoot_script_parser_fetch("recovery_key", "key_max", &key_high, 1);
	ret2 = wBoot_script_parser_fetch("recovery_key", "key_min", &key_low, 1);
	if(ret1)
	{
		__inf("unable to find recovery_key key_max value\n");
		return -1;
	}
	if(ret2)
	{
		__inf("unable to find recovery_key key_min value\n");
		return -1;
	}
	__inf("recovery key high %d, low %d\n", key_high, key_low);
	if((keyvalue >= key_low) && (keyvalue <= key_high))
	{
		__inf("key found, android recovery\n");
		return ANDROID_RECOVERY_MODE;
	}
    #if 0
    ret1 = wBoot_script_parser_fetch("fastboot_key", "key_max", &key_high, 1);
	ret2 = wBoot_script_parser_fetch("fastboot_key", "key_min", &key_low, 1);
	if(ret1)
	{
		__inf("unable to find fastboot_key key_max value\n");
		return -1;
	}
	if(ret2)
	{
		__inf("unable to find fastboot_key key_min value\n");
		return -1;
	}
	__inf("fastboot key high %d, low %d\n", key_high, key_low);
	if((keyvalue >= key_low) && (keyvalue <= key_high))
	{
		__inf("key found, android fastboot\n");
		return ANDROID_FASTBOOT_MODE;
	}

	__inf("key invalid\n");
    #endif
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
static __s32 boot_show_logo(char *logo_name, __u32 logo_show, __u32 logo_address)
{
    if((!logo_show ) || (!logo_name))
    {
        __inf("logo name is invalid or dont need show logo\n");

        return -1;
    }
    ShowPictureEx(logo_name, logo_address);
    __inf("show pic finish\n");

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
static __s32 boot_dsipatch_kernal(boot_sys_img_set_t  *os_img, __u32 *kernal_addr, __u32 *para_addr)
{
    H_FILE  hd_file = NULL;
    __s32   count;
    __s32   i, file_length = 0;
    void   *address;

	__inf("load kernel start\n");
    #ifndef   SPEED_UP_BOOT
    count = os_img->img_count;           //获取镜像的个数
    for(i = 0; i < count; i++)
    {
        if(1 != i)
        {
        	address = (void *)os_img->img_set[i].img_base;
        }
        else
        {
        	address = wBoot_malloc(64 * 1024);
        }
        if(!address)
        {
        	__inf("img file %s base addres is NULL\n", (__u32)address);

        	return -1;
        }
        hd_file = wBoot_fopen(os_img->img_set[i].img_name, "rb");
        if(!hd_file)
        {
            __inf("open img file %s failed\n", os_img->img_set[i].img_name);

            goto dispath_error;
        }
        file_length = wBoot_flen(hd_file);
        if(file_length > os_img->img_set[i].img_size)
        {
            __inf("the img file %s length %d is larger than img max size %d\n", os_img->img_set[i].img_name, file_length, os_img->img_set[i].img_size);

            goto dispath_error;
        }
        wBoot_fread(address, 1, file_length, hd_file);
        wBoot_fclose(hd_file);

        os_img->img_set[i].img_full_size = file_length;
        if(1 == i)
        {
        	do_boot_linux((char *)os_img->img_set[1].img_base, (char *)address, file_length);
        	wBoot_free(address);
        }
        address = NULL;
        hd_file = NULL;
        file_length = 0;
    }
    address = (void *)os_img->script_info.img_base;
    if(!address)
    {
        __inf("no script could be filed\n");
    }
    else
    {
        memcpy(address, (void *)SCRIPT_BASE, os_img->script_info.img_size);
        os_img->script_info.img_full_size = os_img->script_info.img_size;
    }
    *kernal_addr = os_img->img_set[0].img_base;
    *para_addr 	 = os_img->img_set[1].img_base; 
#else
    *kernal_addr = 0x4a000000;
	hd_file = wBoot_fopen("c:\\linux\\u-boot.bin", "rb");
	if(!hd_file)
	{
    	__inf("open img file c:\\linux\\u-boot.bin failed\n");

    	goto dispath_error;
	}
	file_length = wBoot_flen(hd_file);
	wBoot_fread((void *)(*kernal_addr), 1, file_length, hd_file);
	wBoot_fclose(hd_file);
#endif
    __inf("load kernel successed\n");

    return 0;

dispath_error:
    if(hd_file)
    {
        wBoot_fclose(hd_file);
    }
    __inf("load kernel failed\n");

    return -1;
}
/*
*******************************************************************************
*                       PreBootOS
*
* Description:
*    完成引导操作系统必须的准备工作。
*
* Parameters:
*    os_para:  input.  Boot阶段的参数。
*    Type  :  input.  操作系统种类
*
* Return value:
*    返回成功或者失败。
*
* note:
*    无
*
*******************************************************************************
*/
__s32 PreBootOS(char *os_para, __u32 *kernal_addr, __u32 *para_addr, __s32 *logo_status)
{
   
    __s32  ret = -1;
    boot_sys_img_set_t  *os_img;
    
    #ifndef   SPEED_UP_BOOT
    //开始脚本解析，脚本解析初始化
    os_img = (boot_sys_img_set_t *)os_para;
    //此处增加接口，显示开机logo
    *logo_status = os_img->logo_off;
   // boot_show_logo(os_img->logo_name, os_img->logo_show, os_img->logo_address);
   boot_show_logo(os_img->logo_name, os_img->logo_show, BOOT_LOGO_MEM_BASE);
    //os_img->img_set = (char *)os_img + ((boot_sys_img_set_t *)0)->img_set;
    //填充所有参数
    //依次加载OS各个部分到内存中
    //在镜像参数上，不同OS也许需要修正镜像参数
    //现在所有的OS按照同样的规则开始分配到内存
    #endif
    ret = boot_dsipatch_kernal(os_img, kernal_addr, para_addr);
    if(0 > ret)
    {
        goto PreBootOS_error;
    }
    //任意系统所需要的任务都已经加载到内存中，现在根据不同的OS，作各自的参数修正
    //找出需要引导的OS
    __inf("start address = %x\n", *kernal_addr);

    return 0;

PreBootOS_error:

    return -1;
}
/*
*******************************************************************************
*                       BootOS_detect_os_type
*
* Description:
*
*
* Parameters:
*    Para  :
*
* Return value:
*    无
*
* note:
*    无
*
*******************************************************************************
*/
//暂时做法，根据用户输入的参数，来决定启动哪一个OS
__s32 BootOS_detect_os_type(__u32 *para_addr, __u32 *kernal_addr, void *os_info, int *logo_status)
{
    __s32  ret = -1;
    __s32  recovery = 0;
    boot_global_info_t  *global_info = (boot_global_info_t *)os_info;
    boot_sys_img_set_t   os_img_info;    
    char   buffer[MBR_SIZE];
    char   data[2048];
    MBR    *mbr;
    int    start = 0, i;
    bootloader_msg  *bt_msg;

    #ifdef CONFIG_LOAD_BOOTIMG
    __u32 boot_part_start;
    __u32 boot_part_length;
    #endif
    //以下是显示列出所有可用的OS，以后将用display替换
    //如果只有一个OS，则直接进入该OS//
    #ifndef SPEED_UP_BOOT
    memset(&os_img_info, 0, sizeof(boot_sys_img_set_t));
    #endif
    //增加 fast boot 功能, 通过组合按键的方式
	recovery = check_key_to_recovery();
    #ifdef CONFIG_AW_HOMELET_PRODUCT
    if(recovery == ANDROID_AUTOMODIFY_MODE)
    {
    	sprite_led_init();
		ret = card_sprite();
		sprite_led_exit(ret);
		system_reset();
    }
    #endif
	//if(recovery > 0)
    {

        wBoot_block_read(0, MBR_SIZE>>9, buffer);
        mbr = (MBR *)buffer;
        for(i=0;i<mbr->PartCount;i++)
        {
            if(!strcmp("misc", (const char *)mbr->array[i].name))
            {
                start = mbr->array[i].addrlo;
                wBoot_block_read(start, 1, data);
                bt_msg = (bootloader_msg *)data;
                
				if(!strcmp(bt_msg->command, "efex"))
				{
					memset((char *)bt_msg->command, 0, 32);
					wBoot_block_write(start, 1, data);

					wBoot_jump_fel();
				}
                if(recovery > 0)
                {
				    memset((char *)bt_msg->command, 0, 32);
    				if(recovery == ANDROID_FASTBOOT_MODE)
    				{
    					strcpy((char *)bt_msg->command, "boot-fastboot");
    					__inf("fastboot mode\n");
    				}
    				else if(recovery == ANDROID_RECOVERY_MODE)
    				{
    					strcpy((char *)bt_msg->command, "boot-recovery");
    					__inf("recovery mode\n");
    				}
    				wBoot_block_write(start, 1, data);
                }

				break;
            }

        }
    }
    
#ifdef CONFIG_LOAD_BOOTIMG    
    for(i=0;i<mbr->PartCount;i++)
    {
        if(!strcmp("boot", (const char *)mbr->array[i].name))
        {
            boot_part_start  = mbr->array[i].addrlo;
            boot_part_length = mbr->array[i].lenlo;
            __debug("find boot partition!\n");
            __debug("boot_part_start=%x\n",boot_part_start);
            __debug("boot_part_length=%x\n",boot_part_length);
            break;
        }
 
    }
#endif

    #ifndef SPEED_UP_BOOT
    ret = script_patch("c:\\linux\\linux.ini", &os_img_info, 1);
    if(ret < 0)
    {
        __inf("NO OS to Boot\n");

        goto _boot_pre_err;
    }
    #endif
	//获取需要boot的系统的脚本
	#ifdef SPEED_UP_BOOT
    boot_show_logo("c:\\os_show\\bootlogo.bmp", 1, BOOT_LOGO_MEM_BASE);
    #endif

   
	ret = PreBootOS((char *)&os_img_info, kernal_addr, para_addr, logo_status);
	if(ret < 0)
	{
	    goto _boot_pre_err;
    }

    #ifdef CONFIG_LOAD_BOOTIMG
    __debug("begain read bootimg\n");
    ret=read_boot_img(boot_part_start,0x40007800);
    if(ret < 0)
	{
	    goto _boot_pre_err;
    }
    __debug("read bootimg done\n");
    #endif
    
    #ifdef SPEED_UP_BOOT
    #ifndef CONFIG_AW_HOMELET_PRODUCT
     WaitForDeInitFinish();
    #endif
    #endif
	//这里开始跳转到真正的OS入口
_boot_pre_err:
    #ifndef SPEED_UP_BOOT
    wBoot_free(global_info);
    #endif
	return ret;
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
void BootOS(__u32 para_addr, __u32 kernal_addr)
{
    __inf("jump to\n");
    wBoot_jump_to_linux(0, 3495, para_addr, kernal_addr);

    {
        __u32 delay = 0x55;

        while(delay == 0x55);
    }
}

