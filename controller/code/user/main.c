/*
 * main.c
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */
#include "main.h"
extern unsigned int RC_PWM_Value[RC_CH_Num];
void Setup(void);
void Loop_500Hz(void);
void Loop_100Hz(void);
void Loop_10Hz(void);
int count = 0;
void main()
{

	Setup();
	while(1)
	{

	}
}

void Setup(void)
{
	InitSysCtrl();			//Initialize system control
	EALLOW;
	ClkCfgRegs.CLKSRCCTL3.bit.XCLKOUTSEL = 0;    //XCLKOUT = PLLSYSCLK (Default on reset)
	ClkCfgRegs.XCLKOUTDIVSEL.bit.XCLKOUTDIV = 0; //XCLKOUT = SYSCLKOUT
	EDIS;
	Gpio_Init();			//Initialize all of the GPIO
	DINT;                   //Disable global interrupt at CPU level
	InitPieCtrl();          //Initialize PIE control
	IER = 0x0000;			//Disable all interrupts at CPU level
	IFR = 0x0000;			//Clear all interrupt flags at CPU level
	InitPieVectTable();		//Initialize PIE vector table with default ISR address
							//Enable the PIE Vector Table
	CpuTimer0_Init();
	CpuTimer1_Init();
	CpuTimer2_Init();
	Scia_Init();
	Scib_Init();
	I2ca_Init();
	MPU_Init();
	CAP_Init();
	PWM_Init();
	DELAY_US(10000000);
	Radar_Init();
	EINT;
	ERTM;
	StartCpuTimer0();
	StartCpuTimer1();
	StartCpuTimer2();
}

void Loop_500Hz(void)
{
    long int a = ReadCpuTimer2Counter();
    MPU_Get_Raw_Data();
    IMU_Update();
    //PID_Control();
    pidRateCtl();
    Motor_PWM_Update();
}


void Loop_100Hz(void)
{
//	ANO_OSC_MPU();
	ANO_OSC_IMU3();
//	ANO_OSC_Motor_Output();
}
void Loop_10Hz(void)
{
	ANO_OSC_RC();
	Led3_Toggle();
//	Led2_Toggle();
}


