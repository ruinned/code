#include "SpiIf_Type.h"

#include "CPU_TLE_CS.h"
#include "CPU_TLE_SCK.h"
#include "CPU_TLE_SDI.h"
#include "CPU_TLE_SDO.h"
   
static void Spi1_Cs_ClrValOperation()
  {
    CPU_TLE_CS_PutVal(FALSE);
  }
  
static void Spi1_Cs_SetValOperation()
  {
    CPU_TLE_CS_PutVal(TRUE);
  }
  
static void Spi1_Sck_ClrValOperation()
  {
    CPU_TLE_SCK_PutVal(FALSE);
  }
  
static void Spi1_Sck_SetValOperation()
  {
    CPU_TLE_SCK_PutVal(TRUE);
  } 
  
static void Spi1_Sdi_ClrValOperation()
  {
    CPU_TLE_SDI_PutVal(FALSE);
  }
  
static void Spi1_Sdi_SetValOperation()
  {
    CPU_TLE_SDI_PutVal(TRUE);
  }
  
static bool Spi1_Sdo_GetValOperation()
  {
    return CPU_TLE_SDO_GetVal();
  } 
  
const sSpiIf_PropertyType SpiIf_Property[SPIIF_MAX_NUM] = 
{
   SPIIF_TLE8102_NO,
};

const sSpiIf_OperationType SpiIf_Operation[SPIIF_MAX_NUM] = 
{
   /* SPI1 module operation   */ 
  {
    {
       Spi1_Cs_ClrValOperation,
       Spi1_Cs_SetValOperation,
    }, 
    {
       Spi1_Sck_ClrValOperation,
       Spi1_Sck_SetValOperation,
    },
    {
       Spi1_Sdi_ClrValOperation,
       Spi1_Sdi_SetValOperation,
    }, 
    {   
       Spi1_Sdo_GetValOperation,
    }
  },
};

const sSpiIf_HandleType SpiIf_Handle = 
{
   SpiIf_Property,
   SpiIf_Operation
};

