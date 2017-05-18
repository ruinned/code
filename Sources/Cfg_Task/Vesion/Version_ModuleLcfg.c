// Version_ModuleLcfg.c
#include "Version_ModuleLcfg.h"

// include module head file here
#include "Sch_Include.h"
#include "Bsw.h"
#include "Cdd.h"
#include "DataAnalysis.h"
#include "BalM.h"
#include "AppDo.h"
#include "SensorAi.h"
#include "SensorDi.h"
#include "RunModem.h"
//#include "IntRes.h"
#include "BcudMcal_Version.h"
#include "Xgate_Config.h"
#include "GbChg_IfDet.h"
#include "ThM.h"



// need add module get version interface
const sVersion_GetModuleVerOperationType GetModuleVerOperation[VERSION_MODULE_NUMBER] =
{
  /* 0 Not Define */NULL,
  /* 1 */Version_GetVesionModuleVer,
  /* 2 */NULL,// For EV03, keep NULL
  /* 3 */NULL,
  /* 4 */NULL,
  /* 5 */NULL,

  /* 6 */Version_GetVesionBcudMcalModuleVer,

  /* 7 */NULL, //Sch_GetVesionModuleVer,
  /* 8 */NULL, //Bsw_GetVesionModuleVer,
  /* 9 */NULL, //Cdd_GetVesionModuleVer,
  
  /* 10 */NULL,//DataLayer_GetVesionModuleVer,    
  /* 11 */NULL, //DataAnalysis_GetVesionModuleVer,    
  /* 12 */NULL, //Soc_GetVesionModuleVer,
  /* 13 */NULL, //Soh_GetVesionModuleVer,
  /* 14 */NULL, //Sof_GetVesionModuleVer,

  /* 15 */NULL,//IntRes_GetVesionModuleVer,
  /* 16 */NULL, //IntCan_GetVesionModuleVer,
  /* 17 */NULL,//CarCan_GetVesionModuleVer,
  /* 18 */NULL,//ChgCan_GetVesionModuleVer,
  /* 19 */NULL,//Thm_GetVesionModuleVer,

  /* 20 */NULL,//ChgM_GetVesionModuleVer,
  /* 21 */NULL,//HvM_GetVesionModuleVer,  
  /* 22 */NULL, //LvM_GetVesionModuleVer,
  /* 23 */NULL, //BalM_GetVesionModuleVer,
  /* 24 */NULL,
  /* 25 */NULL, //RunModem_GetVesionModuleVer,
  /* 26 */NULL, //AppRelay_GetVesionModuleVer,
  /* 27 */NULL, //AppDo_GetVesionModuleVer,
  /* 28 */NULL, //GbChgDet_GetVesionModuleVer,

  /* 29 */NULL, //SupplyPowDet_GetVesionModuleVer,
  
  /* 30 */NULL, //SumvDet_GetVesionModuleVer,   
  /* 31 */NULL, //CurDet_GetVesionModuleVer,   
  /* 32 */NULL,
  /* 33 */NULL,
  /* 34 */NULL, //IsoDet_GetVesionModuleVer,
  /* 35 */NULL,//SensorAi_GetVesionModuleVer,           
  /* 36 */NULL, //SensorDi_GetVesionModuleVer,
  /* 37 */NULL, //SysConf_GetVesionModuleVer,
  /* 38 */NULL, //EcuConf_GetVesionModuleVer, 
  /* 39 */NULL, //Xgate_GetVesionModuleVer  
};