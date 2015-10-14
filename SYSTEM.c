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
/* Contains system functions.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Clock source frequencies                                                   */
/******************************************************************************/
unsigned long ACLK_Freq = 0;
unsigned long SMCLK_Freq = 0;
unsigned long MCLK_Freq = 0;
unsigned long DCOCLK_Freq = 0;
unsigned long VLOCLK_Freq = 10000;
unsigned long LFXTCLK_Freq = LF_CRYSAL;
unsigned long MODCLK_Freq = 5000000;
unsigned long LFMODCLK_Freq = 0;
unsigned long HFXTCLK_Freq = HF_CRYSAL;

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* SYS_ConfigureOscillator
 *
 * The function waits for the high frequency oscillator to be working and
 * stable.
 *
 * Input: N/A
 * Output: N/A
 * Action: sets the clocks' configurations
 *                                                                            */
/******************************************************************************/
void SYS_ConfigureOscillator(void)
{
	/* DCO frequency to 8 MHz */
	SYS_Unlock();

	CSCTL1 = 0;
	CSCTL2 = 0;

	CSCTL4 &= ~LFXTOFF; // LFXT is on
	CSCTL4 &= ~VLOOFF; // VLO is on

	CSCTL1 |= DCORSEL; // high speed mode
	CSCTL1 |= DCOFSEL1 | DCOFSEL0; // 011b = 8 MHz

	/* Turn on SMCLK */
	CSCTL4 &= ~SMCLKOFF;

	/* ACLK */
	CSCTL2 &= ~(SELA2 | SELA1 | SELA0); // Selects the ACLK source: 000b = LFXTCLK
	CSCTL3 &= ~(DIVA2 | DIVA1 | DIVA0); // ACLK source divider: 000b = /1


	/* SMCLK */
	CSCTL2 |= (SELS1 | SELS0); // Selects the SMCLK source: 011b = DCOCLK
	CSCTL3 &= ~(DIVS2 | DIVS1 | DIVS0); // clear CSCTL3 DIVS bits: 011b = /8
	CSCTL3 |= (DIVS1 | DIVS0); // set CSCTL3 DIVS bits: 011b = /8


	/* MCLK */
	CSCTL2 |= (SELM1 | SELM0); // Selects the MCLK source: 011b = DCOCLK
	CSCTL3 &= ~(DIVM2 | DIVM1 | DIVM0); // clear CSCTL3 DIVS bits: 000b = /1

	CSCTL5 &= ~LFXTOFFG; // clear the LFXT fault
	SYS_CalculateClocks();
}

/******************************************************************************/
/* SYS_Unlock
 *
 * The writes the unlock sequence so that CS registers are available for
 *  writing.
 *                                                                            */
/******************************************************************************/
void SYS_Unlock(void)
{
	CSCTL0 = CSKEY;
}

/******************************************************************************/
/* SYS_CalculateClocks
 *
 * The function calculates the clock values and sets the global variables.
 *
 * Input: N/A
 * Output: N/A
 * Action: sets the clock frequency variables
 *                                                                            */
/******************************************************************************/
void SYS_CalculateClocks(void)
{
	unsigned short temp;

	LFMODCLK_Freq = MODCLK_Freq / 128;

	/* DCO clock */
	temp = (CSCTL1 & (DCOFSEL2 | DCOFSEL1 | DCOFSEL0)) >> 1; // DCO frequency select
	if(CSCTL1 & DCORSEL)
	{
		switch (temp)
		{
		case 0b000:
			DCOCLK_Freq = 1000000;
			break;
		case 0b001:
			DCOCLK_Freq = 5330000;
			break;
		case 0b010:
			DCOCLK_Freq = 6670000;
			break;
		case 0b011:
			DCOCLK_Freq = 8000000;
			break;
		case 0b100:
			DCOCLK_Freq = 1600000;
			break;
		case 0b101:
			DCOCLK_Freq = 2100000;
			break;
		case 0b110:
			DCOCLK_Freq = 2400000;
			break;
		default:
			/* not recommended */
			DCOCLK_Freq = 2400000;
			break;
		}
	}
	else
	{
		switch (temp)
		{
		case 0b000:
			DCOCLK_Freq = 1000000;
			break;
		case 0b001:
			DCOCLK_Freq = 2670000;
			break;
		case 0b010:
			DCOCLK_Freq = 3330000;
			break;
		case 0b011:
			DCOCLK_Freq = 4000000;
			break;
		case 0b100:
			DCOCLK_Freq = 5330000;
			break;
		case 0b101:
			DCOCLK_Freq = 6670000;
			break;
		case 0b110:
			DCOCLK_Freq = 8000000;
			break;
		default:
			/* not recommended */
			DCOCLK_Freq = 8000000;
			break;
		}
	}

	/*~~~~~~~~~~~ clock sources ~~~~~~~~~~*/
	/* ACLK */
	temp = (CSCTL2 & (SELA2 | SELA1 | SELA0)) >> 8;
	switch (temp)
	{
	case 0b000:
		if(LFXTCLK_Freq)
		{
			/* crystal is installed */
			ACLK_Freq = LFXTCLK_Freq; // LFXTCLK
		}
		else
		{
			ACLK_Freq = VLOCLK_Freq; // VLOCLK
		}
		break;
	case 0b001:
		ACLK_Freq = VLOCLK_Freq; // VLOCLK
		break;
	case 0b010:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b011:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b100:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b101:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b110:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	default:
		ACLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	}

	/* SMCLK */
	temp = (CSCTL2 & (SELS2 | SELS1 | SELS0)) >> 4;
	switch (temp)
	{
	case 0b000:
		if(LFXTCLK_Freq)
		{
			/* crystal is installed */
			SMCLK_Freq = LFXTCLK_Freq; // LFXTCLK
		}
		else
		{
			SMCLK_Freq = VLOCLK_Freq; // VLOCLK
		}
		break;
	case 0b001:
		SMCLK_Freq = VLOCLK_Freq; // VLOCLK
		break;
	case 0b010:
		SMCLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b011:
		SMCLK_Freq = DCOCLK_Freq; // DCOCLK
		break;
	case 0b100:
		SMCLK_Freq = MODCLK_Freq; // MODCLK
		break;
	case 0b101:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			SMCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			SMCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	case 0b110:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			SMCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			SMCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	default:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			SMCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			SMCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	}

	/* MCLK */
	temp = (CSCTL2 & (SELM2 | SELM1 | SELM0));
	switch (temp)
	{
	case 0b000:
		if(LFXTCLK_Freq)
		{
			/* crystal is installed */
			MCLK_Freq = LFXTCLK_Freq; // LFXTCLK
		}
		else
		{
			MCLK_Freq = VLOCLK_Freq; // VLOCLK
		}
		break;
	case 0b001:
		MCLK_Freq = VLOCLK_Freq; // VLOCLK
		break;
	case 0b010:
		MCLK_Freq = LFMODCLK_Freq; // LFMODCLK
		break;
	case 0b011:
		MCLK_Freq = DCOCLK_Freq; // DCOCLK
		break;
	case 0b100:
		MCLK_Freq = MODCLK_Freq; // MODCLK
		break;
	case 0b101:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			MCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			MCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	case 0b110:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			MCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			MCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	default:
		if(HFXTCLK_Freq)
		{
			/* crystal is installed */
			MCLK_Freq = HFXTCLK_Freq; // HFXTCLK
		}
		else
		{
			MCLK_Freq = DCOCLK_Freq; // DCOCLK
		}
		break;
	}

	/*~~~~~~~~~~~ clock dividers ~~~~~~~~~~*/
	/* ACLK */
	temp = (CSCTL3 & (DIVA2 | DIVA1 | DIVA0)) >> 8;
	switch (temp)
	{
	case 0b000:
		break;
	case 0b001:
		ACLK_Freq /= 2;
		break;
	case 0b010:
		ACLK_Freq /= 4;
		break;
	case 0b011:
		ACLK_Freq /= 8;
		break;
	case 0b100:
		ACLK_Freq /= 16;
		break;
	case 0b101:
		ACLK_Freq /= 32;
		break;
	case 0b110:
		ACLK_Freq /= 32;
		break;
	default:
		ACLK_Freq /= 32;
		break;
	}

	/* SMCLK */
	temp = (CSCTL3 & (DIVS2 | DIVS1 | DIVS0)) >> 4;
	switch (temp)
	{
	case 0b000:
		break;
	case 0b001:
		SMCLK_Freq /= 2;
		break;
	case 0b010:
		SMCLK_Freq /= 4;
		break;
	case 0b011:
		SMCLK_Freq /= 8;
		break;
	case 0b100:
		SMCLK_Freq /= 16;
		break;
	case 0b101:
		SMCLK_Freq /= 32;
		break;
	case 0b110:
		SMCLK_Freq /= 32;
		break;
	default:
		SMCLK_Freq /= 32;
		break;
	}
	/* MCLK */
	temp = (CSCTL3 & (DIVM2 | DIVM1 | DIVM0));
	switch (temp)
	{
	case 0b000:
		break;
	case 0b001:
		MCLK_Freq /= 2;
		break;
	case 0b010:
		MCLK_Freq /= 4;
		break;
	case 0b011:
		MCLK_Freq /= 8;
		break;
	case 0b100:
		MCLK_Freq /= 16;
		break;
	case 0b101:
		MCLK_Freq /= 32;
		break;
	case 0b110:
		MCLK_Freq /= 32;
		break;
	default:
		SMCLK_Freq /= 32;
		break;
	}
}

/******************************************************************************/
/* SYS_Sleep
 *
 * The function exits the system from sleep mode.puts the system to sleep mode.
 *
 * Input: N/A
 * Output: N/A
 * Action: puts the system into sleep mode by stoping the CPU and some clocks.
 *                                                                            */
/******************************************************************************/
void SYS_Sleep(void)
{
	/* CPU, MCLK are disabled. ACLK is active. SMCLK is disabled */
	_BIS_SR(LPM3_bits + GIE); // Enter LPM3 w/interrupt
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
