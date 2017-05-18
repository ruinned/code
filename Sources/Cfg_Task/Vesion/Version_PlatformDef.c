// Version_PlatformDef.c
#include "Version_PlatformDef.h"

/*
 * define for platform version
 */
#define PLATFORM_NAME               "BHN01"    //  platform name include                                     
#define PLATFORM_SW_MAJOR_VERSION       1     //  size is uByte,max number is 255                                            
#define PLATFORM_SW_MINOR_VERSION        1    //  size is uWord,max number is 65535                                           
#define PLATFORM_SW_REVISION_VERSION     6   //  size is uWord,max number is 65535                                               
#define PLATFORM_DEVELOP_STATUS          "" /* status include "beta" "rc" ""                                 
                                               *  "beta" ---- beta
                                               *  "rc"   ---- Release Candidate
                                               *  ""  or the blank or '\0' represent Release,display blank in monitor
                                               */ 

const sVersion_CodeType PlatformVerDef =
{
  PLATFORM_NAME,
  PLATFORM_SW_MAJOR_VERSION,
  PLATFORM_SW_MINOR_VERSION,
  PLATFORM_SW_REVISION_VERSION,
  0,
  0,
  PLATFORM_DEVELOP_STATUS
};

const sVersion_CodeType * PlatformVerDefPtr = &PlatformVerDef;

