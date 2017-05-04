/*
 * Attitude.c
 *
 *  Created on: 2017Äê2ÔÂ27ÈÕ
 *      Author: Tianchang
 */
#include "Attitude.h"
//void get_pilot_desired_lean_angles(float roll_in, float pitch_in, float roll_out, float pitch_out)
//{
//	float pitch_scaler = 2 * (float)PITCH_MAX_ANGLE / float(pitch_in - RC_CH1_MIN_POINT);
//	float roll_scaler = 2 * (float)ROLL_MAX_ANGLE / float(roll_in - RC_CH2_MIN_POINT);
//	roll_in *= roll_scaler;
//	pitch_in *= pitch_scaler;
//
//	// do circular limit
//    float total_in = pythagorous2((float)pitch_in, (float)roll_in);
//    if (total_in > ANGLE_MAX) {
//        float ratio = ANGLE_MAX / total_in;
//        roll_in *= ratio;
//        pitch_in *= ratio;
//    }
//
//    // do lateral tilt to euler roll conversion
////    roll_in = (18000/M_PI) * atanf(cosf(pitch_in*(M_PI/18000))*tanf(roll_in*(M_PI/18000)));
//
//    // return
//    roll_out = roll_in;
//    pitch_out = pitch_in;
//}

//
//float get_pilot_desired_yaw_rate(int yaw_input)
//{
//    // convert pilot input to the desired yaw rate
//    return (-((float)yaw_input - RC_CH4_MID_POINT) / (RC_CH4_MAX_POINT - RC_CH4_MIN_POINT) * YAW_MAX_ANGLE_VELOCITY * 2);
//}
//
//int get_pilot_desired_throttle(int throttle_control)
//{
//	int throttle_out;
//    int mid_stick = RC_CH3_MID_POINT;
//    throttle_control = throttle_control - mid_stick;
//    throttle_control = (int)(GAS_SCALE * throttle_control);
//    throttle_out = throttle_control;
//    return throttle_out;
//}

