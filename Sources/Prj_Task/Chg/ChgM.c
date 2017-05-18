// ChgM.c

#include "ChgM.h"

#ifdef USE_CHGM_B
#include"ChgM-B.h"
#include"GbChgM-B.h"
#endif

/* Module initial */
void ChgM_Init(void)
{
    /* ChgM module initial */
    ChgM_ModuleInit();
    
#ifdef ECU_M_ENABLE_GBCHGIF
    /* GB charge module init, if used */
    GbChgM_ModuleInit();
#endif        
}

/* Module count call back */
void ChgM_MainCountCbk(void)
{
    /* Count every 5ms */
    ChgM_ModuleCountCbk();
    
    /* GB charge control counter */
#ifdef ECU_M_ENABLE_GBCHGIF
    GbChgM_ModuleCountCbk();
#endif
}

/* Main call back */
void ChgM_MainCbk(void)
{
    ChgM_ModuleCbk();
    /* GB charge control */
#ifdef ECU_M_ENABLE_GBCHGIF
    GbChgM_ModuleCbk();
#endif
}
