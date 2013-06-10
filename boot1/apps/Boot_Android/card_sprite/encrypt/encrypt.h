/*
*******************************************************************************
*                                            Aone project ---- usb peripheral module
*
* File Name 	: encrypt.h
*
* Author 		: javen
*
* Version 		: 1.0
*
* Date 			: 2008.04.24
*
* Description 	:
*
* History 		:
*
********************************************************************************************************************
*/
#ifndef    __ENCRYPT_H__
#define    __ENCRYPT_H__

#include  "../card_sprite_i.h"

__s32  init_code(int encrypt_flag);
__s32  encode(void * ibuf, void * obuf, __u32 len);
__s32  decode(__u32 src_buf, __u32 dest_buf, __u32 len, __u32 *buf_addr);
__s32  exit_code(void);

#endif    //__ENCRYPT_H__
