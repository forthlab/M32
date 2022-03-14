/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : CLK.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.24, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/5, 15:46
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
**                       9             |  PTC3_ADP11
**             ----------------------------------------------------
**
**         Port name                   : PTC
**
**         Bit number (in port)        : 3
**         Bit mask of the port        : $0008
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTCD      [$0004]
**         Port control register       : PTCDD     [$0005]
**
**         Optimization for            : speed
**     Contents  :
**         PutVal - void CLK_PutVal(bool Val);
**         ClrVal - void CLK_ClrVal(void);
**         SetVal - void CLK_SetVal(void);
**         NegVal - void CLK_NegVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE CLK. */

#include "CLK.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  CLK_PutVal (component BitIO)
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
void CLK_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTCD, 0x08);           /* PTCD3=0x01 */
  } else { /* !Val */
    clrReg8Bits(PTCD, 0x08);           /* PTCD3=0x00 */
  } /* !Val */
}

/*
** ===================================================================
**     Method      :  CLK_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CLK_ClrVal(void)

**  This method is implemented as a macro. See CLK.h file.  **
*/

/*
** ===================================================================
**     Method      :  CLK_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CLK_SetVal(void)

**  This method is implemented as a macro. See CLK.h file.  **
*/

/*
** ===================================================================
**     Method      :  CLK_NegVal (component BitIO)
**
**     Description :
**         This method negates (inverts) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CLK_NegVal(void)

**  This method is implemented as a macro. See CLK.h file.  **
*/


/* END CLK. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
