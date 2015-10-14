/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/12/15     4.0_DW0a    Initial project make (branched from
 * 							  "MSP_Launchpad_MSP430FR5969_Test".
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains main function.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "LED.h"
#include "MISC.h"
#include "SYSTEM.h"
#include "TLC5940.h"
#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Global Variable                                                            */
/******************************************************************************/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int main (void)
{
	short temp = 0;

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    /* Initialize */
    SYS_ConfigureOscillator();
    Init_App();
    Init_System();
    UART_PrintBanner();

    LED_DisplayShow();

    while(1)
    {
    	TLC_SetLEDsLinear(temp, FadeDirection);
    	temp++;
    	if(temp > 200)
		{
			temp = 0;
		}
    	MSC_DelayUS(20000);
    }
	return 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/