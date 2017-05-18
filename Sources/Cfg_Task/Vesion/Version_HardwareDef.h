// Version_HardwareDef.h
#ifndef VERSION_HARDWAREDEF_H
#define VERSION_HARDWAREDEF_H

#include "Version_Type.h"
#include "Ecu.h"
/*
 * define for platform version
 */

#define HARDWARE_NAME                  "BHN02AMH6B28"   //  hardware name for bcu d                                      

#define PLATFORM_HW_MAJOR_VERSION        1     //  size is uByte,max number is 255                                            
#define PLATFORM_HW_MINOR_VERSION        0    //  size is uWord,max number is 65535                                           
#define PLATFORM_HW_REVISION_VERSION     0   //  size is uWord,max number is 65535                                               
#define PLATFORM_HW_DEVELOP_STATUS       "rc" /* status include "beta" "rc" ""                                 
                                               *  "beta" ---- beta
                                               *  "rc"   ---- Release Candidate
                                               *  ""  or the blank or '\0' represent Release,display blank in monitor
                                               */ 
                                              
extern const sVersion_CodeType * PlatformHwVerDefPtr;                                            

#endif

