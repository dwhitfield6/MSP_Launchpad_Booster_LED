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
#ifndef SYSTEM_H
#define	SYSTEM_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Crystals
 *
 * This is the frequency of the external crystals. HF_CRYSAL is used to
 *  clock HFXTCLK while LF_CRYSTAL is used to clock LFXTCLK_Freq. If the
 *  crystal is not in place then put 0 here.
 *                                                                            */
/******************************************************************************/
#define LF_CRYSAL 32768
#define HF_CRYSAL 0

/******************************************************************************/
/* Oscillators
 *
 * - LFXTCLK: Low-frequency oscillator that can be used either with low-frequency
 *   32768-Hz watch crystals, standard crystals, resonators, or external clock
 *   sources in the 50 kHz or below range. When in bypass mode, LFXTCLK can
 *   be driven with an external square wave signal.
 * - VLOCLK: Internal very-low-power low-frequency oscillator with 10-kHz
 *   typical frequency
 * - DCOCLK: Internal digitally controlled oscillator (DCO) with selectable
 *   frequencies
 * - MODCLK: Internal low-power oscillator with 5-MHz typical frequency.
 *   LFMODCLK is MODCLK divided by 128.
 * - HFXTCLK: High-frequency oscillator that can be used with standard
 *   crystals or resonators in the 4-MHz to 24-MHz.
 *                                                                            */
/******************************************************************************/
#define LFXT 0 // LFXT oscillator uses a 32768-Hz watch crystal
#define HFXT 1 // HFXT high-frequency oscillator can be used with standard crystals or resonators
#define VLO  2 // VLO is an internal oscillator providing a typical frequency of 10 kHz
#define MODOSC  3 // MODOSC is an internal oscillator
#define DCO  4 // DCO is a digitally controlled oscillator

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Clock source frequencies                                                   */
/******************************************************************************/
extern unsigned long ACLK_Freq;
extern unsigned long SMCLK_Freq;
extern unsigned long MCLK_Freq;
extern unsigned long DCOCLK_Freq;
extern unsigned long VLOCLK_Freq;
extern unsigned long LFXTCLK_Freq;
extern unsigned long MODCLK_Freq;
extern unsigned long LFMODCLK_Freq;
extern unsigned long HFXTCLK_Freq;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* SYS_ExitSleep
 *
 * The function exits the system from sleep mode.
 *
 * Input: N/A
 * Output: N/A
 * Action: puts the system into run mode by starting the CPU
 *                                                                            */
/******************************************************************************/
#define SYS_ExitSleep() (_BIC_SR(LPM3_EXIT)) // wake up from low power mode

/******************************************************************************/
/* SLEEP
 *
 * The function is the a same as SYS_Sleep().
 *                                                                            */
/******************************************************************************/
#define SLEEP() SYS_Sleep()

/******************************************************************************/
/* WAKE
 *
 * The function is the a same as SYS_ExitSleep().
 *                                                                            */
/******************************************************************************/
#define WAKE() SYS_ExitSleep()

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void SYS_Unlock(void);
void SYS_ConfigureOscillator(void);
void SYS_CalculateClocks(void);
void SYS_Sleep(void);

#endif	/* SYSTEM_H */
