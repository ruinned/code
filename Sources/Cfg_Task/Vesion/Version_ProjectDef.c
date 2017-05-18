// Version_ProjectDef.c
#include "Version_ProjectDef.h"

const sVersion_CodeType ProjectVerDef =
{
  PROJECT_NAME,
  PROJECT_SW_MAJOR_VERSION,
  PROJECT_SW_MINOR_VERSION,
  PROJECT_SW_REVISION_VERSION,
    0,
    0,
  PROJECT_DEVELOP_STATUS
};

const sVersion_CodeType * ProjectVerDefPtr = &ProjectVerDef;

