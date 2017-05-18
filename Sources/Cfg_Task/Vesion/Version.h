// version.h

#ifndef VERSION_H
#define VERSION_H

#include "GeneralTypeDef.h"
#include "Version_Type.h" 
#include "Version_ModuleLcfg.h"

// For version 0x4640~0x465F
#define VERSION_ERR_POINT_NULL ERR_CONFIG_VERSION_BASE // 0x4640 
#define VERSION_ERR_MODULEID_UNROMAL (VERSION_ERR_POINT_NULL + 1) //0x4641
#define VERSION_ERR_MODULE_UN_EXIST  (VERSION_ERR_POINT_NULL + 2) // 0x4642
/*
 * Module Version Define 
 */
#define MODULE_VERSION_ID               1 
#define MODULE_VERSION_NAME             "VERSIONM"    //  platform name include                                       
#define VERSION_SW_MAJOR_VERSION         1     //  size is uByte,max number is 255                                            
#define VERSION_SW_MINOR_VERSION         1    //  size is uWord,max number is 65535                                           
#define VERSION_SW_REVISION_VERSION      4   //  size is uWord,max number is 65535                                               
#define VERSION_DEVELOP_STATUS           "" /* status include "beta" "rc" ""                                 
                                               *  "beta" ---- beta
                                               *  "rc"   ---- Release Candidate
                                               *  ""  or the blank or '\0' represent Release,display blank in monitor
                                               */  
                                          

	
GeneralReturnType Version_GetProjectVer(sVersion_CodeType * VerCodePtr);	
GeneralReturnType Version_GetPlatformVer(sVersion_CodeType * VerCodePtr);
GeneralReturnType Version_SearchModule(uWord * ModuleIdPtr,uWord *ModuleNumPtr);
GeneralReturnType Version_GetModuleVer(uWord ModuleId, sVersion_CodeType * VerCodePtr);
GeneralReturnType Version_GetPlatformHwVer(sVersion_CodeType * VerCodePtr);

/* Version Module Get Interface */
ModuleID_Type Version_GetVesionModuleVer(sVersion_CodeType * VerPtr);

#endif
