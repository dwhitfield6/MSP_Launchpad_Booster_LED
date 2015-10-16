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
/* Contains functions for UART.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>
#include <float.h>

#include "MISC.h"
#include "SYSTEM.h"
#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
unsigned char RX_Buffer[RX_BUFFER_SIZE];
unsigned char TX_Buffer[TX_BUFFER_SIZE];
short RX_Buffer_Place = 0;
short TX_Buffer_Place_Remove = 0;
short TX_Buffer_Place_Add = 0;
unsigned char BreakReceived = FALSE;

/******************************************************************************/
/* UCBRSx lookup table
 *
 * This is lookup table to set the UCBRSx bits.
 *                                                                            */
/******************************************************************************/
const double Table_UCBRSx[36][2] =
{
		{0.0000, 0.0},
		{0.0529, 1.0},
		{0.0715, 2.0},
		{0.0835, 4.0},
		{0.1001, 8.0},
		{0.1252, 16.0},
		{0.1430, 32.0},
		{0.1670, 17.0},
		{0.2147, 33.0},
		{0.2224, 34.0},
		{0.2503, 68.0},
		{0.3000, 37.0},
		{0.3335, 73.0},
		{0.3575, 74.0},
		{0.3753, 82.0},
		{0.4003, 146.0},
		{0.4286, 83.0},
		{0.4378, 85.0},
		{0.5002, 170.0},
		{0.5715, 107.0},
		{0.6003, 173.0},
		{0.6254, 181.0},
		{0.6432, 182.0},
		{0.6667, 214.0},
		{0.7001, 183.0},
		{0.7147, 187.0},
		{0.7503, 221.0},
		{0.7861, 237.0},
		{0.8004, 238.0},
		{0.8333, 191.0},
		{0.8464, 223.0},
		{0.8572, 239.0},
		{0.8751, 247.0},
		{0.9004, 251.0},
		{0.9170, 253.0},
		{0.9288, 254.0}
};

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_UART
 *
 * The function initializes the UART attached to eUSCI_A1.
 *                                                                            */
/******************************************************************************/
void Init_UART(void)
{
	/* Pin to TX function */
	P2SEL1 |= Pin_TX;
	P2SEL0 &= ~Pin_TX;

	/* Pin to RX function */
	P2SEL1 |= Pin_RX;
	P2SEL0 &= ~Pin_RX;

	UART_CleanBufferRX();
	UART_CleanBufferTX();
	UART_TransmitInterrupt(OFF);
	UART_ReceiveInterrupt(OFF);
	UART_Module(ON);
	UART_Parameters(9600, NO, NO);
	UART_Sleep(OFF);
	UART_ReceiveInterrupt(ON);
}

/******************************************************************************/
/* UART_Sleep
 *
 * The function controls the receive sleep mode.
 *
 * Input: on or off
 * Output: previous value of sleep mode
 * Action: puts the UART receive to sleep or takes it out of sleep
 *                                                                            */
/******************************************************************************/
unsigned char UART_Sleep(unsigned char state)
{
	unsigned char status = TRUE;

	if(UCA1CTLW0 & UCDORM)
	{
		status = FALSE;
	}

	if(state)
	{
		UCA1CTLW0 |= UCDORM; // Not dormant. All received characters set UCRXIFG.
	}
	else
	{
		UCA1CTLW0 &= ~UCDORM; // 1b = Dormant. Only characters that are preceded by an idle-line or with address bit set UCRXIFG
	}
	return status;
}

/******************************************************************************/
/* UART_Module
 *
 * The function controls the UART module.
 *
 * Input: on or off
 * Output: previous status of module
 * Action: puts the module into operation or hold in reset
 *                                                                            */
/******************************************************************************/
unsigned char UART_Module(unsigned char state)
{
	unsigned char status = TRUE;

	if(UCA1CTLW0 & UCSWRST)
	{
		status = FALSE;
	}

	if(state)
	{
		UCA1CTLW0 &= ~UCSWRST; // eUSCI_A reset released for operation.
	}
	else
	{
		UCA1CTLW0 |= UCSWRST; // eUSCI_A logic held in reset state.
	}
	return status;
}

/******************************************************************************/
/* UART_Parameters
 *
 * The function sets up the parameters for the UART.
 *
 * Input: baud rate (2400 to 115200), parity bit (ODD, EVEN, or NO), stop bit
 *  (1 or 2)
 * Output: N/A
 * Action: sets up the UART parameters
 *                                                                            */
/******************************************************************************/
void UART_Parameters(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
	short temp1,temp2,temp3;
	long double _N;
	double _FracN;

	if(Parity != NO)
	{
		UCA1CTLW0 |= UCPEN; // Parity enabled. Parity bit is generated (UCAxTXD) and expected (UCAxRXD)
		if(Parity == ODD)
		{
			UCA1CTLW0 &= ~UCPAR; // Odd parity
		}
		else
		{
			UCA1CTLW0 |= UCPAR; // Even parity
		}
	}
	else
	{
		UCA1CTLW0 &= ~UCPEN; // Parity enabled. Parity bit is generated (UCAxTXD) and expected (UCAxRXD)
	}
	if(Stop == 1)
	{
		UCA1CTLW0 &= ~UCSPB; // One stop bit
	}
	else
	{
		UCA1CTLW0 |= UCSPB; // Two stop bits
	}

	UCA1CTLW0 &= ~(UCMODE1 | UCMODE0);  // 00b = UART mode

	/* eUSCI_A clock source select. */
	UCA1CTLW0 |= UCSSEL1;  // 10b = SMCLK
	UCA1CTLW0 &= ~UCSSEL0; // 10b = SMCLK

	UCA1CTLW1 |= (UCGLIT1 | UCGLIT0); // 11b = Deglitch time approximately 200 ns

	_N = (double) SMCLK_Freq / (double) Baud;
	temp1 = (short) MSC_Round(_N);
	_FracN = MSC_ABS(_N - (double) temp1);
	if(temp1 <= 16)
	{
		UCA1MCTLW &= ~UCOS16; // oversampling off
		UCA1BRW = temp1;
	}
	else
	{
		UCA1MCTLW |= UCOS16; // oversampling on

		/* UCBRx */
		temp1 /= 16;
		UCA1BRW = temp1;

		/* UCBRFx */
		UCA1MCTLW &= ~(UCBRF3 | UCBRF2 | UCBRF1 | UCBRF0);
		temp2 = (short) (MSC_Round (((_N / 16.0) - (double)temp1) * 16.0));
		UCA1MCTLW |= (temp2 << 4);

		/* UCBRSx */
		UCA1MCTLW &= ~(UCBRS7 | UCBRS6 | UCBRS5 | UCBRS4 | UCBRS3 | UCBRS2 | UCBRS1 | UCBRS0);
		temp3 = UART_LookupUCBRSx(_FracN);
		UCA1MCTLW |= (temp3 << 8);
	}
}

/******************************************************************************/
/* UART_LookupUCBRSx
 *
 * The function chooses the value of UCBRSx based on a lookup table.
 *
 * Input: value of the fractional part of N (baud rate)
 * Output: UCBRSx value
 * Action: looks up the value of UCBRSx from a lookup table
 *                                                                            */
/******************************************************************************/
unsigned short UART_LookupUCBRSx(double value)
{
	unsigned char i;
	short tempS;

	if(value >= Table_UCBRSx[35][0])
	{
		tempS = (short) Table_UCBRSx[35][1];
	}
	else
	{
		for (i=0;i<35; i++)
		{
			if(value >= Table_UCBRSx[i][0] && value < Table_UCBRSx[i+1][0])
			{
				tempS = (short) Table_UCBRSx[i][1];
				break;
			}
		}
	}
	return tempS;
}

/******************************************************************************/
/* UART_TransmitterInterrupt
 *
 * The function controls the transmit interrupt.
 *
 * Input: on or off or GET
 * Output: previous status of transmit interrupt
 * Action: enables or disables the tranmit interrupt
 *                                                                            */
/******************************************************************************/
unsigned char UART_TransmitInterrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(UCA1IE & UCTXIE)
	{
		status = TRUE;
	}

	if(state!= GET)
	{
		if(state)
		{
			UCA1IE |= UCTXIE; // Transmit interrupt enabled
		}
		else
		{
			UCA1IE &= ~UCTXIE; // Transmit interrupt enabled
		}
	}

	return status;
}

/******************************************************************************/
/* UART_ReceiveInterrupt
 *
 * The function controls the receive interrupt.
 *
 * Input: on or off or GET
 * Output: previous status of receive interrupt
 * Action: enables or disables the receive interrupt
 *                                                                            */
/******************************************************************************/
unsigned char UART_ReceiveInterrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(UCA1IE & UCRXIE)
	{
		status = TRUE;
	}

	if(state!= GET)
	{
		if(state)
		{
			UCA1IE |= UCRXIE; // Receive interrupt enabled
		}
		else
		{
			UCA1IE &= ~UCRXIE; // Receive interrupt enabled
		}
	}
	return status;
}

/******************************************************************************/
/* UART_SendChar
 *
 * The function sends a character over the UART port.
 *
 * Input: character
 * Output: N/A
 * Action: puts the character into the TX buffer to be sent over UART
 *                                                                            */
/******************************************************************************/
void UART_SendChar(unsigned char data)
{
	TX_Buffer[TX_Buffer_Place_Add] = data;
	TX_Buffer_Place_Add++;
    if(TX_Buffer_Place_Add >= TX_BUFFER_SIZE)
    {
    	TX_Buffer_Place_Add = 0;
    }
    UART_TransmitInterrupt(ON);
}

/******************************************************************************/
/* UART_SendString
 *
 * The function sends a string over the UART port.
 *
 * Input: pointer to a string
 * Output: N/A
 * Action: puts the string into the TX buffer to be sent over UART
 *                                                                            */
/******************************************************************************/
void UART_SendString(unsigned char* data)
{
	while(*data != 0)
	{
		UART_SendChar(*data);
		data++;
	}
}

/******************************************************************************/
/* UART_SendStringCRLN
 *
 * The function sends a string over the UART port.
 *
 * Input: pointer to a string
 * Output: N/A
 * Action: puts the string into the TX buffer followed by a '\r\n' to be
 *  sent over UART
 *                                                                            */
/******************************************************************************/
void UART_SendStringCRLN(unsigned char* data)
{
	UART_SendString(data);
	UART_SendString(CRLN);
}

/******************************************************************************/
/* UART_CleanBufferRX
 *
 * The function cleans the RX buffer.
 *
 * Input: N/A
 * Output: N/A
 * Action: sets all bytes in the RX buffer to 0 and sets the buffer place to 0
 *                                                                            */
/******************************************************************************/
void UART_CleanBufferRX(void)
{
	MSC_CleanBuffer(RX_Buffer,RX_BUFFER_SIZE);
	RX_Buffer_Place = 0;
}

/******************************************************************************/
/* UART_CleanBufferTX
 *
 * The function cleans the TX buffer.
 *
 * Input: N/A
 * Output: N/A
 * Action: sets all bytes in the TX buffer to 0 and sets the buffer place to 0
 *                                                                            */
/******************************************************************************/
void UART_CleanBufferTX(void)
{
	MSC_CleanBuffer(TX_Buffer,TX_BUFFER_SIZE);
	TX_Buffer_Place_Remove = 0;
	TX_Buffer_Place_Add = 0;
}

/******************************************************************************/
/* UART_PrintBanner
 *
 * The function prints the banner over UART.
 *
 * Input: N/A
 * Output: N/A
 * Action: prints the startup banner
 *                                                                            */
/******************************************************************************/
void UART_PrintBanner(void)
{
	UART_SendStringCRLN("MSP430FR5969 Launchpad with PWM LED daughter card");
	UART_SendStringCRLN("By: David Whitfield");
	UART_SendString("Firmware Version: ");
    UART_SendString(CPU_VERSION);
    UART_SendChar('.');
    UART_SendString(CPU_REVISION);
    if(CPU_ALPHA != 0)
    {
        UART_SendString(CPU_ALPHA);
    }
    if(CPU_BRANCH != 0)
    {
    	UART_SendChar('_');
    	UART_SendString(CPU_BRANCH);
    }
    UART_SendStringCRLN(CRLN);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
