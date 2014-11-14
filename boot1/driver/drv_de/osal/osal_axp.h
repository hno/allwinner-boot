#ifndef __AXP_HEAD__
#define __AXP_HEAD__
#include "p2wi.h"

#define   BOOT_POWER22_OUTPUT_CTL2     	   			(0x12)
#define   BOOT_POWER22_OUTPUT_DLDO3_VO					(0x17)
#define   BOOT_POWER22_OUTPUT_DLDO3_SW					(0x12)
#define   BOOT_POWER22_OUTPUT_ELDO3_VO				(0x1B)
#define   BOOT_POWER22_OUTPUT_DLDO1_VO					(0x15)

int axp221_set_dc1sw(int on_off);
int axp221_set_dldo3(int on_off, int voltage);
int axp221_set_dldo1(int on_off, int voltage);
int axp221_set_eldo3(int on_off, int voltage);

#endif

