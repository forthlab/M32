/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : IFsh1.c
**     Project   : ZJM_M32
**     Processor : MC9S08SH8MTJ
**     Component : IntFLASH
**     Version   : Component 02.364, Driver 01.28, CPU db: 3.00.065
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2022/3/14, 17:01
**     Abstract  :
**         This component "IntFLASH" implements an access to internal FLASH.
**         The component support reading/writing data into FLASH, erasing of
**         selected sector.
**         The component supports events if the write interrupt is supported.
**         The component supports following modes of write operations:
**           - Write - writing without any test.
**           - Destructive write - sector is erased if necessary.
**           - Safe write - user event is invoked to save and resore data
**                          from the current sector.
**         The component requires on-chip FLASH memory (not used/allocated by
**         other components).
**     Settings  :
**         Total FLASH memory size       : 0x2000 bytes
**         Number of FLASH memory blocks : 1
**         Flash block
**           Address range               : 0xE000-0xFFFF
**           Size                        : 0x2000
**           Sector size                 : 0x0200
**         Write method                  : Destructive write (with erase)
**         Wait in RAM                   : yes
**         Virtual page                  : Disabled
**         CPU clock/speed selection
**           FLASH clock                 : 181.82 kHz
**           High speed mode             : This component enabled
**     Contents  :
**         EraseSector   - byte IFsh1_EraseSector(IFsh1_TAddress Addr);
**         SetByteFlash  - byte IFsh1_SetByteFlash(IFsh1_TAddress Addr, byte Data);
**         SetWordFlash  - byte IFsh1_SetWordFlash(IFsh1_TAddress Addr, word Data);
**         SetBlockFlash - byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Dest, word...
**         GetBlockFlash - byte IFsh1_GetBlockFlash(IFsh1_TAddress Source, IFsh1_TDataAddress Dest, word...
**         DataPtr2Addr  - IFsh1_TAddress IFsh1_DataPtr2Addr(void* Addr);
**         FuncPtr2Addr  - IFsh1_TAddress IFsh1_FuncPtr2Addr(void(*Addr)());
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE IFsh1. */

#include "IFsh1.h"


/* Function prototypes, that are located in RAM */
typedef void (*PFnCmdInRam)(byte);

/* Internal method prototypes */
/*
** ===================================================================
**     Method      :  FnCmdInRam_ (component IntFLASH)
**
**     Description :
**         This code burns one byte. It is copied to the RAM.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void FnCmdInRam_(byte Comand_);

/*
** ===================================================================
**     Method      :  LaunchCmdAndTestError (component IntFLASH)
**
**     Description :
**         This method executes flash command and tests result of the 
**         operation.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static byte IFsh1_LaunchCmdAndTestError(byte Command_, IFsh1_TAddress Addr_, byte Value_);


#define FCMD_BLANK_CHECK    0x05       /* Flash command code */
#define FCMD_PROGRAM        0x20       /* Flash command code */
#define FCMD_BURST_PROGRAM  0x25       /* Flash command code */
#define FCMD_PAGE_ERASE     0x40       /* Flash command code */
#define FCMD_MASS_ERASE     0x41       /* Flash command code */
#define BM_FLASH_ERR_MASK   0x30       /* Bit mask to get FLASH error bits from FSTAT */
#define FLASH_PAGE_SIZE     0x0200U    /* Size of flash page */
#define BM_FLASH_INDEX      (FLASH_PAGE_SIZE - 0x01) /* Value to get index part of address */
#define BM_FLASH_PAGE       (~(BM_FLASH_INDEX)) /* Value to get index part of address */

/*lint -save -e970 Disable MISRA rule (13) checking. */
typedef struct {
  unsigned char code[0x30];            /* Structure required to copy code to ram memory */
  /* Size of this structure needs to be at least (but best) the size of the FnCmdInRam_ */
} TFnCmdInRamStruct;
/*lint -restore */



#pragma MESSAGE DISABLE C1805    /* Disable message: Non standard conversion used */

/*
** ===================================================================
**     Method      :  IFsh1_FnCmdInRam_ (component IntFLASH)
**
**     Description :
**         This code burns one byte. It is copied to the RAM.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void FnCmdInRam_(byte Comand_)
{
  FCMD = Comand_;                      /* Initiate command */
  FSTAT = 0x80;                        /* Launch the command */
  asm nop;                             /* Wait at least four bus cycles before checking FSTAT */
  asm nop;
  asm nop;
  asm nop;
  if (!(FSTAT & BM_FLASH_ERR_MASK)) {  /* If no protection violation or access error detected */
    while (!FSTAT_FCCF) {              /* Wait for command completion */
        SRS = 0x55;                    /* Reset watchdog counter write 55, AA */
        SRS = 0xAA;
    }
  }
  return;
}

/*
** ===================================================================
**     Method      :  IFsh1_LaunchCmdAndTestError (component IntFLASH)
**
**     Description :
**         This method executes flash command and tests result of the 
**         operation.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static byte IFsh1_LaunchCmdAndTestError(byte Command_, IFsh1_TAddress Addr_, byte Value_)
{
  TFnCmdInRamStruct FnCmdInRam = *(TFnCmdInRamStruct *)(FnCmdInRam_);

  SaveStatusReg();                     /* Save the PS register */
  FSTAT = 0x00;                        /* Init. flash engine */
  if (FSTAT & BM_FLASH_ERR_MASK) {     /* Protection violation or access error? */
    FSTAT = BM_FLASH_ERR_MASK;         /* Clear FPVIOL & FACERR flag */
  }
  *(volatile byte *) (Addr_) = Value_; /* Write data to the flash memory */
  ((PFnCmdInRam)&FnCmdInRam)(Command_); /* Call code in RAM */
  RestoreStatusReg();                  /* Restore the PS register */
  if (FSTAT & BM_FLASH_ERR_MASK) {     /* Error detected? */
    if (FSTAT_FPVIOL) {                /* Protect violation? */
      return ERR_PROTECT;              /* Return error code ERR_PROTECT */
    } else {
      return ERR_NOTAVAIL;             /* Return error code ERR_NOTAVAIL */
    }
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_NonDestructiveUnsecureWrite (component IntFLASH)
**
**     Description :
**         This method performs Non-destructive unsecure write.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
byte IFsh1_NonDestructiveUnsecureWrite(IFsh1_TAddress src, IFsh1_TAddress dst, word size)
{
  byte tmp;                            /* Temporary read current destination value */
  TFnCmdInRamStruct FnCmdInRam = *(TFnCmdInRamStruct *)(FnCmdInRam_);

  if (!FSTAT_FCCF) {                   /* Is previous command completed ? */
    return ERR_BUSY;                   /* If no then error */
  }
  SaveStatusReg();                     /* Save status information and disable interrupts(if enabled) */
  FSTAT = 0x00;                        /* Init. flash engine */
  if (FSTAT & BM_FLASH_ERR_MASK) {     /* Protection violation or access error? */
    FSTAT = BM_FLASH_ERR_MASK;         /* Clear FPVIOL & FACERR flag */
  }
  /* Burn data into FLASH */
  while (size--) {                     /* For all written bytes do: */
    tmp = *(byte *)dst;                /* Read current byte value from FLASH */
    if (*(byte *)src != tmp) {         /* Is the src. byte equal to the dest. byte? */
      *(byte *)dst = *(byte *)src | ~tmp; /* Write byte to the flash memory, do not modify zero bits */
      ((PFnCmdInRam)&FnCmdInRam)(FCMD_BURST_PROGRAM); /* Call code in ram */
    }
    ++dst;                             /* Increment destination pointer */
    ++src;                             /* Increment source pointer */
  } /* while size */
  RestoreStatusReg();                  /* Restore status information and interrupt state */
  if (FSTAT & BM_FLASH_ERR_MASK) {     /* Error detected? */
    if (FSTAT_FPVIOL) {                /* Protect violation? */
      return ERR_PROTECT;              /* Return error code ERR_PROTECT */
    } else {
      return ERR_NOTAVAIL;             /* Return error code ERR_NOTAVAIL */
    }
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_WriteBlock (component IntFLASH)
**
**     Description :
**         The method writes block of data to a flash memory.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
byte IFsh1_WriteBlock(IFsh1_TAddress src, IFsh1_TAddress dst, word size)
{
  byte res;                            /* Tmp. var. for function call result */
  IFsh1_TAddress src_tmp = src;        /* Tmp. pointer to the src. memory */
  IFsh1_TAddress dst_tmp;              /* Tmp. pointer to the dest. memory */
  IFsh1_TAddress DstEnd = dst + size;  /* Address of the first byte after the end of the dest. block */
  IFsh1_TAddress SrcEnd = src + size;  /* Address of the first byte after the end of the src. block */

  if (dst < 0xE000U) {                 /* Check address range */
    return ERR_RANGE;                  /* Address is out of FLASH memory */
  }
  if ((size > 0x2000U) || (dst + size - 1 < 0xE000U)) { /* Check block size and last address of the block */
    return ERR_RANGE;                  /* Last Address is out of FLASH memory */
  }
  for (dst_tmp = dst; src_tmp < SrcEnd; dst_tmp++) { /* For all bytes in dest. block do: */
    if (~*(byte *)dst_tmp & *(byte *)src_tmp) { /* Test if erase is necessary */
      res = IFsh1_LaunchCmdAndTestError((byte)FCMD_PAGE_ERASE, dst_tmp, (byte)0x00); /* Erase sector */
      if (res != ERR_OK) {             /* Sector erase error? */
        return res;                    /* Yes, return error code */
      }
      src_tmp += (~dst_tmp & BM_FLASH_INDEX); /* Increase pointer to point to src. data that should be written to the next flash sector */
      dst_tmp = ((dst_tmp + FLASH_PAGE_SIZE) & BM_FLASH_PAGE) - 1; /* Increase pointer to point to the next flash sector */
    }
    src_tmp++;                         /* Increment src. address */
  } /* for all bytes in the block */
  res = IFsh1_NonDestructiveUnsecureWrite(src, dst, size); /* Write data */
  if (res != ERR_OK) {                 /* Write error? */
    return res;                        /* Yes, return error code */
  }
  for(;;) {                            /* Compare src. and written block */
    if ((*(byte *)src) != (*(byte *)dst)) { /* Compare source byte and written byte */
      return ERR_VALUE;                /* Source value is different from dest. value, return error */
    }
    if (--size == 0) {
      break;                           /* Finish if compared last written byte */
    }
    /* *** */
    ++src;                             /* Next byte */
    ++dst;
  } /* for */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_SetByteFlash (component IntFLASH)
**
**     Description :
**         Write byte to address in FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH.
**         Data            - Data to write.
**     Returns     :
**         ---             - Error code, possible codes:
**                           - ERR_OK - OK
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available
**                           - ERR_RANGE - Address is out of range
**                           - ERR_VALUE - Read value is not equal to
**                           written value
**                           - ERR_SPEED - This device does not work
**                           in the active speed mode
** ===================================================================
*/
byte IFsh1_SetByteFlash(IFsh1_TAddress Addr, byte Data)
{
  return IFsh1_WriteBlock((IFsh1_TAddress)&Data, Addr, 0x01U); /* Write data to the flash */
}

/*
** ===================================================================
**     Method      :  IFsh1_SetWordFlash (component IntFLASH)
**
**     Description :
**         Write word to address in FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH.
**         Data            - Data to write.
**     Returns     :
**         ---             - Error code, possible codes:
**                           - ERR_OK - OK
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available
**                           - ERR_RANGE - Address is out of range
**                           - ERR_VALUE - Read value is not equal to
**                           written value
**                           - ERR_SPEED - This device does not work
**                           in the active speed mode
** ===================================================================
*/
byte IFsh1_SetWordFlash(IFsh1_TAddress Addr, word Data)
{
  return IFsh1_WriteBlock((IFsh1_TAddress)&Data, Addr, 0x02U); /* Write data to the flash */
}

/*
** ===================================================================
**     Method      :  IFsh1_SetBlockFlash (component IntFLASH)
**
**     Description :
**         Writes data to FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Source          - Source address of the data
**         Dest            - Destination address in FLASH
**         Count           - Count of the data fields in the
**                           smallest addressable units.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*
byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Dest, word Count)

**      This method is implemented as macro. See IFsh1.h file.      **

*/

/*
** ===================================================================
**     Method      :  IFsh1_GetBlockFlash (component IntFLASH)
**
**     Description :
**         Reads data from FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Source          - Source address of the data.
**         Dest            - Destination address
**         Count           - Count of the data fields in the
**                           smallest addressable units.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte IFsh1_GetBlockFlash(IFsh1_TAddress Source, IFsh1_TDataAddress Dest, word Count)
{
  if ((Source < 0xE000) || (Source > ((0xFFFF - Count) + 1))) { /* Check address range */
    return ERR_RANGE;                  /* Address is out of FLASH memory */
  }
  while(Count--) {
    *Dest = *(byte *)Source;
    ++Source;
    ++Dest;
  } /* while Count */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_EraseSector (component IntFLASH)
**
**     Description :
**         Erase sector to which address Addr belongs.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address in FLASH.
**     Returns     :
**         ---             - Error code, possible codes:
**                           - ERR_OK - OK
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available
**                           - ERR_RANGE - Address is out of range
**                           - ERR_SPEED - This device does not work
**                           in the active speed mode
** ===================================================================
*/
byte IFsh1_EraseSector(IFsh1_TAddress Addr)
{
  byte result;

  if (Addr < 0xE000U) {                /* Check address range */
    return ERR_RANGE;                  /* Address is out of FLASH memory */
  }
  if (!FSTAT_FCCF) {                   /* Is previous command complete ? */
    return ERR_BUSY;                   /* If no then error */
  }
  result = IFsh1_LaunchCmdAndTestError((byte)FCMD_PAGE_ERASE, Addr, (byte)0x00); /* Run command and return result */
  return(result);
}

/*
** ===================================================================
**     Method      :  IFsh1_Init (component IntFLASH)
**
**     Description :
**         Description_Init - Initializes the associated peripheral(s) 
**         and the component's internal variables. The method is called 
**         automatically as a part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void IFsh1_Init(void)
{
  /* FSTAT: FCBEF=0,FCCF=0,FPVIOL=1,FACCERR=1,??=0,FBLANK=0,??=0,??=0 */
  setReg8(FSTAT, 0x30);                /* Clear FPVIOL & FACERR flag */ 
  /* FCDIV: DIVLD=0,PRDIV8=1,DIV5=0,DIV4=0,DIV3=1,DIV2=0,DIV1=1,DIV0=0 */
  setReg8(FCDIV, 0x4A);                /* Initialize FCDIV register */ 
}

/*
** ===================================================================
**     Method      :  IFsh1_DataPtr2Addr (component IntFLASH)
**
**     Description :
**         This method converts data pointer to format of a component 
**         method address parameter. Generally a data pointer format is
**         different from format of a method Addr parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Addr            - Data pointer.
**     Returns     :
**         ---             - Address in the format used in the component
**                           methods.
** ===================================================================
*/
/*
IFsh1_TAddress IFsh1_DataPtr2Addr(void * Addr)

**      This method is implemented as macro. See IFsh1.h file.      **

*/

/*
** ===================================================================
**     Method      :  IFsh1_FuncPtr2Addr (component IntFLASH)
**
**     Description :
**         This method converts function pointer to format of a component
**         method address parameter. Generally a function pointer
**         format is different from format of a method Addr parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Addr            - Function pointer.
**     Returns     :
**         ---             - Address in the format used in the component
**                           methods.
** ===================================================================
*/
/*
IFsh1_TAddress IFsh1_FuncPtr2Addr(void(*Addr)())

**      This method is implemented as macro. See IFsh1.h file.      **

*/

/* END IFsh1. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
