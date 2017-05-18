#if 1

/* NvmIf.c  */
#include "Ecu.h"
#include "NvmIf.h"
#include "CpuEEIf.h"
#include "ExternEEIf.h"
#include "ExternFlashIf.h"

const sNvmIf_OperationType NvmIf_Operation[] =
{
    {
        CpuEEIf_Init,
        CpuEEIf_WriteEnable,
        CpuEEIf_WriteDisable,
        CpuEEIf_SetByte,
        CpuEEIf_SetWord,
        CpuEEIf_SetLong,
        CpuEEIf_GetByte,
        CpuEEIf_WriteArray,
        CpuEEIf_ReadArray,
    },

    {
        ExternEEIf_Init,
        ExternEEIf_WriteEnable,
        ExternEEIf_WriteDisable,
        ExternEEIf_SetByte,
        ExternEEIf_SetWord,
        ExternEEIf_SetLong,
        ExternEEIf_GetByte,
        ExternEEIf_WriteArray,
        ExternEEIf_ReadArray
    }
};

const sNvmIf_FlashOperationType NvmIf_FlashOperation =
{
    ExternFlashIf_Init,
    ExternFlashIf_ReadStatusReg,
    ExternFlashIf_WriteBuf,
    ExternFlashIf_ReadBuf,
    ExternFlashIf_WritePage,
    ExternFlashIf_ReadPage,
    ExternFlashIf_CheckPage,
    ExternFlashIf_ChipErase,
    ExternFlashIf_PageErase,
    ExternFlashIf_BlockErase,
    ExternFlashIf_WriteData,
    ExternFlashIf_ReadData,
    ExternFlashIf_InIdle,
    ExternFlashIf_BlockErase,
    ExternFlashIf_ReadBuf,
};


#else

/* NvmIf.c  */
#include "Ecu.h"
#include "NvmIf.h"
#include "CpuEEIf.h"
#include "ExternEEIf.h"
#include "ExternFlashIf.h"

const sNvmIf_OperationType NvmIf_Operation[] =
{
    {
        CpuEEIf_Init,
        CpuEEIf_WriteEnable,
        CpuEEIf_WriteDisable,
        CpuEEIf_SetByte,
        CpuEEIf_SetWord,
        CpuEEIf_SetLong,
        CpuEEIf_GetByte,
        CpuEEIf_WriteArray,
        CpuEEIf_ReadArray,
    },

    {
        ExternEEIf_Init,
        ExternEEIf_WriteEnable,
        ExternEEIf_WriteDisable,
        ExternEEIf_SetByte,
        ExternEEIf_SetWord,
        ExternEEIf_SetLong,
        ExternEEIf_GetByte,
        ExternEEIf_WriteArray,
        ExternEEIf_ReadArray
    }
};

const sNvmIf_FlashOperationType NvmIf_FlashOperation =
{
    ExternFlashIf_Init,
    ExternFlashIf_ReadStatus,
    ExternFlashIf_WritePage,
    ExternFlashIf_ReadPage,
    ExternFlashIf_NormalRead,
    ExternFlashIf_CheckPage,
    ExternFlashIf_InIdle,
    ExternFlashIf_SectorErase,
    ExternFlashIf_ChipErase
};


#endif