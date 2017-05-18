// DataStore.h
#ifndef DATASTORE_H
#define DATASTORE_H

#include "GeneralTypeDef.h"

#include "CanIf_Type.h"
#include "ECU.h"

// errorcode define
typedef enum
{
    DATASTORE_ERR_OK,
    DATASTORE_ERR_CHECK_FAIL = ERR_DATA_DATASTORE_BASE,
    DATASTORE_ERR_INIT_FAIL,
    DATASTORE_ERR_READ_RECORD,
}eDataStore_ErrCodeType;

/* define hardware error detail */
#define DATASTORE_HDERR_INIT      0x0001
#define DATASTORE_HDERR_ERRFULL   0x0002
#define DATASTORE_HDERR_HISFULL   0x0004
#define DATASTORE_HDERR_WRITE     0x0008
#define DATASTORE_HDERR_READ      0x0010
#define DATASTORE_HDERR_CHECK     0x0020

// define event type
typedef enum
{
  ERROR_DATA,            
  HISTORY_DATA,           
  POWOFF_DATA,          
  RLY_ERR,           
  RLY_ERR_OPEN,      
  MFY_SOC,
  MFY_SOH,           
  BAL_START,         
  CHG_ERR,
  CHG_START,
  CHG_END,      
  CHG_NO_CTRL,       
  CAP_CALIBRATION,
  BAT_MAINTENANCE,
  MFY_PARAMETER,
}eDataStoreTypeType;

/* define max record num, three page for 45db041, one page for 45db161 */
#define MAX_RECORD_NUM    1024 

/* define history data store time */
#define DATASTORE_TIME2S      TM_MS(2000)
#define DATASTORE_TIME1MIN     TM_MS(60000)
#define DATASTORE_TIME2MIN     TM_MS(120000)
#define DATASTORE_TIME3MIN     TM_MS(180000)
#define HISTORY_DATA_SAVE_TIME DATASTORE_TIME2S

extern void DataStore_Init(void);
extern void DataStore_MainCountCbk(void);
extern void DataStore_MainCbk(void);
extern void DataStore_EnableEventDataStore(eDataStoreTypeType type);
extern void DataStore_EnableErrorDataStore(eDataStoreTypeType type);
extern void DataStore_ProcSaveData(void);
extern void DataStore_ReadPage(uWord page, uByte * RAMPTR p_data);
extern void DataStore_EraseSector(uWord Sector);
extern void DataStore_UpdateRsvData(uByte* RAMPTR  p_data, uByte dlen);

extern GeneralReturnType DataStore_ReadAndTxData(sCanIf_MsgType msg);
extern void DataStore_EraseFlash(sCanIf_MsgType msg);

#ifdef DATASTORE_DEBUG
extern void DataStore_Test(void);
#endif
extern void DataStore_UdsWrite(uByte * RAMPTR p_data, uByte index);
extern void DataStore_UdsRead(uLWord addr, uByte * RAMPTR p_data, uWord dlen, uByte index);

#endif /* DATASTORE_H */

