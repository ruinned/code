#include "Ecu.h"
#include "Nvm_Lcfg.h"
#include "ExtEE_Sck.h"
#include "ExtEE_Sda.h"
#include "ExtEE_Wp.h"

#include "ExternFlash_Sck.h"
#include "ExternFlash_Si.h"
#include "ExternFlash_Cs.h"
#include "ExternFlash_So.h"
#include "ExternFlash_Rst.h"

/* IO operation for extern EE below */
/*static void ExternEE_Sck_SetInput(void)
{
    //ExtEE_Sck_SetInput();  
}*/

/*static void ExternEE_Sck_SetOutput(void)
{
    //ExtEE_Sck_SetOutput();    
}*/

static void ExternEE_Sck_SetVal(void)
{
    ExtEE_Sck_SetVal(); 
}

static void ExternEE_Sck_ClrVal(void)
{
    ExtEE_Sck_ClrVal();  
}

static bool ExternEE_Sck_GetVal(void)
{
    return ExtEE_Sck_GetVal();    
}

static void ExternEE_Sda_SetInput(void)
{
    ExtEE_Sda_SetInput();  
}

static void ExternEE_Sda_SetOutput(void)
{
    ExtEE_Sda_SetOutput(); 
}

static void ExternEE_Sda_SetVal(void)
{
    ExtEE_Sda_SetVal(); 
}

static void ExternEE_Sda_ClrVal(void)
{
    ExtEE_Sda_ClrVal();  
}

static bool ExternEE_Sda_GetVal(void)
{
    return ExtEE_Sda_GetVal();   
}

static void ExternEE_Wp_SetInput(void)
{
    ExtEE_Wp_SetInput(); 
}

static void ExternEE_Wp_SetOutput(void)
{
    ExtEE_Wp_SetOutput();
}

static void ExternEE_Wp_SetVal(void)
{
    ExtEE_Wp_SetVal();  
}

static void ExternEE_Wp_ClrVal(void)
{
    ExtEE_Wp_ClrVal(); 
}

static bool ExternEE_Wp_GetVal(void)
{
    return ExtEE_Wp_GetVal();   
}

/* IO operation for extern flash below */
static void ExtFlash_Sck_SetInput(void)
{
    ExternFlash_Sck_SetInput();
}

static void ExtFlash_Sck_SetOutput(void)
{
    ExternFlash_Sck_SetOutput();
}

static void ExtFlash_Sck_SetVal(void)
{
    ExternFlash_Sck_SetVal();
}

static void ExtFlash_Sck_ClrVal(void)
{
    ExternFlash_Sck_ClrVal();
}

static bool ExtFlash_Sck_GetVal(void)
{
    return ExternFlash_Sck_GetVal();
}

static void ExtFlash_Si_SetInput(void)
{
    ExternFlash_Si_SetInput();
}

static void ExtFlash_Si_SetOutput(void)
{
    ExternFlash_Si_SetOutput();
}

static void ExtFlash_Si_SetVal(void)
{
    ExternFlash_Si_SetVal();
}

static void ExtFlash_Si_ClrVal(void)
{
    ExternFlash_Si_ClrVal();
}

static bool ExtFlash_Si_GetVal(void)
{
    return ExternFlash_Si_GetVal();
}

static void ExtFlash_Cs_SetInput(void)
{
    ExternFlash_Cs_SetInput();
}

static void ExtFlash_Cs_SetOutput(void)
{
    ExternFlash_Cs_SetOutput();
}

static void ExtFlash_Cs_SetVal(void)
{
    ExternFlash_Cs_SetVal();
}

static void ExtFlash_Cs_ClrVal(void)
{
    ExternFlash_Cs_ClrVal();
}

static bool ExtFlash_Cs_GetVal(void)
{
    return ExternFlash_Cs_GetVal();
}

static void ExtFlash_So_SetInput(void)
{
    ExternFlash_So_SetInput();
}

static void ExtFlash_So_SetOutput(void)
{
    ExternFlash_So_SetOutput();
}

static void ExtFlash_So_SetVal(void)
{
    ExternFlash_So_SetVal();
}

static void ExtFlash_So_ClrVal(void)
{
    ExternFlash_So_ClrVal();
}

static bool ExtFlash_So_GetVal(void)
{
    return ExternFlash_So_GetVal();
}

static void ExtFlash_Rst_SetInput(void)
{
    ExternFlash_Rst_SetInput();
}

static void ExtFlash_Rst_SetOutput(void)
{
    ExternFlash_Rst_SetOutput();
}

static void ExtFlash_Rst_SetVal(void)
{
    ExternFlash_Rst_SetVal();
}

static void ExtFlash_Rst_ClrVal(void)
{
    ExternFlash_Rst_ClrVal();
}

static bool ExtFlash_Rst_GetVal(void)
{
    return ExternFlash_Rst_GetVal();
}

const sCpuEE_OperationType CpuEE_Operation = 
{
    CPUEE_Init,
    CPUEE_SetByte,
    CPUEE_GetByte,
    CPUEE_SetWord,
    CPUEE_SetLong,
};

const sExternEE_OperationType ExternEE_Operation = 
{
    {
        NULL,//ExternEE_Sck_SetInput,
        NULL,//ExternEE_Sck_SetOutput,
        ExternEE_Sck_SetVal,
        ExternEE_Sck_ClrVal,
        ExternEE_Sck_GetVal,
    },
    
    {
        ExternEE_Sda_SetInput,
        ExternEE_Sda_SetOutput,
        ExternEE_Sda_SetVal,
        ExternEE_Sda_ClrVal,
        ExternEE_Sda_GetVal,
    },
    
    {
        ExternEE_Wp_SetInput,
        ExternEE_Wp_SetOutput,
        ExternEE_Wp_SetVal,
        ExternEE_Wp_ClrVal,
        ExternEE_Wp_GetVal,
    },
};

const sExternFlash_OperationType ExternFlash_Operation =
{
    {
        ExtFlash_Sck_SetInput,
        ExtFlash_Sck_SetOutput,
        ExtFlash_Sck_SetVal,
        ExtFlash_Sck_ClrVal,
        ExtFlash_Sck_GetVal,
    },

    {
        ExtFlash_Si_SetInput,
        ExtFlash_Si_SetOutput,
        ExtFlash_Si_SetVal,
        ExtFlash_Si_ClrVal,
        ExtFlash_Si_GetVal,
    },

    {
        ExtFlash_Cs_SetInput,
        ExtFlash_Cs_SetOutput,
        ExtFlash_Cs_SetVal,
        ExtFlash_Cs_ClrVal,
        ExtFlash_Cs_GetVal,
    },     
    
    {
        ExtFlash_So_SetInput,
        ExtFlash_So_SetOutput,
        ExtFlash_So_SetVal,
        ExtFlash_So_ClrVal,
        ExtFlash_So_GetVal,
    },

    {
        ExtFlash_Rst_SetInput,
        ExtFlash_Rst_SetOutput,
        ExtFlash_Rst_SetVal,
        ExtFlash_Rst_ClrVal,
        ExtFlash_Rst_GetVal,

    },
};