/*
 * PWM.c
 *
 *  Created on: 2016Äê11ÔÂ28ÈÕ
 *      Author: gtc
 */

#include "PWM.h"
void PWM_Init(void)
{
	EALLOW;
	ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;  //EPWMCLK = PLLSYSCLKOUT = 200MHz
	EDIS;
	PWM1_Init();
	PWM2_Init();
	PWM3_Init();
	PWM4_Init();
}
/*
 * init pwm1
 *
 * */
void PWM1_Init(void)
{
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	//
	// Setup TBCLK
	//
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm1Regs.TBPRD = (int)(EPWM_TIMER_TBPRD*12.5);       // Set timer period
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
	EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;   // Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;

	//
	// Setup shadow register load on ZERO
	//
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	//
	// Set Compare values
	//
	EPwm1Regs.CMPA.bit.CMPA = (int)(EPWM_MIN*12.5);     // Set compare A value
	EPwm1Regs.CMPB.bit.CMPB = (int)(EPWM_MIN*12.5);     // Set Compare B value

	//
	// Set actions
	//
	EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
												 // up count

	EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
												 // up count

	//
	// Interrupt where we will change the Compare Values
	//
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 0;                // Disable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
}



/*
 * init pwm2
 *
 * */
void PWM2_Init(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //
   // Setup TBCLK
   //
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm2Regs.TBPRD = (int)(EPWM_TIMER_TBPRD*12.5);       // Set timer period
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;   // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV4;
   //
   // Setup shadow register load on ZERO
   //
   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm2Regs.CMPA.bit.CMPA = (int)(EPWM_MIN*12.5);     // Set compare A value
   EPwm2Regs.CMPB.bit.CMPB = (int)(EPWM_MIN*12.5);     // Set Compare B value

   //
   // Set actions
   //
   EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
												 // up count

   EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
												 // up count

   //
   // Interrupt where we will change the Compare Values
   //
   EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
   EPwm2Regs.ETSEL.bit.INTEN = 0;                // Disable INT
   EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

/*
 * init pwm3
 *
 * */
void PWM3_Init(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //
   // Setup TBCLK
   //
   EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm3Regs.TBPRD = (int)(EPWM_TIMER_TBPRD*12.5);       // Set timer period
   EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;   // Clock ratio to SYSCLKOUT
   EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV4;

   //
   // Setup shadow register load on ZERO
   //
   EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm3Regs.CMPA.bit.CMPA = (int)(EPWM_MIN*12.5);     // Set compare A value
   EPwm3Regs.CMPB.bit.CMPB = (int)(EPWM_MIN*12.5);     // Set Compare B value

   //
   // Set actions
   //
   EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
												 // up count

   EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
												 // up count

   //
   // Interrupt where we will change the Compare Values
   //
   EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
   EPwm3Regs.ETSEL.bit.INTEN = 0;                // Disable INT
   EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

/*
 * init pwm4
 *
 * */
void PWM4_Init(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //
   // Setup TBCLK
   //
   EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm4Regs.TBPRD = (int)(EPWM_TIMER_TBPRD*12.5);       // Set timer period
   EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm4Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;   // Clock ratio to SYSCLKOUT
   EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV4;

   //
   // Setup shadow register load on ZERO
   //
   EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm4Regs.CMPA.bit.CMPA = (int)(EPWM_MIN*12.5);     // Set compare A value
   EPwm4Regs.CMPB.bit.CMPB = (int)(EPWM_MIN*12.5);     // Set Compare B value

   //
   // Set actions
   //
   EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
												 // up count

   EPwm4Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
												 // up count

   //
   // Interrupt where we will change the Compare Values
   //
   EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
   EPwm4Regs.ETSEL.bit.INTEN = 0;                // Disable INT
   EPwm4Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}


/*
 * update pluse width
 *
 * */
int PWM_Output_Update(unsigned char chx,int i)
{
	if(i<EPWM_MIN)
		i = EPWM_MIN;
	if(i>EPWM_MAX)
		i = EPWM_MAX;
	switch(chx)
	{
	case PWM_OUTPUT_CH1:
		EPwm1Regs.CMPA.bit.CMPA = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH2:
		EPwm1Regs.CMPB.bit.CMPB = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH3:
		EPwm2Regs.CMPA.bit.CMPA = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH4:
		EPwm2Regs.CMPB.bit.CMPB = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH5:
		EPwm3Regs.CMPA.bit.CMPA = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH6:
		EPwm3Regs.CMPB.bit.CMPB = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH7:
		EPwm4Regs.CMPA.bit.CMPA = (int)(i*12.5);return 1;
	case PWM_OUTPUT_CH8:
		EPwm4Regs.CMPB.bit.CMPB = (int)(i*12.5);return 1;
	default:
		return 0;
	}
}

