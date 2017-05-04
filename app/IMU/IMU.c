/*
 * IMU.c
 *
 *  Created on: 2016Äê12ÔÂ20ÈÕ
 *      Author: Tianchang
 */

#include "IMU.h"
#include "math.h"

float accel_raw_data_x;	//raw data of accelerometer
float accel_raw_data_y;
float accel_raw_data_z;
float gyro_raw_data_x;	//raw data of gyro
float gyro_raw_data_y;
float gyro_raw_data_z;

float pitch = 0,last_pitch = 0;
float roll = 0,last_roll = 0;
float yaw = 0,last_yaw = 0;
float pitch_rate = 0,last_pitch_rate = 0;
float roll_rate = 0,last_roll_rate = 0;
float yaw_rate = 0,last_yaw_rate = 0;

float Px = 0;
float Py = 0;

float q0 = 1, q1 =0, q2 =0, q3 =0;
float exInt = 0, eyInt = 0, ezInt = 0;
float Kp = 2,Ki=0.005;
float halfT = 0.001;

extern unsigned char mpu_raw_data[MPU_RAW_DATA_LEN];

/*
 * IMU Update
 */
//void IMU_Update(void)
//{
//	float accel_angle_x;	//x angle calculated from accelerometer data
//	float accel_angle_y;	//y angle calculated from accelerometer data
//	float pitch_temp = 0;	//temporary value of pitch
//	float roll_temp = 0;	//temporary value of roll
//	float Kx;	//Kalman gain of x
//	float Ky;	//Kalman gain of y
//	accel_raw_data_x = ((int)((mpu_raw_data[0] << 8) | mpu_raw_data[1])) * ACCEL_G_PER_LSB;
//	accel_raw_data_y = ((int)((mpu_raw_data[2] << 8) | mpu_raw_data[3])) * ACCEL_G_PER_LSB;
//	accel_raw_data_z = ((int)((mpu_raw_data[4] << 8) | mpu_raw_data[5])) * ACCEL_G_PER_LSB;
//
//	gyro_raw_data_x = ((int)((mpu_raw_data[8] << 8) | mpu_raw_data[9]) - GYRO_X_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB;
//	gyro_raw_data_y = ((int)((mpu_raw_data[10] <<8) | mpu_raw_data[11]) - GYRO_Y_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB;
//	gyro_raw_data_z = ((int)((mpu_raw_data[12] <<8) | mpu_raw_data[13]) - GYRO_Z_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB;
//
//	accel_angle_x = asin(accel_raw_data_y / sqrt(accel_raw_data_y * accel_raw_data_y + accel_raw_data_z * accel_raw_data_z)) * RADIAN_TO_DEGREE;
//	accel_angle_y = -asin(accel_raw_data_x / sqrt(accel_raw_data_x * accel_raw_data_x + accel_raw_data_y * accel_raw_data_y + accel_raw_data_z * accel_raw_data_z)) * RADIAN_TO_DEGREE;
//
//	//x Kalman Filter
//	pitch_temp = pitch + gyro_raw_data_x * T;
//	Px = Px + GYRO_X_VARIANCE;
//	Kx = Px / (Px + ACCEL_X_VARIANCE);
//	pitch = pitch_temp + Kx * (accel_angle_x - pitch_temp);
//	Px = (1 - Kx) * Px;
//	//y Kalman Filter
//	roll_temp = roll + gyro_raw_data_y * T;
//	Py = Py + GYRO_Y_VARIANCE;
//	Ky = Py / (Py + ACCEL_Y_VARIANCE);
//	roll = roll_temp + Ky * (accel_angle_y - roll_temp);
//	Py = (1 - Ky) * Py;
//
//	yaw_velocity = gyro_raw_data_z;
//}
void IMU_Update(void)
{
	float norm;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;

	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q1q1 = q1*q1;

	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	float ax = ((int)((mpu_raw_data[0] << 8) | mpu_raw_data[1])) * ACCEL_G_PER_LSB;
	float ay = ((int)((mpu_raw_data[2] << 8) | mpu_raw_data[3])) * ACCEL_G_PER_LSB;
	float az = ((int)((mpu_raw_data[4] << 8) | mpu_raw_data[5])) * ACCEL_G_PER_LSB;
	if(ax*ay*az==0)
	 		return;

	float gx = ((int)((mpu_raw_data[8] << 8) | mpu_raw_data[9]) - GYRO_X_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB / RADIAN_TO_DEGREE;
	float gy = ((int)((mpu_raw_data[10] <<8) | mpu_raw_data[11]) - GYRO_Y_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB / RADIAN_TO_DEGREE;
	float gz = ((int)((mpu_raw_data[12] <<8) | mpu_raw_data[13]) - GYRO_Z_OFFSET) * GYRO_DEGREE_PER_SECOND_PER_LSB / RADIAN_TO_DEGREE;


	last_pitch = pitch;
	last_roll = roll;
	last_yaw = yaw;
	last_pitch_rate = pitch_rate;
	last_roll_rate = roll_rate;
	last_yaw_rate = yaw_rate;

	pitch_rate = gx * RADIAN_TO_DEGREE;
	roll_rate = gy * RADIAN_TO_DEGREE - 3.6;
	yaw_rate = gz * RADIAN_TO_DEGREE;

	norm = sqrt(ax*ax +ay*ay + az*az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	ex = (ay*vz - az*vy) ;
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	// integrate quaternion rate and normalise
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	//Q_ANGLE.Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
	roll  = asin(-2 * q1*q3 + 2 * q0*q2)* 57.3; // roll
	pitch = atan2(2 * q2*q3 + 2 * q0*q1, -2 * q1*q1 - 2 * q2*q2 + 1)* 57.3; // pitch
	/*if(GYRO_I.Z>=360)GYRO_I.Z=0;
	Q_ANGLE.Z = GYRO_I.Z;*/
}
