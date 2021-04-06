/*
 * Sci.c
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */
#include "Sci.h"
char Scia_Tx_Data[SCIA_TXFIFO_LEN];		//Scia Tx buffer
char Scia_Rx_Data[SCIA_RXFIFO_LEN];		//Scia Rx buffer

char Scib_Tx_Data[SCIB_TXFIFO_LEN];		//Scia Tx buffer
char Scib_Rx_Data[SCIB_RXFIFO_LEN];		//Scia Rx buffer

interrupt void Scia_TxFifo_Isr(void);
interrupt void Scia_RxFifo_Isr(void);

interrupt void Scib_TxFifo_Isr(void);
interrupt void Scib_RxFifo_Isr(void);

/*********************************************************************
 *
 *
 * SCIA
 *
 *
 *
 */

/*
 * Initialize SCIA
 * BR = 115200
 */
void Scia_Init(void)
{
	SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0;	 //idle-line protocol
	SciaRegs.SCICCR.bit.LOOPBKENA = 0;
	SciaRegs.SCICCR.bit.PARITY    = 0; 		 //No parity
	SciaRegs.SCICCR.bit.PARITYENA = 0;
	SciaRegs.SCICCR.bit.SCICHAR   = 7;		 //Character length = 8 bits
	SciaRegs.SCICCR.bit.STOPBITS  = 0;		 //One stop bit

	SciaRegs.SCICTL1.bit.RXENA = 1;			//Enable RX
	SciaRegs.SCICTL1.bit.RXERRINTENA = 0;
	SciaRegs.SCICTL1.bit.SLEEP = 0;
	SciaRegs.SCICTL1.bit.SWRESET = 0;		//Reset
	SciaRegs.SCICTL1.bit.TXENA = 1; 		//Enable TX
	SciaRegs.SCICTL1.bit.TXWAKE = 0;

	SciaRegs.SCICTL2.bit.TXEMPTY = 1;
	SciaRegs.SCICTL2.bit.TXRDY = 1;
	SciaRegs.SCICTL2.bit.TXINTENA = 1;		//Enable SCITXBUFF interrupt
	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;	//Receive-buffer/break interrupt enable

	SciaRegs.SCIFFTX.bit.SCIRST = 1;
	SciaRegs.SCIFFTX.bit.SCIFFENA = 1;
	SciaRegs.SCIFFTX.bit.TXFIFORESET = 0;
	SciaRegs.SCIFFTX.bit.TXFFST = 0;
	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	SciaRegs.SCIFFTX.bit.TXFFIENA = 0;					//Disable TX FFIO interrupt
	SciaRegs.SCIFFTX.bit.TXFFIL = 16-SCIA_TXFIFO_LEN;		//Interrupt when 0 word in Tx FIFO

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;
	SciaRegs.SCIFFRX.bit.RXFFST = 0;
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
	SciaRegs.SCIFFRX.bit.RXFFIL = SCIA_RXFIFO_LEN;		//Interrupt when 16 words in Rx FIFO

	SciaRegs.SCIFFCT.all = 0;

	unsigned int baud = (unsigned int)(50000000/(SCIA_BAUDRATE*8)-1);//set baud-rate
//	unsigned int baud = (unsigned int)(LSPCLK/(SCIA_BAUDRATE*8)-1);
	SciaRegs.SCIHBAUD.all = (baud>>8)&0xFF;
	SciaRegs.SCILBAUD.all = baud&0x00FF;

	SciaRegs.SCICTL1.bit.SWRESET = 1;  		// Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFORESET = 1; 	// Relinquish TX FIFO from Reset
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;	// Relinquish RX FIFO from Reset

	EALLOW;
    PieVectTable.SCIA_RX_INT = &Scia_RxFifo_Isr;
    PieVectTable.SCIA_TX_INT = &Scia_TxFifo_Isr;
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   			// PIE Group 9, INT1
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;  		 	// PIE Group 9, INT2
    IER |= M_INT9;
    EDIS;
}

void Scia_Send_Byte(char a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST == SCI_TXFIFO_16_WORDS_LEFT) {}
    SciaRegs.SCITXBUF.all = a;
}

//void Scia_Send_Msg(char *a,int len)
//{
//	int i = 0;
//	for(i=0;i<len;i++)
//		Scia_Send_Byte(*(a+i));
//	i++;
//}

/*
 * Scia Send Message
 * Use FIFO for best possible usage of SCI bus bandwidth
 */
void Scia_Send_Msg(char *a,int len)
{
	Uint16 i =0;
	int temp = len;
	while(len>0)
	{
		int empty_buffer_num = SCIA_TXFIFO_LEN - Scia_TxFifo_Status();
		int tx_num = (empty_buffer_num>len)?len:empty_buffer_num;
		for(i=0;i<tx_num;i++)
			{
				SciaRegs.SCITXBUF.all = *(a+temp-len+i);
			}
		len = len-tx_num;
	}
}

/*
 * Enable Scia Tx Fifo interrupt
 */
void Scia_TxFifo_Interrupt_Enable(void)
{
	SciaRegs.SCIFFTX.bit.TXFFIENA = 1;
}

/*
 * Diable Scia Tx Fifo interrupt
 */
void Scia_TxFifo_Interrupt_Disable(void)
{
	SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
}

/*
 *Return how many words left in TXFFIO
 */
int Scia_TxFifo_Status()
{
	return SciaRegs.SCIFFTX.bit.TXFFST;
}

/*
 * Scia Tx Fifo ISR
 */
interrupt void Scia_TxFifo_Isr(void)
{
    SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;   // Clear SCI Interrupt flag
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;	 //ACK other interrupts in group 9
}

/*
 * Scia Rx Fifo ISR
 * Receive 16 Bytes
 */
interrupt void Scia_RxFifo_Isr(void)
{
//    Uint16 i;

//    for(i=0;i<SCIA_RXFIFO_LEN;i++)
//    {
//       Scia_Rx_Data[i]=SciaRegs.SCIRXBUF.bit.SAR;		//Read data from FIFO
//    }

	unsigned char data = SciaRegs.SCIRXBUF.bit.SAR;

//	Get_PID_from_ANO_OSC(data);
	ANO_DT_Data_Receive_Prepare(data);
    Led2_Toggle();
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   	 // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;  	 // Clear Interrupt flag

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;         // Issue PIE ack
}


/*****************************************************************************
 *
 *
 * SCIB
 *
 *
 *
 */


/*
 * Initialize SCIB
 * BR = 115200
 */
void Scib_Init(void)
{
	ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;	 //idle-line protocol
	ScibRegs.SCICCR.bit.LOOPBKENA = 0;
	ScibRegs.SCICCR.bit.PARITY    = 0; 		 //No parity
	ScibRegs.SCICCR.bit.PARITYENA = 0;
	ScibRegs.SCICCR.bit.SCICHAR   = 7;		 //Character length = 8 bits
	ScibRegs.SCICCR.bit.STOPBITS  = 0;		 //One stop bit

	ScibRegs.SCICTL1.bit.RXENA = 1;			//Enable RX
	ScibRegs.SCICTL1.bit.RXERRINTENA = 0;
	ScibRegs.SCICTL1.bit.SLEEP = 0;
	ScibRegs.SCICTL1.bit.SWRESET = 0;		//Reset
	ScibRegs.SCICTL1.bit.TXENA = 1; 		//Enable TX
	ScibRegs.SCICTL1.bit.TXWAKE = 0;

	ScibRegs.SCICTL2.bit.TXEMPTY = 1;
	ScibRegs.SCICTL2.bit.TXRDY = 1;
	ScibRegs.SCICTL2.bit.TXINTENA = 1;		//Enable SCITXBUFF interrupt
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	//Receive-buffer/break interrupt enable

	ScibRegs.SCIFFTX.bit.SCIRST = 1;
	ScibRegs.SCIFFTX.bit.SCIFFENA = 1;
	ScibRegs.SCIFFTX.bit.TXFIFORESET = 0;
	ScibRegs.SCIFFTX.bit.TXFFST = 0;
	ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	ScibRegs.SCIFFTX.bit.TXFFIENA = 0;					//Disable TX FFIO interrupt
	ScibRegs.SCIFFTX.bit.TXFFIL = 16-SCIB_TXFIFO_LEN;		//Interrupt when 0 word in Tx FIFO

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 0;
	ScibRegs.SCIFFRX.bit.RXFFST = 0;
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
	ScibRegs.SCIFFRX.bit.RXFFIL = SCIB_RXFIFO_LEN;		//Interrupt when 16 words in Rx FIFO

	ScibRegs.SCIFFCT.all = 0;

	unsigned int baud = (unsigned int)(50000000/(SCIB_BAUDRATE*8)-1);//set baud-rate
	ScibRegs.SCIHBAUD.all = (baud>>8)&0xFF;
	ScibRegs.SCILBAUD.all = baud&0x00FF;

	ScibRegs.SCICTL1.bit.SWRESET = 1;  		// Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFORESET = 1; 	// Relinquish TX FIFO from Reset
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	// Relinquish RX FIFO from Reset

	EALLOW;
    PieVectTable.SCIB_RX_INT = &Scib_RxFifo_Isr;
    PieVectTable.SCIB_TX_INT = &Scib_TxFifo_Isr;
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   			// PIE Group 9, INT1
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;  		 	// PIE Group 9, INT2
    IER |= M_INT9;
    EDIS;
}

/*
 * Scib Send a Byte
 */
void Scib_Send_Byte(char a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST == SCI_TXFIFO_16_WORDS_LEFT) {}
    ScibRegs.SCITXBUF.all =a;
}

//void Scib_Send_Msg(char *a,int len)
//{
//	int i = 0;
//	for(i=0;i<len;i++)
//		Scib_Send_Byte(*(a+i));
//	i++;
//}

/*
 * Scib Send Message
 * Use FIFO for best possible usage of SCI bus bandwidth
 */
void Scib_Send_Msg(char *a,int len)
{
	Uint16 i =0;
	int temp = len;
	while(len>0)
	{
		int empty_buffer_num = SCIB_TXFIFO_LEN - Scib_TxFifo_Status();
		int tx_num = (empty_buffer_num>len)?len:empty_buffer_num;
		for(i=0;i<tx_num;i++)
			{
				ScibRegs.SCITXBUF.all = *(a+temp-len+i);
			}
		len = len-tx_num;
	}
}

/*
 * Enable Scib Tx Fifo interrupt
 */
void Scib_TxFifo_Interrupt_Enable(void)
{
	ScibRegs.SCIFFTX.bit.TXFFIENA = 1;
}

/*
 * Diable Scib Tx Fifo interrupt
 */
void Scib_TxFifo_Interrupt_Disable(void)
{
	ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
}

/*
 *Return how many words left in TXFFIO
 */
int Scib_TxFifo_Status()
{
	return ScibRegs.SCIFFTX.bit.TXFFST;
}

/*
 * Scib Tx Fifo ISR
 */
interrupt void Scib_TxFifo_Isr(void)
{
    ScibRegs.SCIFFTX.bit.TXFFINTCLR=1;   // Clear SCI Interrupt flag
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;	 //ACK other interrupts in group 9
}

/*
 * Scib Rx Fifo ISR
 * Receive 16 Bytes
 */
interrupt void Scib_RxFifo_Isr(void)
{
    Uint16 i;

    for(i=0;i<SCIB_RXFIFO_LEN;i++)
    {
       Scib_Rx_Data[i]=ScibRegs.SCIRXBUF.bit.SAR;		//Read data from FIFO
    }

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   	 // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;  	 // Clear Interrupt flag

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;         // Issue PIE ack
}
