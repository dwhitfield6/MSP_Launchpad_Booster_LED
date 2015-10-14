/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/12/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the TLC5940 LED driver.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>
#include <math.h>

#include "MISC.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "TLC5940.h"
#include "USER.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
unsigned short LEDs[16];
unsigned char FadeDirection = FORWARD;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_TLC5940
 *
 * The function initializes the pins used to communicate with the TLC5940.
 *                                                                            */
/******************************************************************************/
void Init_TLC5940(void)
{
	TLC_Blank(ON);
	TLC_XLAT(OFF);
	TLC_GSCLK(OFF);
	MSC_CleanBuffer(LEDs,sizeof(LEDs));
	P1IES |= Pin_XERR; // flag is set with a high-to-low transition
	P1IFG &= ~Pin_XERR; // clear flag
	TLC_XERRInterrupt(ON);
	TMR_ModeTimerA2(UP);
}

/******************************************************************************/
/* TLC_GSCLK
 *
 * The function controls the GSCLK signal.
 *
 * Input: ON or OFF
 * Output: N/A
 * Action: clocks the GS signal or stops the clocking.
 *                                                                            */
/******************************************************************************/
void TLC_GSCLK(unsigned char state)
{
	if(state)
	{
		P3SEL1 |= Pin_PWM_CLK; // pin 3.4 is set SMCLK
		P3SEL0 &= ~Pin_PWM_CLK; // pin 3.4 is set SMCLK
	}
	else
	{
		P3OUT &= ~Pin_PWM_CLK;
		P3SEL1 &= ~Pin_PWM_CLK; // pin 3.4 is set SMCLK
		P3SEL0 &= ~Pin_PWM_CLK; // pin 3.4 is set SMCLK
	}
}

/******************************************************************************/
/* TLC_UpdateLEDs
 *
 * The function writes the grayscale data to the TLC5940.
 *
 * Input: N/A
 * Output: N/A
 * Action: writes the brighness of the LEDS over the SPI bus.
 *                                                                            */
/******************************************************************************/
void TLC_UpdateLEDs(void)
{
	short i;
	unsigned short data;

	TLC_GSCLK(OFF);
	NOP();
	NOP();

	/*~~~~~~~~~ send to TLC5940 ~~~~~~~~~*/
	for(i=15;i>0;i-=2)
	{
		/* LED x MSB */
		data = (LEDs[i] & 0x0FF0) >> 4;
		SPI_WriteB(0,data);
		/* LED x LSB and LED x-1 MSB*/
		data = ((LEDs[i] & 0x000F) << 4) | ((LEDs[i-1] & 0x0F00) >> 8);
		SPI_WriteB(0,data);
		/* LED x-1 LSB*/
		data = (LEDs[i-1] & 0x00FF);
		SPI_WriteB(0,data);
	}

	TLC_Blank(ON);
	NOP();
	NOP();
	TLC_XLAT(ON);
	NOP();
	NOP();
	TLC_XLAT(OFF);
	NOP();
	NOP();
	TLC_Blank(OFF);
	NOP();
	NOP();
	TLC_GSCLK(ON);
}

/******************************************************************************/
/* TLC_SetLEDsLinear
 *
 * The function sets the brighness of the LEDs based on 1 value.
 *
 * Input: brightness(0 to 8192), direction (FORWARD or BACKWARD)
 * Output: N/A
 * Action: controls the brighness of the LEDS.
 *                                                                            */
/******************************************************************************/
void TLC_SetLEDsLinear(short value, unsigned char direction)
{
	short temp[16];
	unsigned char i;

	if(direction == FORWARD)
	{
		temp[0] = value;
		if(temp[0] < 0)
		{
			temp[0] = 0;
		}
		if(temp[0] >0x0FFF)
		{
			temp[0] = 0x0FFF;
		}
		for(i=1;i<16;i++)
		{
			temp[i] = value - (2 * i); // 273 for full scale
			if(temp[i] >0x0FFF)
			{
				temp[i] = 0x0FFF;
			}
			if(temp[i] <= 0)
			{
				temp[i] = 0;
			}
		}
	}
	else
	{
		temp[15] = value;
		if(temp[15] < 0)
		{
			temp[15] = 0;
		}
		if(temp[15] >0x0FFF)
		{
			temp[15] = 0x0FFF;
		}
		for(i=1;i<16;i++)
		{
			temp[15 - i] = value - (10 * i); // 273 for full scale
			if(temp[15 - i] >0x0FFF)
			{
				temp[15 - i] = 0x0FFF;
			}
			if(temp[15 - i] <= 0)
			{
				temp[15 - i] = 0;
			}
		}
	}
	TLC_SetLEDs((unsigned short*)temp);
}

/******************************************************************************/
/* TLC_SetLEDs
 *
 * The function sets the brighness of the LEDs.
 *
 * Input: pointer to an array holding the LED brighness values.
 * Output: N/A
 * Action: controls the brighness of the LEDS.
 *                                                                            */
/******************************************************************************/
void TLC_SetLEDs(unsigned short* temp)
{
	unsigned short* global = LEDs;
	short i;

	/* copy the array to global LEDs */
	for(i=0;i<NUMBER_LEDS;i++)
	{
		*global = *temp;
		global++;
		temp++;
	}
}

/******************************************************************************/
/* TLC_Blank
 *
 * The function is used to Blank all outputs. When "ON", all outputs are
 *  forced OFF. GS counter is also reset. When "OFF", outputs are controlled
 *   by grayscale PWM control.
 *
 * Input: on or off
 * Output: N/A
 * Action: controls the Blank pin on the TLC5940
 *                                                                            */
/******************************************************************************/
void TLC_Blank(unsigned char state)
{
	if(state)
	{
		P4OUT |= Pin_Blank;
	}
	else
	{
		P4OUT &= ~Pin_Blank;
	}
}

/******************************************************************************/
/* TLC_XLAT
 *
 * The function is used to control the Level triggered latch signal. When "ON"
 *  the TLC5940 writes data from the input shift register to the GS register.
 *  When "OFF" the data in GS is held constant.
 *
 * Input: on or off
 * Output: N/A
 * Action: controls the XLAT pin on the TLC5940
 *                                                                            */
/******************************************************************************/
void TLC_XLAT(unsigned char state)
{
	if(state)
	{
		P1OUT |= Pin_XLAT;
	}
	else
	{
		P1OUT &= ~Pin_XLAT;
	}
}

/******************************************************************************/
/* TLC_XERRInterrupt
 *
 * The function controls the interrupt for the XERR signal.
 *
 * Input: state (ON or OFF)
 * Output: the previous state of the port interrupt
 * Action: controls the port interrupt
 *                                                                            */
/******************************************************************************/
unsigned char TLC_XERRInterrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(P1IE & Pin_XERR)
	{
		status = TRUE;
	}

	if(state)
	{
		P1IE |= Pin_XERR; // port interrrrupt enabled
	}
	else
	{
		P1IE &= ~Pin_XERR; // port interrrrupt disabled
	}
	return status;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
