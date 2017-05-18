#include "PE_cfg.h"
#ifdef PE_CBK_EN
#ifndef BCUDVERSION_H
#define BCUDVERSION_H

#include "GeneralTypeDef.h"
#include "Version_Type.h" 


/*
 * Module Version Define 
 */
#define BCUDMCAL_MODULE_VERSION_ID               6 
#define BCUDMCAL_MODULE_VERSION_NAME             "BCUDMACL"    //  platform name include                                       
#define BCUDMCAL_VERSION_SW_MAJOR_VERSION         1     //  size is uByte,max number is 255                                            
#define BCUDMCAL_VERSION_SW_MINOR_VERSION         1    //  size is uWord,max number is 65535                                           
#define BCUDMCAL_VERSION_SW_REVISION_VERSION      1   //  size is uWord,max number is 65535                                               
#define BCUDMCAL_VERSION_DEVELOP_STATUS           "" /* status include "beta" "rc" ""                                 
                                               *  "beta" ---- beta
                                               *  "rc"   ---- Release Candidate
                                               *  ""  or the blank or '\0' represent Release,display blank in monitor
                                               */  
                                          
/* Version Module Get Interface */
ModuleID_Type Version_GetVesionBcudMcalModuleVer(sVersion_CodeType * VerPtr);

#endif
#endif