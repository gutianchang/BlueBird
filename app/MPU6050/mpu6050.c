/*
 * mpu6050.c
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: gtc
 */
#include "mpu6050.h"
unsigned char mpu_raw_data[MPU_RAW_DATA_LEN];
unsigned char MPU_Init(void)
{
	delay_ms(100);								//important! wait the MPU start
	unsigned char res;
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);		//reset the MPU6050
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);		//wake the MPU6050
	delay_ms(10);
	MPU_Set_Gyro_Fsr(MPU_GYRO_RANGE);			//set the gyro range
	delay_ms(1);
	MPU_Set_Accel_Fsr(MPU_ACCEL_RANGE);			//set the accel range
	delay_ms(1);
	MPU_Set_Rate(MPU_SAMPLE_RATE);//set the sample rate
	delay_ms(1);
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);		//close all of the interrupts
	delay_ms(1);
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);		//close the I2C master mode
	delay_ms(1);
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//close the FIFO
	delay_ms(1);
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);		//INT works when LOW
	delay_ms(1);
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);   	//read the MPU6050's ID
	delay_ms(1);
	if(res==MPU_ADDR)							//the MPU6050's ID is right
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//set CLKSEL and PLL   set X as the reference
		delay_ms(1);
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//enable both the accel and gyro
		delay_ms(1);
//		MPU_Set_Rate(MPU_SAMPLE_RATE);	//set the sample rate
//		delay_ms(1);
 	}else return 1;
	I2ca_RxFifo_Init();
	return 0;
}

/*
 * Set gyro range
 * fsr:0,¡À250dps;1,¡À500dps;2,¡À1000dps;3,¡À2000dps   dps = degree per second
 */
unsigned char MPU_Set_Gyro_Fsr(unsigned char  fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}

/*
 * Set accel range
 * fsr:0,¡À2g;1,¡À4g;2,¡À8g;3,¡À16g
 */
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/*
 * Set LPF
 */
unsigned char MPU_Set_LPF(unsigned int lpf)
{
	unsigned char data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU_Write_Byte(MPU_CFG_REG,data);
}

/*
 * Set sample rate
 * Range form 4Hz to 1000Hz
 */
unsigned char MPU_Set_Rate(unsigned int rate)
{
	unsigned char data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
 	return MPU_Set_LPF(rate/2);	//Set LPF half of the sample rate
}

/*
 * Get temperature form MPU
 */
float MPU_Get_Temperature(void)
{
    unsigned char buf[2];
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((unsigned int)buf[0]<<8)|buf[1];
    temp=36.53+((float)raw)/340;
    return temp;
}

/*
 * Get gyro value form MPU
 * 6 bytes
 */
unsigned char MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    unsigned char buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((unsigned int)buf[0]<<8)|buf[1];
		*gy=((unsigned int)buf[2]<<8)|buf[3];
		*gz=((unsigned int)buf[4]<<8)|buf[5];
	}
    return res;
}

/*
 * Get accel value form MPU
 * 6 bytes
 */
unsigned char MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    unsigned char buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((unsigned int)buf[0]<<8)|buf[1];
		*ay=((unsigned int)buf[2]<<8)|buf[3];
		*az=((unsigned int)buf[4]<<8)|buf[5];
	}
    return res;
}

/*
 * Write len bytes in buf to MPU's register
 */
unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,int len,unsigned char *buf)
{
	I2ca_Write(addr,reg,len,buf);
	return 0;
}

/*
 * Read len bytes to buf from MPU's regsiter
 */
unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,int len,unsigned char *buf)
{
	I2ca_Read(addr,reg,len,buf);
	return 0;
}

/*
 * Write one byte to MPU's register
 */
unsigned char MPU_Write_Byte(unsigned char reg,unsigned char data)
{
	unsigned char buf[3];
	buf[0] = data;
	I2ca_Write(MPU_ADDR,reg,1,buf);
	return 0;
}

/*
 * Read one byte from MPU's register
 */
unsigned char MPU_Read_Byte(unsigned char reg)
{

	unsigned char res[1];
	I2ca_Read(MPU_ADDR,reg,1,res);
	return (res[0]);
}

void MPU_Get_Raw_Data(void)
{
	I2ca_Read_with_RxFifo(MPU_ADDR,MPU_ACCEL_XOUTH_REG,mpu_raw_data);
//	I2ca_Read(MPU_ADDR,MPU_ACCEL_XOUTH_REG,14,mpu_raw_data);
}


/*
 * delay ms
 */
void delay_ms(int i)
{
	for(;i>0;i--)
	{
		DELAY_US(1000);
	}
}


