/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : AD.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : ADC
**     Version   : Component 01.587, Driver 01.29, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/14, 17:01
**     Abstract  :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings  :
**         AD control register         : ADCSC1      [$0010]
**         AD control register         : ADCCFG      [$0016]
**         AD control register         : ADCRH       [$0012]
**         AD control register         : ADCRL       [$0013]
**         AD control register         : ADCCVH      [$0014]
**         AD control register         : ADCCVL      [$0015]
**         AD control register         : ADCSC2      [$0011]
**         AD control register         : APCTL1      [$0017]
**         AD control register         : APCTL2      [$0018]
**         Interrupt name              : Vadc
**         Priority                    : 
**         User handling procedure     : not specified
**         Number of conversions       : 1
**         AD resolution               : 10-bit
**
**         Input pins
**
**              Port name              : PTB
**              Bit number (in port)   : 2
**              Bit mask of the port   : $0004
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**              Port name              : PTC
**              Bit number (in port)   : 0
**              Bit mask of the port   : $0001
**              Port data register     : PTCD        [$0004]
**              Port control register  : PTCDD       [$0005]
**
**              Port name              : PTB
**              Bit number (in port)   : 3
**              Bit mask of the port   : $0008
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**              Port name              : PTC
**              Bit number (in port)   : 1
**              Bit mask of the port   : $0002
**              Port data register     : PTCD        [$0004]
**              Port control register  : PTCDD       [$0005]
**
**         Initialization:
**              Conversion             : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-8
**     Contents  :
**         Measure    - byte AD_Measure(bool WaitForResult);
**         GetValue16 - byte AD_GetValue16(word *Values);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE AD. */

#include "AD.h"



static void ClrSumV(void);
/*
** ===================================================================
**     Method      :  ClrSumV (component ADC)
**
**     Description :
**         The method clears the internal buffers used to store sum of a 
**         number of last conversions.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define STOP            0              /* STOP state           */
#define MEASURE         1              /* MESURE state         */
#define CONTINUOUS      2              /* CONTINUOS state      */
#define SINGLE          3              /* SINGLE state         */


static const  byte Channels[4] = {0x46,0x48,0x47,0x49};  /* Contents for the device control register */

static volatile bool OutFlg;           /* Measurement finish flag */
static volatile byte SumChan;          /* Number of measured channels */
static volatile byte ModeFlg;          /* Current state of device */

volatile word AD_OutV[4];              /* Sum of measured values */





/*
** ===================================================================
**     Method      :  AD_Interrupt (component ADC)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(AD_Interrupt)
{
  ((TWREG volatile*)(&AD_OutV[SumChan]))->b.high = ADCRH; /* Save measured value */
  ((TWREG volatile*)(&AD_OutV[SumChan]))->b.low = ADCRL; /* Save measured value */
  SumChan++;                           /* Number of measurement */
  if (SumChan == 4) {                  /* Is number of measurement equal to the number of conversions? */
    SumChan = 0;                       /* If yes then set the number of measurement to 0 */
    OutFlg = TRUE;                     /* Measured values are available */
    ModeFlg = STOP;                    /* Set the device to the stop mode */
    return;                            /* Return from interrupt */
  }
  ADCSC1 = Channels[SumChan];          /* Start measurement of next channel */
}

/*
** ===================================================================
**     Method      :  ClrSumV (component ADC)
**
**     Description :
**         The method clears the internal buffers used to store sum of a 
**         number of last conversions.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ClrSumV(void)
{
  AD_OutV[0] = 0;                      /* Set variable for storing measured values to 0 */
  AD_OutV[1] = 0;                      /* Set variable for storing measured values to 0 */
  AD_OutV[2] = 0;                      /* Set variable for storing measured values to 0 */
  AD_OutV[3] = 0;                      /* Set variable for storing measured values to 0 */
}

/*
** ===================================================================
**     Method      :  AD_HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD_HWEnDi(void)
{
  if (ModeFlg) {                       /* Start or stop measurement? */
    OutFlg = FALSE;                    /* Output values aren't available */
    SumChan = 0;                       /* Set the number of measured channels to 0 */
    ClrSumV();                         /* Clear measured values */
    ADCSC1 = Channels[SumChan];        /* If yes then start the conversion */
  }
}

/*
** ===================================================================
**     Method      :  AD_Measure (component ADC)
**
**     Description :
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the <number of
**         conversions> is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     Parameters  :
**         NAME            - DESCRIPTION
**         WaitForResult   - Wait for a result of a
**                           conversion. If <interrupt service> is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the <number of channel> is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the <interrupt
**                           service> is disabled and a <number of
**                           conversions> is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
** ===================================================================
*/
#pragma MESSAGE DISABLE C5703 /* WARNING C5703: Parameter declared but not referenced */
byte AD_Measure(bool WaitForResult)
{
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = MEASURE;                   /* Set state of device to the measure mode */
  AD_HWEnDi();                         /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == MEASURE) {}      /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD_GetValue16 (component ADC)
**
**     Description :
**         This method returns the last measured values of all channels
**         justified to the left. Compared with <GetValue> method this
**         method returns more accurate result if the <number of
**         conversions> is greater than 1 and <AD resolution> is less
**         than 16 bits. In addition, the user code dependency on <AD
**         resolution> is eliminated.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Values          - Pointer to the array that contains
**                           the measured data.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
** ===================================================================
*/
byte AD_GetValue16(word *Values)
{
  if (!OutFlg) {                       /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  Values[0] = (word)((AD_OutV[0]) << 6); /* Save measured values to the output buffer */
  Values[1] = (word)((AD_OutV[1]) << 6); /* Save measured values to the output buffer */
  Values[2] = (word)((AD_OutV[2]) << 6); /* Save measured values to the output buffer */
  Values[3] = (word)((AD_OutV[3]) << 6); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD_Init(void)
{
  /* ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 */
  setReg8(ADCSC1, 0x1F);               /* Disable the module */ 
  /* ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 */
  setReg8(ADCSC2, 0x00);               /* Disable HW trigger and autocompare */ 
  OutFlg = FALSE;                      /* No measured value */
  ModeFlg = STOP;                      /* Device isn't running */
  /* ADCCFG: ADLPC=0,ADIV1=1,ADIV0=1,ADLSMP=1,MODE1=1,MODE0=0,ADICLK1=1,ADICLK0=1 */
  setReg8(ADCCFG, 0x7B);               /* Set prescaler bits */ 
}


/* END AD. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
