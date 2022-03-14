/** ###################################################################
**     Filename  : Events.h
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2019/9/19, 10:56
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         AD_OnEnd        - void AD_OnEnd(void);
**         T20ms_OnInterrupt - void T20ms_OnInterrupt(void);
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "STB.h"
#include "DIO.h"
#include "CLK.h"
#include "Relay1.h"
#include "Relay2.h"
#include "Relay3.h"
#include "Relay4.h"
#include "SDA2.h"
#include "SDA1.h"
#include "T20ms.h"
#include "AD.h"
#include "us.h"
#include "IFsh1.h"
#include "WDog.h"
#include "Alarm.h"

void T20ms_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  T20ms_OnInterrupt (module Events)
**
**     Component   :  T20ms [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
