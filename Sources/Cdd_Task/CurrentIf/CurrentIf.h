//CurrentIf.h
#ifndef CURRENTIF_H
#define CURRENTIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"




#define MAX_CURRENT_IC_MODULE_NUM   1

#define WRCUROFF		0x42         /*write current offset register*/
#define WRCURGAIN		0x44         /*write current gain register*/
#define WRVOLOFF		0x46         /*write voltage offset register*/
#define WRVOLGAIN		0x48         /*write voltage gain register*/

#define RDCUROFF    0x02				 /*read current offset register*/
#define RDCURGAIN   0x04				 /*read current gain register*/
#define RDVOLOFF    0x06				 /*read voltage offset register*/
#define RDVOLGAIN   0x08 				 /*read voltage gain register*/

#define RDENERGY    0x14         /* read energy register */
#define RDCURR      0x16		     /* read RMS current register */
#define RDCURRINS   0x0E		     /* read instance current register */
#define RDVRMS      0x18         /* read RMS voltage register */
#define RDVINS      0x10		     /* read instance voltage register */

#define CLBRTCUROFF   0xc9       /*calibrate current offset*/
#define CLBRTCURGAIN  0xca       /*calibrate current gain*/
#define CLBRTVOLTOFF  0xd1       /*calibrate voltage offset*/
#define CLBRTVOLTGAIN 0xd2       /*calibrate voltage gain*/

#define STARTCONV    0xe8        /* start continous conversion*/
#define STARTSICON   0xe0        /* start a single conversion*/

#define WRCONFIG    0x40         /* write config register*/
#define WRSTATUS    0x5e         /* write status register*/
#define WRMASK      0x74         /* write mask register*/
#define WRCC        0x4a         /* write cycle-count register */

#define RDCONFIG    0x00         /* read config register*/
#define RDSTATUS    0x1e         /* read status register*/
#define RDMASK      0x34         /* read mask register*/
#define RDCC        0x0a         /* read cycle-count register */

#define SYNC0       0xfe         /*end of resynchronize sequence*/
#define SYNC1       0xff         /*part of resynchronize sequence*/

#define VOLTGAIN 				 8
#define VOLTOFFSET			 6
#define CURRGAIN			   4
#define CURROFFSET		   2

/* flag indicate charge or discharge */
#define CURRENTIF_FLAG_CHG  1			 /* flag for charge */
#define CURRENTIF_FLAG_DCH  0			 /* flag for discharge */

//#define CHG_SIGN   0x8000

//#define CUR_AMPLIFY  100000

//#define MAX_CURRENT_IC_MODULE_NUM   1

typedef enum
{
    CURRENTIF_ERR_OK,
    CURRENTIF_ERR_IICACK = ERR_CDD_CURRENTIF_BASE,
    CURRENTIF_ERR_WRCONFIG,
    CURRENTIF_ERR_WRGAINOFFSET,
    CURRENTIF_ERR_WRMASK,
    CURRENTIF_ERR_WRCC,
    CURRENTIF_ERR_CLBCUR,
    CURRENTIF_ERR_RDCUR,
    CURRENTIF_ERR_RDARRAY,
    CURRENTIF_ERR_CHECKREG,
    CURRENTIF_ERR_INT,
    CURRENTIF_ERR_POWERVOLT,
    CURRENTIF_ID_INVALID,
} eCurrentIf_ErrCodeType;

typedef enum
{
    CURRENTIF_CS5460A_1,

} eCurrentIf_DeviceIdType;

/* Chip Channel  Config */
typedef struct
{
   uByte Cmd;
   uByte  Conf[3];
}sCurrentIf_ConfigFormatType;

typedef struct
{
  sCurrentIf_ConfigFormatType CurChnOffsetConf;  // current channel offset config
  sCurrentIf_ConfigFormatType CurChnGainConf;    // current channel gain config
  sCurrentIf_ConfigFormatType VoltChnOffsetConf;  // Voltage channel offset config 
  sCurrentIf_ConfigFormatType VoltChnGainConf;    // Voltage channel gain config
  sCurrentIf_ConfigFormatType ChipCycle;              // Chip cycle config
}sCurrent_ChipConfigType;



extern GeneralReturnType CurrentIf_Init(eCurrentIf_DeviceIdType DeviceId, sCurrent_ChipConfigType  *RAMPTR ChipConfPtr); 	  /*initialize pins,data-directions*/
extern GeneralReturnType CurrentIf_ClbCur(eCurrentIf_DeviceIdType DeviceId, uByte Type, sCurrentIf_ConfigFormatType* RAMPTR ChnConfPtr);            /*calibrate 540*/
extern GeneralReturnType CurrentIf_RdCur(eCurrentIf_DeviceIdType DeviceId, uByte Type, uWord* CurAdPtr, uByte * FlgChgDch);			/*read current*/
GeneralReturnType CurrentIf_CheckStatusReg(eCurrentIf_DeviceIdType DeviceId);   /* check status register */
GeneralReturnType CurrentIf_Check_ConfigReg(eCurrentIf_DeviceIdType DeviceId, uByte* p_config);/* check config register */

#endif