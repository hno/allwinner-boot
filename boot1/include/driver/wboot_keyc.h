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
   WBOOT_KEY_NULL      = 0,//Î´¶¨Òå°´¼ü
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
