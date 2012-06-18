/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON twi sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : wboot_twi.h
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

