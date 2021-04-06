/*
 * Led.c
 *
 *  Created on: 2016Äê11ÔÂ22ÈÕ
 *      Author: gtc
 */
#include "led.h"

/*
 * Turn on the led2 on controlCARD
 */
void Led2_On(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
}

/*
 * Turn off the led2
 */
void Led2_Off(void)
{
	GpioDataRegs.GPASET.bit.GPIO31 = 1;
}

/*
 * Toggle the led2
 */
void Led2_Toggle(void)
{
	GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;
}

/*
 * Turn on the led3 on controlCARD
 */
void Led3_On(void)
{
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
}

/*
 * Turn off the led3
 */
void Led3_Off(void)
{
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
}

/*
 * Toggle the led2
 */
void Led3_Toggle(void)
{
	GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
}
