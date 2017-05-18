// EE.h
#ifndef EE_H
#define EE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "SysConf.h"

#include "Version_Type.h" 

/*
 * Module Version Define 
 */
#define MODULE_EE_ID               112 
#define MODULE_EE_NAME             "EE-E" //  platform name include                                      
#define EE_SW_MAJOR_VERSION        1      //  size is uByte,max number is 255                                            
#define EE_SW_MINOR_VERSION        1      //  size is uWord,max number is 65535                                           
#define EE_SW_REVISION_VERSION     0      //  size is uWord,max number is 65535                                               
#define EE_DEVELOP_STATUS          "" /* status include "beta" "rc" ""                                 
                                               *  "beta" ---- beta
                                               *  "rc"   ---- Release Candidate
                                               *  ""  or the blank or '\0' represent Release,display blank in monitor
                                               */

typedef enum
{
    CPUEE,
    EXTERNEE,
    BOTHEE,
} eEE_DeviceType;

// errorcode define
typedef enum
{
    EE_ERR_OK,
    EE_ERR_CRC = ERR_DATA_EE_BASE,
    EE_ERR_DEVICE,
    EE_ERR_COMPARE_ARRAY,
    EE_ERR_READ_ARRAY,
    EE_ERR_WRITE_ARRAY,
    EE_ERR_READBLOCK1,
    EE_ERR_READBLOCK3,
    EE_ERR_WRITEBLOCK1,
    EE_ERR_WRITEBLOCK3,
    EE_ERR_DATA_ADDR,
    EE_ERR_DATA_LENGTH,
    EE_ERR_GET_POINTER,
    EE_ERR_GET_ADDR,
    EE_ERR_INITEE,
    EE_ERR_LOADVAR,
    EE_ERR_SAVEVAR,
}eEE_ErrCodeType;

/* define hardware error detail */
#define EE_HDERR_INIT      0x0001
#define EE_HDERR_FULL      0x0002
#define EE_HDERR_WRITE     0x0004
#define EE_HDERR_READ      0x0008

extern uByte EE_GetEeStatus(void);
extern void EE_UpdateEeStatus(uByte status);

extern GeneralReturnType EE_SaveVarForCalib(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo);
extern GeneralReturnType EE_SaveVar(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo);
extern GeneralReturnType EE_LoadVar(eSysConf_ParameterNOType ParameterNo);
extern void EE_Dealy5msForSaveVar(void);
extern void EE_Init(void);
extern void EE_MainCountCbk(void);
extern void EE_MainCbk(void);
extern void EE_UpdateEeErrFlag(unsigned char flag);
//extern ModuleID_Type EE_GetVesionModuleVer(sVersion_CodeType * VerPtr);

#ifdef EE_DEBUG
extern void EE_Test(void);
#endif

#endif    /*EE_H */