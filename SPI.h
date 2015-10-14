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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef SPI_H
#define	SPI_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* SPI_TX_TIMEOUT
 *
 * This is the number of counts to wait until we call a timeout and fail to
 *  send.
 *                                                                            */
/******************************************************************************/
#define SPI_TX_TIMEOUT 	10000

/******************************************************************************/
/* SPI_RX_TIMEOUT
 *
 * This is the number of counts to wait until we call a timeout and fail to
 *  receive.
 *                                                                            */
/******************************************************************************/
#define SPI_RX_TIMEOUT 	10000

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define SLAVE 	0
#define MASTER 	1

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
extern unsigned char SPI_Receive_Available;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_SPI(void);
void SPI_SetParametersB(unsigned char x, unsigned char Master_nSlave, unsigned char mode, unsigned long speed);
unsigned char SPI_ModuleB(unsigned char x, unsigned char state);
void SPI_ModuleInterruptsB(unsigned char x, unsigned char receive, unsigned char transmit);
unsigned char SPI_BusyB(unsigned char x);
unsigned char SPI_WriteB(unsigned char x, unsigned char WriteData);
unsigned char SPI_WriteReadB(unsigned char x, unsigned char WriteData, unsigned char* pReadData);


#endif	/* SPI_H */
