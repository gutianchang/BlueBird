/*
 * motor.c
 *
 *  Created on: 2017Äê1ÔÂ14ÈÕ
 *      Author: Tianchang
 */

#include "motor.h"
extern unsigned int RC_PWM_Value[RC_CH_Num];
unsigned int Motor1_PWM_Value = MIN_MOTOR_PWM_VALUE;
unsigned int Motor2_PWM_Value = MIN_MOTOR_PWM_VALUE;
unsigned int Motor3_PWM_Value = MIN_MOTOR_PWM_VALUE;
unsigned int Motor4_PWM_Value = MIN_MOTOR_PWM_VALUE;

void Motor_PWM_Update(void)
{
	if(RC_PWM_Value[RC_CH5] > ARMED_PWM_VALUE)
	{
		PWM_Output_Update(PWM_OUTPUT_CH1,Motor1_PWM_Value);
		PWM_Output_Update(PWM_OUTPUT_CH2,Motor2_PWM_Value);
		PWM_Output_Update(PWM_OUTPUT_CH3,Motor3_PWM_Value);
		PWM_Output_Update(PWM_OUTPUT_CH4,Motor4_PWM_Value);
	}
	else{
		PWM_Output_Update(PWM_OUTPUT_CH1,MIN_MOTOR_PWM_VALUE);
		PWM_Output_Update(PWM_OUTPUT_CH2,MIN_MOTOR_PWM_VALUE);
		PWM_Output_Update(PWM_OUTPUT_CH3,MIN_MOTOR_PWM_VALUE);
		PWM_Output_Update(PWM_OUTPUT_CH4,MIN_MOTOR_PWM_VALUE);
	}
}


