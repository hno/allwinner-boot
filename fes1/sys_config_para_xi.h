/*
************************************************************************************************************************
*
*                             Copyright(C), 2006-2009, softwinner Co., Ltd.
*								        All Rights Reserved
*
* File Name   : sys_config_para.h
*
* Author      : javen
*
* Version     : 1.1.0
*
* Date        : 2009.05.20
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
* javen           2009.05.20       1.1.1        build the file
*
************************************************************************************************************************
*/
#ifndef  __sys_config_para_h
#define  __sys_config_para_h


#define  MAGIC_SYSCFGPARA  "SYS_PARA"


//----------------------------------------------------
//    target_para
//----------------------------------------------------
typedef struct ___TARGET_PARA
{
    unsigned int           debugenable;
    unsigned int           burnClk;//CPU clock in MHz
}___target_para_t;

//----------------------------------------------------
//   p1 Battery
//----------------------------------------------------
typedef struct ___P1_PARA
{
    unsigned int           Battery_cap;
    unsigned int           Discharge_current_rate;		    //35			//�ŵ�ʱ�����ⲿ������MOSFET
}___p1_para_t;

//struct to describe port:pb23<func><mul-drv><pull><data>
typedef struct _gpio
{
    unsigned int         grpNo       :5;//if pin PB23, then phyGrp = 1
    unsigned int         pinNo       :5;//if pin PB23, then phyOfst = 23
    unsigned int         funcSel     :4;//0-3, input/output/others
    unsigned int         mulDrv      :2;//-1~3, -1 if default, other as spec
    unsigned int         pull        :2;//-1~3, -1 if default, other as spec
    unsigned int         data        :1;//-1~3, -1 if default, other as spec, this only valid when funcSel==0/1

    unsigned int         grpNOIsCfg  :1;//grp No is configured
    unsigned int         pinNOIsCfg  :1;//pin No is configured
    unsigned int         funSelIsCfg :1;//funsel is configured
    unsigned int         mulDrvIsCfg :1;//mulDrv is configured
    unsigned int         pullIsCfg   :1;//pull   is configured
    unsigned int         dataIsCfg   :1;//data   is configured
    unsigned int         reseved     :7;

}PinGpio_t;

typedef struct _UartCfg_s{
    PinGpio_t       pinTx;
    /*PinGpio_t       pinRx;*/
    unsigned int             portNo;
}UartCfg_t;

//----------------------------------------------------
//   epos_para
//----------------------------------------------------
typedef struct ___EPOS_PARA
{
    unsigned int           chip;            /* chip version*/
    unsigned int           pid;             /* product version*/
    unsigned int           sid;             /* solution version*/
    unsigned int           bid;             /* board version*/

    ___target_para_t target;         /* 0 debug, 1 release*///pos = 4 * 4

    UartCfg_t       cfgUart;
    __dram_para_t   dram_para;       //pos: 8 * 4

    ___p1_para_t    p1_para;         //pos: (8 + 24) * 4
    unsigned char            reserved[512 - 10 * sizeof(int) - sizeof(__dram_para_t)];  //

}___epos_para_t;

#endif

