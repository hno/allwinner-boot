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
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//    I MMMMMMMMMMI                                                                                         //
//      I FF      BBI                                                                                       //
//      I FF      I EE  FFMMFFI MMMMLL  I MMMMMMEE          EEMMBBLLBBFF    FFMMMMBBI   I MMBBBBMMMMI       //
//      I FF        EEI   I BBBBI   I     I     LLFF      EELL  I BBFF    FFFF    I BBI   I BBI   I EE      //
//      I FF        EEI   I FF          I BBMMMMMMFF      FF      I FF  I EE        I LL  I FF      EEI     //
//      I FF      I EE    I FF          EEI     I FF      FF      I FF  I EE        I LL  I FF      EEI     //
//      I FF      FFI     I FF          EEI     BBFF      FFI     EEFF    FFI       FFI   I FF      EEI     //
//    I MMMMMMMMMMFF    BBMMMMMMMMFF    LLMMMMMMFFMMFF    I BBMMMMFFFF    I BBMMMMMMLL  I MMMMFF  BBMMBBI   //
//                                                                I FF                                      //
//                                                                FFLL                                      //
//                                                          BBMMMMEE                                        //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                              Dragon System                                               //
//                                                                                                          //
//                               (c) Copyright 2007-2008, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : imagefile.h                                                                                    //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-11-03 9:36:12                                                                             //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-11-03 9:36:16                                                                                     //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#ifndef __IMAGE_FORMAT__H__
#define __IMAGE_FORMAT__H__	1


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
/*
#define ITEM_COMMON 		  "COMMON  "
#define ITEM_INFO   		  "INFO    "
#define ITEM_BOOTROM 		  "BOOTROM "
#define ITEM_FES 			  "FES     "
#define ITEM_FET 			  "FET     "
#define ITEM_FED 			  "FED     "
#define ITEM_FEX 			  "FEX     "
#define ITEM_BOOT0 			  "BOOT0   "
#define ITEM_BOOT1 			  "BOOT1   "
#define ITEM_ROOTFSFAT12	  "RFSFAT12"
#define ITEM_ROOTFSFAT16	  "RFSFAT16"
#define ITEM_ROOTFSFAT32	  "FFSFAT32"
#define ITEM_USERFSFAT12	  "UFSFAT12"
#define ITEM_USERFSFAT16	  "UFSFAT16"
#define ITEM_USERFSFAT32	  "UFSFAT32"
#define ITEM_PHOENIX_SCRIPT   "PXSCRIPT"
#define ITEM_PHOENIX_TOOLS 	  "PXTOOLS "
#define ITEM_AUDIO_DSP 		  "AUDIODSP"
#define ITEM_VIDEO_DSP 		  "VIDEODSP"
#define ITEM_FONT			  "FONT    "
#define ITEM_FLASH_DRV 		  "FLASHDRV"
#define ITEM_OS_CORE 		  "OS_CORE "
#define ITEM_DRIVER 		  "DRIVER  "
#define ITEM_PIC 			  "PICTURE "
#define ITEM_AUDIO 			  "AUDIO   "
#define ITEM_VIDEO 			  "VIDEO   "
#define ITEM_APPLICATION 	  "APP     "
*/

//#define IMAGE_VER	100
//------------------------------------------------------------------------------------------------------------
#define IMAGE_MAGIC			"IMAGEWTY"
#define	IMAGE_HEAD_VERSION	0x00000100

//------------------------------------------------------------------------------------------------------------
///Image文件头数据结构
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_ImageHead
{
    __u8  magic[8];                     // IMAGE_MAGIC
    __u32 version;                      // 本结构的版本号，IMAGE_HEAD_VERSION
    __u32 size;                         // 本结构的长度
    __u32 attr;                         // 本结构的属性，格式按照version来确定，加密，压缩等
    __u32 imagever;                     // image的版本，由脚本指定
    __u32 lenLo;                        // image文件的总长度，低位
    __u32 lenHi;                        // image文件的总长度，高位
    __u32 align;                        // 数据的对齐边界，缺省1024
    __u32 pid;                          // PID信息
    __u32 vid;                          // VID信息
    __u32 hardwareid;                   // 硬件平台ID
    __u32 firmwareid;                   // 软件平台ID
    __u32 itemattr;                     // item表的属性,“加密”
    __u32 itemsize;                     // item数据项的大小
    __u32 itemcount;                    // item数据项的个数
    __u32 itemoffset;                   // item表偏移量
    __u32 imageattr;                    // image文件属性
    __u32 appendsize;                   // 附加数据的长度
    __u32 appendoffsetLo;               // 附加数据的偏移量
    __u32 appendoffsetHi;               // 附加数据的偏移量
    __u8  res[12];                      // 保留

}ImageHead_t;
#pragma pack(pop)


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define ALIGN_SIZE				0x400
#define HEAD_ATTR_NO_COMPRESS 	0x4d //1001101

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tagImageHeadAttr{
	__u32	res		: 12;
	__u32 len		: 8;
	__u32 encode	: 7;		///HEAD_ATTR_NO_COMPRESS
	__u32 compress: 5;
}ImageHeadAttr_t;
#pragma pack(pop)

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define	IMAGE_ITEM_VERSION	0x00000100
#define MAINTYPE_LEN		8
#define SUBTYPE_LEN			16
#define FILE_PATH			256
//------------------------------------------------------------------------------------------------------------
///数据项数据结构
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_ImageItem
{
											// 考虑是否需要加magic
	__u32 version;						// 本结构的版本号IMAGE_ITEM_VERSION
	__u32 size; 						// 本结构的长度
	__u8  mainType[MAINTYPE_LEN];		// 描述的文件的类型
	__u8  subType[SUBTYPE_LEN]; 		// 描述子类型，默认由image配置脚本指定
	__u32 attr; 						// 描述的文件的属性,格式按照version来确定，加密，压缩等
	__u8  name[FILE_PATH];				// 文件名称 260
	__u32 datalenLo;					// 文件数据在image文件中的长度，低位
	__u32 datalenHi;					// 文件数据在image文件中的长度，高位
	__u32 filelenLo;					// 文件的实际长度，低位
	__u32 filelenHi;					// 文件的实际长度，高位
	__u32 offsetLo; 					// 文件起始位置偏移量，低位
	__u32 offsetHi; 					// 文件起始位置偏移量，高位
	__u8  encryptID[64];				// 加密文件ID
	__u32 checksum; 					// 描述的文件的校验和
	__u8  res[640]; 					// 保留

}ImageItem_t;
#pragma pack(pop)




//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //__IMAGE_FORMAT__H__

