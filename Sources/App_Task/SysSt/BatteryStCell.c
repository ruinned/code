// BatterySt.c
#include "BatterySt.h"

#if(ECU_RAM_USE_MODE)
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM
#endif
#else
#pragma DATA_SEG DEFAULT
#endif

extern sBat_AllInfoType  Bat_AllInfo;
extern sCell_BalanceTimeType Bat_LocalPackBalTime;
/*
* Cell Info
*/

#ifdef BMS_ENABLE_SINGLE_BAL_TIME

  uWord Bat_GetCellChgBalanceTimeSingle(uWord local_cell_no)
  {
      __ASSERT(local_cell_no > 0);

      if(local_cell_no > 0)
      {
          local_cell_no -= 1;
      }

      return Bat_LocalPackBalTime.ChgBalTimeSingle[local_cell_no];
  }

  eBat_ReturnType Bat_UpdateCellChgBalanceTimeSingle(uWord local_cell_no, uWord bal_time_single)
  {
      __ASSERT(local_cell_no > 0);

      if(local_cell_no > 0)
      {
          local_cell_no -= 1;
      }

      Bat_LocalPackBalTime.ChgBalTimeSingle[local_cell_no] = bal_time_single;

      return (eBat_ReturnType)ERR_OK;
  }

  uWord Bat_GetCellDchBalanceTimeSingle(uWord local_cell_no)
  {
      __ASSERT(local_cell_no > 0);

      if(local_cell_no > 0)
      {
          local_cell_no -= 1;
      }

      return Bat_LocalPackBalTime.DchBalTimeSingle[local_cell_no];
  }

  eBat_ReturnType Bat_UpdateCellDchBalanceTimeSingle(uWord local_cell_no, uWord bal_time_single)
  {
      __ASSERT(local_cell_no > 0);

      if(local_cell_no > 0)
      {
          local_cell_no -= 1;
      }

      Bat_LocalPackBalTime.DchBalTimeSingle[local_cell_no] = bal_time_single;

      return (eBat_ReturnType)ERR_OK;
  }
#endif  /* #ifdef BMS_ENABLE_SINGLE_BAL_TIME */


uWord Bat_GetCellDchBalanceTimeTotal(uWord local_cell_no)
{
    uWord wtmp;
    wtmp = 0;

    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    if(local_cell_no % 4 == 0)
    {
         wtmp |= (Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x0FFF);    
    }
    else if(local_cell_no % 4 == 1)
    {
         wtmp |= ((Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xF000) >> 12); 
         wtmp |= ((Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x00FF) << 4) ;
    }
    else if(local_cell_no % 4 == 2)
    {
         wtmp |= ((Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xFF00) >> 8); 
         wtmp |= ((Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x000F) << 8);
    }
    else if(local_cell_no % 4 == 3)
    {
         wtmp |= ((Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xFFF0) >> 4); 
       
    }
    else
    {
    }
    
    return wtmp;
}

eBat_ReturnType Bat_UpdateCellDchBalanceTimeTotal(uWord local_cell_no, uWord bal_time_total)
{

    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    if(local_cell_no % 4 == 0)
    {
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no  - (local_cell_no >> 2)]  |= (bal_time_total & 0x0fff) ;   
    }
    else if(local_cell_no % 4 == 1)
    {
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x000F) << 12) ; 
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - (local_cell_no >> 2)] |= ((bal_time_total & 0x0FF0) >> 4);
    }
    else if(local_cell_no % 4 == 2)
    {
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x00FF) << 8) ; 
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - (local_cell_no >> 2)] |= ((bal_time_total & 0x0F00) >> 8);
    }
    else if(local_cell_no % 4 == 3)
    {
        Bat_LocalPackBalTime.DchBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x0FFF) << 4) ; 
       
    }
    else
    {
    }
    
    return (eBat_ReturnType)ERR_OK;

}

uWord Bat_GetCellChgBalanceTimeTotal(uWord local_cell_no)
{
    uWord wtmp;
    wtmp = 0;

    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    
    
   if(local_cell_no % 4 == 0)
    {
    
        wtmp |= (Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x0FFF);    
    }
    else if(local_cell_no % 4 == 1)
    {
         wtmp |= ((Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xF000) >> 12); 
         wtmp |= ((Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x00FF) << 4) ;
    }
    else if(local_cell_no % 4 == 2)
    {
         wtmp |= ((Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xFF00) >> 8); 
         wtmp |= ((Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - (local_cell_no >> 2)] & 0x000F) << 8);
    }
    else if(local_cell_no % 4 == 3)
    {
         wtmp |= ((Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] & 0xFFF0) >> 4); 
       
    }
    else
    {
    }
    
    return wtmp;
}

eBat_ReturnType Bat_UpdateCellChgBalanceTimeTotal(uWord local_cell_no, uWord bal_time_total)
{
    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    if(local_cell_no % 4 == 0)
    {
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no  - (local_cell_no >> 2)]  |= (bal_time_total & 0x0fff) ;   
    }
    else if(local_cell_no % 4 == 1)
    {
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x000F) << 12) ; 
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - (local_cell_no >> 2)] |= ((bal_time_total & 0x0FF0) >> 4);
    }
    else if(local_cell_no % 4 == 2)
    {
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x00FF) << 8) ; 
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - (local_cell_no >> 2)] |= ((bal_time_total & 0x0F00) >> 8);
    }
    else if(local_cell_no % 4 == 3)
    {
        Bat_LocalPackBalTime.ChgBalTimeTotal[local_cell_no - 1 - (local_cell_no >> 2)] |= ((bal_time_total & 0x0FFF) << 4) ; 
       
    }
    else
    {
    }
    
    return (eBat_ReturnType)ERR_OK;
}


uByte Bat_GetCellBalanceSt(uByte bmu_no, uWord local_cell_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );
    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return (((PackInfoPtr->CellStPtr) + local_cell_no)->Cell_Info.Info & 0x3c);
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdateCellBalanceSt(uByte bmu_no, uWord local_cell_no, uByte bal_st)
{
    uByte btmp;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );
    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        btmp = (((PackInfoPtr->CellStPtr) + local_cell_no)->Cell_Info.Info & 0xc3);
        btmp |= bal_st;
        ((PackInfoPtr->CellStPtr) + local_cell_no)->Cell_Info.Info = btmp;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return (eBat_ReturnType)0xff;
    }

    return (eBat_ReturnType)ERR_OK;
}



uWord Bat_GetCellV(uByte bmu_no, uWord local_cell_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );
    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return ((PackInfoPtr->CellBasicPtr)+local_cell_no)->Voltage;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdateCellV(uByte bmu_no, uWord local_cell_no, uWord voltage)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        ((PackInfoPtr->CellBasicPtr) + local_cell_no)->Voltage = voltage;

    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return (eBat_ReturnType)0xff;
    }

    return (eBat_ReturnType)ERR_OK;
}



#ifdef BMS_ENABLE_CALC_INTRES
uByte Bat_GetCellR(uByte bmu_no, uWord local_cell_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];

    if(PackInfoPtr != NULL)
    {
        return ((PackInfoPtr->CellBasicPtr) + local_cell_no)->Int_Res;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdateCellR(uByte bmu_no, uWord local_cell_no, uByte intres)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;


    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );


    __ASSERT(local_cell_no > 0);

    if(local_cell_no > 0)
    {
        local_cell_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        ((PackInfoPtr->CellBasicPtr) + local_cell_no)->Int_Res = intres;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return (eBat_ReturnType)0xFF;
    }
    return (eBat_ReturnType)ERR_OK;
}

#endif

/*
uByte Bat_GetCellSOC(uByte bmu_no)
{


}

void Bat_UpdateCellSOC(Bat_SOCType soc)
{

}

*/


/*
** End of Cell Information Operation
*/

#pragma DATA_SEG DEFAULT
/*
** End Of Data Process
*/
