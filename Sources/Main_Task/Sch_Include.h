// Sch_Include.h
#ifndef SCH_INCLUDE_H
#define SCH_INCLUDE_H


/* Common module for BCU_D, BCU_D, BMU, BMU_R */
#include "SchIf.h"              /* SchIf Module */
#include "SysConf.h"            /* Config --- SysConf Module */
#include "Version.h"            /* Config --- Version Module */
#include "Ecu.h"                /* Bsw --- Ecu Module, Include EcuConf Module */
#include "WdgIf.h"               /* Bsw --- Wdgm Module */
#include "SupplyPowDet.h"       /* Sensors --- SupplyPowDet Module */
#include "AppRelay.h"           /* Actors --- Relay Module */
#include "IntCan.h"             /* Protocols --- IntCan Module, Inlcude Bsw CanIf.h */
#include "BatterySt.h"          /* DataAnalysis --- BatterySt Module */
#include "BmsSysSt.h"           /* DataAnalysis --- BmsSysSt Module */ 
#include "BatterySysst.h"       /* DataAnalysis --- BatterySysst Module */ 
#include "ErrorAlarm.h"         /* DataAnalysis --- ErrorAlarm Module */  
#include "EE.h"                 /* DataAnalysis --- EE module */
#include "Lvm.h"                /* Manage --- Lvm Module */
#include "Thm.h"                /* Manage --- Thm Module */
#include "Balm.h"               /* Manage --- Thm Module */
#include "RunModem.h"           /* Manage --- RunModem Module */
#include "Version.h"
#include "Project.h"

#include "CurDet.h"             /* Sensors --- CurDet Module */
#include "SumvDet.h"            /* Sensors --- SumvDet Module */
#include "ChargeSt.h"           /* DataAnalysis --- ChargeSt Module */
#include "Chgm.h"               /* Manage --- Chgm Module */
#include "SOC.h"                /* Soc --- Soc Module */

#include "IsoDet.h"             /* Sensors --- IsoDet Module */
#include "ClockIf.h"            /* CDD --- ClockIf Module, special include !!!! */
#include "CarCan.h"             /* Protocols --- CarCan Module, Inlcude Bsw CanIf.h */
#include "ChgCan.h"             /* Protocols --- ChgCan Module, Inlcude Bsw CanIf.h */
#include "DataStore.h"          /* DataAnalysis --- DataStore module */
#include "Hvm.h"                /* Manage --- Hvm Module */
#include "SOH.h"                /* SOH --- SOH Module */
#include "SOF.h"                /* SOF --- SOF Module */
#include "IntRes.h"             /* IntRes --- IntRes Module */
#include "IocIf.h"

#ifdef ECU_M_ENABLE_XGATE 
#include "Xgate_StartIf.h"
#endif

#include "CellvDet.h"
#include "CellvIf.h"

/* Version Module Get Interface */
//ModuleID_Type DataLayer_GetVesionModuleVer(sVersion_CodeType * VerPtr); 

#endif /* SCH_INCLUDE_H  */