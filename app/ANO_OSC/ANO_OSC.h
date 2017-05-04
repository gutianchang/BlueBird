/*
 * ANO_OSC.h
 *
 *  Created on: 2016Äê11ÔÂ24ÈÕ
 *      Author: gtc
 */

#ifndef ANO_OSC_H_
#define ANO_OSC_H_

#include "..\app.h"
#define GET_DATA_FROM_ANO_OSC_LEN	23

#define PID1	0x10
#define PID2	0x11
#define PID3	0x12
#define PID4	0x13

void ANO_OSC_MPU(void);
void ANO_OSC_RC(void);
void ANO_OSC_IMU(void);
void ANO_OSC_IMU2(void);
void ANO_OSC_IMU3(void);
void ANO_OSC_Motor_Output(void);
void ANO_OSC_PID(void);
void Get_PID_from_ANO_OSC(unsigned char data);
void ANO_DT_Data_Receive_Prepare(unsigned char data);

#endif /* ANO_OSC_H_ */
