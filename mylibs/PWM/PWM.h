/*
 * PWM.h
 *
 *  Created on: 2016Äê11ÔÂ28ÈÕ
 *      Author: gtc
 */

#ifndef PWM_H_
#define PWM_H_

#include "..\mylibs.h"

#define EPWM_TIMER_TBPRD  	2040  // Period register  490Hz
#define EPWM_MAX     		RC_CH3_MAX_POINT
#define EPWM_MIN     		RC_CH3_MIN_POINT
#define EPWM_CMP_UP           1
#define EPWM_CMP_DOWN         0

#define PWM_OUTPUT_CH1      1
#define PWM_OUTPUT_CH2     	2
#define PWM_OUTPUT_CH3      3
#define PWM_OUTPUT_CH4      4
#define PWM_OUTPUT_CH5      5
#define PWM_OUTPUT_CH6      6
#define PWM_OUTPUT_CH7      7
#define PWM_OUTPUT_CH8      8

void PWM_Init(void);
void PWM1_Init(void);
void PWM2_Init(void);
void PWM3_Init(void);
void PWM4_Init(void);
int PWM_Output_Update(unsigned char chx,int i);

#endif /* PWM_H_ */
