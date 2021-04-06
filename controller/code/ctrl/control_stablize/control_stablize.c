/*
 * control_stablize.c
 *
 *  Created on: 2017Äê3ÔÂ1ÈÕ
 *      Author: Tianchang
 */
#include "control_stablize.h"

extern unsigned int RC_PWM_Value[RC_CH_Num];
extern float pitch,last_pitch;
extern float roll,last_roll;
extern float yaw,last_yaw;
extern float pitch_rate,last_pitch_rate;
extern float roll_rate,last_roll_rate;
extern float yaw_rate,last_yaw_rate;

void run_control_stablize(void)
{
	float target_roll,target_pitch,target_yaw_rate;
	int target_throttle;
	get_pilot_desired_lean_angles(target_roll,target_pitch);
	target_throttle = get_pilot_desired_throttle();
	target_yaw_rate = get_pilot_desired_yaw_rate();
}

void get_pilot_desired_lean_angles(float roll_out, float pitch_out)
{
	float target_pitch = ((float)RC_PWM_Value[RC_CH1] - RC_CH1_MID_POINT) / (RC_CH1_MAX_POINT - RC_CH1_MIN_POINT) * PITCH_MAX_ANGLE * 2;
	float target_roll  = ((float)RC_PWM_Value[RC_CH2] - RC_CH2_MID_POINT) / (RC_CH2_MAX_POINT - RC_CH2_MIN_POINT) * ROLL_MAX_ANGLE * 2;
	pitch_out = target_pitch;
	roll_out = target_roll;
}

int get_pilot_desired_throttle(void)
{
	int target_throttle = MIN_MOTOR_PWM_VALUE + (int)(((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE);
	return target_throttle;
}

float get_pilot_desired_yaw_rate(void)
{
	float target_yaw_rate = -((float)RC_PWM_Value[RC_CH4] - RC_CH4_MID_POINT) / (RC_CH4_MAX_POINT - RC_CH4_MIN_POINT) * YAW_MAX_ANGLE_RATE * 2;
	return target_yaw_rate;
}

void stablize_PID_control(float target_roll,float target_pitch,float target_yaw_rate,float current_roll,float current_pitch,float current_yaw_rate)
{
	//static
}
