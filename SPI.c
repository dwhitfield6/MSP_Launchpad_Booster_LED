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
/* Contains functions for the SPI module.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "MISC.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "USER.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
unsigned char SPI_Receive_Available = FALSE;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_SPI
 *
 * The function initializes the SPI module.
 *                                                                            */
/******************************************************************************/
void Init_SPI(void)
{
	/* UCB0CLK connected to Launchpad pins */

	/* Set function of SCLK */
	P2SEL1 |= Pin_SCLK;
	P2SEL0 &= ~Pin_SCLK;

	/* Set function of MISO */
	P1SEL1 |= Pin_SOUT_MISO;
	P1SEL0 &= ~Pin_SOUT_MISO;

	/* Set function of MOSI */
	P1SEL1 |= Pin_SIN_MOSI;
	P1SEL0 &= ~Pin_SIN_MOSI;

	UCB0CTLW0 |= UCMSB; // MSB first
	SPI_ModuleInterruptsB(0,OFF,OFF);
	SPI_ModuleB(0, ON);
	SPI_ModuleInterruptsB(0,OFF,OFF);
	SPI_SetParametersB(0,MASTER, 0, 4000000);
}

/******************************************************************************/
/* SPI_SetParametersB
 *
 * The function sets the parameters of eUSCI_Bx.
 *
 * Input: channel 0 or 1, master or slave, mode 0-3, clock speed when in maseter mode
 * Output: N/A
 * Action: sets up eUSCI_Bx
 *                                                                            */
/******************************************************************************/
void SPI_SetParametersB(unsigned char x, unsigned char Master_nSlave, unsigned char mode, unsigned long speed)
{
	switch(x)
	{
	case 0:
		/* eUSCI clock source select */
		UCB0CTLW0 |= UCSSEL1;
		UCB0CTLW0 &= ~UCSSEL0; // 10b = SMCLK in master mode. Don't use in slave mode.

		/* eUSCI mode */
		UCB0CTLW0 &= ~(UCMODE1 | UCMODE0); // 00b = 3-pin SPI

		if(Master_nSlave == MASTER)
		{
			UCB0CTLW0 |= UCMST; // master mode
		}
		else
		{
			UCB0CTLW0 &= ~UCMST; // slave mode
		}
		switch (mode)
		{
		case 0:
			UCB0CTLW0 |= UCCKPH;
			UCB0CTLW0 &= ~UCCKPL;
			break;
		case 1:
			UCB0CTLW0 &= ~UCCKPH;
			UCB0CTLW0 &= ~UCCKPL;
		case 2:
			UCB0CTLW0 |= UCCKPH;
			UCB0CTLW0 |= UCCKPL;
		default:
			UCB0CTLW0 &= ~UCCKPH;
			UCB0CTLW0 |= UCCKPL;
			break;
		}
		UCB0BRW = (short) MSC_Round ((double) SMCLK_Freq / (double) speed);
		break;
	}
}

/******************************************************************************/
/* SPI_ModuleB
 *
 * The function controls the channels of eUSCI_Bx.
 *
 * Input: channel 0 or 1, on or off
 * Output: returns previous value of the status
 * Action: sets or clears UCSWRST which is used to stop the SPI logic
 *                                                                            */
/******************************************************************************/
unsigned char SPI_ModuleB(unsigned char x, unsigned char state)
{
	unsigned char status = FALSE;

	switch(x)
	{
	case 0:
		if(UCB0CTLW0 & UCSWRST)
		{
			status = TRUE;
		}
		if(state)
		{
			/* hold in reset */
			UCB0CTLW0 &= ~UCSWRST;
		}
		else
		{
			/* run */
			UCB0CTLW0 |= UCSWRST;
		}
		break;
	}
	return status;
}

/******************************************************************************/
/* SPI_ModuleInterruptsB
 *
 * The function controls the interrupt of the channels of eUSCI_Bx.
 *
 * Input: channel 0 or 1, receive interupt on or off, transmit interrupt on or off
 * Output: N/A
 * Action: sets or clears the the transmit and receive interrupts
 *                                                                            */
/******************************************************************************/
void SPI_ModuleInterruptsB(unsigned char x, unsigned char receive, unsigned char transmit)
{
	switch(x)
	{
	case 0:
		if(receive)
		{
			/* recieve interrupt enable*/
			UCB0IE |= UCRXIE;
		}
		else
		{
			/* recieve interrupt disable*/
			UCB0IE &= ~UCRXIE;
		}
		if(transmit)
		{
			/* transmit interrupt enable*/
			UCB0IE |= UCTXIE;
		}
		else
		{
			/* transmit interrupt disable*/
			UCB0IE &= ~UCTXIE;
		}
		break;
	}
}

/******************************************************************************/
/* SPI_BusyB
 *
 * The function returns the activity state of the module.
 *
 * Input: channel 0 or 1
 * Output: Busy or not (TRUE or FALSE of being busy)
 * Action: returns the UCBUSY bit indicating the activity of the module
 *                                                                            */
/******************************************************************************/
unsigned char SPI_BusyB(unsigned char x)
{
	switch(x)
	{
	case 0:
		if(UCB0STATW & UCBUSY)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

/******************************************************************************/
/* SPI_WriteB
 *
 * The function sends a byte over the SPI bus.
 *
 * Input: channel 0 or 1, data to be written
 * Output: if data was written or not (timeout occurance)
 * Action: writes data over the spi bus
 *                                                                            */
/******************************************************************************/
unsigned char SPI_WriteB(unsigned char x, unsigned char WriteData)
{
	unsigned long timer = 0;

	while(SPI_BusyB(x)) // wait for the module to not be busy
	{
		timer++;
		if(timer > SPI_TX_TIMEOUT)
		{
			return FAIL;
		}
	}
	switch(x)
	{
	case 0:
		timer = 0;
		while(!(UCB0IFG & UCTXIFG)); // wait for any previous TX to finish
		{
			timer++;
			if(timer > SPI_TX_TIMEOUT)
			{
				return FAIL;
			}
		}
		UCB0TXBUF = WriteData;
		break;
	}
	timer = 0;
	while(SPI_BusyB(x)) // wait for the module to not be busy
	{
		timer++;
		if(timer > SPI_TX_TIMEOUT)
		{
			return FAIL;
		}
	}
	return PASS;
}

/******************************************************************************/
/* SPI_WriteReadB
 *
 * The function sends a byte over the SPI bus and reads data over the bus.
 *
 * Input: channel 0 or 1, data to be written, pointer to the data that was read
 * Output: if data was written or not (timeout occurance)
 * Action: writes data over the spi bus and read data that is written from
 *  the slave device
 *                                                                            */
/******************************************************************************/
unsigned char SPI_WriteReadB(unsigned char x, unsigned char WriteData, unsigned char* pReadData)
{
	unsigned long timer = 0;
	unsigned char dummy;

	switch(x)
	{
	case 0:
		dummy = UCB0RXBUF;
		SPI_Receive_Available = FALSE;
		SPI_WriteB(x, WriteData);
		while(!SPI_Receive_Available)
		{
			timer++;
			if(timer > SPI_RX_TIMEOUT)
			{
				return FAIL;
			}
		}
		*pReadData = UCB0RXBUF;
		return PASS;
	}
	return PASS;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
