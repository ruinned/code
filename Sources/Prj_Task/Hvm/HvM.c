// HvM.c
#include "HvM.h"



/* module count call back */
void HvM_MainCountCbk(void)
{
    HvM_ModuleCountCbk();
}

/* main call back */
void HvM_MainCbk(void)
{
    HvM_ModuleCbk();
}
