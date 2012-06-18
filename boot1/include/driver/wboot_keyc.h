/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                                                            All Rights Reserved
*
* File        : drv_keyc.h
*
* By         : Victor
*
* Version  : V1.00
*
* Date	  :2010-09-21
*
* Description:
*            lradc key的驱动，维护一个按键消息队列，按键按下或释放 时会放到
*      消息队列去，上层用户获取按键消息时，直接通过ioctl函数获取。
*
**********************************************************************************************************************
*/
#ifndef  _WBOOT_KEYC_H_
#define  _WBOOT_KEYC_H_

/* key driver module id */
#define EGON2_KEYC_MID  EGON2_EMOD_TYPE_DRV_KEY

/* IOcontrol command set */
typedef enum
{
    WBOOT_KEYMSG_QUERY_EMPTY  = 0, /* Query the circle queue is empty. if empty return eGON2_OK. */
    WBOOT_KEYMSG_QUERY_LENGTH   ,  /* Get the circle queue's length. return length. */
    WBOOT_KEYMSG_QUERY_SIZE     ,  /* Get the size of circle queue. return size. */
    WBOOT_KEYMSG_FLUSH_ALL      ,  /* Flush the circle queue. if succeed return eGON2_OK. */
    WBOOT_KEYMSG_PRINT_ALL      ,  /* Print the Key message in the circle queue. if succeed return eGON2_OK. */
    WBOOT_KEYMSG_READ_KEYCODE   ,  /* Read the key from the circle queue. if succeed return eGON2_OK. */
    WBOOT_KEYMSG_GET_KEYTIME    ,  /* Get key response time, Unit: ms.if succeed return Key Response time */	
    WBOOT_KEYMSG_SET_KEYTIME    ,  /* Set key repeat response time,Unit:ms.if succeed return eGON2_OK. */
}
wboot_key_cmd_set_t;

/* key_value */
typedef enum
{
   WBOOT_KEY_NULL      = 0,//未定义按键
   WBOOT_KEY_POWER     = 1,// 0x01    
   
   WBOOT_KEY_ENTER        ,// 0x02
   WBOOT_KEY_ESC          ,// 0x03 
   WBOOT_KEY_LEFT         ,// 0x04
   WBOOT_KEY_RIGHT        ,// 0x05
   WBOOT_KEY_UP           ,// 0x06 
   WBOOT_KEY_DOWN         ,// 0x07
   WBOOT_KEY_VOICE_UP     ,// 0x08 
   WBOOT_KEY_VOICE_DOWN   ,// 0x09   
   WBOOT_KEY_HOLDKEY      ,// 0x0a
   
   WBOOT_KEY_POWEROFF     ,// 0x0b
   WBOOT_KEY_RST          ,// 0x0c
   WBOOT_KEY_DISPLAY      ,//0x0d
   WBOOT_KEY_REPEATE      ,// 0x0e
   WBOOT_KEY_PLAY_PAUSE   ,//0x0f
   WBOOT_KEY_MENU         ,//0x10
}
wboot_key_msg_set_t;

/* KEY Message */
typedef struct tag_key_msg_info
{
    u32 key_value;  //key value.
    u32 flag;       //1:keydown; 0:keyup.		
}
key_msg_t;

#endif  /* _WBOOT_KEYC_H_ */
