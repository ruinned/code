// Version_ProjectDef.h 

#ifndef VERSION_PROJECTDEF_H
#define VERSION_PROJECTDEF_H

#include "Version_Type.h"
#include "EcuDef.h"
/*
 * define for project version
 */
#define PROJECT_NAME ACTUAL_PROJECT_NAME /*  project name = PROJECT CODE  + Old version number 
                                     *  Such as: B32601MS3409  "B32601" + "MS3409"
                                     *     The first charactor in old version number:
                                     *        M ---- bcu
                                     *        S ---- bmu
                                     *        H ---- hmu
                                     *        D ---- Display
                                     *     The second charactor:
                                     *        S ---- software
                                     *     The rest of number: represent time,such as 
                                     *        3409 ---- 2013-4-09
                                     */ 
#define PROJECT_SW_MAJOR_VERSION       ACTUAL_PROJECT_SW_MAJOR_VERSION  //  size is uByte,max number is 255                                            
#define PROJECT_SW_MINOR_VERSION       ACTUAL_PROJECT_SW_MINOR_VERSION //  size is uWord,max number is 65535                                           
#define PROJECT_SW_REVISION_VERSION    ACTUAL_PROJECT_SW_REVISION_VERSION //  size is uWord,max number is 65535                                               
#define PROJECT_DEVELOP_STATUS          ACTUAL_PROJECT_DEVELOP_STATUS /* status include "beta" "rc" ""                                 
                                              *  "beta" ---- beta
                                              *  "rc"   ---- Release Candidate
                                              *  ""  or the blank or '\0' represent Release,display blank in monitor
                                              */
extern const sVersion_CodeType * ProjectVerDefPtr;

#endif

