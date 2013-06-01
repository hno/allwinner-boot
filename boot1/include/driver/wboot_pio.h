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
#ifndef	_WBOOT_PIO_H_
#define	_WBOOT_PIO_H_


#define   DATA_TYPE_SINGLE_WORD  (1)
#define   DATA_TYPE_STRING       (2)
#define   DATA_TYPE_MULTI_WORD   (3)
#define   DATA_TYPE_GPIO_WORD    (4)

#define   SCRIPT_PARSER_OK       (0)
#define   SCRIPT_PARSER_EMPTY_BUFFER   	(-1)
#define   SCRIPT_PARSER_KEYNAME_NULL   	(-2)
#define   SCRIPT_PARSER_DATA_VALUE_NULL	(-3)
#define   SCRIPT_PARSER_KEY_NOT_FIND    (-4)

typedef struct
{
	char  gpio_name[32];
	int port;
	int port_num;
	int mul_sel;
	int pull;
	int drv_level;
	int data;
}
script_gpio_set_t;

typedef struct
{
    char  gpio_name[32];
    int port;
    int port_num;
    int mul_sel;
    int pull;
    int drv_level;
    int data;
}
user_gpio_set_t;


#endif	//_WBOOT_TWI_H_

/* end of _WBOOT_TWI_H_ */

