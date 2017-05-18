// Version_HardwareDef.c
#include "Version_HardwareDef.h"

const sVersion_CodeType PlatformHwVerDef =
{
  HARDWARE_NAME,
  PLATFORM_HW_MAJOR_VERSION,
  PLATFORM_HW_MINOR_VERSION,
  PLATFORM_HW_REVISION_VERSION,
  0,
  0,
  PLATFORM_HW_DEVELOP_STATUS
};

const sVersion_CodeType * PlatformHwVerDefPtr = &PlatformHwVerDef;

