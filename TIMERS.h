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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef TIMERS_H
#define	TIMERS_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Timer modes
 *
 * This is the modes for the timer to set the interrupt.
 *                                                                            */
/******************************************************************************/
#define UP 			1
#define CONTINUOUS 	2
#define UP_DOWN		3

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_Timers(void);
void Init_TimerA0(void);
void Init_TimerA1(void);
void Init_TimerA2(void);
unsigned char TMR_ModeTimerA0(unsigned char state);
unsigned char TMR_ModeTimerA1(unsigned char state);
unsigned char TMR_ModeTimerA2(unsigned char state);
unsigned char TMR_InterruptTimerA0(unsigned char state);
unsigned char TMR_InterruptTimerA1(unsigned char state);
unsigned char TMR_InterruptTimerA2(unsigned char state);
void TMR_ResetTimerA0(void);
void TMR_ResetTimerA1(void);
void TMR_ResetTimerA2(void);
void TMR_SetTimerA0(unsigned short time);
void TMR_SetTimerA1(unsigned short time);
void TMR_SetTimerA2(unsigned short time);
unsigned short TMR_GetTimerA0(void);
unsigned short TMR_GetTimerA1(void);
unsigned short TMR_GetTimerA2(void);

#endif	/* TIMERS_H */
