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
/* 系统提供的twi操作函数 */
extern  int   wlibc_twi_init   (unsigned int twi_base,    unsigned int clock);
extern  int   wlibc_twi_exit   (void                                        );
extern  int   wlibc_twi_setfreq(unsigned int clock                          );
extern  int   wlibc_twi_read   (unsigned int slave_addr,  unsigned int if_slave_addr_10bit, unsigned char *byte_addr, unsigned int byte_addr_width, unsigned int byte_count, unsigned char *data, unsigned int if_restart);
extern  int   wlibc_twi_write  (unsigned int slave_addr,  unsigned int if_slave_addr_10bit, unsigned char *byte_addr, unsigned int byte_addr_width, unsigned int byte_count, unsigned char *data                         );



#define __inf(...)          (wlibc_uprintf(__VA_ARGS__)

#define __msg(...)    		(wlibc_uprintf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     wlibc_uprintf(__VA_ARGS__)									        )

#define __wrn(...)          (wlibc_uprintf("WRN:L%d(%s):", __LINE__, __FILE__),                 \
    						     wlibc_uprintf(__VA_ARGS__)									        )


#endif  /* _eGon2_common_H_ */
