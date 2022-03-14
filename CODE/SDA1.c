/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : SDA1.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.24, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/5, 15:46
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       20            |  PTA0_PIA0_TPM1CH0_ADP0_ACMPPLUS
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 0
**         Bit mask of the port        : $0001
**
**         Initial direction           : Input (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTAD      [$0000]
**         Port control register       : PTADD     [$0001]
**
**         Optimization for            : speed
**     Contents  :
**         SetDir - void SDA1_SetDir(bool Dir);
**         GetVal - bool SDA1_GetVal(void);
**         PutVal - void SDA1_PutVal(bool Val);
**         ClrVal - void SDA1_ClrVal(void);
**         SetVal - void SDA1_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE SDA1. */

#include "SDA1.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  SDA1_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool SDA1_GetVal(void)

**  This method is implemented as a macro. See SDA1.h file.  **
*/

/*
** ===================================================================
**     Method      :  SDA1_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void SDA1_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTAD, 0x01);           /* PTAD0=0x01 */
    Shadow_PTA |= (byte)0x01;          /* Set-up shadow variable */
  } else { /* !Val */
    clrReg8Bits(PTAD, 0x01);           /* PTAD0=0x00 */
    Shadow_PTA &= (byte)~0x01;         /* Set-up shadow variable */
  } /* !Val */
}

/*
** ===================================================================
**     Method      :  SDA1_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void SDA1_ClrVal(void)

**  This method is implemented as a macro. See SDA1.h file.  **
*/

/*
** ===================================================================
**     Method      :  SDA1_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void SDA1_SetVal(void)

**  This method is implemented as a macro. See SDA1.h file.  **
*/

/*
** ===================================================================
**     Method      :  SDA1_SetDir (component BitIO)
**
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void SDA1_SetDir(bool Dir)
{
  if (Dir) {
    setReg8(PTAD, (getReg8(PTAD) & (~0x01)) | (Shadow_PTA & 0x01)); /* PTAD0=Shadow_PTA[bit 0] */
    setReg8Bits(PTADD, 0x01);          /* PTADD0=0x01 */
  } else { /* !Dir */
    clrReg8Bits(PTADD, 0x01);          /* PTADD0=0x00 */
  } /* !Dir */
}


/* END SDA1. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
