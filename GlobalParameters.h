/*
 * GlobalParameters.h
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */

#ifndef GLOBALPARAMETERS_H_
#define GLOBALPARAMETERS_H_

#define SYSTEMCLK    		200 		//100MHz
#define LSPCLK				(SYSTEMCLK/4)
#define CPUTIMER0_PRD		2			//2ms
#define	CPUTIMER1_PRD		10			//10ms
#define CPUTIMER2_PRD		100			//100ms

#define SCIA_BAUDRATE   	115200
#define SCIB_BAUDRATE   	115200


/********************************
 * Sensors
 */
#define MPU_SAMPLE_RATE		1000
#define MPU_ACCEL_RANGE			MPU_ACCEL_RANGE_4G
#define MPU_GYRO_RANGE			MPU_GYRO_RANGE_2000DPS

/*
 * PID
 */
#define PITCH_ANGLE_P	1
#define PITCH_ANGLE_I	0.001
#define PITCH_ANGLE_D	0
#define ROLL_ANGLE_P	1
#define ROLL_ANGLE_I	0.001
#define ROLL_ANGLE_D	0
#define YAW_ANGLE_VELOCITY_P	1
#define YAW_ANGLE_VELOCITY_I	0.001
#define YAW_ANGLE_VELOCITY_D	0

/*
 * RC information
 */
#define RC_CH1_MIN_POINT	1108
#define RC_CH1_MID_POINT	1532
#define RC_CH1_MAX_POINT	1929

#define RC_CH2_MIN_POINT	1108
#define RC_CH2_MID_POINT	1521
#define RC_CH2_MAX_POINT	1929

#define RC_CH3_MIN_POINT	1104
#define RC_CH3_MID_POINT	1511
#define RC_CH3_MAX_POINT	1929

#define RC_CH4_MIN_POINT	1108
#define RC_CH4_MID_POINT	1516
#define RC_CH4_MAX_POINT	1929

#define RC_CH5_MIN_POINT	969
#define RC_CH5_MID_POINT	1519
#define RC_CH5_MAX_POINT	2069

#define RC_CH6_MIN_POINT	969
#define RC_CH6_MID_POINT	1519
#define RC_CH6_MAX_POINT	2069

#define ARMED_PWM_VALUE		2000

/*
 * Control configuration
 */
#define ROLL_MAX_ANGLE		ANGLE_MAX
#define PITCH_MAX_ANGLE		ANGLE_MAX
#define ANGLE_MAX			30.0f
#define YAW_MAX_ANGLE_RATE		60
#define ROLL_MAX_ANGLE_RATE		60
#define PITCH_MAX_ANGLE_RATE	60

#define MIN_MOTOR_PWM_VALUE		RC_CH3_MIN_POINT
#define MAX_MOTOR_PWM_VALUE		1600
#define MAX_PID_OUTPUT			150

#define GAS_SCALE			0.6

#endif /* GLOBALPARAMETERS_H_ */
