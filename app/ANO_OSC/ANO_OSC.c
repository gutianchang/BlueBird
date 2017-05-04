/*
 * ANO_OSC.c
 *
 *  Created on: 2016Äê11ÔÂ24ÈÕ
 *      Author: gtc
 */
#include "ANO_OSC.h"

unsigned int get_data_len = 0;
unsigned char get_data_state = 0;
unsigned char get_data_from_ANO_OSC[GET_DATA_FROM_ANO_OSC_LEN] = {0};
extern unsigned int RC_PWM_Value[RC_CH_Num];
extern unsigned char mpu_raw_data[MPU_RAW_DATA_LEN];
extern float pitch;
extern float roll;
extern float yaw;
extern unsigned int Motor1_PWM_Value;
extern unsigned int Motor2_PWM_Value;
extern unsigned int Motor3_PWM_Value;
extern unsigned int Motor4_PWM_Value;

//extern float pitch_p;
//extern float roll_p;
//extern float yaw_rate_p;
//extern float pitch_d;
//extern float roll_d;
//extern float yaw_rate_d;
//extern float pitch_i;
//extern float roll_i;
//extern float yaw_rate_i;

extern float pitch_rate_p;
extern float roll_rate_p;
extern float yaw_rate_p;
extern float pitch_rate_d;
extern float roll_rate_d;
extern float yaw_rate_d;
extern float pitch_rate_i;
extern float roll_rate_i;
extern float yaw_rate_i;


extern float pitch_rate_err;
extern float roll_rate_err;
extern float yaw_rate_err;

extern float last_pitch_rate_err;
extern float last_roll_rate_err;
extern float last_yaw_rate_err;

extern float pitch_rate_err_sum;
extern float roll_rate_err_sum;
extern float yaw_rate_err_sum;

extern float target_pitch;
extern float target_roll;
extern float target_yaw;

extern float roll_err;

extern float target_pitch_rate;
extern float target_roll_rate;
extern float target_yaw_rate;

extern float pitch_rate,last_pitch_rate;
extern float roll_rate,last_roll_rate;
extern float yaw_rate,last_yaw_rate;


extern char Scia_Rx_Data[SCIA_RXFIFO_LEN];		//Scia Rx buffer

void ANO_OSC_PID(void);
static void ANO_DT_Send_Check(unsigned char head, unsigned char check_sum);
void ANO_DT_Data_Receive_Anl(unsigned char *data_buf,unsigned char num);
/*
 * Send raw mpu6050 data to ANO OSC
 */
void ANO_OSC_MPU(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x02,0x0C};
	char data_send_buf[13];
	for(i=0;i<6;i++)
	{
	sum = sum + mpu_raw_data[i];
	data_send_buf[i] = mpu_raw_data[i];
	}
	for(i=8;i<14;i++)
	{
	sum = sum + mpu_raw_data[i];
	data_send_buf[i-2] = mpu_raw_data[i];
	}
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	data_send_buf[12] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,13);
}

/*
 * Send RC data to ANO OSC
 */
void ANO_OSC_RC(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x03,0x0c};
	char data_send_buf[13];
	data_send_buf[0]  = (RC_PWM_Value[RC_CH1]>>8)&0xff;
	data_send_buf[1]  = RC_PWM_Value[RC_CH1]&0xff;
	data_send_buf[2]  = (RC_PWM_Value[RC_CH2]>>8)&0xff;
	data_send_buf[3]  = RC_PWM_Value[RC_CH2]&0xff;
	data_send_buf[4]  = (RC_PWM_Value[RC_CH3]>>8)&0xff;
	data_send_buf[5]  = RC_PWM_Value[RC_CH3]&0xff;
	data_send_buf[6]  = (RC_PWM_Value[RC_CH4]>>8)&0xff;
	data_send_buf[7]  = RC_PWM_Value[RC_CH4]&0xff;
	data_send_buf[8]  = (RC_PWM_Value[RC_CH5]>>8)&0xff;
	data_send_buf[9]  = RC_PWM_Value[RC_CH5]&0xff;
	data_send_buf[10] = (RC_PWM_Value[RC_CH6]>>8)&0xff;
	data_send_buf[11] = RC_PWM_Value[RC_CH6]&0xff;
	for(i=0;i<12;i++)
	{
		sum = sum + data_send_buf[i];
	}
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	data_send_buf[12] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,13);
}


/*
 * Send IMU data to ANO OSC
 */
void ANO_OSC_IMU(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x01,0x06};
	char data_send_buf[7];
	data_send_buf[0] = (((int)(roll * 100)) >> 8) & 0xff;
	data_send_buf[1] = ((int)(roll * 100)) & 0xff;
	data_send_buf[2] = (((int)(pitch * 100)) >> 8) & 0xff;
	data_send_buf[3] = ((int)(pitch * 100)) & 0xff;
	data_send_buf[4] = (((int)(yaw * 100)) >> 8) &0xff;
	data_send_buf[5] = ((int)(yaw * 100)) & 0xff;
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	for(i=0;i<6;i++)
	{
		sum = sum + data_send_buf[i];
	}
	data_send_buf[6] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,7);
}

void ANO_OSC_IMU2(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x01,0x06};
	char data_send_buf[7];
	data_send_buf[0] = (((int)(target_roll_rate * 100)) >> 8) & 0xff;
	data_send_buf[1] = ((int)(target_roll_rate * 100)) & 0xff;
	data_send_buf[2] = (((int)(roll_rate * 100)) >> 8) & 0xff;
	data_send_buf[3] = ((int)(roll_rate * 100)) & 0xff;
	data_send_buf[4] = (((int)(roll_rate_err * 100)) >> 8) &0xff;
	data_send_buf[5] = ((int)(roll_rate_err * 100)) & 0xff;
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	for(i=0;i<6;i++)
	{
		sum = sum + data_send_buf[i];
	}
	data_send_buf[6] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,7);
}

void ANO_OSC_IMU3(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x01,0x06};
	char data_send_buf[7];
	data_send_buf[0] = (((int)(target_roll * 100)) >> 8) & 0xff;
	data_send_buf[1] = ((int)(target_roll * 100)) & 0xff;
	data_send_buf[2] = (((int)(roll * 100)) >> 8) & 0xff;
	data_send_buf[3] = ((int)(roll * 100)) & 0xff;
	data_send_buf[4] = (((int)(roll_err * 100)) >> 8) &0xff;
	data_send_buf[5] = ((int)(roll_err * 100)) & 0xff;
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	for(i=0;i<6;i++)
	{
		sum = sum + data_send_buf[i];
	}
	data_send_buf[6] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,7);
}

/*
 * Send Motor output to ANO OSC
 */
void ANO_OSC_Motor_Output(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x06,0x08};
	char data_send_buf[9];
	data_send_buf[0] = ((Motor1_PWM_Value-MIN_MOTOR_PWM_VALUE) >> 8) & 0xff;
	data_send_buf[1] = (Motor1_PWM_Value-MIN_MOTOR_PWM_VALUE) & 0xff;
	data_send_buf[2] = ((Motor2_PWM_Value-MIN_MOTOR_PWM_VALUE) >> 8) & 0xff;
	data_send_buf[3] = (Motor2_PWM_Value-MIN_MOTOR_PWM_VALUE) & 0xff;
	data_send_buf[4] = ((Motor3_PWM_Value-MIN_MOTOR_PWM_VALUE) >> 8) &0xff;
	data_send_buf[5] = (Motor3_PWM_Value-MIN_MOTOR_PWM_VALUE) & 0xff;
	data_send_buf[6] = ((Motor4_PWM_Value-MIN_MOTOR_PWM_VALUE) >> 8) &0xff;
	data_send_buf[7] = (Motor4_PWM_Value-MIN_MOTOR_PWM_VALUE) & 0xff;
	for(i=0;i<8;i++)
	{
		sum = sum + data_send_buf[i];
	}
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	data_send_buf[8] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,9);
}

/*
 * Send PID1 to ANO OSC
 */
void ANO_OSC_PID(void)
{
	int i = 0;
	unsigned char sum = 0;
	char head[4] = {0xAA,0xAA,0x10,0x12};
	char data_send_buf[19];
	data_send_buf[0] = ((int)(1000 * pitch_rate_p) >>8) & 0xff;
	data_send_buf[1] = (int)(1000 * pitch_rate_p) &0xff;
	data_send_buf[2] = ((int)(1000 * pitch_rate_i) >>8) & 0xff;
	data_send_buf[3] = (int)(1000 * pitch_rate_i) &0xff;
	data_send_buf[4] = ((int)(1000 * pitch_rate_d) >>8) & 0xff;
	data_send_buf[5] = (int)(1000 * pitch_rate_d) &0xff;
	data_send_buf[6] = ((int)(1000 * roll_rate_p) >>8) & 0xff;
	data_send_buf[7] = (int)(1000 * roll_rate_p) &0xff;
	data_send_buf[8] = ((int)(1000 * roll_rate_i) >>8) & 0xff;
	data_send_buf[9] = (int)(1000 * roll_rate_i) &0xff;
	data_send_buf[10] = ((int)(1000 * roll_rate_d) >>8) & 0xff;
	data_send_buf[11] = (int)(1000 * roll_rate_d) &0xff;
	data_send_buf[12] = ((int)(1000 * yaw_rate_p) >>8) & 0xff;
	data_send_buf[13] = (int)(1000 * yaw_rate_p) &0xff;
	data_send_buf[14] = ((int)(1000 * yaw_rate_i) >>8) & 0xff;
	data_send_buf[15] = (int)(1000 * yaw_rate_i) &0xff;
	data_send_buf[16] = ((int)(1000 * yaw_rate_d) >>8) & 0xff;
	data_send_buf[17] = (int)(1000 * yaw_rate_d) &0xff;
	for(i=0;i<18;i++)
	{
		sum = sum + data_send_buf[i];
	}
	for(i=0;i<4;i++)
	{
		sum = sum + head[i];
	}
	data_send_buf[18] = sum;
	Scia_Send_Msg(head,4);
	Scia_Send_Msg(data_send_buf,19);
}


/*
 * Get data from ANO OSC
 */
//void Get_Data_from_ANO_OSC(void)
//{
//	int i = 0;
//	unsigned char sum = 0;
//	if((Scia_Rx_Data[0] == 0xAA)&&(Scia_Rx_Data[1] == 0xAF))
//	{
//		for(i=0;i<GET_DATA_FROM_ANO_OSC_LEN-1;i++)
//		{
//			sum = sum + Scia_Rx_Data[i];
//		}
//		if(sum == Scia_Rx_Data[GET_DATA_FROM_ANO_OSC_LEN-1])
//		{
//			switch(Scia_Rx_Data[2])
//			{
//			case PID1:
//				Get_PID1_from_ANO_OSC();
//			case PID2:
//				Get_PID2_from_ANO_OSC();
//			}
//		}
//	}
//}
void Get_PID_from_ANO_OSC(unsigned char data)
{
	int i=0;
	unsigned char sum = 0;
	unsigned char check_sum = 0;
	char data_send_buf[7] = {0};
	if(get_data_state==0&&data==0xAA)
	{
		ANO_OSC_PID();
		get_data_state = 1;
		get_data_from_ANO_OSC[0] = data;
	}
	else if(get_data_state==1&&data==0xAF)
	{
		get_data_state = 2;
		get_data_from_ANO_OSC[1] = data;
	}
	else if(get_data_state==2&&data==0x10)
	{
		get_data_state = 3;
		get_data_from_ANO_OSC[2] = data;
	}
	else if(get_data_state==3&&data==18)
	{
		get_data_state = 4;
		get_data_from_ANO_OSC[3] = data;
	}
	else if(get_data_state==4&&(get_data_len<19))
	{
		get_data_from_ANO_OSC[get_data_len+4] = data;
		get_data_len = get_data_len + 1;
	}
	else
	{
		get_data_state = 0;
	}

	if(get_data_len == 18)
	{
		get_data_len = 0;
		for(i=0;i<GET_DATA_FROM_ANO_OSC_LEN-1;i++)
		{
			sum = sum + get_data_from_ANO_OSC[i];
		}
		if(sum == get_data_from_ANO_OSC[GET_DATA_FROM_ANO_OSC_LEN-1])
		{
			pitch_rate_p = (((unsigned int)get_data_from_ANO_OSC[4]<<8)|get_data_from_ANO_OSC[5]) * 0.001;
			pitch_rate_i = (((unsigned int)get_data_from_ANO_OSC[6]<<8)|get_data_from_ANO_OSC[7]) * 0.001;
			pitch_rate_d = (((unsigned int)get_data_from_ANO_OSC[8]<<8)|get_data_from_ANO_OSC[9]) * 0.001;

			roll_rate_p = (((unsigned int)get_data_from_ANO_OSC[10]<<8)|get_data_from_ANO_OSC[11]) * 0.001;
			roll_rate_i = (((unsigned int)get_data_from_ANO_OSC[12]<<8)|get_data_from_ANO_OSC[13]) * 0.001;
			roll_rate_d = (((unsigned int)get_data_from_ANO_OSC[14]<<8)|get_data_from_ANO_OSC[15]) * 0.001;

			yaw_rate_p = (((unsigned int)get_data_from_ANO_OSC[16]<<8)|get_data_from_ANO_OSC[17]) * 0.001;
			yaw_rate_i = (((unsigned int)get_data_from_ANO_OSC[18]<<8)|get_data_from_ANO_OSC[19]) * 0.001;
			yaw_rate_d = (((unsigned int)get_data_from_ANO_OSC[20]<<8)|get_data_from_ANO_OSC[21]) * 0.001;

			data_send_buf[0] = 0xAA;
			data_send_buf[1] = 0xAA;
			data_send_buf[2] = 0xEF;
			data_send_buf[3] = 2;
			data_send_buf[4] = 0x10;
			data_send_buf[5] = sum;
			for(i=0;i<6;i++)
			{
				check_sum = check_sum + data_send_buf[i];
			}
			data_send_buf[6] = check_sum;
			Scia_Send_Msg(data_send_buf,7);
		}
	}
}



void ANO_DT_Data_Receive_Prepare(unsigned char data)
{
	static unsigned char RxBuffer[50];
	static unsigned char _data_len = 0,_data_cnt = 0;
	static unsigned char state = 0;

	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}


void ANO_DT_Data_Receive_Anl(unsigned char *data_buf,unsigned char num)
{
	unsigned char sum = 0;
	unsigned char i;
	for(i=0;i<(num-1);i++)
		sum += *(data_buf+i);
//	if(!(sum==*(data_buf+num-1)))		return;
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;

//	if(*(data_buf+2)==0X01)
//	{
//		if(*(data_buf+4)==0X01)
//			mpu6050.Acc_CALIBRATE = 1;
//		if(*(data_buf+4)==0X02)
//			mpu6050.Gyro_CALIBRATE = 1;
//		if(*(data_buf+4)==0X03)
//		{
//			mpu6050.Acc_CALIBRATE = 1;
//			mpu6050.Gyro_CALIBRATE = 1;
//		}
//	}

	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			ANO_OSC_PID();
		}
		if(*(data_buf+4)==0X02)
		{

		}
		if(*(data_buf+4)==0XA0)
		{

		}
		if(*(data_buf+4)==0XA1)
		{

		}
	}

	if(*(data_buf+2)==0X10)								//PID1
    {
        pitch_rate_p  = 0.001*( (unsigned int)(*(data_buf+4)<<8)|*(data_buf+5) );
        pitch_rate_i  = 0.001*( (unsigned int)(*(data_buf+6)<<8)|*(data_buf+7) );
        pitch_rate_d  = 0.001*( (unsigned int)(*(data_buf+8)<<8)|*(data_buf+9) );
        roll_rate_p = 0.001*( (unsigned int)(*(data_buf+10)<<8)|*(data_buf+11) );
        roll_rate_i = 0.001*( (unsigned int)(*(data_buf+12)<<8)|*(data_buf+13) );
        roll_rate_d = 0.001*( (unsigned int)(*(data_buf+14)<<8)|*(data_buf+15) );
        yaw_rate_p 	= 0.001*( (unsigned int)(*(data_buf+16)<<8)|*(data_buf+17) );
        yaw_rate_i 	= 0.001*( (unsigned int)(*(data_buf+18)<<8)|*(data_buf+19) );
        yaw_rate_d 	= 0.001*( (unsigned int)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
//        ctrl_1.PID[PID4].kp 	= 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_1.PID[PID4].ki 	= 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_1.PID[PID4].kd 	= 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_1.PID[PID5].kp 	= 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_1.PID[PID5].ki 	= 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_1.PID[PID5].kd 	= 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_1.PID[PID6].kp	  = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_1.PID[PID6].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_1.PID[PID6].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
//        ANO_DT_Send_Check(*(data_buf+2),sum);
//				Param_SavePID();
    }
    if(*(data_buf+2)==0X12)								//PID3
    {
//        ctrl_2.PID[PIDROLL].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_2.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_2.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_2.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_2.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_2.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_2.PID[PIDYAW].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_2.PID[PIDYAW].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_2.PID[PIDYAW].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
//        ANO_DT_Send_Check(*(data_buf+2),sum);
//				Param_SavePID();
    }
	if(*(data_buf+2)==0X13)								//PID4
	{

	}
	if(*(data_buf+2)==0X14)								//PID5
	{

	}
	if(*(data_buf+2)==0X15)								//PID6
	{

	}
}


static void ANO_DT_Send_Check(unsigned char head, unsigned char check_sum)
{
	char data_to_send[7];
	unsigned char sum = 0;
	unsigned char i;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;

	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	Scia_Send_Msg(data_to_send, 7);
}
