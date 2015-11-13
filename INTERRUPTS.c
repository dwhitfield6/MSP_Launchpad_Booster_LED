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
/* Interrupt Vector Options
 *
 * VECTOR NAMES:
 *
 * AES256_VECTOR
 * RTC_VECTOR
 * PORT4_VECTOR
 * PORT3_VECTOR
 * TIMER3_A1_VECTOR
 * TIMER3_A0_VECTOR
 * PORT2_VECTOR
 * TIMER2_A1_VECTOR
 * TIMER2_A0_VECTOR
 * PORT1_VECTOR
 * TIMER1_A1_VECTOR
 * TIMER1_A0_VECTOR
 * DMA_VECTOR
 * USCI_A1_VECTOR
 * TIMER0_A1_VECTOR
 * TIMER0_A0_VECTOR
 * ADC12_VECTOR
 * USCI_B0_VECTOR
 * USCI_A0_VECTOR
 * WDT_VECTOR
 * TIMER0_B1_VECTOR
 * TIMER0_B0_VECTOR
 * COMP_E_VECTOR
 * UNMI_VECTOR
 * SYSNMI_VECTOR
 * RESET_VECTOR
 *
 * Vector information found here:
 * C:\ti\ccsv6\ccs_base\msp430\include
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "ADC.h"
#include "BUTTON.h"
#include "LED.h"
#include "PROCESSING.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "TLC5940.h"
#include "TIMERS.h"
#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/******************************************************************************/
/* ADC interrupt
 *                                                                            */
/******************************************************************************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	if(ADC12IFGR0 & ADC12IFG0)
	{
		/* ADC12MEM0 is loaded with a conversion result */
		ADC_Ready= TRUE;
		ADC_Read(CurrentChannel); // read the ADC then change the channel for the next reading
	}
	ADC12IFGR0 = 0;
}

/******************************************************************************/
/* Port 1 interrupt (used for button 2)
 *                                                                            */
/******************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
	if(P1IFG & Pin_Button2)
	{
		/* Button 2 was pressed */
		LED_Green(ON);
		if(ProcessingWindow < DATA_BUFFER_SIZE)
		{
			ProcessingWindow += 5;
		}
		TMR_ResetTimerA0();
		TMR_ModeTimerA0(UP);
		BUT_Button2Interrupt(OFF);
		P1IFG &= ~Pin_Button2;
	}
	if(P1IFG & Pin_XERR)
	{
		/* TLC5940 detected an LOD or TEF */
		P1IFG &= ~Pin_XERR;
	}
}

/******************************************************************************/
/* Port 4 interrupt (used for button 1)
 *                                                                            */
/******************************************************************************/
#pragma vector=PORT4_VECTOR
__interrupt void Port4_ISR(void)
{
	if(P4IFG & Pin_Button1)
	{
		/* Button 1 was pressed */
		LED_Red(ON);
		if(ProcessingWindow > 5)
		{
			ProcessingWindow -= 5;
		}
		TMR_ResetTimerA1();
		TMR_ModeTimerA1(UP);
		BUT_Button1Interrupt(OFF);
		P4IFG &= ~Pin_Button1;
	}
}

/******************************************************************************/
/* Timer A0 interrupt (used for button 2)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	LED_Green(OFF);
	TMR_ModeTimerA0(OFF);
	P1IFG &= ~Pin_Button2;
	BUT_Button2Interrupt(ON);
	TA0CCTL0 &= ~CCIFG;
}

/******************************************************************************/
/* Timer A0 interrupt (timer overflow)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
	unsigned short source;

	source = TA0IV;
}

/******************************************************************************/
/* Timer A1 interrupt (used for button 1)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
	LED_Red(OFF);
	TMR_ModeTimerA1(OFF);
	P4IFG &= ~Pin_Button1;
	BUT_Button1Interrupt(ON);
	TA1CCTL0 &= ~CCIFG;
}

/******************************************************************************/
/* Timer A1 interrupt (timer overflow)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
	unsigned short source;

	source = TA1IV;
}

/******************************************************************************/
/* Timer A2 interrupt (used for updating the PWM driver)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR(void)
{
	TLC_UpdateLEDs();
	TA2CCTL0 &= ~CCIFG;
}

/******************************************************************************/
/* Timer A2 interrupt (timer overflow)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER2_A1_VECTOR
__interrupt void Timer2_A1_ISR(void)
{
	unsigned short source;

	source = TA2IV;
}

/******************************************************************************/
/* Timer B0 interrupt (used for the filter clock)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
	TB0CCTL1 &= ~CCIFG;
}

/******************************************************************************/
/* Timer B0 interrupt (timer overflow)
 *                                                                            */
/******************************************************************************/
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void)
{
	unsigned short source;

	source = TB0IV;
}

/******************************************************************************/
/* eUSCIB0 interrupt (SPI to communicate with TLC5940)
 *                                                                            */
/******************************************************************************/
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
	if(UCB0STATW & UCFE)
	{
		/* Framing error flag */
		UCB0STATW &= ~UCFE;
	}
	if(UCB0STATW & UCOE)
	{
		/* Overrun error flag */
		UCB0STATW &= ~UCOE;
	}
	if((UCB0IFG & UCTXIFG) && (UCB0IE & UCTXIE))
	{
		/* Transmit interrupt flag. UCTXIFG is set when UCxxTXBUF empty */
		UCB0IFG &= ~UCTXIFG;
	}
	if((UCB0IFG & UCRXIFG) && (UCB0IE & UCRXIE))
	{
		/* Receive interrupt flag. UCRXIFG is set when UCxxRXBUF has received a complete character */
		SPI_Receive_Available = TRUE;
		UCB0IFG &= ~UCRXIFG;
	}
}

/******************************************************************************/
/* eUSCIA1 interrupt (UART)
 *                                                                            */
/******************************************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	unsigned char data;

	if((UCA1IE & UCRXIE) && (UCA1IFG & UCRXIFG))
	{
		/* character received */
		if(UCA1STATW & UCRXERR)
		{
			/* receive error */
			if(UCA1STATW & UCFE)
			{
				/* Framing error flag */
				UCA1STATW &= ~UCFE;
			}
			if(UCA1STATW & UCPE)
			{
				/* Parity error flag */
				UCA1STATW &= ~UCFE;
			}
			if(UCA1STATW & UCOE)
			{
				/* Overrun error flag */
				UCA1STATW &= ~UCOE;
			}
			if(UCA1STATW & UCBRK)
			{
				/* Break detect flag */
				BreakReceived = TRUE;
				data = UCA1RXBUF;
				UCA1STATW &= ~UCBRK;
			}
		}
		else
		{
			data = UCA1RXBUF;
			UART_SendChar(data);
			if(RX_Buffer_Place < RX_BUFFER_SIZE)
			{
				RX_Buffer[RX_Buffer_Place] = data;
				RX_Buffer_Place++;
			}
			if(data == '\r')
			{
				UART_SendChar('\n');
			}
		}
	}
	else if((UCA1IE & UCTXIE) && (UCA1IFG & UCTXIFG))
	{
		/* Transmit interrupt flag. UCTXIFG is set when UCAxTXBUF empty */
        if(TX_Buffer_Place_Remove != TX_Buffer_Place_Add)
        {
        	UCA1TXBUF = TX_Buffer[TX_Buffer_Place_Remove];
        	TX_Buffer_Place_Remove++;
            if(TX_Buffer_Place_Remove >= TX_BUFFER_SIZE)
            {
            	TX_Buffer_Place_Remove = 0;
            }
        }
        else
        {
        	UART_TransmitInterrupt(OFF);
        }
	}
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
