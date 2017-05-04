/*
 * Sci.h
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */

#ifndef SCI_H_
#define SCI_H_

#include "..\mylibs.h"

#define SCIA_TXFIFO_LEN				16
#define SCIA_RXFIFO_LEN				1
#define SCIB_TXFIFO_LEN				16
#define SCIB_RXFIFO_LEN				16
#define SCI_TXFIFO_EMPTY			0
#define SCI_TXFIFO_1_WORDS_LEFT	1
#define SCI_TXFIFO_2_WORDS_LEFT	2
#define SCI_TXFIFO_3_WORDS_LEFT	3
#define SCI_TXFIFO_4_WORDS_LEFT	4
#define SCI_TXFIFO_5_WORDS_LEFT	5
#define SCI_TXFIFO_6_WORDS_LEFT	6
#define SCI_TXFIFO_7_WORDS_LEFT	7
#define SCI_TXFIFO_8_WORDS_LEFT	8
#define SCI_TXFIFO_9_WORDS_LEFT	9
#define SCI_TXFIFO_10_WORDS_LEFT	10
#define SCI_TXFIFO_11_WORDS_LEFT	11
#define SCI_TXFIFO_12_WORDS_LEFT	12
#define SCI_TXFIFO_13_WORDS_LEFT	13
#define SCI_TXFIFO_14_WORDS_LEFT	14
#define SCI_TXFIFO_15_WORDS_LEFT	15
#define SCI_TXFIFO_16_WORDS_LEFT	16
void Scia_Init(void);
void Scia_Send_Byte(char a);
void Scia_Send_Msg(char *a,int len);
void Scia_TxFifo_Interrupt_Enable(void);
void Scia_TxFifo_Interrupt_Disable(void);
int Scia_TxFifo_Status();

void Scib_Init(void);
void Scib_Send_Byte(char a);
void Scib_Send_Msg(char *a,int len);
void Scib_TxFifo_Interrupt_Enable(void);
void Scib_TxFifo_Interrupt_Disable(void);
int Scib_TxFifo_Status();


#endif /* SCI_H_ */
