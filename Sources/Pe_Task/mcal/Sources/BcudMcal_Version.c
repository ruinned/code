#include "PE_cfg.h"
#ifdef PE_CBK_EN
#include "BcudMcal_Version.h"

const sVersion_CodeType Vesion_BcudMcalVerDef =
{
  BCUDMCAL_MODULE_VERSION_NAME,
  BCUDMCAL_VERSION_SW_MAJOR_VERSION,
  BCUDMCAL_VERSION_SW_MINOR_VERSION,
  BCUDMCAL_VERSION_SW_REVISION_VERSION,
  0,
  0,
  BCUDMCAL_VERSION_DEVELOP_STATUS
};

/* Version Get Interface of Version Module  */
ModuleID_Type Version_GetVesionBcudMcalModuleVer(sVersion_CodeType * VerPtr)
{      
    *VerPtr = Vesion_BcudMcalVerDef;
    return BCUDMCAL_MODULE_VERSION_ID;   
}
#endif