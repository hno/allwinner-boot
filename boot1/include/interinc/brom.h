/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : brom.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.05.21
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.05.21       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __brom_h
#define  __brom_h


/******************************************************************************/
/*                              file head of Brom                             */
/******************************************************************************/
typedef struct _brom_file_head
{	
	__u32  jump_instruction;   // one intruction jumping to real code
	__u8   magic[8];           // ="eGON.BRM", not C-style string.
	__u32  Brom_head_size;     // the size of brom_file_head_t
	__u8   file_head_vsn[4];   // the version of brom_file_head_t
	__u8   Brom_vsn[4];        // Brom version
	__u8   platform[8];        // platform information
}brom_file_head_t;

#define BROM_FILE_HEAD_VERSION         "1100"     // X.X.XX
#define BROM_MAGIC                     "eGON.BRM"


#endif     //  ifndef __brom_h

/* end of brom.h */
