/*
 * Gpio.c
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */
#include "Gpio.h"
/*
 * Initialize all of the GPIO
 * GpioCtrlRegs:
 * 				GPyCR:		GPIO y Lock Commit Register(0-31)
 * 							1:Lock changes to the bit in GPyLOCK register which controls the same pin
 * 							0:Bit in GPyLOCK register which control the same pin can be changed(DEFAULT)
 * 				GPyCSEL1:	GPIO y Core Select Register(0-7)
 * 							Select which master's GPIODAT/SET/CLEAR/TOGGLE registers control this GPIO pin
 * 							xx00:CPU1 selected(DEFAULT)
 * 							xx01:CPU1.CLA1 selected
 * 							xx10:CPU2 selected
 * 							xx11:CPU2.CLA1 selected
 * 				GPyCSEL2:	GPIO y Core Select Register(8-15)
 * 							Select which master's GPIODAT/SET/CLEAR/TOGGLE registers control this GPIO pin
 * 							xx00:CPU1 selected(DEFAULT)
 * 							xx01:CPU1.CLA1 selected
 * 							xx10:CPU2 selected
 * 							xx11:CPU2.CLA1 selected
 * 				GPyCSEL3:	GPIO y Core Select Register(16-23)
 * 							Select which master's GPIODAT/SET/CLEAR/TOGGLE registers control this GPIO pin
 * 							xx00:CPU1 selected(DEFAULT)
 * 							xx01:CPU1.CLA1 selected
 * 							xx10:CPU2 selected
 * 							xx11:CPU2.CLA1 selected
 * 				GPyCSEL4:	GPIO y Core Select Register(24-31)
 * 							Select which master's GPIODAT/SET/CLEAR/TOGGLE registers control this GPIO pin
 * 							xx00:CPU1 selected(DEFAULT)
 * 							xx01:CPU1.CLA1 selected
 * 							xx10:CPU2 selected
 * 							xx11:CPU2.CLA1 selected
 * 				GPyCTRL:	GPIO y Qualification Sampling Period Control(0-31)
 * 							0x00:QUyLPRDx = PLLSYSOUT(DEFAULT)
 * 				GPyDIR:		GPIO y Direction Register(0-31)
 * 							0:input(DEFAULT)
 * 							1:output
 * 				GPyGMUX1:	GPIO y Peripheral Group Mux(0-15)
 * 							NOTE:Must to be setted prior to GPyMUX1
 * 							00:GPIO(DEFAULT)
 * 				GPyGMUX2:	GPIO y Peripheral Group Mux(16-31)
 * 							NOTE:Must to be setted prior to GPyMUX2
 * 							00:GPIO(DEFAULT)
 * 				GPyMUX1:	GPIO y Mux Register(0-15)
 * 							00:GPIO(DEFAULT)
 * 				GPyMUX2:	GPIO y Mux Register(16-31)
 * 							00:GPIO(DEFAULT)
 * 				GPyINV:		GPIO y Input Polority Invert Registers(0-31)
 * 							0:select non-inverted GPIO input(DEFAULT)
 * 							1:select inverted GPIO input
 * 				GPyLOCK:	GPIO y Lock Configuration Register(0-31)
 * 							0:Bits which control the same pin can be changed(DEFAULT)
 * 							1:Locks changes to the bits which control the same pin
 * 				GPyODR:		GPIO y Open Drain Output Register(0-31)
 * 							Selects between open-drain and normal output for the GPIO pin
 * 							0:Normal Output(DEFAULT)
 * 							1:Open Drain Output
 * 				GPyPUD:		GPIO y Pull Up Disable Register(0-31)
 * 							0:Enable the Pull-Up
 * 							1:Disable the Pull-Up(DEFAULT)
 * 				GPyQSEL1:	GPIO y Qualifier Select 1 Register(0-15)
 * 							Input qualification type:
 * 							00:Sync(DEFAULT)
 * 							01:Qualification (3 samples)
 * 							10:Qualification (6 samples)
 * 							11:Async(no Sync or Qualification)
 * 				GPyQSEL2:	GPIO y Qualifier Select 2 Register(16-31)
 * 							Input qualification type:
 * 							00:Sync(DEFAULT)
 * 							01:Qualification (3 samples)
 * 							10:Qualification (6 samples)
 * 							11:Async(no Sync or Qualification)
 */
void Gpio_Init(void)
{
	InitGpio();   //Set as default: Disable pin lock;Fill all registers with zeros;
	Pull_Up_All_Pins();
	EALLOW;

	//GPIO-0   EPWM1A
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A

	//GPIO-1   EPWM1B
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

	//GPIO-2   EPWM2A
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A

	//GPIO-3   EPWM2B
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B

	//GPIO-4   EPWM3A
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A

	//GPIO-5   EPWM3B
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

	//GPIO-6   EPWM4A
	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A

	//GPIO-7   EPWM4B
	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B

	//GPIO18		SCITXDB
	//MUX:
	GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;

	//GPIO19		SCIRXDB
	//MUX
	GpioCtrlRegs.GPAGMUX2.bit.GPIO28 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO28  = 2;
	GpioCtrlRegs.GPADIR.bit.GPIO28   = 0;
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;

	//GPIO28		SCIRXDA
	//MUX
	GpioCtrlRegs.GPAGMUX2.bit.GPIO28 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO28  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO28   = 0;
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;
	//GPIO29		SCITXDA
	//MUX
	GpioCtrlRegs.GPAGMUX2.bit.GPIO29 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO29  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO28   = 1;

	//GPIO31		LED2 on controlCARD
	//MUX:
	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;

	//GPIO32		I2CA-SDA
	//MUX:
	GpioCtrlRegs.GPBGMUX1.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO32	 = 1;
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;

	//GPIO33		I2CA-SCL
	//MUX:
	GpioCtrlRegs.GPBGMUX1.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO33	 = 1;
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;
	GpioCtrlRegs.GPBODR.bit.GPIO33 = 1;

	//GPIO34		LED3 on controlCARD
	//MUX:
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;

	//GPIO73  XCLKOUT
	//MUX:0->GPIO-73 2->EM1D11 3->XCLKOUT 5->CANRXB 6->SCIRXDC
//	GPIO_SetupPinMux(73, GPIO_MUX_CPU1, 3);
//	GPIO_SetupPinOptions(73, GPIO_OUTPUT, GPIO_PUSHPULL);

	//GPIO73  RC_CH1/XCLKOUT
	InputXbarRegs.INPUT7SELECT = 73;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO73 = 3;

//	GpioCtrlRegs.GPCPUD.bit.GPIO73 = 1;

	//GPIO74  RC_CH2
	InputXbarRegs.INPUT8SELECT = 74;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO74 = 3;

	//GPIO75  RC_CH3
	InputXbarRegs.INPUT9SELECT = 75;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO75 = 3;

	//GPIO76  RC_CH4
	InputXbarRegs.INPUT10SELECT = 76;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO76 = 3;

	//GPIO77  RC_CH5
	InputXbarRegs.INPUT11SELECT = 77;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO77 = 3;

	//GPIO78  RC_CH6
	InputXbarRegs.INPUT12SELECT = 78;
	GpioCtrlRegs.GPCQSEL1.bit.GPIO78 = 3;

	EDIS;
	Lock_All_Pins();//Prevent to be changed
}

/*
 * Lock all pins to prevent the GPIO's configuration being changed
 */
void Lock_All_Pins(void)
{
    EALLOW;
    GpioCtrlRegs.GPALOCK.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPBLOCK.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPCLOCK.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPDLOCK.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPELOCK.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPFLOCK.all = 0xFFFFFFFF;
    EDIS;
}

/*
 * Pull up all the pins
 */
void Pull_Up_All_Pins(void)
{
	EALLOW;
	GpioCtrlRegs.GPAPUD.all = 0x00000000;
	GpioCtrlRegs.GPBPUD.all = 0x00000000;
	GpioCtrlRegs.GPCPUD.all = 0x00000000;
	GpioCtrlRegs.GPDPUD.all = 0x00000000;
	GpioCtrlRegs.GPEPUD.all = 0x00000000;
	GpioCtrlRegs.GPFPUD.all = 0x00000000;
	EDIS;
}
