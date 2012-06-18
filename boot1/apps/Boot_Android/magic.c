/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : magic.c
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
#include  "egon2.h"
#include  "BootMain.h"

const eGon2_mod_t modinfo =
{
    {'e','G','o','n','2','a','p','p'},		//MAGIC字符，用于标识是eGON2的驱动/应用代码
    0x01000000,                			//版本
    0x0,                                    	//模块的ID，每个elf都应该有一个模块ID，不论驱动还是应用，且各不相同
    BootMain,                          		//pcb里第一个任务的首地址
    0						//驱动函数列表，列出了驱动所必要的7个函数
};
