// CellvDet.h
#ifndef CELLVDET_H
#define CELLVDET_H

#include "GeneralTypeDef.h"
#include "CellvIf.h"

/* define hardware error detail */
#define CELLVDET_HDERR_INIT      0x0001
#define CELLVDET_HDERR_COM       0x0002
#define CELLVDET_HDERR_UPDATE    0x0004
#define CELLVDET_HDERR_NOCALIB   0x0008
#define CELLVDET_HDERR_NEGVOLT   0x0010
#define CELLVDET_HDERR_OPENWIRE  0x0020
#define CELLVDET_HDERR_ADCONVERT 0x0040

#define CELLVDET_MAX_WORD  0xFFFE

#define VOLT_RD_CNT   20           /* cell voltage read filter counter */
#define ERR_READCELLV_CNT 100       /* cell voltage read error counter */
#define BALANCE_CHECK_FAULT_VOLT 18000 /* 1.8V */


#define  LT_TEMPER_DET_NTC_OPEN_ERR   0x1000
#define  LT_TEMPER_DET_NTC_SHORT_ERR  0x4000

#define LT_TEMPER_DET_OPEN_AD_VAL  0x7148
#define LT_TEMPER_DET_SHORT_AD_VAL 0x960 

#define LT_TEMPER_NTC_OPEN_ERR_VALUE  0xFB
#define LT_TEMPER_NTC_SHORT_ERR_VALUE 0xFC 

#define LT_TEMPER_DET_NTC_OPEN_ERR   0x1000
#define LT_TEMPER_DET_NTC_SHORT_ERR  0x4000



extern void CellvDetDet_Init(void);
extern void CellvDetDet_MainCountCbk(void);
extern void CellvDet_MainCbk(void);



#endif