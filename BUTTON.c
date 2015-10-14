/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/04/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the pushbutton switches.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "BUTTON.h"
#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_Buttons
 *
 * The function initializes the pushbutton switches.
 *                                                                            */
/******************************************************************************/
void Init_Buttons(void)
{
	P4OUT |= Pin_Button1; // pull-up
	P1OUT |= Pin_Button2; // pull-up
	P4REN |= Pin_Button1; // Enable pull-up resistor
	P1REN |= Pin_Button2; // Enable pull-up resistor
	P4IES |= Pin_Button1; // flag is set with a high-to-low transition
	P1IES |= Pin_Button2; // flag is set with a high-to-low transition
	P4IFG &= ~Pin_Button1; // clear flag
	P1IFG &= ~Pin_Button2; // clear flag
	BUT_Button1Interrupt(ON);
	BUT_Button2Interrupt(ON);
}

/******************************************************************************/
/* BUT_Button1Interrupt
 *
 * The function controls the interrupt for pushbutton 1 on the launchpad.
 *
 * Input: state (ON or OFF)
 * Output: the previous state of the port interrupt
 * Action: controls the port interrupt
 *                                                                            */
/******************************************************************************/
unsigned char BUT_Button1Interrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(P4IE & Pin_Button1)
	{
		status = TRUE;
	}

	if(state)
	{
		P4IE |= Pin_Button1; // port interrrrupt enabled
	}
	else
	{
		P4IE &= ~Pin_Button1; // port interrrrupt disabled
	}
	return status;
}

/******************************************************************************/
/* BUT_Button2Interrupt
 *
 * The function controls the interrupt for pushbutton 2 on the launchpad.
 *
 * Input: state (ON or OFF)
 * Output: the previous state of the port interrupt
 * Action: controls the port interrupt
 *                                                                            */
/******************************************************************************/
unsigned char BUT_Button2Interrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(P1IE & Pin_Button2)
	{
		status = TRUE;
	}

	if(state)
	{
		P1IE |= Pin_Button2; // port interrrrupt enabled
	}
	else
	{
		P1IE &= ~Pin_Button2; // port interrrrupt disabled
	}
	return status;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
