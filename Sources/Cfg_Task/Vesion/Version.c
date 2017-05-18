// version.c
#include "Version.h"
#include "Version_PlatformDef.h"
#include "Version_ProjectDef.h"
#include "GeneralTypeDef.h"
#include "Version_ModuleLcfg.h"
#include "Version_HardwareDef.h"

GeneralReturnType Version_GetProjectVer(sVersion_CodeType * VerCodePtr)
{
    uByte i;
    const sVersion_CodeType * VerDefPtr = ProjectVerDefPtr;

    if(NULL == VerCodePtr)
    {
        return ERR_FAILED;
    }
    for(i = 0; i < 14; i++)
    {
        VerCodePtr->Name[i] = VerDefPtr->Name[i];
    }
    VerCodePtr->SW_major_version = VerDefPtr->SW_major_version;
    VerCodePtr->SW_minor_version = VerDefPtr->SW_minor_version;
    VerCodePtr->SW_revision_version = VerDefPtr->SW_revision_version;

    for(i = 0; i < 7; i++)
    {
        VerCodePtr->Status[i] = VerDefPtr->Status[i];
    }

    return ERR_OK;
}

GeneralReturnType Version_GetPlatformVer(sVersion_CodeType * VerCodePtr)
{
    uByte i;
    const sVersion_CodeType * VerDefPtr = PlatformVerDefPtr;

    if(NULL == VerCodePtr)
    {
        return ERR_FAILED;
    }
    for(i = 0; i < 14; i++)
    {
        VerCodePtr->Name[i] = VerDefPtr->Name[i];
    }
    VerCodePtr->SW_major_version = VerDefPtr->SW_major_version;
    VerCodePtr->SW_minor_version = VerDefPtr->SW_minor_version;
    VerCodePtr->SW_revision_version = VerDefPtr->SW_revision_version;

    for(i = 0; i < 7; i++)
    {
        VerCodePtr->Status[i] = VerDefPtr->Status[i];
    }

    return ERR_OK;
}

GeneralReturnType Version_SearchModule(uWord * ModuleIdPtr,uWord *ModuleNumPtr)
{
   GeneralReturnType rslt;
   uWord wcnt,wnum;
   
   if((NULL == ModuleIdPtr) || (NULL == ModuleNumPtr))
   {
      return VERSION_ERR_POINT_NULL;
   }
   
   wnum = 0;
   for(wcnt = 0; wcnt < (sizeof(GetModuleVerOperation) / sizeof(sVersion_GetModuleVerOperationType)); wcnt++)
   {
     if(GetModuleVerOperation[wcnt].GetVerPtr != NULL)
     {
        *(ModuleIdPtr++) = wcnt;  // Get moduleid of exist module
        wnum++;
     }
   }
   *ModuleNumPtr = wnum;
   
   return ERR_OK;
}

GeneralReturnType Version_GetModuleVer(uWord ModuleId, sVersion_CodeType * VerCodePtr)
{
    GeneralReturnType rslt;
    uWord ModuleId_bak; 
    rslt = ERR_OK;
    if(NULL == VerCodePtr)
    {
        return VERSION_ERR_POINT_NULL;
    }
    
    if(ModuleId >= VERSION_MODULE_NUMBER)
    {
       return VERSION_ERR_MODULE_UN_EXIST;
    }
    
    // need check ModuleId here
    if(GetModuleVerOperation[ModuleId].GetVerPtr != NULL)
    {
    
       ModuleId_bak = (*(GetModuleVerOperation[ModuleId].GetVerPtr))(VerCodePtr);
      if(ModuleId_bak != ModuleId)
      {
          return VERSION_ERR_MODULEID_UNROMAL;
      }
    }
    else
    {
        return VERSION_ERR_MODULE_UN_EXIST;
    }
   return rslt;

   
}

GeneralReturnType Version_GetPlatformHwVer(sVersion_CodeType * VerCodePtr)
{
  if(VerCodePtr == NULL)
  {
     return VERSION_ERR_POINT_NULL;
  }
  *VerCodePtr = *PlatformHwVerDefPtr;
  
  return ERR_OK;
}   

const sVersion_CodeType Vesion_VerDef =
{
  MODULE_VERSION_NAME,
  VERSION_SW_MAJOR_VERSION,
  VERSION_SW_MINOR_VERSION,
  VERSION_SW_REVISION_VERSION,
  0,
  0,
  VERSION_DEVELOP_STATUS
};
const sVersion_CodeType * VesionVerDefPtr = &Vesion_VerDef;
/* Version Get Interface of Version Module  */
ModuleID_Type Version_GetVesionModuleVer(sVersion_CodeType * VerCodePtr)
{      
    if(VerCodePtr == NULL)
    {
      return NULL_MODULE_ID;
    }
    *VerCodePtr = Vesion_VerDef;
    return MODULE_VERSION_ID;   
}
