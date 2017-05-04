/*
 * IMU.h
 *
 *  Created on: 2016Äê12ÔÂ20ÈÕ
 *      Author: Tianchang
 */

#ifndef APP_IMU_IMU_H_
#define APP_IMU_IMU_H_

#include "..\app.h"

#define RADIAN_TO_DEGREE				57.296f
#define T	(CPUTIMER0_PRD*0.001)
#define ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_X					100
#define ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Y					100
#define ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Z					200

//gyroscope
#define GYRO_DEGREE_PER_SECOND_PER_LSB	0.061035				// 1/16.384

#define GYRO_X_OFFSET					-23.69					//gyroscope offset
#define GYRO_Y_OFFSET					-54.73
#define GYRO_Z_OFFSET					-11.73

#define GYRO_X_STD_DEV					0.1577					//gyroscope standard deviation
#define GYRO_Y_STD_DEV					0.2654
#define GYRO_Z_STD_DEV					0.0395

#define GYRO_X_VARIANCE					0.02487					//gyroscope variance
#define GYRO_Y_VARIANCE					0.07044
#define GYRO_Z_VARIANCE					0.00156

//accelerometer
#define ACCEL_G_PER_LSB					0.00012207				// 1/8192

#define ACCEL_X_OFFSET					0						//accelerometer offset
#define ACCEL_Y_OFFSET					0
#define ACCEL_Z_OFFSET					0



#define ACCEL_X_STD_DEV					0.7946					//accelerometer standard deviation
#define ACCEL_Y_STD_DEV					0.6268
#define ACCEL_Z_STD_DEV					0.1497

#define ACCEL_X_VARIANCE				(ACCEL_X_STD_DEV*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_X*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_X)
#define ACCEL_Y_VARIANCE				(ACCEL_Y_STD_DEV*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Y*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Y)
#define ACCEL_Z_VARIANCE				(ACCEL_Z_STD_DEV*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Z*ACCEL_DYNAMIC_STD_DEV_TO_STATIC_STD_DEV_Z)

void IMU_Update(void);

#endif /* APP_IMU_IMU_H_ */
