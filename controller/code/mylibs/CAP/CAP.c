/*
 * CAP.c
 *
 *  Created on: 2016Äê12ÔÂ7ÈÕ
 *      Author: gtc
 */

#include "CAP.h"

unsigned int RC_PWM_Value[RC_CH_Num] = {0,0,0,0,0,0};

void CAP1_Init(void);
void CAP2_Init(void);
void CAP3_Init(void);
void CAP4_Init(void);
void CAP5_Init(void);
void CAP6_Init(void);
__interrupt void ecap1_isr(void);
__interrupt void ecap2_isr(void);
__interrupt void ecap3_isr(void);
__interrupt void ecap4_isr(void);
__interrupt void ecap5_isr(void);
__interrupt void ecap6_isr(void);
void CAP_Init(void)
{
	CAP1_Init();
	CAP2_Init();
	CAP3_Init();
	CAP4_Init();
	CAP5_Init();
	CAP6_Init();
}

void CAP1_Init(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP1_INT = &ecap1_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;

	ECap1Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap1Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;   // one-shot mode
	ECap1Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap1Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap1Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap1Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap1Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap1Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap1Regs.ECEINT.bit.CEVT2 = 1;
}


__interrupt void ecap1_isr(void)
{
	RC_PWM_Value[RC_CH1] = (int)(ECap1Regs.CAP2 / 200);
	ECap1Regs.ECCLR.bit.CEVT2 = 1;
	ECap1Regs.ECCLR.bit.INT = 1;
	ECap1Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


void CAP2_Init(void)
{
	ECap2Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap2Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap2Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap2Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
	ECap2Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap2Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap2Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap2Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap2Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap2Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap2Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap2Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap2Regs.ECEINT.bit.CEVT2 = 1;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP2_INT = &ecap2_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
}


__interrupt void ecap2_isr(void)
{
	RC_PWM_Value[RC_CH2] = (int)(ECap2Regs.CAP2 / 200);
	ECap2Regs.ECCLR.bit.CEVT2 = 1;
	ECap2Regs.ECCLR.bit.INT = 1;
	ECap2Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


void CAP3_Init(void)
{
	ECap3Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap3Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap3Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap3Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
	ECap3Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap3Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap3Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap3Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap3Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap3Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap3Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap3Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap3Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap3Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap3Regs.ECEINT.bit.CEVT2 = 1;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP3_INT = &ecap3_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx3 = 1;
}


__interrupt void ecap3_isr(void)
{
	RC_PWM_Value[RC_CH3] = (int)(ECap3Regs.CAP2 / 200);
	ECap3Regs.ECCLR.bit.CEVT2 = 1;
	ECap3Regs.ECCLR.bit.INT = 1;
	ECap3Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


void CAP4_Init(void)
{
	ECap4Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap4Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap4Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap4Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap4Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
	ECap4Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap4Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap4Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap4Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap4Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap4Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap4Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap4Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap4Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap4Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap4Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap4Regs.ECEINT.bit.CEVT2 = 1;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP4_INT = &ecap4_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx4 = 1;
}


__interrupt void ecap4_isr(void)
{
	RC_PWM_Value[RC_CH4] = (int)(ECap4Regs.CAP2 / 200);
	ECap4Regs.ECCLR.bit.CEVT2 = 1;
	ECap4Regs.ECCLR.bit.INT = 1;
	ECap4Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}



void CAP5_Init(void)
{
	ECap5Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap5Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap5Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap5Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
	ECap5Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap5Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap5Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap5Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap5Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap5Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap5Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap5Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap5Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap5Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap5Regs.ECEINT.bit.CEVT2 = 1;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP5_INT = &ecap5_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx5 = 1;
}


__interrupt void ecap5_isr(void)
{
	RC_PWM_Value[RC_CH5] = (int)(ECap5Regs.CAP2 / 200);
	ECap5Regs.ECCLR.bit.CEVT2 = 1;
	ECap5Regs.ECCLR.bit.INT = 1;
	ECap5Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}



void CAP6_Init(void)
{
	ECap6Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap6Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap6Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped
	//
	// Configure peripheral registers
	//
	ECap6Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
	ECap6Regs.ECCTL2.bit.STOP_WRAP = 1;		//Stop at 2 events
	ECap6Regs.ECCTL1.bit.CAP1POL = 0;		//Rising edge
	ECap6Regs.ECCTL1.bit.CAP2POL = 1;		//Falling edge
	ECap6Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
	ECap6Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
	ECap6Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
	ECap6Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
	ECap6Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
	ECap6Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
	ECap6Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
	ECap6Regs.ECEINT.bit.CEVT2 = 1;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP6_INT = &ecap6_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//
	// Enable CPU INT4 which is connected to ECAP1-4 INT:
	//
	IER |= M_INT4;

	//
	// Enable eCAP INTn in the PIE: Group 3 __interrupt 1-6
	//
	PieCtrlRegs.PIEIER4.bit.INTx6 = 1;
}


__interrupt void ecap6_isr(void)
{
	RC_PWM_Value[RC_CH6] = (int)(ECap6Regs.CAP2 / 200);
	ECap6Regs.ECCLR.bit.CEVT2 = 1;
	ECap6Regs.ECCLR.bit.INT = 1;
	ECap6Regs.ECCTL2.bit.REARM = 1;
	//
	// Acknowledge this __interrupt to receive more __interrupts from group 4
	//
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}
