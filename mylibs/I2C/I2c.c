/*
 * I2c.c
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: gtc
 */
#include "I2c.h"
interrupt void I2ca_RxFifo_Isr(void);
static unsigned char *I2C_ReceiveField;
static unsigned char *I2C_TransmitField;
static unsigned char *I2ca_RxFifo_Buf;
/*
 * Initialize I2ca
 */
void I2ca_Init(void)
{
	I2caRegs.I2CMDR.bit.IRS = 0;    //Reset the I2C

	I2caRegs.I2COAR.bit.OAR = 0xAA;       // I2C own address as Master.

	I2caRegs.I2CPSC.all = (SYSTEMCLK/10) - 1;		// Prescaler - need 7-12 Mhz on module clk
									// I2C module clock frequency = SYSCLKOUT / (I2CPSC + 1) = 200 / (1+19) = 10MHz
									// Can be changed only when I2CMDR.IRS = 0;

	I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
									// ICCL determines the amount of time the signal is low
									// Low-time duration = Tmod * (ICCL +d)
	I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
									// ICCH determines the amount of time the sigal is high
									// High-time duration = Tmod * (ICCH +d)
									// | I2CPSC |  d    |
									// |--------|-------|
									// |   0    |  7    |
									// |--------|-------|
									// |   1    |  6    |
									// |--------|-------|
									// |   >1   |  5    |
									//
									// Tmod = 1 / 10MHz = 0.1us
									// I2C rate = 1 / Tmod * (ICCL+d+ICCH+d) = 1 / 0.1 * (10 +5 +5 +5) = 400Kbps


	//Each interrupt request has a flag bit in the status register(I2CSTR)
	//and an enable bit in the interrupt enable register(I2CIER)
	//When one of the specified events occurs, its flag bit is set
	//If the corresponding enable bit is 0, the interrupt request is blocked
	//If the corresponding enable bit is 1, the interrupt request is forwarded to the CPU as an I2C interrupt
	I2caRegs.I2CIER.bit.AAS  = 0; 	 //Addressed as slave condition
	I2caRegs.I2CIER.bit.ARBL = 0;	 //Not involved in the datasheet   Arbitration-lost condition?
	I2caRegs.I2CIER.bit.ARDY = 0;    //Register-access condition: The I2C module registers are ready to be read
									 //because the perviously programmed address, data, and command valus have
									 //been used
	I2caRegs.I2CIER.bit.NACK = 0;    //No-acknowladge condition
	I2caRegs.I2CIER.bit.RRDY = 0;    //Receive ready condition: The data receive register(I2CDRR) is ready to be read
									 //because data has been copied from the receive shift register (I2CRSR) to I2CDRR
	I2caRegs.I2CIER.bit.SCD  = 0;	 //Stop condition detected
	I2caRegs.I2CIER.bit.XRDY = 0;    //Transmit ready condition: The data transmit register(I2CDXR) is ready to be read
									 //because data has been copied from I2CDXR to the transmit shift register (I2CXSR)




	//	I2caRegs.I2CMDR.bit.BC  = 0;    //8 bits per data byte
	//	I2caRegs.I2CMDR.bit.FDF = 0;    //Disable the free data mode. Transfers use the 7-/10-bit addressing format
	//									//selected by XA bit
	//	I2caRegs.I2CMDR.bit.STB = 0;    //The I2C module is not in the START byte mode
	////	I2caRegs.I2CMDR.bit.IRS = 1;    //Enable the I2C module
	//	I2caRegs.I2CMDR.bit.DLB = 0;    //Digital loopback mode is disabled
	//	I2caRegs.I2CMDR.bit.RM  = 1;    //!!!!!
	//									//ONLY APPLICIABLE WHEN THE I2C MODULE IS A MASTER-TRANSMITTER
	//									//1-->Repeat mode. A data byte is transmitted each time the I2CDXR register is written
	//									//to (or until transmit FIFO is empty when in FIFO mode)until the STP bit is maually
	//									//set. The value of I2CCNT is ignored. The ARDY bit/interrupt can be used to determine
	//									//when the I2CDXR(or FIFO)is ready for more date, or when the data has all been sent
	//									//and the CPU is allowed to write to the STP bit.
	//									//0--> Non-repeat mode  The value in I2CCNT determine how many bytes are
	//									//received/transmitted by the I2C module
	//	I2caRegs.I2CMDR.bit.XA  = 0;    //7-bit addressing mode
	////	I2caRegs.I2CMDR.bit.TRX = 0;    //0 --> Receive mode
	//									//1 --> Transmit mode
	//	I2caRegs.I2CMDR.bit.MST = 1;    //Master mode
	////	I2caRegs.I2CMDR.bit.STP = 0;	//Not written when IRS = 0;
	//									//Can be used to generate a STOP condition
	////	I2caRegs.I2CMDR.bit.rsvd1 = 0;  //Reserved
	////	I2caRegs.I2CMDR.bit.STT = 1;	//Not wirtten when ISR = 0;
	//									//Can be used to generate a START condition
	//	I2caRegs.I2CMDR.bit.FREE = 0;	//For breakpoint Debug
	//	I2caRegs.I2CMDR.bit.NACKMOD = 0;//In the master-receive mode: The I2C module sends an ACK bit during each acknowledge
	//									//cycle until the internal data counter(I2CCNT) counts down to 0;



	//	I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
	//	I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

		I2caRegs.I2CMDR.bit.IRS  = 1;	// Take I2C out of reset
}

/*
 * Write data to slave's register
 */
void I2ca_Write(unsigned char slave_address,unsigned char register_address,int length,unsigned char *buf)
{
	I2C_TransmitField = buf;
	DELAY_US(2);
	I2caRegs.I2CSAR.bit.SAR = slave_address;
	I2caRegs.I2CMDR.all = 0x2E20;
	I2caRegs.I2CCNT = length + 1;
	I2caRegs.I2CDXR.bit.DATA = register_address;
	while(I2caRegs.I2CSTR.bit.XRDY ==0){}
	while(length > 0)
	{
		I2caRegs.I2CDXR.bit.DATA = *I2C_TransmitField;
		while(I2caRegs.I2CSTR.bit.XRDY ==0){}
		I2C_TransmitField ++;
		length --;
	}
}

/*
 * Read data from slave's register
 */
void I2ca_Read(unsigned char slave_address,unsigned char register_address,int length,unsigned char *buf)
{
	I2C_ReceiveField = buf;
//	DELAY_US(2);
	I2caRegs.I2CSAR.bit.SAR = slave_address;
	I2caRegs.I2CMDR.all = 0x2620;
	I2caRegs.I2CCNT = 1;
	I2caRegs.I2CDXR.bit.DATA = register_address;
	while(I2caRegs.I2CSTR.bit.XRDY ==0){}
	I2caRegs.I2CMDR.all = 0x2C20;
	I2caRegs.I2CCNT = length;
	while(length > 0)
	{
		while(I2caRegs.I2CSTR.bit.RRDY ==0){}
		*I2C_ReceiveField = I2caRegs.I2CDRR.bit.DATA;
		I2C_ReceiveField ++;
		length --;
	}
}

void I2ca_RxFifo_Init(void)
{
	I2caRegs.I2CMDR.bit.IRS = 0;    //Reset the I2C

	I2caRegs.I2COAR.bit.OAR = 0xAA;       // I2C own address as Master.

	I2caRegs.I2CPSC.all = (SYSTEMCLK/10) - 1;		// Prescaler - need 7-12 Mhz on module clk
									// I2C module clock frequency = SYSCLKOUT / (I2CPSC + 1) = 200 / (1+19) = 10MHz
									// Can be changed only when I2CMDR.IRS = 0;

	I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
									// ICCL determines the amount of time the signal is low
									// Low-time duration = Tmod * (ICCL +d)
	I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
									// ICCH determines the amount of time the sigal is high
									// High-time duration = Tmod * (ICCH +d)
									// | I2CPSC |  d    |
									// |--------|-------|
									// |   0    |  7    |
									// |--------|-------|
									// |   1    |  6    |
									// |--------|-------|
									// |   >1   |  5    |
									//
									// Tmod = 1 / 10MHz = 0.1us
									// I2C rate = 1 / Tmod * (ICCL+d+ICCH+d) = 1 / 0.1 * (10 +5 +5 +5) = 400Kbps


	//Each interrupt request has a flag bit in the status register(I2CSTR)
	//and an enable bit in the interrupt enable register(I2CIER)
	//When one of the specified events occurs, its flag bit is set
	//If the corresponding enable bit is 0, the interrupt request is blocked
	//If the corresponding enable bit is 1, the interrupt request is forwarded to the CPU as an I2C interrupt
	I2caRegs.I2CIER.bit.AAS  = 0; 	 //Addressed as slave condition
	I2caRegs.I2CIER.bit.ARBL = 0;	 //Not involved in the datasheet   Arbitration-lost condition?
	I2caRegs.I2CIER.bit.ARDY = 0;    //Register-access condition: The I2C module registers are ready to be read
									 //because the perviously programmed address, data, and command valus have
									 //been used
	I2caRegs.I2CIER.bit.NACK = 0;    //No-acknowladge condition
	I2caRegs.I2CIER.bit.RRDY = 0;    //Receive ready condition: The data receive register(I2CDRR) is ready to be read
									 //because data has been copied from the receive shift register (I2CRSR) to I2CDRR
	I2caRegs.I2CIER.bit.SCD  = 0;	 //Stop condition detected
	I2caRegs.I2CIER.bit.XRDY = 0;    //Transmit ready condition: The data transmit register(I2CDXR) is ready to be read
									 //because data has been copied from I2CDXR to the transmit shift register (I2CXSR)




	//	I2caRegs.I2CMDR.bit.BC  = 0;    //8 bits per data byte
	//	I2caRegs.I2CMDR.bit.FDF = 0;    //Disable the free data mode. Transfers use the 7-/10-bit addressing format
	//									//selected by XA bit
	//	I2caRegs.I2CMDR.bit.STB = 0;    //The I2C module is not in the START byte mode
	////	I2caRegs.I2CMDR.bit.IRS = 1;    //Enable the I2C module
	//	I2caRegs.I2CMDR.bit.DLB = 0;    //Digital loopback mode is disabled
	//	I2caRegs.I2CMDR.bit.RM  = 1;    //!!!!!
	//									//ONLY APPLICIABLE WHEN THE I2C MODULE IS A MASTER-TRANSMITTER
	//									//1-->Repeat mode. A data byte is transmitted each time the I2CDXR register is written
	//									//to (or until transmit FIFO is empty when in FIFO mode)until the STP bit is maually
	//									//set. The value of I2CCNT is ignored. The ARDY bit/interrupt can be used to determine
	//									//when the I2CDXR(or FIFO)is ready for more date, or when the data has all been sent
	//									//and the CPU is allowed to write to the STP bit.
	//									//0--> Non-repeat mode  The value in I2CCNT determine how many bytes are
	//									//received/transmitted by the I2C module
	//	I2caRegs.I2CMDR.bit.XA  = 0;    //7-bit addressing mode
	////	I2caRegs.I2CMDR.bit.TRX = 0;    //0 --> Receive mode
	//									//1 --> Transmit mode
	//	I2caRegs.I2CMDR.bit.MST = 1;    //Master mode
	////	I2caRegs.I2CMDR.bit.STP = 0;	//Not written when IRS = 0;
	//									//Can be used to generate a STOP condition
	////	I2caRegs.I2CMDR.bit.rsvd1 = 0;  //Reserved
	////	I2caRegs.I2CMDR.bit.STT = 1;	//Not wirtten when ISR = 0;
	//									//Can be used to generate a START condition
	//	I2caRegs.I2CMDR.bit.FREE = 0;	//For breakpoint Debug
	//	I2caRegs.I2CMDR.bit.NACKMOD = 0;//In the master-receive mode: The I2C module sends an ACK bit during each acknowledge
	//									//cycle until the internal data counter(I2CCNT) counts down to 0;



	//	I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
//	I2caRegs.I2CFFRX.bit.RXFFRST = 0;	//Hold on reset
	//	I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

	I2caRegs.I2CFFTX.bit.TXFFRST = 0;
	I2caRegs.I2CFFTX.bit.I2CFFEN = 1; 		//NOTE:This bit must be enabled for either the transmit or the receive FIFO to
											//operate correctly
											//!!!!!
	I2caRegs.I2CFFTX.bit.TXFFIENA = 0;		//Disable TX FIFO interrupt
	I2caRegs.I2CFFTX.bit.TXFFIL	  =16;
	I2caRegs.I2CFFTX.bit.TXFFINTCLR =1;
	I2caRegs.I2CFFTX.bit.TXFFST = 0;


	I2caRegs.I2CFFRX.bit.RXFFRST = 0;		//RXFIFO reset
	I2caRegs.I2CFFRX.bit.RXFFIENA = 1;		//Enable Rx FIFO interrupt
	I2caRegs.I2CFFRX.bit.RXFFINTCLR = 1;
	I2caRegs.I2CFFRX.bit.RXFFST = 0;
	I2caRegs.I2CFFRX.bit.RXFFIL =I2CA_RXFIFO_LEN;	//Should be set before reset the RXFIFO

	I2caRegs.I2CMDR.bit.IRS  = 1;	// Take I2C out of reset
	I2caRegs.I2CFFRX.bit.RXFFRST = 1;	//Relinquish I2ca Rx Fifo from reset
	I2caRegs.I2CFFTX.bit.TXFFRST = 1;	//Relinquish I2ca Tx Fifo from reset

	EALLOW;
	PieVectTable.I2CA_FIFO_INT = &I2ca_RxFifo_Isr;
	EDIS;
	IER |= M_INT8;
	PieCtrlRegs.PIEIER8.bit.INTx2 = 1;   			// PIE Group 8, INT2
}


void I2ca_Read_with_RxFifo(unsigned char slave_address,unsigned char register_address,unsigned char *buf)
{
	I2ca_RxFifo_Buf = buf;
	I2caRegs.I2CSAR.bit.SAR = slave_address;
	I2caRegs.I2CMDR.all = 0x2620;
	I2caRegs.I2CCNT = 1;
	I2caRegs.I2CDXR.bit.DATA = register_address;
	while(I2caRegs.I2CSTR.bit.XRDY ==0){}
	I2caRegs.I2CCNT = I2CA_RXFIFO_LEN;
	I2caRegs.I2CMDR.all = 0x2C20;
}

/*
 * I2ca Rx Fifo interrupt ISR
 * Reveive MPU data
 */
interrupt void I2ca_RxFifo_Isr(void)
{
	int i;
	for(i=0;i<I2CA_RXFIFO_LEN;i++)
	{
		*(I2ca_RxFifo_Buf+i) = I2caRegs.I2CDRR.bit.DATA;
	}
	I2caRegs.I2CFFRX.bit.RXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
