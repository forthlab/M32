/** ###################################################################
**     Filename  : ZJM_M32.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2019/9/19, 10:56
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE ZJM_M32 */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
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
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
void my_main( void );




void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */


   
   for(;;) {
    
      //TestTM1629();
      my_main();
   }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ZJM_M32 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
