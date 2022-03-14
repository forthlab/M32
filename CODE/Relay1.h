/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Relay1.h
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.24, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/14, 17:01
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       18            |  PTA2_PIA2_SDA_ADP2
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 2
**         Bit mask of the port        : $0004
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTAD      [$0000]
**         Port control register       : PTADD     [$0001]
**
**         Optimization for            : speed
**     Contents  :
**         PutVal - void Relay1_PutVal(bool Val);
**         ClrVal - void Relay1_ClrVal(void);
**         SetVal - void Relay1_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef Relay1_H_
#define Relay1_H_

/* MODULE Relay1. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  Relay1_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void Relay1_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  Relay1_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Relay1_ClrVal() ( \
    (void)clrReg8Bits(PTAD, 0x04)      /* PTAD2=0x00 */ \
  )

/*
** ===================================================================
**     Method      :  Relay1_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Relay1_SetVal() ( \
    (void)setReg8Bits(PTAD, 0x04)      /* PTAD2=0x01 */ \
  )



/* END Relay1. */
#endif /* #ifndef __Relay1_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
