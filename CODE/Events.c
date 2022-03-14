/** ###################################################################
**     Filename  : Events.c
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
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "GlobalVar.h"
#include "TM1629C.h"
#include "key.h"
#include "AD.h"
#include "IncDec.h"

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
void T20ms_OnInterrupt(void)
{
  /* Write your code here ... */
    
   unsigned char n; 
  
  
   SysTick++;           //20ms tick++
   
   key_proc();      
   
   
   AD_GetValue16( AD ); //读取负载检测AD值
   AD_Measure(0);       //启动负载检测AD
   
   for( n=0;n<4;n++) {
      
      if( Parameter[n].Working ) {
        INCINT( Parameter[n].WorkingTick );   //测量值控制计时
      }
      else  {
        DECINT( Parameter[n].WorkingTick );
      }
      
      LoadVal[n] = LoadVal[n] - LoadVal[n]/8 + AD[n]/8;  //负载电流滤波 
   } 
   
                                                
          
   TM1629_WriteDateBuf( LED_buf, 0, 16, 3 );  //LED定时刷新,原始数据LED_buf发送到TM1629的地址0开始的16个数据,亮度7       
   
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
