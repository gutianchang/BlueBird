/*
 * Radar.c
 *
 *  Created on: 2017Äê3ÔÂ21ÈÕ
 *      Author: Tianchang
 */
#include "Radar.h"
char *ra = "channelCfg 15 5 0";//17
char *rb = "adcCfg 2 1";		//10
char *rc = "adcbufCfg 15 0 1 0 1";//20
char *rd = "profileCfg 0 77 7 7 58 0 0 34 1 225 4500 0 0 30";//47
char *re = "chirpCfg 0 0 0 0 0 0 0 1";//24
char *rf = "chirpCfg 1 1 0 0 0 0 0 4";//24
char *rg = "frameCfg 0 1 16 0 225 100 1 0";//29
char *rh = "guiMonitor 1 0 0 0";//18
char *ri = "cfarCfg 2 8 4 3 0 0 1200";//24
char *rj = "sensorStart";//11
void Radar_Init(void)
{
	Scib_Send_Msg(ra,17);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rb,10);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rc,20);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rd,47);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(re,24);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rf,24);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rg,29);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rh,18);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(ri,24);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);

	Scib_Send_Msg(rj,11);
	Scib_Send_Byte(0x0D);
	Scib_Send_Byte(0x0A);
	DELAY_US(10000);
}



