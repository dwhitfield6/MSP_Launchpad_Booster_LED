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
#ifndef UART_H
#define	UART_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Buffer sizes
 *
 * This is the number of characters that fit in the UART 1 receive/transmit
 *  buffers.
 *                                                                            */
/******************************************************************************/
#define RX_BUFFER_SIZE 512
#define TX_BUFFER_SIZE 512

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define NO      0
#define ODD     1
#define EVEN    2
#define GET    2
#define CRLN    "\r\n"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
extern unsigned char RX_Buffer[RX_BUFFER_SIZE];
extern unsigned char TX_Buffer[TX_BUFFER_SIZE];
extern short RX_Buffer_Place;
extern short TX_Buffer_Place_Remove;
extern short TX_Buffer_Place_Add;
extern unsigned char BreakReceived;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_UART(void);
unsigned char UART_Sleep(unsigned char state);
unsigned char UART_Module(unsigned char state);
void UART_Parameters(unsigned long Baud,unsigned char Parity, unsigned char Stop);
unsigned short UART_LookupUCBRSx(double value);
unsigned char UART_TransmitInterrupt(unsigned char state);
unsigned char UART_ReceiveInterrupt(unsigned char state);
void UART_SendChar(unsigned char data);
void UART_SendString(unsigned char* data);
void UART_SendStringCRLN(unsigned char* data);
void UART_CleanBufferRX(void);
void UART_CleanBufferTX(void);
void UART_PrintBanner(void);

#endif	/* UART_H */
