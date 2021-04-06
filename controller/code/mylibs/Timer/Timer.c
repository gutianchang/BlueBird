/*
 * Timer.c
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */
#include "Timer.h"
__interrupt void Cpu_Timer0_Isr(void);
__interrupt void Cpu_Timer1_Isr(void);
__interrupt void Cpu_Timer2_Isr(void);

extern unsigned int RC_PWM_Value[RC_CH_Num];

/*
 * Initialize CPU Timer0
 * PRD = 1ms
 * T = 1us
 */
void CpuTimer0_Init(void)
{
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	CpuTimer0Regs.PRD.all  = (unsigned long int)(CPUTIMER0_PRD*1000-1);
	CpuTimer0Regs.TPR.all  = (SYSTEMCLK-1);     //Pre-scale = 200   T = 200 * 1/200 = 1us
	CpuTimer0Regs.TPRH.all = 0x00;
	CpuTimer0Regs.TCR.bit.TSS = 1;     //Stop the counter
	CpuTimer0Regs.TCR.bit.TRB = 1;     //Reload the counter with period value
	CpuTimer0Regs.TCR.bit.TIE = 1;	   //Enable the interrupt
	CpuTimer0.InterruptCount = 0;      //Reset interrupt counter
	EALLOW;
	PieVectTable.TIMER0_INT = &Cpu_Timer0_Isr;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;// Enable TINT0 in the PIE: Group 1 interrupt 7
	IER |= M_INT1;
	EDIS;
}

/*
 * CPU Timer0 ISR
 * Run every 2ms with priority 5
 */
__interrupt void Cpu_Timer0_Isr(void)
{
   CpuTimer0.InterruptCount++;
   Loop_500Hz();
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/*
 * Initialize CPU Timer1
 * PRD = 10ms
 * T = 1us
 */
void CpuTimer1_Init(void)
{
	CpuTimer1.RegsAddr = &CpuTimer1Regs;
	CpuTimer1Regs.PRD.all  = (unsigned long int)(CPUTIMER1_PRD*1000-1);
	CpuTimer1Regs.TPR.all  = (SYSTEMCLK-1);     //Pre-scale = 200   T = 200 * 1/200 = 1us
	CpuTimer1Regs.TPRH.all = 0x00;
	CpuTimer1Regs.TCR.bit.TSS = 1;     //Stop the counter
	CpuTimer1Regs.TCR.bit.TRB = 1;     //Reload the counter with period value
	CpuTimer1Regs.TCR.bit.TIE = 1; 	   //Enable the interrupt
	CpuTimer1.InterruptCount = 0;      //Reset interrupt counter
	EALLOW;
	PieVectTable.TIMER1_INT = &Cpu_Timer1_Isr;
	IER |= M_INT13;
	EDIS;
}

/*
 * CPU Timer1 ISR
 * Run every 10ms with priority 17
 */
__interrupt void Cpu_Timer1_Isr(void)
{
   CpuTimer1.InterruptCount++;
//   float temperature = MPU_Get_Temperature();
//
   Loop_100Hz();
}


/*
 * Initialize CPU Timer2
 * PRD = 100ms
 * T = 1us
 */
void CpuTimer2_Init(void)
{
	CpuTimer2.RegsAddr = &CpuTimer2Regs;
	CpuTimer2Regs.PRD.all  = (unsigned long int)CPUTIMER2_PRD*1000-1;
	CpuTimer2Regs.TPR.all  = (SYSTEMCLK-1);     //Pre-scale = 200   T = 200 * 1/200 = 1us
	CpuTimer2Regs.TPRH.all = 0x00;
	CpuTimer2Regs.TCR.bit.TSS = 1;     //Stop the counter
	CpuTimer2Regs.TCR.bit.TRB = 1;     //Reload the counter with period value
	CpuTimer2Regs.TCR.bit.TIE = 1;     //Enable interrupt
 	CpuTimer2.InterruptCount = 0;      //Reset interrupt counter
	EALLOW;
	PieVectTable.TIMER2_INT = &Cpu_Timer2_Isr;
	IER |= M_INT14;
	EDIS;
}

/*
 * CPU Timer2 ISR
 * Run every 100ms with priority 18
 */
__interrupt void Cpu_Timer2_Isr(void)
{
   CpuTimer2.InterruptCount++;
   Loop_10Hz();
}
