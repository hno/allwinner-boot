/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : drv_demo.h
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
#ifndef  _eGon2_common_H_
#define  _eGon2_common_H_

/* 系统提供的打印函数，输出端在PC屏幕上 */
extern  void  wlibc_uprintf( const char * str, ...);
extern  void  wlibc_ntprintf( const char * str, ...);

#if 1
#define __inf(...)          (wlibc_uprintf(__VA_ARGS__))

#define __msg(...)    		(wlibc_uprintf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     wlibc_ntprintf(__VA_ARGS__))

#define __wrn(...)          (wlibc_uprintf("WRN:L%d(%s):", __LINE__, __FILE__),                 \
    						     wlibc_ntprintf(__VA_ARGS__))
#ifdef BOOT1_DEBUG
#define __debug(...)        (wlibc_uprintf("DEBUG:L%d(%s):", __LINE__, __FILE__),                \
    						     wlibc_ntprintf(__VA_ARGS__))
#else
#define __debug(...)
#endif

#else

#define __inf(...)
#define __msg(...)
#define __wrn(...)
#define __debug(...)

#endif
/* 系统提供的twi操作函数 */
extern  int   wlibc_twi_init   (unsigned int twi_base,    unsigned int clock);
extern  int   wlibc_twi_exit   (void                                        );
extern  int   wlibc_twi_setfreq(unsigned int clock                          );
extern  int   wlibc_twi_read   (unsigned int slave_addr,  unsigned int if_slave_addr_10bit, unsigned char *byte_addr, unsigned int byte_addr_width, unsigned int byte_count, unsigned char *data, unsigned int if_restart);
extern  int   wlibc_twi_write  (unsigned int slave_addr,  unsigned int if_slave_addr_10bit, unsigned char *byte_addr, unsigned int byte_addr_width, unsigned int byte_count, unsigned char *data                         );

/* 系统提供的cache服务 */
extern  void  wlibc_CleanDCacheRegion       (void *adr, unsigned int bytes);
extern  void  wlibc_CleanFlushDCacheRegion  (void *adr, unsigned int bytes);
extern  void  wlibc_CleanFlushCacheRegion   (void *adr, unsigned int bytes);
extern  void  wlibc_FlushICacheRegion       (void *adr, unsigned int bytes);
extern  void  wlibc_FlushDCacheRegion       (void *adr, unsigned int bytes);
extern  void  wlibc_FlushCacheRegion        (void *adr, unsigned int bytes);

extern  void  wlibc_DCacheEnable            (void);
extern  void  wlibc_DCacheDisable           (void);
extern  void  wlibc_ICacheEnable            (void);
extern  void  wlibc_ICacheDisable           (void);
/* 系统提供的MMU服务 */
extern  void   wlibc_mmu_enable( void );
extern  void   wlibc_mmu_disable( void );
/* 系统提供的中断开启/关闭服务 */
extern  void   wlibc_int_enable( void );
extern  void   wlibc_int_disable( void );

#endif  /* _eGon2_common_H_ */
