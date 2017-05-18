#ifndef SUMVDET_H
#define SUMVDET_H

#include "GeneralTypeDef.h"
#include "SumvIf.h"

#include "Version_Type.h"


#define SUMVDET_NUM   6       /* inner sumv, inner sumv backup, extern sumv */
#define SUMVOLT_GAIN  1000    /* make sure sumv unit */
#define SUMV_1000V    10000   /* 1000V */
#define SUMV_ERR_CNT  5       /* sumv error count */



/* define defaule gain and offset */
#define INNERSUMV_DEFAULT_GAIN     478
#define INNERSUMV_DEFAULT_OFFSET   0

#define EXTERN_INNERBAK_SUMV_DEFAULT_GAIN     645
#define EXTERN_INNERBAK_SUMV_DEFAULT_OFFSET   0

/* define hardware error detail */
#define SUMVDET_HDERR_INIT      0x0001
#define SUMVDET_HDERR_COM       0x0002
#define SUMVDET_HDERR_UPDATE    0x0004
#define SUMVDET_HDERR_NOCALIB   0x0008
#define SUMVDET_HDERR_NEGVOLT   0x0010
#define SUMVDET_HDERR_OUTRANG   0x0020
#define SUMVDET_HDERR_ADCONVERT 0x0040
#define SUMVDET_HDERR_PTR_NULL  0x0080
#define SUMVDET_HDERR_IDNO_OUTRANGE 0x0100
/* define return error */
#define ERR_SUMVDET_PARA_UPDATE    ERR_APP_SENSOR_SUMVDET_BASE
#define ERR_SUMVDET_GET_SUMVAD     (ERR_APP_SENSOR_SUMVDET_BASE + 1)

extern void SumvDet_Init(void);
extern void SumvDet_MainCountCbk(void);
extern void SumvDet_MainCbk(void);
 
 /* interface for calibration */
extern GeneralReturnType SumvDet_GetSumvAd(eSumv_IdNoType Sumv_IdNo, uWord * Sumv_Ad);
extern GeneralReturnType SumvDet_UpdateGainAndOffset(eSumv_IdNoType Sumv_IdNo,uWord Gain, uLWord Offset);
extern void SumvDet_UpdateRunStep(eSumv_IdNoType Sumv_IdNo);

#endif
     