
#include "IocIf_Type.h"


#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
#include "AC_CP_IOC.h"

#define  RISING_EDGE  0x04
#define  FALLING_EDGE 0x08

static uByte IocIf_EnableEventOperation(void) 
{
    return AC_CP_IOC_EnableEvent();
}

static uByte IocIf_DisableEventOperation(void) 
{
    return AC_CP_IOC_DisableEvent();
}

static uByte IocIf_ResetOperation(void) 
{
    return AC_CP_IOC_Reset();
}

static uByte IocIf_GetValueOperation(uWord *ValuePtr) 
{
    return AC_CP_IOC_GetCaptureValue(ValuePtr);
}

static void IocIf_RisingEdgeTriggerOperation(void) 
{
    //ECT_TCTL3 &= 0xF3;
    ECT_TCTL3 = RISING_EDGE;//0x04;
}

static void IocIf_FallingEdgeTriggerOperation(void) 
{
    //ECT_TCTL3 &= 0xF3;
    ECT_TCTL3 = FALLING_EDGE;//0x08;
}

const sIocIf_PropertyType IocIf_Property[] =  //initial the do property
{ 
  {
     IOCIF_AC_CP_NO,
     IOCIF_ABNORMAL_LOGIC_TYPE,
  },
};

const sIocIf_OperationType IocIf_Operation[] =  //initial the do operation pointer
{ 
   AC_CP_IOC_Enable,
   AC_CP_IOC_Disable,
   IocIf_EnableEventOperation,
   IocIf_DisableEventOperation,
   IocIf_ResetOperation,
   IocIf_GetValueOperation,
   IocIf_RisingEdgeTriggerOperation,
   IocIf_FallingEdgeTriggerOperation,
};

const sIocIf_HandleType IocIf_Handle = 
{
   IocIf_Property,
   IocIf_Operation,
};
#endif




