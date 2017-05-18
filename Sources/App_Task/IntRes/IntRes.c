#include "IntRes.h"

#ifdef USE_INTRES_AB
#include "IntRes-AB.h"
#endif


/* module initial */
void IntRes_Init(void)
{
    /* intres module initial */
    IntRes_ModuleInit();
}

/* module count call back */
void IntRes_MainCountCbk(void)
{
    /* count every 5ms */
    IntRes_ModuleCountCbk();
}

/* main call back */
void IntRes_MainCbk(void)
{
    /* intres estimate process */
    IntRes_ModuleCbk();
}