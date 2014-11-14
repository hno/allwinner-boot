/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#include "include.h"

extern   int  force_debug;
extern   int  cue;


H_FILE   log_file  = NULL;
char     logfile[] = "z:\\slog0.txt";

void sprite_wrn_init(void)
{
//	H_FILE  log_file0  = NULL;
//
//	if(1 == cue)
//	{
//		__inf("zdisk mount\n");
//		if(wBoot_fsmount('Z'))		//其实是挂标准MBR的用户盘
//		{
//			__inf("log error: cant find user disk\n");
//
//			return;
//		}
//		__inf("zdisk mount ok\n");
//		log_file0 = NULL;
//		do
//		{
//			log_file0 = wBoot_fopen(logfile, "rb");
//			if(!log_file0)
//			{
//				break;
//			}
//			wBoot_fclose(log_file0);
//			logfile[7] ++;
//		}
//		while(1);
//		__inf("log file name %s\n", logfile);
//		log_file = wBoot_fopen(logfile, "wb");
//		if(!log_file)
//		{
//			__inf("log error: cant open log file\n");
//
//			return;
//		}
//		__inf("log file %s open ok\n", logfile);
//	}
}

void sprite_wrn_exit(int status)
{
//	if(1 == cue)
//	{
//		if(log_file)
//		{
//			wBoot_fclose(log_file);
//			log_file = NULL;
//			if(!status)
//			{
//				wBoot_rmfile(logfile);
//			}
//		}
//
//	}
}

void sprite_wrn(const char *str, ...)
{
	__inf(str);
//	if(!cue)
	{
		boot_ui_printf(str);
	}
////	else if(1 == cue)
//	{
//		if(log_file)
//		{
//			char buf[512];
//
//			memset(buf, 0, 512);
//			wlibc_sprintf(buf, str);
//			wBoot_write(buf, 1, strlen(buf), log_file);
//		}
//	}

	return;
}


