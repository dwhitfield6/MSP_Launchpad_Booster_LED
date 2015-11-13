/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 11/06/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the switching Capacitor filter MAX7404.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "FILTER.h"
#include "MISC.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "USER.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_Filter
 *
 * The function initializes the ADC module .
 *                                                                            */
/******************************************************************************/
void Init_Filter(void)
{
	FTR_SetCutoff(100);
	TB0CCTL1 |= CLLD1 + CLLD0; // TBxCLn loads when TBxR counts to TBxCLn
	TB0CCTL1 |= OUTMOD1 + OUTMOD0; // 011b = Set/reset
	TMR_ModeTimerB0(UP);
	FTR_Shutdown(OFF);
	FTR_CLK(ON);
}

/******************************************************************************/
/* FTR_CLK
 *
 * The function controls the Filter CLK frequency.
 *
 * Input: frequency of cutoff.
 * Output: N/A
 * Action: sets the clock frequency for the filter IC.
 *                                                                            */
/******************************************************************************/
void FTR_SetCutoff(unsigned long frequency)
{
	unsigned char status;
	unsigned long temp = 0;

	/* FC = FCLK / 100 */
	frequency *= 100;
	temp = (unsigned long) MSC_Round((double) SMCLK_Freq / (double)frequency);
	if(temp > 0xFFFF)
	{
		temp = 0xFFFF;
	}
	else if(temp <2)
	{
		temp = 2;
	}
	status = TMR_ModeTimerB0(OFF);
	TMR_SetTimerB0(temp);
	TB0CCR1 = temp >> 1; // for 50% duty cycle
	TMR_ResetTimerB0();
	if(status)
	{
		TMR_ModeTimerB0(status);
	}
}

/******************************************************************************/
/* FTR_CLK
 *
 * The function controls the Filter CLK signal.
 *
 * Input: ON or OFF
 * Output: N/A
 * Action: clocks the CLK signal or stops the clocking.
 *                                                                            */
/******************************************************************************/
void FTR_CLK(unsigned char state)
{
	if(state)
	{
		P1SEL1 &= ~Pin_FilterCLK; // pin 1.4 is set TB0.1
		P1SEL0 |= Pin_FilterCLK; // pin 1.4 is set TB0.1
	}
	else
	{
		P1OUT &= ~Pin_FilterCLK;
		P1SEL1 &= ~Pin_FilterCLK; // pin 1.4 is set to P1.4 GPIO
		P1SEL0 &= ~Pin_FilterCLK; // pin 1.4 is set to P1.4 GPIO
	}
}

/******************************************************************************/
/* FTR_Shutdown
 *
 * The function controls the Filter shutdown signal.
 *
 * Input: ON or OFF
 * Output: N/A
 * Action: puts the filter in shutdown or makes it active.
 *                                                                            */
/******************************************************************************/
void FTR_Shutdown(unsigned char state)
{
	if(state)
	{
		P3OUT &= ~Pin_FilterSHDN; // activate
	}
	else
	{
		P3OUT |= Pin_FilterSHDN; // shutdown
	}
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
