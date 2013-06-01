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
#ifndef __SYSCALL_H_
#define __SYSCALL_H_	1

#include "eGon2_syscall_define.h"
//--------------------------------------------------------------------
//  函数定义区
//--------------------------------------------------------------------

//__swi (EGON2_SWI_FOR_TEST) 			    int 		swi_for_test          (void );                             //swi测试之用
//__swi (EGON2_SWI_GET_PARA)              int 		wBoot_get_para        (unsigned int  /* para_name */, void * /* para address */);  //传递参数
//__swi (EGON2_SWI_STANDBY)				int 		wBoot_standby		  (void);
//
//
//__swi (EGON2_SWI_DRV_REG)				int          wBoot_driver_install  (void *        /* class_header */                                                                  );
//__swi (EGON2_SWI_DRV_UNREG)			    int          wBoot_driver_uninstall(unsigned int  /* mod_id       */                                                                  );
//__swi (EGON2_SWI_DRV_OPEN)				unsigned int wBoot_driver_open     (unsigned int  /* mod_id  */, void * /*open_arg */                                                 );
//__swi (EGON2_SWI_DRV_CLOSE)			    int          wBoot_driver_close    (unsigned int  /* hd      */                                                                       );
//__swi (EGON2_SWI_DRV_READ)				int          wBoot_driver_read     (void *        /* pBuffer */, unsigned int /* blk */, unsigned int /*   n */, unsigned int /* hd */);
//__swi (EGON2_SWI_DRV_WRITE)			    int          wBoot_driver_write    (void *        /* pBuffer */, unsigned int /* blk */, unsigned int /*   n */, unsigned int /* hd */);
//__swi (EGON2_SWI_DRV_IOCTL)			    int          wBoot_driver_ioctl    (unsigned int  /* hd      */, unsigned int /* cmd */, int          /* aux */, void *  /* pBuffer */);
//
//__swi (EGON2_SWI_RUN_APP)               int          wBoot_run_app         (int           /* argc    */, char **      /* argv */                                              );
//
//__swi (EGON2_SWI_BLOCK_DEV_INIT)        int          wBoot_block_init      (void           /* arg     */                                                                       );
//__swi (EGON2_SWI_BLOCK_DEV_READ)        int          wBoot_block_read      (unsigned int  /* blk start */,unsigned int /* nblock */,                     void */* pBuffer */  );
//__swi (EGON2_SWI_BLOCK_DEV_WRITE)       int          wBoot_block_write     (unsigned int  /* blk start */,unsigned int /* nblock */,                     void */* pBuffer */  );
//__swi (EGON2_SWI_BLOCK_DEV_EXIT)        int          wBoot_block_exit      (void           /* arg     */                                                                       );
//
//__swi (EGON2_SWI_BLOCK_SIZE)			int			 wBoot_block_size	   (void);
//__swi (EGON2_SWI_BLOCK_DDEV_READ)       int          wBoot_block_dread     (unsigned int  /* blk start */,unsigned int /* nblock */,                     void */* pBuffer */  );
//__swi (EGON2_SWI_BLOCK_DDEV_WRITE)      int          wBoot_block_dwrite    (unsigned int  /* blk start */,unsigned int /* nblock */,                     void */* pBuffer */  );
///*
//     中断部分 系统服务
//   1.注册中断服务程序
//   2.注销中断服务程序
//   3.使能中断
//   4.禁止中断
//*/
//__swi (EGON2_SWI_IRQ_REG)			    int wBoot_InsINT_Func     (unsigned int /* irq_no */, int * /* func_addr */, void * /* p_arg */);
//__swi (EGON2_SWI_IRQ_UNREG)		        int wBoot_UnsInt_Func     (unsigned int /* irq_no */                                          );
//__swi (EGON2_SWI_ENABLE_IRQ)            int wBoot_EnableInt       (unsigned int /* irq_no */                                          );
//__swi (EGON2_SWI_DISABLE_IRQ)           int wBoot_DisableInt      (unsigned int /* irq_no */                                          );
//
///*
//    文件操作 系统服务
//    文件操作相对比较多，处于效率的考虑，不提供出目录操作，只有文件操作。具体的服务见下方列表
//    所有的操作都是标准C接口
//    1.文件打开，需要绝对路径名称
//    2.文件读
//    3.文件写
//    4.文件定位：文件指针距离文件头部偏移量，即TELL操作
//    5.文件长度
//    6.文件指针偏移，即SEEK操作
//    7.文件结束，    即eof操作
//    8.文件关闭
//    9.文件删除，需要绝对路径
//*/
//__swi (EGON2_SWI_FS_OPEN)               void *        wBoot_fopen (const char * /* filename */, char * /* openmode   */                                              );
//__swi (EGON2_SWI_FS_READ)               int           wBoot_fread (void * /* buffer   */, unsigned int /* block_size */, unsigned int /* count */, void * /* hfile */);
//__swi (EGON2_SWI_FS_WRITE)              int           wBoot_write (void * /* buffer   */, unsigned int /* block_size */, unsigned int /* count */, void * /* hfile */);
//__swi (EGON2_SWI_FS_TELL)               unsigned int  wBoot_ftell (void * /* hfile    */                                                                             );
//__swi (EGON2_SWI_FS_LEN)                unsigned int  wBoot_flen  (void * /* hfile    */                                                                             );
//__swi (EGON2_SWI_FS_SEEK)               int           wBoot_fseek (void * /* hfile    */, int          /* offset     */, int         /* whence */                    );
//__swi (EGON2_SWI_FS_EOF)                int           wBoot_feof  (void * /* hfile    */                                                                             );
//__swi (EGON2_SWI_FS_CLOSE)              int           wBoot_fclose(void * /* hfile    */                                                                             );
//__swi (EGON2_SWI_FS_RM)                 int           wBoot_rmfile(char * /* filename */                                                                             );
//
//__swi (EGON2_SWI_PART_INFO)             int           wBoot_part_info(const char * /* class_name */,          const char * /* name */, part_info_t * /* part_info */ );
//__swi (EGON2_SWI_PART_MOUNT  )          int           wBoot_fsmount  (char         /* disk       */                                                                  );
//__swi (EGON2_SWI_PART_UNMOUNT)          int           wBoot_fsunmount(char         /* disk       */                                                                  );
//__swi (EGON2_SWI_PART_COUNT)			int			  wBoot_part_count(int);
//__swi (EGON2_SWI_PART_START)            int			  wBoot_part_start(int         /* part index */);
//__swi (EGON2_SWI_PART_CAPACITY)			int			  wBoot_part_capacity(int      /* part index */);
///*
//    目录操作 系统服务
//    操作基于标准C接口
//    1.目录打开操作
//    2.目录关闭操作
//    3.目录创建，绝对路径
//    4.目录删除，绝对路径
//    5.读取一个目录
//*/
//__swi (EGON2_SWI_DIR_OPEN)              void *       wBoot_opendir(const char * /* dir name */);
//__swi (EGON2_SWI_DIR_CLOSE)             int          wBoot_closedir( void * /* dir handle */);
//__swi (EGON2_SWI_DIR_MAKE)              int          wBoot_mkdir(char * /* dir name */);
//__swi (EGON2_SWI_DIR_ERASE)             int          wBoot_rmdir(char * /* dir name */);
//__swi (EGON2_SWI_DIR_READ)              int          wBoot_readdir(void * /* dir handle */, ENTRY * /* dir struct */);
//
///*
//     内存管理 系统服务
//    1.申请内存，字节单位，初始内容未知
//    2.申请内存，字节单位，初始内容全是0
//    3.根据前一次申请的内存，在尾部追加内存，内容未知
//    4.释放申请的内存
//*/
//__swi(EGON2_SWI_MALLOC)                 void *wBoot_malloc          (unsigned int /* num_bytes */                              );
//__swi(EGON2_SWI_CALLOC)                 void *wBoot_calloc          (unsigned int /* size      */, unsigned int /* num_bytes */);
//__swi(EGON2_SWI_REALLOC)                void *wBoot_realloc         (void *       /* p         */, unsigned int /* num_bytes */);
//__swi(EGON2_SWI_FREE)                   void  wBoot_free            (void *       /* p         */                              );
//
//
///*
//    内置电源芯片管理服务
//*/
//__swi(EGON2_SWI_POWER_GET_SOURCE)       int   wBoot_power_get_dcin_battery_exist(unsigned int * /*dcin_exist*/, unsigned int */*battery_exist*/);
//__swi(EGON2_SWI_POWER_GET_BAT_VOL)      int   wBoot_power_get_battery_vol       (unsigned int * /*battery_vol*/);
//__swi(EGON2_SWI_POWER_GET_KEY)          int   wBoot_power_get_key               (void);
//__swi(EGON2_SWI_POWER_CHECK_STARTUP)    int   wBoot_power_check_startup         (void);
//__swi(EGON2_SWI_POWER_SET_SW1)          int   wBoot_power_set_sw1               (unsigned int on_off);
//__swi(EGON2_SWI_POWER_SET_OFF)          int   wBoot_power_set_off               (void);
//__swi(EGON2_SWI_POWER_GET_LEVEL_STATUS) int   wBoot_power_get_level				(void);
//__swi(EGON2_SWI_POWER_SET_OFF_VOL)		int	  wBoot_power_set_off_vol			(void);
//__swi(EGON2_SWI_POWER_BATTERY_CAL)		int   wBoot_power_get_cal				(void);
//__swi(EGON2_SWI_POWER_CUR_LIMIT)		int	  wBoot_power_cur_limit				(int current);
//__swi(EGON2_SWI_POWER_VOL_LIMIT)		int   wBoot_power_vol_limit				(int vol);
//__swi(EGON2_SWI_POWER_TYPE)				int   wBoot_power_type					(void);
//
//__swi(EGON2_SWI_TWI_READ   )            int   wBoot_twi_read   (void  *      /* twi_arg   */);
//__swi(EGON2_SWI_TWI_WRITE  )            int   wBoot_twi_write  (void  *      /* twi_arg   */);
////__swi(EGON2_SWI_TWI_INIT   )            int   wBoot_twi_init   (unsigned int /* twi_index */, unsigned int /* clock  */);
////__swi(EGON2_SWI_TWI_EXIT   )            int   wBoot_twi_exit   (void                        );
////__swi(EGON2_SWI_TWI_SETFREQ)            int   wBoot_twi_setfreq(unsigned int /* twi run clock*/);
///*
//    定时器使用，系统服务
//    1.申请一个定时器，需要传进一个函数作为回调。当时钟中断来到的时候，将调用这个函数
//    2.释放定时器
//    3.启动定时器，需要告诉系统：申请的定时器句柄，定时时间，是否需要自动重新启动定时器
//*/
//__swi(EGON2_SWI_TIMER_REQ)              unsigned int wBoot_timer_request(void *       /* func_addr */, void * /* p_arg */);
//__swi(EGON2_SWI_TIMER_REL)              int          wBoot_timer_release(unsigned int /* hd        */);
//__swi(EGON2_SWI_TIMER_START)            int          wBoot_timer_start  (unsigned int /* hd        */, int /* delay_time*/, int /* auto_restart */);
//__swi(EGON2_SWI_TIMER_STOP)             int          wBoot_timer_stop   (unsigned int /* hd        */);
//__swi(EGON2_SWI_SYSTEM_RESET)           void         wBoot_system_reset (void						 );
//__swi(EGON2_SWI_TIMER_DELAY)			void		 wBoot_timer_delay  (unsigned int /* delay  ms */);
///*
//     DMA管理 系统服务
//    1.申请DMA通道
//    2.释放DMA通道
//    3.启动DMA
//    4.停止DMA
//    5.设置DMA参数
//*/
//__swi(EGON2_SWI_DMA_REQ)               unsigned int wBoot_dma_request  (unsigned int /* dmatype */                                );
//__swi(EGON2_SWI_DMA_REL)               int          wBoot_dma_release  (unsigned int /* hDma    */                                );
//__swi(EGON2_SWI_DMA_START)             int          wBoot_dma_start    (unsigned int /* hDMA    */, unsigned int /* saddr */, unsigned int /* daddr */, unsigned int /* bytes */);
//__swi(EGON2_SWI_DMA_STOP)              int          wBoot_dma_stop     (unsigned int /* hDMA    */                                );
//__swi(EGON2_SWI_DMA_SETTING)           unsigned int wBoot_dma_Setting  (unsigned int /* hDMA    */, void *       /* cfg    */     );
//__swi(EGON2_SWI_DMA_RESTART)           int          wBoot_dma_Restart  (unsigned int /* hDMA    */                                );
//__swi(EGON2_SWI_DMA_QUERYSTATE)        int          wBoot_dma_QueryState(unsigned int /* hDMA    */                               );
//__swi(EGON2_SWI_DMA_QUERYCHAN)         int          wBoot_dma_QueryChan(unsigned int /* hDMA    */                                );
//
///*
//    打印(调试) 系统服务
//    1.uart printf   : PC端屏幕打印服务
//    2.display printf: LCD端屏幕打印服务
//*/
//__swi (EGON2_SWI_UART_DEBUG)			void         wBoot_uprintf      ( const char * /* str */                    );
//__swi (EGON2_SWI_UART_NTDEBUG)			void		 wBoot_ntprintf     ( const char * /* str */                    );
////__swi (EGON2_SWI_DISPLAY_DEBUG)		void wBoot_printf               ( const char * /* str */                    );
//__swi (EGON2_SWI_UART_GETC )            char         wBoot_getc         ( void                                      );
//__swi (EGON2_SWI_UART_GETS )            unsigned int wBoot_getall       ( char *       /* str */                    );
//__swi (EGON2_SWI_UART_PUTC )            void         wBoot_putc         ( char         /* str */                    );
//__swi (EGON2_SWI_UART_GETC_DELAY )      char         wBoot_getc_delay   ( unsigned int /* delay */					);
///*
//    跳转服务 系统服务
//*/
//__swi (EGON2_SWI_JUMP_TO )              void 		 wBoot_jump_to      ( unsigned int  /* addr */                  );
//__swi (EGON2_SWI_JUMP_FEL)				void         wBoot_jump_fel     ( void										);
//__swi (EGON2_SWI_JUMP_TO_LINUX)			void		 wBoot_jump_to_linux( int addr,  int mod_id, unsigned int paddr, unsigned int kernal_addr);
///*
//	频率服务 系统服务
//*/
//__swi (EGON2_SWI_CLOCK )				unsigned int wBoot_get_clock    ( unsigned int  /* clock name */            );
///*
//	脚本服务 系统服务
//	1. 获取配置项数据，包括普通数据和字符串数据
//	2. 获取主键名下的子键个数
//	3. 获取主键的个数
//	4. 获取主键名下的GPIO个数
//	5. 获取主键名下的GPIO参数
//*/
//__swi(EGON2_SWI_SCRIPT_FETCH)              int  wBoot_script_parser_fetch(char *main_name, char *sub_name, int value[], int count);
//__swi(EGON2_SWI_SCRIPT_PATCH)              int  wBoot_script_parser_patch(char *main_name, char *sub_name, int value);
//__swi(EGON2_SWI_SCRIPT_SUBKEY_COUNT)       int  wBoot_script_parser_subkey_count(char *main_name);
//__swi(EGON2_SWI_SCRIPT_MAINKEY_COUNT)      int  wBoot_script_parser_mainkey_count(void);
//__swi(EGON2_SWI_SCRIPT_MAINKEY_GPIO_COUNT) int  wBoot_script_parser_mainkey_get_gpio_count(char *main_name);
//__swi(EGON2_SWI_SCRIPT_MAINKEY_GPIO_DATA)  int  wBoot_script_parser_mainkey_get_gpio_cfg(char *main_name, void *gpio_cfg, int gpio_count);
///*
//    GPIO服务 系统服务
//*/
//__swi(EGON2_SWI_GPIO_REQUEST)        unsigned int wBoot_GPIO_Request				 (user_gpio_set_t *gpio_list, 	    unsigned int group_count_max			 );
//__swi(EGON2_SWI_GPIO_RELEASE)                 int wBoot_GPIO_Release				 (unsigned int p_handler, 	            		 int if_release_to_default_status);
//__swi(EGON2_SWI_GPIO_GET_ALL_PIN_STATUS)      int wBoot_GPIO_Get_All_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, unsigned int gpio_count_max,  unsigned int if_get_from_hardware);
//__swi(EGON2_SWI_GPIO_GET_ONE_PIN_STATUS)      int wBoot_GPIO_Get_One_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, 		 const char *gpio_name, unsigned int if_get_from_hardware);
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_STATUS)      int wBoot_GPIO_Set_One_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, 	     const char *gpio_name, unsigned int if_set_to_current_input_status);
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_IO)          int wBoot_GPIO_Set_One_PIN_IO_Status   (unsigned int p_handler, unsigned int if_set_to_output_status, const char *gpio_name);
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_PULL)        int wBoot_GPIO_Set_One_PIN_Pull	     (unsigned int p_handler, unsigned int set_pull_status,   	     const char *gpio_name);
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_DLEVEL)      int wBoot_GPIO_Set_One_PIN_driver_level(unsigned int p_handler, unsigned int set_driver_level,  		 const char *gpio_name);
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_DATA)        int wBoot_GPIO_Write_One_PIN_Value	 (unsigned int p_handler, unsigned int value_to_gpio,     		 const char *gpio_name);
//__swi(EGON2_SWI_GPIO_GET_ONE_PIN_DATA)        int wBoot_GPIO_Read_One_PIN_Value	     (unsigned int p_handler, 								   		 const char *gpio_name);
///*
//    KEY检测服务
//*/
//__swi(EGON2_SWI_KEY_GET_STATUS)               int wBoot_key_get_status               (void);
//__swi(EGON2_SWI_KEY_GET_VALUE)				  int wBoot_key_get_value				 (void);








#endif  /*#ifndef __SYSCALL_H_*/


