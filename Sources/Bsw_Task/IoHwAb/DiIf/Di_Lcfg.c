
#include "DiIf_Type.h"

#include "PORTA0.h"
#include "PORTA1.h"
#include "CPUDI1.h"
#include "CPUDI2.h"
#include "CPUDI3.h"
#include "CPUDI4.h"
#include "OV_Power.h"
#include "UV_Power.h"
#include "PORTA2.h"



static bool Di1_GetValOperation()
  {
    return PORTA0_GetVal();
     
  }
static bool Di2_GetValOperation()
  {
    return PORTA1_GetVal();
  }
static bool Di3_GetValOperation()
  {
    return CPUDI1_GetVal();
  } 
  

static bool Di4_GetValOperation()
  {
    return CPUDI2_GetVal();
  }
 
static bool Di5_GetValOperation()
  {
    return CPUDI3_GetVal();
  } 

static bool Di6_GetValOperation()
  {
    return CPUDI4_GetVal();
  }
   
static bool Di7_GetValOperation()
  {
    return OV_Power_GetVal();
  }
  
static bool Di8_GetValOperation()
  {
    return UV_Power_GetVal();
  }
  
static bool Di9_GetValOperation()
  {
    return PORTA2_GetVal();
  }

sDiIf_PropertyType DiIf_Property[DIIF_MAX_NUM] =  //initial the property
{ 
  {
     DIIF_PORTA0_NO,
     DIIF_NORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_PORTA1_NO,
     DIIF_NORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_CPUDI1_NO,
     DIIF_ABNORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },

  {
     DIIF_CPUDI2_NO,
     DIIF_ABNORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },

  {
     DIIF_CPUDI3_NO,
     DIIF_ABNORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_CPUDI4_NO,
     DIIF_ABNORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_OV_POWER_NO,
     DIIF_NORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_UV_POWER_NO,
     DIIF_NORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },
  {
     DIIF_PORTA2_NO,
     DIIF_NORMAL_LOGIC_TYPE,
     DIIF_VALUE_LOW,
  },

};

const sDiIf_OperationType DiIf_Operation[DIIF_MAX_NUM] =  //initial the operation pointer
{  
    Di1_GetValOperation,
    Di2_GetValOperation,
    Di3_GetValOperation,
    Di4_GetValOperation,

    Di5_GetValOperation, 

    Di6_GetValOperation,
    Di7_GetValOperation,
    Di8_GetValOperation,
    Di9_GetValOperation,
};

const sDiIf_HandleType DiIf_Handle = 
{ 
   DiIf_Property,
   DiIf_Operation 
};




