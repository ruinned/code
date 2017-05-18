// EcuDef.h
// This file is the top config for EV05 platform

/* 
 * ECU_CATEGORY:
 * define for config program to use different ecu
   0: BCU_D --- Battery Control Unit- Distributed
   1: BCU_C --- Battery Control Unit- Centralized
   2: BMU --- Battery Measurement Unit
   3: BMU_R --- Battery Measurement Unit - Replace 
 * Relative Macro is in EcuConf.h such as:
   #define M_BCU_D 0
   #define M_BCU_C 1
   #define M_BMU   2
   #define M_BMU_R 3  
 */ 
#define ECU_CATEGORY 0 

/*
 * PROJECT VERSION
 * define Version of project which use this platform,
 * These define will be used in Version_ProjectDef.h
 */
#define ACTUAL_PROJECT_NAME "BHN02AMS7408" /*  project name = PROJECT CODE  + Old version number 
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
#define ACTUAL_PROJECT_SW_MAJOR_VERSION       1  //  size is uByte,max number is 255                                            
#define ACTUAL_PROJECT_SW_MINOR_VERSION       0 //  size is uWord,max number is 65535                                           
#define ACTUAL_PROJECT_SW_REVISION_VERSION    0 //  size is uWord,max number is 65535                                               
#define ACTUAL_PROJECT_DEVELOP_STATUS          "beta" /* status include "beta" "rc" ""                                 
                                              *  "beta" ---- beta
                                              *  "rc"   ---- Release Candidate
                                              *  ""  or the blank or '\0' represent Release,display blank in monitor
                                              */

