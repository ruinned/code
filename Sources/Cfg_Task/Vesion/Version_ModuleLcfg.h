// Version_ModuleLcfg.h
#ifndef VERSION_MODULELCFG_H
#define VERSION_MODULELCFG_H

#include "Version_Type.h"  

typedef struct
{
 ModuleID_Type ( *GetVerPtr)(const sVersion_CodeType * VerCodePtr); 
}sVersion_GetModuleVerOperationType;
   
  
#define VERSION_MODULE_NUMBER 40 // include 0 -- NOT define   
extern const sVersion_GetModuleVerOperationType GetModuleVerOperation[VERSION_MODULE_NUMBER];                                           

#endif

