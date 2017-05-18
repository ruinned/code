#ifndef CURRENTDET_H
#define CURRENTDET_H

#include "GeneralTypeDef.h"

// define cur sensor for energe thrhd
#define FLA_50A_ENERGE_THRHD  2248
#define FLA_100A_ENERGE_THRHD 1124
#define FLA_200A_ENERGE_THRHD 562
#define FLA_300A_ENERGE_THRHD 375
#define FLA_400A_ENERGE_THRHD 281
#define FLA_500A_ENERGE_THRHD 225
#define FLA_600A_ENERGE_THRHD 188


/* define return error */
// For CurrentDet 0x4160~0x417F
#define ERR_CURDET_CHGORDCH_FLAG_WRONG   ERR_APP_SENSOR_CURRENTDET_BASE
#define ERR_CURDET_PARA_STORE_TO_EE     (ERR_CURDET_CHGORDCH_FLAG_WRONG + 1)
#define ERR_CURVDET_HDERR_NOCALIB       (ERR_CURDET_CHGORDCH_FLAG_WRONG + 2)

/* define hardware error detail */
#define CURDET_HDERR_INIT      0x0001
#define CURDET_HDERR_PARAMETER_UNNORMAL       0x0002
#define CURDET_HDERR_NOT_UPDATE    0x0004
#define CURDET_HDERR_NOT_CALIB   0x0008
#define CURDET_HDERR_MEASURE_LINE_OPEN   0x0010
#define CURDET_HDERR_ZERO_DRIFT   0x0020
#define CURDET_HDERR_SENSOR_REVERSE 0x0040  // just for LEM
#define CURDET_HDERR_RD_INSCUR_FAILED  0x0080 // new,need add in protocol
#define CURDET_HDERR_RD_AVGCUR_FAILED  0x0100 // new,need add in protocol

extern void CurDet_Init(void);
extern void CurDet_Count(void);
extern void CurDet_Main(void);
extern void CurDet_InitCurDet(void);

/* interface for calibration */
extern GeneralReturnType CurDet_UpdateGainAndOffset(uWord Gain, uLWord Offset);
extern GeneralReturnType CurDet_ClbCurCurChnOffset(uLWord * OffsetPtr);
extern GeneralReturnType CurDet_ClbCurCurChnGain(uLWord * GainPtr);
extern GeneralReturnType CurDet_ClbCurVolChnOffset(uLWord * OffsetPtr);
extern GeneralReturnType CurDet_ClbCurVolChnGain(uLWord * GainPtr);
extern GeneralReturnType CurDet_UpdateAhCalcThrhd(uByte ChgOrDch, uLWord Thrhd);
extern void CurDet_EnableCalcAh(void);
extern void CurDet_DisableCalcAh(void);

#endif  /* CURRENTDET_H */