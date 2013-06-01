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
#ifndef  __eGon2_sys_h
#define  __eGon2_sys_h  1


extern  int          eGon2_standby_mode(void);
extern  void 		 eGon2_block_ratio(void);
extern  int 		 eGon2_force_to_debug(void);
/* 系统提供的打印服务 */
extern  int          serial_init( int uart_ctrl_port, void  *uart_ctrl, int baudrate, unsigned int sys_clock);
extern  int 		 serial_exit( int uart_prot);
extern  void         eGon2_printf( const char * str, ...);
extern  void 		 eFG_printf( const char * str, ...);
extern  char         eGon2_getc  ( __u32 timeout );
extern  unsigned int eGon2_getall( char *str );
extern  void         eGon2_putc  ( char   str );

/* 系统提供的内存管理服务 */
extern  int          eGon2_create_heap   (unsigned int pHeapHead, unsigned int nHeapSize                                );
extern  void *       eGon2_malloc        (unsigned int num_bytes                                                        );
extern  void         eGon2_free          (void *p                                                                       );
extern  void *       eGon2_realloc       (void *p, unsigned int num_bytes                                               );
extern  void *       eGon2_calloc        (unsigned int size, unsigned int num_bytes                                     );
/* 系统提供的中断管理服务 */
extern  void 		 eGon2_Int_Init		 (void																			);
extern  void 		 eGon2_Int_Exit		 (void																			);
extern  int          eGon2_InsINT_Func   (unsigned int irq_no,  int *func_addr,   void *p_arg                           );
extern  int          eGon2_UnsInt_Func   (unsigned int irq_no                                                           );
extern  int          eGon2_EnableInt     (unsigned int irq_no                                                           );
extern  int          eGon2_DisableInt    (unsigned int irq_no                                                           );
extern  int			 eGon2_int_enter_standby(void																		);
extern  int			 eGon2_int_exit_standby(void																		);
/* 系统提供的驱动管理服务 */
extern  int          eGon2_driver_reg    (void *class_header                                                            );
extern  int 	     eGon2_driver_unreg  (unsigned int mod_id                                                           );
extern  unsigned int eGon2_driver_open   (unsigned int mod_id,  void *open_arg                                          );
extern  int          eGon2_driver_close  (unsigned int hd                                                               );
extern  int          eGon2_driver_read   (void      *  pBuffer, unsigned int blk, unsigned int n, unsigned int hd       );
extern  int          eGon2_driver_write  (void      *  pBuffer, unsigned int blk, unsigned int n, unsigned int hd       );
extern  int          eGon2_driver_ioctl  (unsigned int hd,      unsigned int cmd, int aux       , void *  pBuffer       );
extern  int 		 eGon2_driver_standby(unsigned int mod_id, unsigned int  cmd, void *pBuffer							);
/* 系统提供的基础块设备访问服务 */
extern  int          eGon2_block_device_init (void		                                                                );
extern  int          eGon2_block_device_exit (void                                                                      );
extern  int          eGon2_block_device_read (unsigned int start_block,         unsigned int nblock, void *pbuffer      );
extern  int          eGon2_block_device_write(unsigned int start_block,         unsigned int nblock, void *pbuffer      );
extern  int 		 eGon2_block_size	 	 (void																		);
/* 系统提供的驱动安装，程序运行服务 */
extern  int          eGon2_driver_install(const char *pstr                                                              );
extern  int          eGon2_run_app       (int argc,         char **argv                                                 );
extern  int          eGon2_inter_driver_install(void *addr                                                              );
/* 系统提供的timer延时服务 */
extern  unsigned int eGon2_timer_request (void *func_addr , void *p_arg                                                 );
extern  int          eGon2_timer_release (unsigned int hd                                                               );
extern  int          eGon2_timer_start   (unsigned int hd , int delay_time, int auto_restart                            );
extern  int          eGon2_timer_stop    (unsigned int hd                                                               );
extern  void         eGon2_watchdog_disable(void																		);
extern  void         eGon2_watchdog_enable (void																		);
extern  void         eGon2_timer_init (void																				);
extern  void         eGon2_timer_exit (void																				);
extern  void         eGon2_timer_delay(unsigned int ms																	);
/* 系统提供的dma服务 */
extern  unsigned int eGon2_RequestDMA    (unsigned int dmatype                                                          );
extern  int          eGon2_ReleaseDMA    (unsigned int hDma                                                             );
extern  int          eGon2_SettingDMA    (unsigned int hDMA, void         * cfg                                         );
extern  int          eGon2_StartDMA      (unsigned int hDMA, unsigned int saddr, unsigned int daddr, unsigned int bytes );
extern  int          eGon2_StopDMA       (unsigned int hDma                                                             );
extern  int          eGon2_RestartDMA    (unsigned int hDma                                                             );
extern  int          eGon2_QueryDMAStatus(unsigned int hDma                                                             );
extern  int          eGon2_QueryDMAChannalNo(unsigned int hDma                                                          );
extern  int			 eGon2_dma_enter_standby(void																		);
extern  int			 eGon2_dma_exit_standby(void																		);
/* 系统提供的TWI服务 */
extern  int 		 BOOT_TWI_Read		 (unsigned int arg1,  unsigned char *arg2, unsigned char *arg3					);
extern  int 		 BOOT_TWI_Write		 (unsigned int arg1,  unsigned char *arg2, unsigned char *arg3					);
extern  int 		 BOOT_TWI_ReadEx	 (unsigned int arg1,  unsigned char *arg2, unsigned char *arg3, unsigned char count	);
extern  int 		 BOOT_TWI_WriteEx	 (unsigned int arg1,  unsigned char *arg2, unsigned char *arg3, unsigned char count	);
extern  int          eGon2_twi_init      (int twi_port, void *boot_twi_ctl, unsigned int system_clock, unsigned int twi_run_clock);
extern  int          eGon2_twi_setfreq   (unsigned int clock, unsigned int twi_run_clock                                );
extern  int          eGon2_twi_exit      (void                                                                          );
extern  int          eGon2_twi_read      (void        *twi_arg                                                          );
extern  int          eGon2_twi_write     (void        *twi_arg                                                          );
/* 系统提供的POWER服务  */
extern  int          eGon2_power_init                  (void         *power_para                                        );
extern  int          eGon2_power_exit                  (void                                                            );
extern  int 		 eGon2_power_set_dcdc2			   (int     set_vol													);
extern  int 		 eGon2_power_set_dcdc3			   (int  	set_vol													);
extern  int 		 eGon2_power_set_ldo2			   (int  	set_vol													);
extern  int 		 eGon2_power_set_ldo3			   (int  	set_vol													);
extern  int 		 eGon2_power_set_ldo4			   (int  	set_vol													);
extern  int          eGon2_power_get_dcin_battery_exist(unsigned int *dcin_exist, unsigned int *battery_exist           );
extern  int          eGon2_power_get_battery_vol       (unsigned int *battery_vol                                       );
extern  int          eGon2_power_set_off               (void                                                            );
extern  int          eGon2_power_get_short_key         (void                                                            );
extern  int          eGon2_power_get_long_key          (void                                                            );
extern  int			 eGon2_power_get_key			   (void															);
extern  int          eGon2_power_check_startup         (void                                                            );
extern  int          eGon2_power_set_sw1               (unsigned int on_off                                             );
extern  int 		 eGon2_power_set_vol			   (void															);
extern  int 		 eGon2_set_power_on_vol			   (void															);
extern  int 		 eGon2_set_power_off_vol		   (void															);
extern  int			 eGon2_get_power_vol_level		   (void															);
extern  void		 eGon2_power_info	 			   (void															);
extern  int 		 eGon2_set_charge_current		   (int current														);
extern  int			 eGon2_get_charge_current		   (void															);
extern  int 		 eGon2_power_axp_rest_cal		   (void															);
extern  int 		 eGon2_power_battery_charge_status (void															);
extern  int 		 eGon2_config_charge_current	   (int start_time													);
extern  int 		 eGon2_power_int_enable			   (void															);
extern  int 		 eGon2_power_int_disable		   (void															);
extern  int 		 eGon2_power_int_query			   (unsigned char *status											);
extern  int 		 eGon2_power_vbus_cur_limit		   (int	current														);
extern  int 		 eGon2_power_vbus_vol_limit		   (int vol															);
extern  int 		 eGon2_power_set_data_buffer	   (int value														);
extern  int 		 eGon2_power_type				   (void															);
/* 系统提供的参数获取参数 */
extern  int          eGon2_dispatch_parameters (unsigned int para_name, void *para_addr                                 );
/* 系统提供的跳转服务 */
extern  void         eGon2_jump_to        (unsigned int addr                                                            );
extern  void         eGon2_jump_Fel       (void			                                                                );
extern  void         eGon2_simple_jump_Fel(void                                                                         );
extern  void 		 eGon2_jump_to_android_linux(int zero, int mod_id, unsigned int paddr, unsigned int kernal_addr		);
extern  void 		 eGon2_power_off	  (void																			);
/* 系统提供的频率获取服务，只支持获取coreclock 频率 */
extern  unsigned int eGon2_clock_get 	 (unsigned int clock_name														);
extern  unsigned int eGon2_clock_set     (unsigned int clock_name, 		unsigned int clock_frequency                    );
extern  unsigned int eGon2_clock_set_ext (unsigned int clock_frequency, unsigned int core_vol							);
/* 系统提供的配置脚本参数获取服务 */
extern  int 		 eGon2_script_parser_init(char *script_buf);
extern  int 		 eGon2_script_parser_exit(void);
extern  int 		 eGon2_script_parser_fetch(char *main_name, char *sub_name, int value[], int count);
extern  int 		 eGon2_script_parser_patch(char *main_name, char *sub_name, int value);
extern  int 		 eGon2_script_parser_subkey_count(char *main_name);
extern  int 		 eGon2_script_parser_mainkey_count(void);
extern  int 		 eGon2_script_parser_mainkey_get_gpio_count(char *main_name);
extern  int 		 eGon2_script_parser_mainkey_get_gpio_cfg(char *main_name, void *gpio_cfg, int gpio_count);
/* 系统提供的GPIO配置服务 */
extern  int 		 eGon2_GPIO_init				    (void);
extern  int 		 eGon2_GPIO_exit				    (void);
extern  unsigned int eGon2_GPIO_Request				    (user_gpio_set_t *gpio_list, 	    unsigned int group_count_max			      );
extern  int 		 eGon2_GPIO_Release				    (unsigned int p_handler, 	            		 int if_release_to_default_status );
extern  int		     eGon2_GPIO_Get_All_PIN_Status	    (unsigned int p_handler, user_gpio_set_t *gpio_status, unsigned int  gpio_count_max, unsigned int if_get_from_hardware);
extern  int			 eGon2_GPIO_Get_One_PIN_Status	    (unsigned int p_handler, user_gpio_set_t *gpio_status, 		  const char *gpio_name, unsigned int if_get_from_hardware);
extern  int			 eGon2_GPIO_Set_One_PIN_Status	    (unsigned int p_handler, user_gpio_set_t *gpio_status, 	      const char *gpio_name, unsigned int if_set_to_current_input_status);
extern  int		     eGon2_GPIO_Set_One_PIN_IO_Status   (unsigned int p_handler, unsigned int if_set_to_output_status,const char *gpio_name);
extern  int  		 eGon2_GPIO_Set_One_PIN_Pull	    (unsigned int p_handler, unsigned int set_pull_status,   	  const char *gpio_name);
extern  int  		 eGon2_GPIO_Set_One_PIN_driver_level(unsigned int p_handler, unsigned int set_driver_level,  	  const char *gpio_name);
extern  int			 eGon2_GPIO_Read_One_PIN_Value	    (unsigned int p_handler, 								   	  const char *gpio_name);
extern  int  		 eGon2_GPIO_Write_One_PIN_Value	    (unsigned int p_handler, unsigned int value_to_gpio,     	  const char *gpio_name);
extern  int 		 eGon2_GPIO_Set_cfg                 (void  *gpio_list,       unsigned int group_count_max, 		           int set_gpio);
/* 系统提供的按键服务 */
extern  int          eGon2_key_init                     (void);
extern  int          eGon2_key_exit                     (void);
extern  int          eGon2_key_get_status               (void);
extern  int 		 eGon2_key_get_value				(void);
/* 系统提供的异常检查 */
extern  unsigned int eGon2_boot_detect                  (void);

#endif     //  ifndef __eGon2_sys_h

/* end of eGon2_sys.h */

