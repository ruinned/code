 // ClockIf.h
#ifndef CLOCKIF_H
#define CLOCKIF_H

#include "GeneralTypeDef.h"
#include "ExternTimer_Scl.h"
#include "ExternTimer_Sda.h"
#include "ExternTimer_Int.h"
#include "Ecu.h"

#define SD2405_WR   0x64
#define SD2405_RD   0x65

typedef enum
{
    CLOCKIF_ERR_OK,
    CLOCKIF_ERR_IICACK = ERR_CDD_CLOCKIF_BASE,
    CLOCKIF_ERR_WRTIME,
    CLOCKIF_ERR_RDTIME,
} eClockIf_ErrCodeType;

typedef struct
{
    uByte year;
    uByte month;
    uByte day;
    uByte hour;
    uByte minute;
    uByte second;
}sClockIf_TimeType;

extern void ClockIf_Init(void);
extern GeneralReturnType ClockIf_ReadTime(sClockIf_TimeType *time);
extern GeneralReturnType ClockIf_WriteTime(sClockIf_TimeType *time);

#endif 