/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : us.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : FreeCntr
**     Version   : Component 01.115, Driver 01.11, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/14, 17:01
**     Abstract  :
**         This device "FreeCntr" implements a free running counter for
**         time measurement.
**     Settings  :
**         Timer name                  : TPM2 (16-bit)
**         Compare name                : TPM20
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-16
**             Clock                   : 1000000 Hz
**           Period
**             Xtal ticks              : 2048
**             microseconds            : 65535
**             milliseconds            : 66
**             seconds (real)          : 0.065535
**             Hz                      : 15
**           Frequency of counting (Bus clock / prescaler)
**             Hz                      : 1000000
**
**         Initialization:
**              Timer                  : Enabled
**
**         Timer registers
**              Counter                : TPM2CNT   [$0061]
**              Mode                   : TPM2SC    [$0060]
**              Run                    : TPM2SC    [$0060]
**              Prescaler              : TPM2SC    [$0060]
**              Compare                : TPM2C0V   [$0066]
**
**         User handling procedure     : not specified
**     Contents  :
**         Reset           - byte us_Reset(void);
**         GetCounterValue - byte us_GetCounterValue(us_TTimerValue *Value);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE us. */

#include "us.h"


/*** Internal macros and method prototypes ***/

/*
** ===================================================================
**     Method      :  SetCV (component FreeCntr)
**
**     Description :
**         The method computes and sets compare/modulo/reload value for 
**         time measuring.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define us_SetCV(_Val) ( \
  TPM2C0V = (us_TTimerValue)(_Val) )


/*** End of Internal methods declarations ***/


/*
** ===================================================================
**     Method      :  us_Reset (component FreeCntr)
**
**     Description :
**         This method resets the timer-counter (sets to zero for
**         up-counting HW or to appropriate value for down-counting HW).
**         The method is available only if the counter register can be
**         set by software and the counter is not shared.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte us_Reset(word *Value)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  us_GetCounterValue (component FreeCntr)
**
**     Description :
**         This method returns a content of the counter.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Value           - Pointer to returned value.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte us_GetCounterValue(us_TTimerValue *Value)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  us_Init (component FreeCntr)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void us_Init(void)
{
  /* TPM2SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x00);               /* Stop HW; disable overflow interrupt and set prescaler to 0 */ 
  /* TPM2MOD: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0,BIT7=0,BIT6=0,BIT5=0,BIT4=0,BIT3=0,BIT2=0,BIT1=0,BIT0=0 */
  setReg16(TPM2MOD, 0x00U);            /* Clear modulo register: e.g. set free-running mode */ 
  /* TPM2C0SC: CH0F=0,CH0IE=0,MS0B=0,MS0A=1,ELS0B=0,ELS0A=0,??=0,??=0 */
  setReg8(TPM2C0SC, 0x10);             /* Set output compare mode and disable compare interrupt */ 
  us_SetCV(0xFFFFU);                   /* Inicialize appropriate value to the compare/modulo/reload register */
  /* TPM2CNTH: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0 */
  setReg8(TPM2CNTH, 0x00);             /* Reset HW Counter */ 
  /* TPM2SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=1,PS2=1,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x0C);               /* Set prescaler and run counter */ 
}

/* END us. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
