/*
 * PID.c
 *
 *  Created on: 2017Äê1ÔÂ13ÈÕ
 *      Author: Tianchang
 */

#include "PID.h"

#define Ts	0.002
#define fs	500
#define MIN_SUM_DIF_PITCH 	-50
#define MAX_SUM_DIF_PITCH	50
#define MIN_SUM_DIF_ROLL	-50
#define MAX_SUM_DIF_ROLL	50

#define MIN_PITCH_RATE_ERR_SUM	-100
#define MAX_PITCH_RATE_ERR_SUM	100
#define MIN_ROLL_RATE_ERR_SUM	-100
#define MAX_ROLL_RATE_ERR_SUM	100
#define MIN_YAW_RATE_ERR_SUM	-100
#define MAX_YAW_RATE_ERR_SUM 	100

#define MAX_PID1_OUTPUT			200
#define MIN_PID1_OUTPUT			-200

//extern float pitch,roll,yaw_rate;
//extern float gyro_raw_data_z;
extern unsigned int RC_PWM_Value[RC_CH_Num];
extern unsigned int Motor1_PWM_Value;
extern unsigned int Motor2_PWM_Value;
extern unsigned int Motor3_PWM_Value;
extern unsigned int Motor4_PWM_Value;

float pitch_rate_err = 0;
float roll_rate_err = 0;
float yaw_rate_err = 0;

float last_pitch_rate_err = 0;
float last_roll_rate_err = 0;
float last_yaw_rate_err = 0;

float pitch_rate_err_sum = 0;
float roll_rate_err_sum = 0;
float yaw_rate_err_sum = 0;

float target_pitch = 0;
float target_roll = 0;
float target_yaw = 0;

float target_pitch_rate = 0;
float target_roll_rate = 0;
float target_yaw_rate = 0;

float pitch_err = 0;
float roll_err = 0;
float yaw_err = 0;

//extern float last_pitch;
//extern float last_roll;
//extern float last_yaw_rate;
//
//float cur_dif_pitch = 0;
//float cur_dif_roll = 0;
//float cur_dif_yaw_rate= 0;
//
//float last_dif_pitch = 0;
//float last_dif_roll = 0;
//float last_dif_yaw_rate = 0;
//
//float dev_dif_pitch = 0;
//float dev_dif_roll;
//float dev_dif_yaw_rate;
//
//float sum_dif_pitch = 0;
//float sum_dif_roll = 0;
//float sum_dif_yaw_rate = 0;

////P
//float pitch_p = 1.8;
//float roll_p = 1.8;
//float yaw_rate_p = 1.14;
////D
//float pitch_d = 0.4;
//float roll_d = 0.4;
//float yaw_rate_d = 0;
////I
//float pitch_i = 0;
//float roll_i = 0;
//float yaw_rate_i = 0;

float roll_p = 2;
float pitch_p = 2;

//float pitch_rate_p = 0;
//float pitch_rate_i = 0;
//float pitch_rate_d = 0;
//
//float roll_rate_p = 0;
//float roll_rate_i = 0;
//float roll_rate_d = 0;
//
//float yaw_rate_p = 0;
//float yaw_rate_i = 0;
//float yaw_rate_d = 0;

float pitch_rate_p = 0.65;
float pitch_rate_i = 0.4;
float pitch_rate_d = 0.001;

float roll_rate_p = 0.65;
float roll_rate_i = 0.4;
float roll_rate_d = 0.001;

float yaw_rate_p = 1.14;
float yaw_rate_i = 0;
float yaw_rate_d = 0;

extern float roll;
extern float pitch;
extern float pitch_rate,last_pitch_rate;
extern float roll_rate,last_roll_rate;
extern float yaw_rate,last_yaw_rate;

//void PID_Control(void)
//{
//	float Motor1_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
//	float Motor2_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
//	float Motor3_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
//	float Motor4_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
//
////	pitch_p = 1.2;//1.2;//= ((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 400;
////	roll_p = 1.0;//((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 400;
////	yaw_rate_p = 1.14;
////
////	pitch_d = 0.472;//(((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 500) * 500; //0.002ms = 1/500 s is the control period
////	roll_d = 0.428;//((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 500;
////	yaw_rate_d = ((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 600;
//
////	pitch_i = 0.64;//((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 200;
////	roll_i = 1.07;
////	yaw_rate_i = 0;
//
//	float target_pitch = ((float)RC_PWM_Value[RC_CH1] - RC_CH1_MID_POINT) / (RC_CH1_MAX_POINT - RC_CH1_MIN_POINT) * PITCH_MAX_ANGLE * 2;
//	float target_roll  = ((float)RC_PWM_Value[RC_CH2] - RC_CH2_MID_POINT) / (RC_CH2_MAX_POINT - RC_CH2_MIN_POINT) * ROLL_MAX_ANGLE * 2;
//	float target_yaw_rate = -((float)RC_PWM_Value[RC_CH4] - RC_CH4_MID_POINT) / (RC_CH4_MAX_POINT - RC_CH4_MIN_POINT) * YAW_MAX_ANGLE_RATE * 2;
//
//	cur_dif_pitch = target_pitch - pitch;
//	cur_dif_roll = target_roll - roll;
//	cur_dif_yaw_rate = target_yaw_rate - yaw_rate;
//
//	Motor1_PWM_Value_temp = (  pitch_p * cur_dif_pitch - roll_p * cur_dif_roll - yaw_rate_p * cur_dif_yaw_rate);
//	Motor2_PWM_Value_temp = (- pitch_p * cur_dif_pitch + roll_p * cur_dif_roll - yaw_rate_p * cur_dif_yaw_rate);
//	Motor3_PWM_Value_temp = (  pitch_p * cur_dif_pitch + roll_p * cur_dif_roll + yaw_rate_p * cur_dif_yaw_rate);
//	Motor4_PWM_Value_temp = (- pitch_p * cur_dif_pitch - roll_p * cur_dif_roll + yaw_rate_p * cur_dif_yaw_rate);
//
//	dev_dif_pitch = last_pitch - pitch;
//	dev_dif_roll = last_roll - roll;
//	dev_dif_yaw_rate = last_yaw_rate - yaw_rate;
//
//	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + fs * pitch_d * dev_dif_pitch - fs * roll_d * dev_dif_roll - fs * yaw_rate_d * dev_dif_yaw_rate;
//	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp - fs * pitch_d * dev_dif_pitch + fs * roll_d * dev_dif_roll - fs * yaw_rate_d * dev_dif_yaw_rate;
//	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + fs * pitch_d * dev_dif_pitch + fs * roll_d * dev_dif_roll + fs * yaw_rate_d * dev_dif_yaw_rate;
//	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp - fs * pitch_d * dev_dif_pitch - fs * roll_d * dev_dif_roll + fs * yaw_rate_d * dev_dif_yaw_rate;
//
//	if(sum_dif_pitch < MIN_SUM_DIF_PITCH)
//	{
//		if(cur_dif_pitch > 0)
//		{
//			sum_dif_pitch = sum_dif_pitch + Ts * pitch_i * cur_dif_pitch;
//		}
//	}
//	else if(sum_dif_pitch > MAX_SUM_DIF_PITCH)
//	{
//		if(cur_dif_pitch < 0)
//		{
//			sum_dif_pitch = sum_dif_pitch + Ts * pitch_i * cur_dif_pitch;
//		}
//	}
//	else{
//		sum_dif_pitch = sum_dif_pitch + Ts * pitch_i * cur_dif_pitch;
//	}
//
//	if(sum_dif_roll < MIN_SUM_DIF_ROLL)
//	{
//		if(cur_dif_roll > 0)
//		{
//			sum_dif_roll = sum_dif_roll + Ts * roll_i * cur_dif_roll;
//		}
//	}
//	else if(sum_dif_roll > MAX_SUM_DIF_ROLL)
//	{
//		if(cur_dif_roll < 0)
//		{
//			sum_dif_roll = sum_dif_roll + Ts * roll_i * cur_dif_roll;
//		}
//	}
//	else{
//		sum_dif_roll = sum_dif_roll + Ts * roll_i * cur_dif_roll;
//	}
//
//
//	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + sum_dif_pitch - sum_dif_roll;
//	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp - sum_dif_pitch + sum_dif_roll;
//	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + sum_dif_pitch + sum_dif_roll;
//	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp - sum_dif_pitch - sum_dif_roll;
//
//
//	if(Motor1_PWM_Value_temp > MAX_PID_OUTPUT)
//		Motor1_PWM_Value_temp = MAX_PID_OUTPUT;
//	if(Motor2_PWM_Value_temp > MAX_PID_OUTPUT)
//		Motor2_PWM_Value_temp = MAX_PID_OUTPUT;
//	if(Motor3_PWM_Value_temp > MAX_PID_OUTPUT)
//		Motor3_PWM_Value_temp = MAX_PID_OUTPUT;
//	if(Motor4_PWM_Value_temp > MAX_PID_OUTPUT)
//		Motor4_PWM_Value_temp = MAX_PID_OUTPUT;
//
//	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
//	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
//	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
//	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
//
//	if(Motor1_PWM_Value_temp < MIN_MOTOR_PWM_VALUE)
//		Motor1_PWM_Value_temp = MIN_MOTOR_PWM_VALUE;
//	else if(Motor1_PWM_Value_temp > MAX_MOTOR_PWM_VALUE)
//		Motor1_PWM_Value_temp = MAX_MOTOR_PWM_VALUE;
//	if(Motor2_PWM_Value_temp < MIN_MOTOR_PWM_VALUE)
//		Motor2_PWM_Value_temp = MIN_MOTOR_PWM_VALUE;
//	else if(Motor2_PWM_Value_temp > MAX_MOTOR_PWM_VALUE)
//		Motor2_PWM_Value_temp = MAX_MOTOR_PWM_VALUE;
//	if(Motor3_PWM_Value_temp < MIN_MOTOR_PWM_VALUE)
//		Motor3_PWM_Value_temp = MIN_MOTOR_PWM_VALUE;
//	else if(Motor3_PWM_Value_temp > MAX_MOTOR_PWM_VALUE)
//		Motor3_PWM_Value_temp = MAX_MOTOR_PWM_VALUE;
//	if(Motor4_PWM_Value_temp < MIN_MOTOR_PWM_VALUE)
//		Motor4_PWM_Value_temp = MIN_MOTOR_PWM_VALUE;
//	else if(Motor4_PWM_Value_temp > MAX_MOTOR_PWM_VALUE)
//		Motor4_PWM_Value_temp = MAX_MOTOR_PWM_VALUE;
//
//	Motor1_PWM_Value = (unsigned int)Motor1_PWM_Value_temp;
//	Motor2_PWM_Value = (unsigned int)Motor2_PWM_Value_temp;
//	Motor3_PWM_Value = (unsigned int)Motor3_PWM_Value_temp;
//	Motor4_PWM_Value = (unsigned int)Motor4_PWM_Value_temp;
//
//	last_pitch = pitch;
//	last_roll = roll;
//	last_yaw_rate = yaw_rate;
//
//	last_dif_pitch = cur_dif_pitch;
//	last_dif_roll = cur_dif_roll;
//	last_dif_yaw_rate = cur_dif_yaw_rate;
//}

void pidRateCtl(void)
{
	float Motor1_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
	float Motor2_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
	float Motor3_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;
	float Motor4_PWM_Value_temp = MIN_MOTOR_PWM_VALUE * 1.0f;

	target_pitch = ((float)RC_PWM_Value[RC_CH1] - RC_CH1_MID_POINT) / (RC_CH1_MAX_POINT - RC_CH1_MIN_POINT) * PITCH_MAX_ANGLE * 2;
	target_roll  = ((float)RC_PWM_Value[RC_CH2] - RC_CH2_MID_POINT) / (RC_CH2_MAX_POINT - RC_CH2_MIN_POINT) * ROLL_MAX_ANGLE * 2;

	roll_p = ((float)RC_PWM_Value[RC_CH6] - RC_CH6_MIN_POINT) / 200;
	pitch_p = roll_p;

	roll_err = target_roll - roll;
	pitch_err = target_pitch - pitch;

	target_roll_rate = roll_p * roll_err;
	target_pitch_rate = pitch_p * pitch_err;
//	target_pitch_rate = ((float)RC_PWM_Value[RC_CH1] - RC_CH1_MID_POINT) / (RC_CH1_MAX_POINT - RC_CH1_MIN_POINT) * PITCH_MAX_ANGLE_RATE * 2;
//	target_roll_rate  = ((float)RC_PWM_Value[RC_CH2] - RC_CH2_MID_POINT) / (RC_CH2_MAX_POINT - RC_CH2_MIN_POINT) * ROLL_MAX_ANGLE_RATE * 2;
////	target_roll_rate  = ((float)RC_PWM_Value[RC_CH6] - RC_CH6_MID_POINT) / (RC_CH6_MAX_POINT - RC_CH6_MIN_POINT) * ROLL_MAX_ANGLE_RATE * 2;
	target_yaw_rate = -((float)RC_PWM_Value[RC_CH4] - RC_CH4_MID_POINT) / (RC_CH4_MAX_POINT - RC_CH4_MIN_POINT) * YAW_MAX_ANGLE_RATE * 2;

	pitch_rate_err = target_pitch_rate - pitch_rate;
	roll_rate_err = target_roll_rate - roll_rate;
	yaw_rate_err = target_yaw_rate - yaw_rate;

	Motor1_PWM_Value_temp = (  pitch_rate_p * pitch_rate_err - roll_rate_p * roll_rate_err - yaw_rate_p * yaw_rate_err);
	Motor2_PWM_Value_temp = (- pitch_rate_p * pitch_rate_err + roll_rate_p * roll_rate_err - yaw_rate_p * yaw_rate_err);
	Motor3_PWM_Value_temp = (  pitch_rate_p * pitch_rate_err + roll_rate_p * roll_rate_err + yaw_rate_p * yaw_rate_err);
	Motor4_PWM_Value_temp = (- pitch_rate_p * pitch_rate_err - roll_rate_p * roll_rate_err + yaw_rate_p * yaw_rate_err);

	float pitch_rate_dif = last_pitch_rate - pitch_rate;
	float roll_rate_dif = last_roll_rate - roll_rate;
	float yaw_rate_dif = last_yaw_rate - yaw_rate;

	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + fs * pitch_rate_d * pitch_rate_dif - fs * roll_rate_d * roll_rate_dif - fs * yaw_rate_d * yaw_rate_dif;
	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp - fs * pitch_rate_d * pitch_rate_dif + fs * roll_rate_d * roll_rate_dif - fs * yaw_rate_d * yaw_rate_dif;
	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + fs * pitch_rate_d * pitch_rate_dif + fs * roll_rate_d * roll_rate_dif + fs * yaw_rate_d * yaw_rate_dif;
	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp - fs * pitch_rate_d * pitch_rate_dif - fs * roll_rate_d * roll_rate_dif + fs * yaw_rate_d * yaw_rate_dif;

	pitch_rate_err_sum = pitch_rate_err_sum + Ts * pitch_rate_i * pitch_rate_err;

	if(pitch_rate_err_sum < MIN_PITCH_RATE_ERR_SUM)
	{
		pitch_rate_err_sum = MIN_PITCH_RATE_ERR_SUM;
	}
	else if(pitch_rate_err_sum > MAX_PITCH_RATE_ERR_SUM)
	{

		pitch_rate_err_sum = MAX_PITCH_RATE_ERR_SUM;
	}

	roll_rate_err_sum = roll_rate_err_sum + Ts * roll_rate_i * roll_rate_err;

	if(roll_rate_err_sum < MIN_ROLL_RATE_ERR_SUM)
	{
		roll_rate_err_sum = MIN_ROLL_RATE_ERR_SUM;
	}
	else if(roll_rate_err_sum > MAX_ROLL_RATE_ERR_SUM)
	{

		roll_rate_err_sum = MAX_ROLL_RATE_ERR_SUM;
	}

	yaw_rate_err_sum = yaw_rate_err_sum + Ts * yaw_rate_i * yaw_rate_err;

	if(yaw_rate_err_sum < MIN_YAW_RATE_ERR_SUM)
	{
		yaw_rate_err_sum = MIN_YAW_RATE_ERR_SUM;
	}
	else if(yaw_rate_err_sum > MAX_YAW_RATE_ERR_SUM)
	{

		yaw_rate_err_sum = MAX_YAW_RATE_ERR_SUM;
	}

	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + pitch_rate_err_sum - roll_rate_err_sum - yaw_rate_err_sum;
	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp - pitch_rate_err_sum + roll_rate_err_sum - yaw_rate_err_sum;
	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + pitch_rate_err_sum + roll_rate_err_sum + yaw_rate_err_sum;
	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp - pitch_rate_err_sum - roll_rate_err_sum + yaw_rate_err_sum;

	if(Motor1_PWM_Value_temp > MAX_PID1_OUTPUT)
		Motor1_PWM_Value_temp = MAX_PID1_OUTPUT;
	else if(Motor1_PWM_Value_temp < MIN_PID1_OUTPUT)
		Motor1_PWM_Value_temp = MIN_PID1_OUTPUT;
	if(Motor2_PWM_Value_temp > MAX_PID1_OUTPUT)
		Motor2_PWM_Value_temp = MAX_PID1_OUTPUT;
	else if(Motor2_PWM_Value_temp < MIN_PID1_OUTPUT)
		Motor2_PWM_Value_temp = MIN_PID1_OUTPUT;
	if(Motor3_PWM_Value_temp > MAX_PID1_OUTPUT)
		Motor3_PWM_Value_temp = MAX_PID1_OUTPUT;
	else if(Motor3_PWM_Value_temp < MIN_PID1_OUTPUT)
		Motor3_PWM_Value_temp = MIN_PID1_OUTPUT;
	if(Motor4_PWM_Value_temp > MAX_PID1_OUTPUT)
		Motor4_PWM_Value_temp = MAX_PID1_OUTPUT;
	else if(Motor4_PWM_Value_temp < MIN_PID1_OUTPUT)
		Motor4_PWM_Value_temp = MIN_PID1_OUTPUT;

	Motor1_PWM_Value_temp = Motor1_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
	Motor2_PWM_Value_temp = Motor2_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
	Motor3_PWM_Value_temp = Motor3_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;
	Motor4_PWM_Value_temp = Motor4_PWM_Value_temp + MIN_MOTOR_PWM_VALUE + ((float)RC_PWM_Value[RC_CH3] - RC_CH3_MIN_POINT) * GAS_SCALE;

	Motor1_PWM_Value = (unsigned int)Motor1_PWM_Value_temp;
	Motor2_PWM_Value = (unsigned int)Motor2_PWM_Value_temp;
	Motor3_PWM_Value = (unsigned int)Motor3_PWM_Value_temp;
	Motor4_PWM_Value = (unsigned int)Motor4_PWM_Value_temp;

	last_pitch_rate_err = pitch_rate_err;
	last_roll_rate_err = roll_rate_err;
	last_yaw_rate_err = yaw_rate_err;
}
