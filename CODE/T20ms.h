/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : T20ms.h
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : TimerInt
**     Version   : Component 02.158, Driver 01.20, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/14, 17:01
**     Abstract  :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM10
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-8
**             Clock                   : 2000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 625
**             microseconds            : 20000
**             milliseconds            : 20
**             seconds (real)          : 0.02
**             Hz                      : 50
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : TPM1CNT   [$0021]
**              Mode                   : TPM1SC    [$0020]
**              Run                    : TPM1SC    [$0020]
**              Prescaler              : TPM1SC    [$0020]
**
**         Compare registers
**              Compare                : TPM1C0V   [$0026]
**
**         Flip-flop registers
**              Mode                   : TPM1C0SC  [$0025]
**     Contents  :
**         Enable       - byte T20ms_Enable(void);
**         Disable      - byte T20ms_Disable(void);
**         EnableEvent  - byte T20ms_EnableEvent(void);
**         DisableEvent - byte T20ms_DisableEvent(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __T20ms
#define __T20ms

/* MODULE T20ms. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
extern bool T20ms_EnEvent;             /* Enable/Disable events */


byte T20ms_Enable(void);
/*
** ===================================================================
**     Method      :  T20ms_Enable (component TimerInt)
**
**     Description :
**         This method enables the component - it starts the timer.
**         Events may be generated (<DisableEvent>/<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte T20ms_Disable(void);
/*
** ===================================================================
**     Method      :  T20ms_Disable (component TimerInt)
**
**     Description :
**         This method disables the component - it stops the timer. No
**         events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define T20ms_EnableEvent() (T20ms_EnEvent = TRUE, (byte)ERR_OK) /* Set the flag "events enabled" */
/*
** ===================================================================
**     Method      :  T20ms_EnableEvent (component TimerInt)
**
**     Description :
**         This method enables the events.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define T20ms_DisableEvent() (T20ms_EnEvent = FALSE, (byte)ERR_OK) /* Set the flag "events disabled" */
/*
** ===================================================================
**     Method      :  T20ms_DisableEvent (component TimerInt)
**
**     Description :
**         This method disables the events.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

__interrupt void T20ms_Interrupt(void);
/*
** ===================================================================
**     Method      :  T20ms_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void T20ms_Init(void);
/*
** ===================================================================
**     Method      :  T20ms_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END T20ms. */

#endif /* ifndef __T20ms */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
