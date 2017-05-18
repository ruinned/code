// Sumv_Lcfg.c
#include "Sumv_Lcfg.h"
#include "CPU_SUMV_RDY.h"
#include "CPU_SUMV_SCL.h"
#include "CPU_SUMV_SDA.h"
#include "SUMV_POW.h"
#include "CPU_SUMV_EXT_SCL.h"
#include "CPU_SUMV_EXT_SDA.h"

//scl
static void Sumv_Scl_SetVal(void)
{
    CPU_SUMV_SCL_PutVal(1);
}

static void Sumv_Scl_ClrVal(void)
{
    CPU_SUMV_SCL_PutVal(0);
}

/*static bool Cpu_Sumv_Scl_GetVal(bool val)
{
    return Cpu_Sumv_Scl_GetVal();
}*/

// sda
static void Sumv_Sda_SetIoDirIn(void)
{
    CPU_SUMV_SDA_SetDir(0);
}

static void Sumv_Sda_SetIoDirOut(void)
{
    CPU_SUMV_SDA_SetDir(1);
}

static void Sumv_Sda_SetVal(void)
{
    CPU_SUMV_SDA_PutVal(1);
}

static void Sumv_Sda_ClrVal(void)
{
    CPU_SUMV_SDA_PutVal(0);
}

static bool Sumv_Sda_GetVal(void)
{
    return  CPU_SUMV_SDA_GetVal() ;
}

// pow
/*static void Sumv_Pow_SetInput(bool val)
{
    Sumv_Pow_SetInput();
}

static void Sumv_Pow_SetOutput(bool val)
{
    Sumv_Pow_SetOutput();
}*/

static void Sumv_Pow_SetVal(void)
{
    SUMV_POW_PutVal(1);
}

static void Sumv_Pow_ClrVal(void)
{
    SUMV_POW_PutVal(0);
}

/*static bool Sumv_Pow_GetVal(bool val)
{
    return Sumv_Pow_GetVal();
}*/

//Rdy
/*static void Cpu_Sumv_Rdy_SetInput(bool val)
{
   return Cpu_Sumv_Rdy_SetInput();
} */

/*static void Cpu_Sumv_Rdy_SetOutput(bool val)
{
    Cpu_Sumv_Rdy_SetOutput();
}

static void Cpu_Sumv_Rdy_SetVal(bool val)
{
    Cpu_Sumv_Rdy_SetVal();
}

static void Cpu_Sumv_Rdy_ClrVal(bool val)
{
    Cpu_Sumv_Rdy_ClrVal();
} */

static bool Sumv_Rdy_GetVal(void)
{
    return  CPU_SUMV_RDY_GetVal() ;
}

// ExtSumv_scl
static void Sumv_Ext_Scl_SetVal(void)
{
    CPU_SUMV_EXT_SCL_PutVal(1);
}

static void Sumv_Ext_Scl_ClrVal(void)
{
    CPU_SUMV_EXT_SCL_PutVal(0);
}

/*static bool Sumv_Ext_Scl_GetVal(void)
{
    return Cpu_Sumv_Ext_Scl_GetVal();
} */

// ExtSumv_sda
static void Sumv_Ext_Sda_SetIoDirIn(void)
{
    CPU_SUMV_EXT_SDA_SetDir(0);
}

static void Sumv_Ext_Sda_SetIoDirOut(void)
{
    CPU_SUMV_EXT_SDA_SetDir(1);
}

static void Sumv_Ext_Sda_SetVal(void)
{
    CPU_SUMV_EXT_SDA_PutVal(1);
}

static void Sumv_Ext_Sda_ClrVal(void)
{
    CPU_SUMV_EXT_SDA_PutVal(0);
}

static bool Sumv_Ext_Sda_GetVal(void)
{
    return CPU_SUMV_EXT_SDA_GetVal() ;
}

// ExtSumv_pow
/*static void Sumv_Ext_Pow_SetInput(void)
{
   Sumv_Ext_Pow_SetInput();
}

static void Sumv_Ext_Pow_SetOutput(void)
{
    Sumv_Ext_Pow_SetOutput();
} */

static void Sumv_Ext_Pow_SetVal(void)
{
    //SUMV_EXT_POW_PutVal(1);
}

static void Sumv_Ext_Pow_ClrVal(void)
{
    //SUMV_EXT_POW_PutVal(0);
}

/*static bool Sumv_Ext_Pow_GetVal(void)
{
    return Sumv_Ext_Pow_GetVal();
}*/

//ExtSumv_Rdy
/*static void Cpu_Sumv_Ext_Rdy_SetInput(void)
{
   return Cpu_Sumv_Ext_Rdy_SetInput();
} */

/*static void SCpu_Sumv_Ext_Rdy_SetOutput(void)
{
    Cpu_Sumv_Ext_Rdy_SetOutput();
}

static void Cpu_Sumv_Ext_Rdy_SetVal(void)
{
    Cpu_Sumv_Ext_Rdy_SetVal();
}

static void Cpu_Sumv_Ext_Rdy_ClrVal(void)
{
    Cpu_Sumv_Ext_Rdy_ClrVal();
}  */

static bool Sumv_Ext_Rdy_GetVal(void)
{
    //return  CPU_SUMV_EXT_RDY_GetVal();
}

const sSumv_OperationType Sumv_Operation[] =
{
    {
        {
            NULL,
            NULL,
            Sumv_Scl_SetVal,
            Sumv_Scl_ClrVal,
            NULL,
        },

        {
            Sumv_Sda_SetIoDirIn,
            Sumv_Sda_SetIoDirOut,
            Sumv_Sda_SetVal,
            Sumv_Sda_ClrVal,
            Sumv_Sda_GetVal,
        },

        {
            NULL,
            NULL,
            NULL,
            NULL,
            Sumv_Rdy_GetVal,
        },

        {
            NULL,
            NULL,
            Sumv_Pow_SetVal,
            Sumv_Pow_ClrVal,
            NULL
        },

    },

    {
        {
            NULL,
            NULL,
            Sumv_Ext_Scl_SetVal,
            Sumv_Ext_Scl_ClrVal,
            NULL,
        },

        {
            Sumv_Ext_Sda_SetIoDirIn,
            Sumv_Ext_Sda_SetIoDirOut,
            Sumv_Ext_Sda_SetVal,
            Sumv_Ext_Sda_ClrVal,
            Sumv_Ext_Sda_GetVal,
        },

        {
            NULL,
            NULL,
            NULL,
            NULL,
            Sumv_Ext_Rdy_GetVal,
        },

        {
            NULL,
            NULL,
            Sumv_Ext_Pow_SetVal,
            Sumv_Ext_Pow_ClrVal,
            NULL,
        },


    },

    {
        {
            NULL,
            NULL,
            Sumv_Scl_SetVal,
            Sumv_Scl_ClrVal,
            NULL,
        },

        {
            Sumv_Sda_SetIoDirIn,
            Sumv_Sda_SetIoDirOut,
            Sumv_Sda_SetVal,
            Sumv_Sda_ClrVal,
            Sumv_Sda_GetVal,
        },

        {
            NULL,
            NULL,
            NULL,
            NULL,
            Sumv_Rdy_GetVal,
        },

        {
            NULL,
            NULL,
            Sumv_Pow_SetVal,
            Sumv_Pow_ClrVal,
            NULL
        },
    }
};


