/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, Allwinner Microelectronic Co., Ltd.
*											       All Rights Reserved
*
*File Name£º format_transformed.h
*
*Author£º Gary.Wang
*
*Version 1.1.0
*
*Date  2007.07.30
*
*Description£º
*
*Others : None at present.
*
*
* History :
*
*  <Author>         <time>      <version>     <description>
*
* Wangyugang      2007.07.30      1.1.0        build the file
*
*******************************************************************************************************************
*/

#ifndef  __format_transformed_h
#define  __format_transformed_h




#define  HEX_x   'x'
#define  HEX_X   'X'




//This function is to convert an 'int' data 'input' to a string in decimalism,
//and the string converted is in 'str'.
extern void int_to_string_dec( int input, char * str );


//This function is to convert an 'int' data 'input' to a string in hex,
//and the string converted is in 'str'.
extern void int_to_string_hex( int input, char * str, int hex_flag );


// This function is to convert an 'unsigned int' data 'input' to a string in decimalism, and
// the  string converted is in 'str'.
extern void Uint_to_string_dec( unsigned int input, char * str );






#endif     //  ifndef __format_transformed_h

/* end of format_transformed.h  */
