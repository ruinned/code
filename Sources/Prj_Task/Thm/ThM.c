// ThM.c
#include "ThM.h"

#ifdef USE_THM_A
#include"ThM-A.h"
#endif

#ifdef USE_THM_AB
#include"ThM-AB.h"
#endif



/* module initial */
void ThM_Init(void)
{
    ThM_ModuleInit();
}

/* module count call back */
void ThM_MainCountCbk(void)
{
    ThM_ModuleCountCbk();
}

/* main call back */
void ThM_MainCbk(void)
{
    ThM_ModuleCbk();
}

