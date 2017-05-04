/*
 * I2c.h
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: gtc
 */

#ifndef I2C_H_
#define I2C_H_


#include "..\mylibs.h"
#define I2CA_RXFIFO_LEN  MPU_RAW_DATA_LEN
void I2ca_Init(void);
void I2ca_Write(unsigned char slave_address,unsigned char register_address,int length,unsigned char *buf);
void I2ca_Read(unsigned char slave_address,unsigned char register_address,int length,unsigned char *buf);

void I2ca_RxFifo_Init(void);
void I2ca_Read_with_RxFifo(unsigned char slave_address,unsigned char register_address,unsigned char *buf);

#endif /* I2C_H_ */
