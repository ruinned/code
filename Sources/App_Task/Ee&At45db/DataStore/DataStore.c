// DataStore.c
#include "DataStore.h"
#include "NvmIf.h"
#include "SchIf.h"
#include "CanIf.h"
#include "WdgIf.h"
#include "EE.h"
#include "BatterySt.h"
#include "ErrorAlarm.h"
#include "BatterySysSt.h"
#include "BmsSysSt.h"
#include "Lvm.h"
#include "Version.h"
#include "Aiif.h"

#ifdef ECU_M_ENABLE_XGATE
#include "Xgate_ExtFlash.h"
#endif

#define REF_VOLT    500
#define VOLT_GAIN   110

//#define __DEBUG_DATASTORE
#ifdef __DEBUG_DATASTORE
uByte  gby_test_bmu_num ;
uByte  gby_test_temper_num ;
uWord gwd_test_bmu_bat_num;
#endif

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


uWord gwd_RecordNum;
uWord gwd_MaxRecordNumPerPage;
uWord gwd_MinErrPage;
uWord gwd_MaxErrPage;
uWord gwd_MinHisPage;
uWord gwd_MaxHisPage;
uByte gby_RsvData[4];

uByte gby_Record[MAX_RECORD_NUM];
uByte gby_Record1[256];

typedef struct
{
    uByte WrHisData;
    uByte WrErrData;
    uByte WrEventData;
    uByte WrTwoErrPage;
    uByte WrTwoHisPage;
    uByte WrThdErrPage;
    uByte WrThdHisPage;
    uByte WrFourErrPage;
    uByte WrFourHisPage;
    uByte WrOnlyTwoHisPage;
    uByte WrOnlyTwoErrPage;
    uByte WrOnlyThreeHisPage;
    uByte WrOnlyThreeErrPage;

    uByte EventDataType;
    uByte ErrDataType;
    uByte HisDataType;
    uByte FlashType;

    uByte CheckData;
    uByte CheckOnceFlag;
    uByte WrData;
    uByte FlagGetData;
    uWord CheckDataPage;
    uWord CheckDataStartAddr;
    uWord CheckDataEndAddr;
    
    uWord HisDataTimeCnt;
 #ifdef __DEBUG_DATASTORE
    uWord ErrDataTimeCnt;
 #endif
} sDataStore_CtrlInfoType;

sDataStore_CtrlInfoType DataStore_CtrlInfo;

typedef struct
{
    uByte PageMode;
    uByte ErrorFull;
    uByte HistoryFull;
    uWord ErrorPage;
    uWord HistoryPage;
} sDataStore_PageInfoType;


sDataStore_PageInfoType DataStore_PageInfo;


typedef struct
{
    uByte cnt;
    uLWord SavePageCnt;
} sDataStore_TmCntType;

sDataStore_TmCntType DataStore_TmCnt;

#ifdef DATASTORE_DEBUG
uByte gby_test_bmu_num;
uWord gwd_test_bmu_bat_num;
uByte gby_test_temper_num;
uByte gby_test_Record[MAX_RECORD_NUM];
#endif



/* define page mode */
#define  ONE_PAGE     0x01
#define  TWO_PAGE     0x02
#define  THREE_PAGE   0x03
#define  FOUR_PAGE    0x04

/* define for read record */
#define  DATASTORE_READ_ERR_RECORD 1
#define  DATASTORE_READ_HIS_RECORD 2

/* define for check awake st */
#define  DATASTORE_AWAKEST_OFF 0
#define  DATASTORE_AWAKEST_ON  1 

/* define flag for use */
#define DATASTORE_FLAG_DISABLE  0
#define DATASTORE_FLAG_ENABLE   1

#define DATASTORE_FLAG_FAIL  0
#define DATASTORE_FLAG_OK    1

#define FLASH_READY   0x01  // 0x80

/* define flash type */
#define FLASH_SMALL_CAP	  0x18
#define FLASH_LAGRGE_CAP  0x28

/* define page infor */
#define MAX_DATA_PAGE_SMALLCAP  264
#define MAX_DATA_PAGE_LARGECAP  528

#define MIN_ERR_PAGE_SMALLCAP   0
#define MAX_ERR_PAGE_SMALLCAP   1007
#define MIN_ERR_PAGE_LARGECAP   0
#define MAX_ERR_PAGE_LARGECAP   999

#define MIN_HIS_PAGE_SMALLCAP   1008
#define MAX_HIS_PAGE_SMALLCAP   2039
#define MIN_HIS_PAGE_LARGECAP   1000
#define MAX_HIS_PAGE_LARGECAP   2047

#define MAX_DATA_PAGE_CAP  256

#define MIN_ERR_PAGE_CAP   0
#define MAX_ERR_PAGE_CAP   5039

#define MIN_HIS_PAGE_CAP   5040
#define MAX_HIS_PAGE_CAP   65519

/* define for xgate operate type */
#define WRITE_TYPE 0
#define CHECK_TYPE 1


/* inner function */
static void DataStore_InitDevice(void);
static uByte DataStore_WriteRecord(void);
static uWord DataStore_GetRecord(uByte type);
static eDataStore_ErrCodeType DataStore_CheckRecord(void);
static void DataStore_UdsWriteCbk(void);
uByte gby_EraseHisSector;


void DataStore_Init(void)
{
    DataStore_TmCnt.cnt = 0;
    DataStore_InitDevice();
    DataStore_CtrlInfo.WrData = DATASTORE_FLAG_ENABLE;
    gby_EraseHisSector = 0;
}

void DataStore_MainCountCbk(void)
{
    (DataStore_TmCnt.cnt < 255) ? (DataStore_TmCnt.cnt++) : (DataStore_TmCnt.cnt = 255);
    (DataStore_TmCnt.SavePageCnt < 100000) ? (DataStore_TmCnt.SavePageCnt++) : (DataStore_TmCnt.SavePageCnt = 100000);    
    (DataStore_CtrlInfo.HisDataTimeCnt < 65535) ? (DataStore_CtrlInfo.HisDataTimeCnt++) : (DataStore_CtrlInfo.HisDataTimeCnt = 65535);
    
 #ifdef __DEBUG_DATASTORE
    (DataStore_CtrlInfo.ErrDataTimeCnt < 65535) ? (DataStore_CtrlInfo.ErrDataTimeCnt++) : (DataStore_CtrlInfo.ErrDataTimeCnt = 65535);
 #endif
}

void DataStore_MainCbk(void)
{    
   uWord rslt = 0;
   if(DataStore_TmCnt.SavePageCnt >= 1000)//TM_MS(361000))
   {
       DataStore_TmCnt.SavePageCnt = 0;
       rslt = EE_SaveVar(BOTHEE, SYSTEMST_DATA_RECORD_PAGE);
   }
    
    /* save history data every 3min */
    if(DataStore_CtrlInfo.HisDataTimeCnt >= HISTORY_DATA_SAVE_TIME)
    {    
        DataStore_CtrlInfo.HisDataTimeCnt = 0;
        DataStore_CtrlInfo.WrHisData = DATASTORE_FLAG_ENABLE;
        DataStore_CtrlInfo.HisDataType = (uByte)HISTORY_DATA;
    }
 #ifdef __DEBUG_DATASTORE   
    if(DataStore_CtrlInfo.ErrDataTimeCnt >= TM_MS(3000))
    {    
        DataStore_CtrlInfo.ErrDataTimeCnt = 0;
        DataStore_EnableErrorDataStore(ERROR_DATA);
    }   
#endif        
    if(DataStore_TmCnt.cnt >= TM_MS(50))
    {
        if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrData)
        {
           DataStore_TmCnt.cnt = 0;
        }
        if(0 == gby_EraseHisSector)
        {
            gby_EraseHisSector = 1;
            if((0 == (DataStore_PageInfo.HistoryPage % 16)) || (1 == (DataStore_PageInfo.HistoryPage % 16)))
            {
                DataStore_EraseSector(DataStore_PageInfo.HistoryPage / 16);
                DataStore_TmCnt.cnt = 0; 
                return;
            }
        }
        DataStore_ProcSaveData();
	DataStore_UdsWriteCbk();	
    }
}

void DataStore_EnableEventDataStore(eDataStoreTypeType type)
{
    
    if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrHisData)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrEventData) )
    {
        DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_DISABLE;
    }
    else
    {
        DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_ENABLE;
        gwd_RecordNum = DataStore_GetRecord((uByte)type);
    }
    
    DataStore_CtrlInfo.WrEventData = DATASTORE_FLAG_ENABLE;
    DataStore_CtrlInfo.EventDataType = (uByte)type;
}

void DataStore_EnableErrorDataStore(eDataStoreTypeType type)
{
    if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourErrPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourHisPage)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrHisData)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
              || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrEventData) )
    {
        DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_DISABLE;
    }
    else
    {
        DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_ENABLE;
        gwd_RecordNum = DataStore_GetRecord((uByte)type);
    }
   
    DataStore_CtrlInfo.WrErrData = DATASTORE_FLAG_ENABLE;
    DataStore_CtrlInfo.ErrDataType = (uByte)type;
    
}

void DataStore_UpdateRsvData(uByte* RAMPTR p_data, uByte dlen)
{
    uByte i;
    for(i = 0; i < dlen; i++)
    {
        if(i < 4)
        {
            gby_RsvData[i] = *p_data++;
        }
        else
        {
            break;
        }
    }
}

//************************************************************/
// Function: initial
// Trans para: void
//************************************************************/
static void DataStore_InitDevice(void)
{
    uByte i;
    uByte status;
    uByte *RAMPTR p_data;
    uWord hd_errcode;
    GeneralReturnType rslt;

    p_data = (uByte *RAMPTR)(&DataStore_CtrlInfo);
    for(i = 0; i < sizeof(DataStore_CtrlInfo); i++)
    {
        *p_data++ = 0;
    }

    rslt = SysConf_InitParaTable(SYSTEMST_DATA_RECORD_PAGE, (uLWord)(&DataStore_PageInfo), sizeof(DataStore_PageInfo));
    rslt = EE_LoadVar(SYSTEMST_DATA_RECORD_PAGE);

    if(DATASTORE_FLAG_OK == DataStore_PageInfo.ErrorFull)
    {
        hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_ERRFULL;
        rslt = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
    }

    if(DATASTORE_FLAG_OK == DataStore_PageInfo.HistoryFull)
    {
        hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_HISFULL;
        rslt = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
    }

    DataStore_CtrlInfo.FlashType = FLASH_SMALL_CAP; // init value

    for(i = 0; i < 3; i++)
    {
        status = (*NvmIf_FlashOperation.Init)();
        status = FLASH_SMALL_CAP;

        if((status & FLASH_SMALL_CAP) == FLASH_SMALL_CAP)
        {
            DataStore_CtrlInfo.FlashType = FLASH_SMALL_CAP;

            gwd_MaxRecordNumPerPage = MAX_DATA_PAGE_CAP;
            gwd_MinErrPage = MIN_ERR_PAGE_CAP;
            gwd_MaxErrPage = MAX_ERR_PAGE_CAP;
            gwd_MinHisPage = MIN_HIS_PAGE_CAP;
            gwd_MaxHisPage = MAX_HIS_PAGE_CAP;

            break;
        }
        else if((status & FLASH_LAGRGE_CAP) == FLASH_LAGRGE_CAP)
        {
            DataStore_CtrlInfo.FlashType = FLASH_LAGRGE_CAP;

            gwd_MaxRecordNumPerPage = MAX_DATA_PAGE_LARGECAP;
            gwd_MinErrPage = MIN_ERR_PAGE_LARGECAP;
            gwd_MaxErrPage = MAX_ERR_PAGE_LARGECAP;
            gwd_MinHisPage = MIN_HIS_PAGE_LARGECAP;
            gwd_MaxHisPage = MAX_HIS_PAGE_LARGECAP;

            break;
        }
        else
        {
            rslt = Err_UpdateHwErrLevel(ERR_HW_FLASH_EXT, ERR_LEVEL_TWO);

            hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_INIT;
            rslt = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);

            // default value
            gwd_MaxRecordNumPerPage = MAX_DATA_PAGE_SMALLCAP;
            gwd_MinErrPage = MIN_ERR_PAGE_SMALLCAP;
            gwd_MaxErrPage = MAX_ERR_PAGE_SMALLCAP;
            gwd_MinHisPage = MIN_HIS_PAGE_SMALLCAP;
            gwd_MaxHisPage = MAX_HIS_PAGE_SMALLCAP;
        }
    }

    if(DataStore_PageInfo.ErrorPage > gwd_MaxErrPage)
    {
        DataStore_PageInfo.ErrorPage = gwd_MinErrPage;
    }

    if(DataStore_PageInfo.HistoryPage < gwd_MinHisPage)
    {
        DataStore_PageInfo.HistoryPage = gwd_MinHisPage;
    }

    if(DataStore_PageInfo.HistoryPage > gwd_MaxHisPage)
    {
        DataStore_PageInfo.HistoryPage = gwd_MinHisPage;
    }
    if((0 == (DataStore_PageInfo.ErrorPage % 16)) || (1 == (DataStore_PageInfo.ErrorPage % 16)))
    {
        DataStore_EraseSector(DataStore_PageInfo.ErrorPage / 16);
    }
    BmsSys_UpdateHisRecStartPage(gwd_MinHisPage);
    BmsSys_UpdateHisRecEndPage(gwd_MaxHisPage);
    BmsSys_UpdateErrRecStartPage(gwd_MinErrPage);
    BmsSys_UpdateErrRecEndPage(gwd_MaxErrPage);
    BmsSys_UpdateRecPageMode(DataStore_PageInfo.PageMode);
    BmsSys_UpdateHisRecFullSt(DataStore_PageInfo.HistoryFull);
    BmsSys_UpdateErrRecFullSt(DataStore_PageInfo.ErrorFull);
    BmsSys_UpdateHisRecCurrentPage(DataStore_PageInfo.HistoryPage);
    BmsSys_UpdateErrRecCurrentPage(DataStore_PageInfo.ErrorPage);
    (void)rslt;
}

//************************************************************/
// Function: write record
// Trans para: void
//************************************************************/
static uByte DataStore_WriteRecord(void)
{
    uByte i;
    GeneralReturnType rslt[2];
    uByte status;
    uByte flg_err_save;
    uByte flg_his_save;
    uByte flg_ready_to_save;
    uByte type;
    uWord dlen;
    uWord hd_errcode;
    
#ifdef ECU_M_ENABLE_XGATE
    Xgate_ExtFlashType Xgate_ExtFlash;
#endif        

    flg_err_save = DATASTORE_FLAG_FAIL;
    flg_his_save = DATASTORE_FLAG_FAIL;
    flg_ready_to_save = DATASTORE_FLAG_FAIL;

//#ifdef ECU_M_ENABLE_XGATE

//#else
    for(i = 0; i < 5; i++)
    {
        status = (*NvmIf_FlashOperation.IndleCheck)();

        if(1 == status)
        {
            // chip is ready to save data
            flg_ready_to_save = DATASTORE_FLAG_OK;
            break;
        }
    }
    if(DATASTORE_FLAG_FAIL == flg_ready_to_save)
    {
        return 1;
    }
//#endif    
     // begin save data to flash
#ifdef ECU_M_ENABLE_XGATE
    if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData)
#else     
    if(DATASTORE_FLAG_OK == flg_ready_to_save)
#endif    
    {        
        if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourErrPage)
        {
        
            dlen = gwd_RecordNum - (gwd_MaxRecordNumPerPage * 3);
            
#ifdef ECU_M_ENABLE_XGATE
            Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage * 3];
            Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
            Xgate_ExtFlash.DataLen = dlen;
            Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
//            Xgate_ExtFlash.CpltFlag = 0;
            //Xgate_ExtFlash.Rslt = 0;
            Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
            Xgate_CpuSetStart();
#else            
            (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage * 3]), dlen);
#endif
            DataStore_CtrlInfo.WrFourErrPage = DATASTORE_FLAG_DISABLE;
            flg_err_save = DATASTORE_FLAG_OK;

            // prepare for check data
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.ErrorPage;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
            DataStore_CtrlInfo.CheckDataStartAddr = (gwd_MaxRecordNumPerPage * 3);
            DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;

        }
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdErrPage)  /*only write third error page */
        {
           
            if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrOnlyThreeErrPage)	/* only write three error pages */
            {
                dlen = gwd_RecordNum - (gwd_MaxRecordNumPerPage << 1);
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage << 1];
                Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                 
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage << 1]), dlen);
#endif
                DataStore_CtrlInfo.WrOnlyThreeErrPage = DATASTORE_FLAG_DISABLE;

                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = 2 * gwd_MaxRecordNumPerPage;
                DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
            }
            else		/* need write four error pages */
            {
                dlen = gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage << 1];
                Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage << 1]), dlen);
#endif
                DataStore_CtrlInfo.WrFourErrPage = DATASTORE_FLAG_ENABLE;

                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = (gwd_MaxRecordNumPerPage * 2);
                DataStore_CtrlInfo.CheckDataEndAddr = (gwd_MaxRecordNumPerPage * 3);
            }
            
            
            DataStore_CtrlInfo.WrThdErrPage = DATASTORE_FLAG_DISABLE;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.ErrorPage;
            flg_err_save = DATASTORE_FLAG_OK;
            
                            
        }
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoErrPage)  /* write second error page */
        {
            if(FLASH_LAGRGE_CAP == DataStore_CtrlInfo.FlashType)
            {
                dlen = gwd_RecordNum - gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
            }
            else
            {
                if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrOnlyTwoErrPage)	/* only write two error pages */
                {
                    dlen = gwd_RecordNum - gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = dlen;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                    
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                    DataStore_CtrlInfo.WrOnlyTwoErrPage = DATASTORE_FLAG_DISABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
                }
                else		/* write second error pages */
                {
                    dlen = gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = dlen;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                    
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                    DataStore_CtrlInfo.WrThdErrPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                    DataStore_CtrlInfo.CheckDataEndAddr = (gwd_MaxRecordNumPerPage << 1);
                }
            }

            DataStore_CtrlInfo.WrTwoErrPage = DATASTORE_FLAG_DISABLE;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.ErrorPage;
            flg_err_save = DATASTORE_FLAG_OK;
        }
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourHisPage)
        {        
            dlen = gwd_RecordNum - (gwd_MaxRecordNumPerPage * 3);
#ifdef ECU_M_ENABLE_XGATE
            Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage * 3];
            Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
            Xgate_ExtFlash.DataLen = dlen;
            Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
            //Xgate_ExtFlash.CpltFlag = 0;
            //Xgate_ExtFlash.Rslt = 0;
            Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
            Xgate_CpuSetStart();
#else               
            (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage * 3]), dlen);
#endif
            DataStore_CtrlInfo.WrFourHisPage = DATASTORE_FLAG_DISABLE;
            flg_his_save = DATASTORE_FLAG_OK;

            // prepare for check data
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.HistoryPage;
            DataStore_CtrlInfo.CheckDataStartAddr = (gwd_MaxRecordNumPerPage * 3);
            DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
        
        
        }
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdHisPage)	 /* write third history page */
        {
          
            if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrOnlyThreeHisPage)	/* write only three history pages for 041*/
            {
                dlen = gwd_RecordNum - (gwd_MaxRecordNumPerPage << 1);
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage << 1];
                Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage << 1]), dlen);
#endif
                DataStore_CtrlInfo.WrOnlyThreeHisPage = DATASTORE_FLAG_DISABLE;

                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = (gwd_MaxRecordNumPerPage << 1);
                DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
            }
            else	 /* need to write four pages for 45db041*/
            {
                dlen = gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage << 1];
                Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage << 1]), dlen);
#endif
                DataStore_CtrlInfo.WrFourHisPage = DATASTORE_FLAG_ENABLE;

                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = (gwd_MaxRecordNumPerPage << 1);
                DataStore_CtrlInfo.CheckDataEndAddr = (gwd_MaxRecordNumPerPage * 3);
            }
            
            DataStore_CtrlInfo.WrThdHisPage = DATASTORE_FLAG_DISABLE;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.HistoryPage;
            flg_his_save = DATASTORE_FLAG_OK;
            
                
        }
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoHisPage)	 /* write second history page */
        {
            if(FLASH_LAGRGE_CAP == DataStore_CtrlInfo.FlashType)
            {
                dlen = gwd_RecordNum - gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                Xgate_ExtFlash.DataLen = dlen;
                Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                //Xgate_ExtFlash.CpltFlag = 0;
                //Xgate_ExtFlash.Rslt = 0;
                Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                Xgate_CpuSetStart();
#else                
                (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                // prepare for check data
                DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
            }
            else
            {
                if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrOnlyTwoHisPage)	/* write only two history pages for 041*/
                {
                    dlen = gwd_RecordNum - gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = dlen;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                    
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                    DataStore_CtrlInfo.WrOnlyTwoHisPage = DATASTORE_FLAG_DISABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;
                }
                else	 /* need to write three pages for 45db041*/
                {
                    dlen = gwd_MaxRecordNumPerPage;
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[gwd_MaxRecordNumPerPage];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = dlen;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                    
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, &(gby_Record[gwd_MaxRecordNumPerPage]), dlen);
#endif
                    DataStore_CtrlInfo.WrThdHisPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = gwd_MaxRecordNumPerPage;
                    DataStore_CtrlInfo.CheckDataEndAddr = (gwd_MaxRecordNumPerPage << 1);
                }
            }
            DataStore_CtrlInfo.WrTwoHisPage = DATASTORE_FLAG_DISABLE;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.HistoryPage;
            flg_his_save = DATASTORE_FLAG_OK;
        }

        /* begin to write error data and event data */
        else if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
                 || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrEventData) )
        {
            if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
            {
                DataStore_CtrlInfo.WrErrData = DATASTORE_FLAG_DISABLE;
                type = DataStore_CtrlInfo.ErrDataType;
            }
            else
            {
                DataStore_CtrlInfo.WrEventData = DATASTORE_FLAG_DISABLE;
                type = DataStore_CtrlInfo.EventDataType;
            }

            /* write error data below */
            if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.FlagGetData)
            {
               gwd_RecordNum = DataStore_GetRecord(type);
            }
            else
            {
               DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_DISABLE;
            }
            
            if(gwd_RecordNum > MAX_RECORD_NUM)
            {
                gwd_RecordNum = MAX_RECORD_NUM;
            }
            
            if(FLASH_LAGRGE_CAP == DataStore_CtrlInfo.FlashType)
            {
                if(gwd_RecordNum <= gwd_MaxRecordNumPerPage)
                {
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_RecordNum;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_RecordNum);
                    
#endif
                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;

                    DataStore_PageInfo.PageMode = ONE_PAGE;
                }
                else
                {
                    // check whether last error data page is ok?    
                    if((DataStore_PageInfo.ErrorPage % 2) != 0)
                    {
                        DataStore_PageInfo.ErrorPage--;
                    }
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else  
                    // write first page
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoErrPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = TWO_PAGE;
                }
            }
            else// CHIP_45DB041B
            {
                if(gwd_RecordNum <= gwd_MaxRecordNumPerPage)// only need write 1 page to 45db041
                {
                    // write only 1 page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_RecordNum;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                      
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_RecordNum);
                    
#endif
                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;

                    DataStore_PageInfo.PageMode = ONE_PAGE;
                }
                else if((gwd_RecordNum > gwd_MaxRecordNumPerPage)
                        && (gwd_RecordNum <= (gwd_MaxRecordNumPerPage << 1)))// need write 2 page to 45db041
                {
                    // check whether last error page is ok?
                    if((DataStore_PageInfo.ErrorPage % 2) != 0)
                    {
                        DataStore_PageInfo.ErrorPage--;
                    }

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_MaxRecordNumPerPage);
                  /*  while(!(*NvmIf_FlashOperation.IndleCheck)())
                    {
                        WdgIf_Trig();
                    }
                    ECU_DELAYMS(10);
                    (*NvmIf_FlashOperation.ReadPage)(DataStore_PageInfo.ErrorPage, gby_Record1, gwd_MaxRecordNumPerPage);
                    for(i = 0;i < 255;i++)
                    {
                        if(gby_Record[i] != gby_Record1[i])
                        {
                            asm nop;
                            asm nop;
                        }
                    } */
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoErrPage = DATASTORE_FLAG_ENABLE;
                    DataStore_CtrlInfo.WrOnlyTwoErrPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;
                    DataStore_PageInfo.PageMode = TWO_PAGE;
                }
                else if((gwd_RecordNum > (gwd_MaxRecordNumPerPage << 1))
                        && (gwd_RecordNum <= (gwd_MaxRecordNumPerPage * 3)))
                {
                    if(DataStore_PageInfo.ErrorPage % 3 != 0)
                    {
                        if(1 == (DataStore_PageInfo.ErrorPage % 3))
                        {
                            DataStore_PageInfo.ErrorPage -= 1;
                        }
                        else if(2 == (DataStore_PageInfo.ErrorPage % 3))
                        {
                            DataStore_PageInfo.ErrorPage -= 2;
                        }
                    }

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoErrPage = DATASTORE_FLAG_ENABLE;
                    DataStore_CtrlInfo.WrOnlyThreeErrPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;
                    DataStore_PageInfo.PageMode = THREE_PAGE;
                }
                else
                {
                    if(DataStore_PageInfo.ErrorPage % 4 != 0)
                    {
                        if(1 == (DataStore_PageInfo.ErrorPage % 4))
                        {
                            DataStore_PageInfo.ErrorPage -= 1;
                        }
                        else if(2 == (DataStore_PageInfo.ErrorPage % 4))
                        {
                            DataStore_PageInfo.ErrorPage -= 2;
                        }
                        else if(3 == (DataStore_PageInfo.ErrorPage % 4))
                        {
                            DataStore_PageInfo.ErrorPage -= 3;
                        }
                    }

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.ErrorPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.ErrorPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoErrPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = FOUR_PAGE;
                }
            }

            flg_err_save = DATASTORE_FLAG_OK;

            // prepare for check data
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.ErrorPage;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
        }
        /* end of write error data and event data */

        /* begin to write history data */
        else if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrHisData)
        {
            type = DataStore_CtrlInfo.HisDataType;
            DataStore_CtrlInfo.WrHisData = DATASTORE_FLAG_DISABLE;

            if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.FlagGetData)
            {
               gwd_RecordNum = DataStore_GetRecord(type);
            }
            else
            {
               DataStore_CtrlInfo.FlagGetData = DATASTORE_FLAG_DISABLE;
            }

            if(gwd_RecordNum > MAX_RECORD_NUM)
            {
                gwd_RecordNum = MAX_RECORD_NUM;
            }
            
            if(FLASH_LAGRGE_CAP == DataStore_CtrlInfo.FlashType)
            {
                if(gwd_RecordNum <= gwd_MaxRecordNumPerPage)
                {
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_RecordNum;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_RecordNum);
#endif
                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;

                    DataStore_PageInfo.PageMode = ONE_PAGE;
                }
                else
                {
                    // check whether last history page is ok?
                    if((DataStore_PageInfo.HistoryPage % 2) != 0)
                    {
                        DataStore_PageInfo.HistoryPage--;
                    }
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else
                    // write first page
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoHisPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = TWO_PAGE;
                }
            }
            else// CHIP_45DB041B
            {
                if(gwd_RecordNum <= gwd_MaxRecordNumPerPage)// only need write 1 page to 45db041
                {
                    // write only 1 page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_RecordNum;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                    
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_RecordNum);
#endif
                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_RecordNum;

                    DataStore_PageInfo.PageMode = ONE_PAGE;
                }
                else if((gwd_RecordNum > gwd_MaxRecordNumPerPage)
                        && (gwd_RecordNum <= (gwd_MaxRecordNumPerPage << 1)) )// need write 2 page to 45db041
                {
                    // check whether last history page is ok?
                    if((DataStore_PageInfo.HistoryPage % 2) != 0)
                    {
                        DataStore_PageInfo.HistoryPage--;
                    }

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoHisPage = DATASTORE_FLAG_ENABLE;
                    DataStore_CtrlInfo.WrOnlyTwoHisPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = TWO_PAGE;
                }
                else if((gwd_RecordNum > (gwd_MaxRecordNumPerPage << 1))
                        && (gwd_RecordNum <= (gwd_MaxRecordNumPerPage * 3)) )	 //need write 3 page
                {
                    if((DataStore_PageInfo.HistoryPage % 3) != 0)
                    {
                        if(1 == (DataStore_PageInfo.HistoryPage % 3))
                        {
                            DataStore_PageInfo.HistoryPage -= 1;
                        }
                        else if(2 == (DataStore_PageInfo.HistoryPage % 3))
                        {
                            DataStore_PageInfo.HistoryPage -= 2;
                        }
                    } 

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoHisPage = DATASTORE_FLAG_ENABLE;
                    DataStore_CtrlInfo.WrOnlyThreeHisPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = THREE_PAGE;
                }
                else       //need write 4 page
                {
                    if((DataStore_PageInfo.HistoryPage % 4) != 0)
                    {
                        if(1 == (DataStore_PageInfo.HistoryPage % 4))
                        {
                            DataStore_PageInfo.HistoryPage -= 1;
                        }
                        else if(2 == (DataStore_PageInfo.HistoryPage % 4))
                        {
                            DataStore_PageInfo.HistoryPage -= 2;
                        }
                        else if(3 == (DataStore_PageInfo.HistoryPage % 4))
                        {
                            DataStore_PageInfo.HistoryPage -= 3;
                        }
                    } 

                    // write first page
#ifdef ECU_M_ENABLE_XGATE
                    Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[0];
                    Xgate_ExtFlash.Page = DataStore_PageInfo.HistoryPage;
                    Xgate_ExtFlash.DataLen = gwd_MaxRecordNumPerPage;
                    Xgate_ExtFlash.ChkOrWr = WRITE_TYPE;
                    //Xgate_ExtFlash.CpltFlag = 0;
                    //Xgate_ExtFlash.Rslt = 0;
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash);             
                    Xgate_CpuSetStart();
#else                     
                    (*NvmIf_FlashOperation.WritePage)(DataStore_PageInfo.HistoryPage, gby_Record, gwd_MaxRecordNumPerPage);
#endif
                    // prepare data to write second page
                    DataStore_CtrlInfo.WrTwoHisPage = DATASTORE_FLAG_ENABLE;

                    // prepare for check data
                    DataStore_CtrlInfo.CheckDataStartAddr = 0;
                    DataStore_CtrlInfo.CheckDataEndAddr = gwd_MaxRecordNumPerPage;

                    DataStore_PageInfo.PageMode = FOUR_PAGE;
                }
            }

            flg_his_save = DATASTORE_FLAG_OK;

            // prepare for check data
            DataStore_CtrlInfo.CheckDataPage = DataStore_PageInfo.HistoryPage;
            DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_ENABLE;
        }
        /* end of write history data */
    }

    // save page infor to EE
    if(DATASTORE_FLAG_OK == flg_err_save)
    {
        // write error data successful, error page address add 1
        DataStore_PageInfo.ErrorPage++;
        if(DataStore_PageInfo.ErrorPage > gwd_MaxErrPage)
        {
            // 45db error data is full
            DataStore_PageInfo.ErrorPage = gwd_MinErrPage;
            DataStore_PageInfo.ErrorFull = DATASTORE_FLAG_OK;

            BmsSys_UpdateErrRecFullSt(DataStore_PageInfo.ErrorFull);

            hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_ERRFULL;
            rslt[0] = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
        }
                  
        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_DATA_RECORD_PAGE);
       
        BmsSys_UpdateErrRecCurrentPage(DataStore_PageInfo.ErrorPage);
        BmsSys_UpdateRecPageMode(DataStore_PageInfo.PageMode);
    }

    if(DATASTORE_FLAG_OK == flg_his_save)
    {
        // write history data successful, history page address add 1
        DataStore_PageInfo.HistoryPage++;
        if(DataStore_PageInfo.HistoryPage > gwd_MaxHisPage)
        {
            // 45db history data is full
            DataStore_PageInfo.HistoryPage = gwd_MinHisPage;
            DataStore_PageInfo.HistoryFull = DATASTORE_FLAG_OK;

            BmsSys_UpdateHisRecFullSt(DataStore_PageInfo.HistoryFull);

            hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_HISFULL;
            rslt[0] = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
        }
    
       
        BmsSys_UpdateHisRecCurrentPage(DataStore_PageInfo.HistoryPage);
        BmsSys_UpdateRecPageMode(DataStore_PageInfo.PageMode);
    }
    
    return 0;
}

//************************************************************/
// Function: check record
// Trans para: void
//************************************************************/
static eDataStore_ErrCodeType DataStore_CheckRecord(void)
{
    GeneralReturnType rslt[2];
    uWord num;
    uWord page;
    uWord hd_errcode;

    if((*NvmIf_FlashOperation.IndleCheck)())
    {
        num = DataStore_CtrlInfo.CheckDataEndAddr - DataStore_CtrlInfo.CheckDataStartAddr;
        page = DataStore_CtrlInfo.CheckDataPage;

        rslt[0] = (*NvmIf_FlashOperation.CheckPage)(page, &gby_Record[DataStore_CtrlInfo.CheckDataStartAddr], num);

        /* rewrite page when check page error */
        if(DATASTORE_ERR_CHECK_FAIL == (eDataStore_ErrCodeType)rslt[0])
        {
            page = DataStore_CtrlInfo.CheckDataPage;
            (*NvmIf_FlashOperation.WritePage)(page, &gby_Record[DataStore_CtrlInfo.CheckDataStartAddr], num);

            hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) | DATASTORE_HDERR_CHECK;
            rslt[1] = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
        }
        else
        {
            hd_errcode = Err_GetHwErrDetail(ERR_HW_FLASH_EXT) & (~DATASTORE_HDERR_CHECK);
            rslt[1] = Err_UpdateHwErrDetail(ERR_HW_FLASH_EXT, hd_errcode);
        }

        return (eDataStore_ErrCodeType)rslt[0];
    }
    else
    {
        return 1;
    }   
}

void DataStore_ReadPage(uWord page, uByte * RAMPTR p_data)
{
    (*NvmIf_FlashOperation.ReadPage)(page, p_data, gwd_MaxRecordNumPerPage);
}

void DataStore_EraseSector(uWord Sector)
{
    (*NvmIf_FlashOperation.SectorErase)(Sector);
}

//************************************************************/
// Function: get record
// Trans para: data type
//************************************************************/
static uWord DataStore_GetRecord(uByte type)
{
    uByte i, *pByte;
    uWord j;
    uByte btmp;
    uByte bmu_num;
    uByte date[6];
    uWord wcnt;
    uWord checksum;
    uWord bmu_bat_num;
    uWord bmu_temper_num;
    uWord wtmp;
    uLWord lwtmp;
    sErr_ByteNType error;
    sVersion_CodeType version;
    uByte data[30];
/*
    pByte = (uByte*)data;
    for(i  = 0; i < sizeof(data); i++)
    {
        *(pByte++) = 0;
    }
*/
#ifdef __DEBUG_DATASTORE
    Bat_UpdateSumV(INT_SUMV, 1000);
    Bat_UpdateCurrent(AVGCUR, 40000);
    Bat_UpdateSOC(100);
    Bat_UpdateSoh(100);
    Bat_UpdateTotalChgAh(10000);
    Bat_UpdateTotalDChAh(20000);
    Bat_UpdateMaxV(3700);
    Bat_UpdateMinV(3200);
    Bat_UpdateMaxVTotalNo(10);
    Bat_UpdateMinVTotalNo(20);
    Bat_UpdateMaxT(70);
    Bat_UpdateMinT(50);
    Bat_UpdateMaxTTotalNo(4);
    Bat_UpdateMinTTotalNo(10);
    Bat_UpdateMaxR(100);
    Bat_UpdateMinR(110);
    Bat_UpdateMaxRTotalNo(10);
    Bat_UpdateMinRTotalNo(20);
    BatSys_UpdateIsoPosResistor(1000);
    BatSys_UpdateIsoNegResistor(1000);

    gby_test_temper_num = 8;
    gwd_test_bmu_bat_num = 36;;
    gby_test_bmu_num = 8;
    for(i = 1; i <= gby_test_bmu_num; i++)

    {
        for(j = 0; j < gwd_test_bmu_bat_num; j++)
        {
            lwtmp = Bat_UpdateCellV(i, (j + 1), 3000 + (j + 1) * 10);

        }

        for(j = 0; j < gby_test_temper_num; j++)
        {
            Bat_UpdateTemper(i, (j + 1), 40 + (j + 1));
        }
    }
#endif


    wcnt = 0;
    /* Check Sum , sum 2 byte */
    /* 1 */
    gby_Record[wcnt++] =  0; // init value
    /* 2 */
    gby_Record[wcnt++] =  0;

    /* total byte , sum 2 byte */
    /* 3 */
    gby_Record[wcnt++] =  0; // init value
    /* 4 */
    gby_Record[wcnt++] =  0;

    (void)BmsSys_GetSystemTime(date);
    /* record time , sum 6 byte */
    /* 5 */
    gby_Record[wcnt++] = date[0]; // year
    /* 6 */
    gby_Record[wcnt++] = date[1]; // month
    /* 7 */
    gby_Record[wcnt++] = date[2]; // day
    /* 8 */
    gby_Record[wcnt++] = date[3]; // hour
    /* 9 */
    gby_Record[wcnt++] = date[4]; // minute
    /* 10 */
    gby_Record[wcnt++] = date[5]; // second

    /* sumv , sum 2 byte */
    wtmp = Bat_GetSumV(INT_SUMV);
    /* 11 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 12 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* current , sum 2 byte */
    wtmp = Bat_GetCurrent(AVGCUR);  //0.1A
    /* 13 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 14 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* SOC , sum 2 byte */
    wtmp = Bat_GetSOC();
    /* 15 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 16 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* SOH , sum 2 byte */
    wtmp = Bat_GetSoh();
    /* 17 */
    gby_Record[wcnt++] =  (uByte)(((uByte)(GET_ECU_RESET_SOURCE() << 4)) | ((uByte)((wtmp & 0x0f00) >> 8)));
    /* 18 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* total_in_ah , sum 4 byte */
    lwtmp = Bat_GetTotalChgAh();
    /* 19 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0xff000000) >> 24);
    /* 20 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x00ff0000) >> 16);
    /* 21 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x0000ff00) >> 8);
    /* 22 */
    gby_Record[wcnt++] = (uByte)(lwtmp & 0x000000ff);

    /* total_out_ah , sum 4 byte */
    lwtmp = Bat_GetTotalDchAh();
    /* 23 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0xff000000) >> 24);
    /* 24 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x00ff0000) >> 16);
    /* 25 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x0000ff00) >> 8);
    /* 26 */
    gby_Record[wcnt++] = (uByte)(lwtmp & 0x000000ff);

    /* relay_satus , sum 2 byte */
    wtmp = BatSys_GetPosRlySt() & 0x03;
    wtmp = (wtmp << 2) + BatSys_GetNegRlySt() & 0x03;
    wtmp = (wtmp << 2) + BatSys_GetPreRlySt() & 0x03;
    wtmp = (wtmp << 2) + BatSys_GetChgRlySt() & 0x03;
    wtmp = (wtmp << 2) + BatSys_GetAuxASt() & 0x03;
    wtmp = (wtmp << 2) + BatSys_GetAuxBSt() & 0x03;
    wtmp = (wtmp << 2) + 0; // rsever
    wtmp = (wtmp << 2) + 0; // rsever
    /* 27 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 28 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* battery error code , sum 8 byte */
    error = Err_GetBatAllErrLevel();
    /* 29 */
    gby_Record[wcnt++] = error.By[0];
    /* 30 */
    gby_Record[wcnt++] = error.By[1];
    /* 31 */
    gby_Record[wcnt++] = error.By[2];
    /* 32 */
    gby_Record[wcnt++] = error.By[3];
    /* 33 */
    gby_Record[wcnt++] = error.By[4];
    /* 34 */
    gby_Record[wcnt++] = error.By[5];
    /* 35 */
    gby_Record[wcnt++] = error.By[6];
    /* 36 */
    gby_Record[wcnt++] = error.By[7];

    /* bms hardware error code , sum 8 byte */
    error = Err_GetHwAllErrLevel();
    /* 37 */
    gby_Record[wcnt++] = error.By[0];
    /* 38 */
    gby_Record[wcnt++] = error.By[1];
    /* 39 */
    gby_Record[wcnt++] = error.By[2];
    /* 40 */
    gby_Record[wcnt++] = error.By[3];
    /* 41 */
    gby_Record[wcnt++] = error.By[4];
    /* 42 */
    gby_Record[wcnt++] = error.By[5];
    /* 43 */
    gby_Record[wcnt++] = error.By[6];
    /* 44 */
    gby_Record[wcnt++] = error.By[7];

    /* battery system error code , sum 16 byte */
    error = Err_GetBatSysAllErrLevel(PAGE0);
    /* 45 */
    gby_Record[wcnt++] = error.By[0];
    /* 46 */
    gby_Record[wcnt++] = error.By[1];
    /* 47 */
    gby_Record[wcnt++] = error.By[2];
    /* 48 */
    gby_Record[wcnt++] = error.By[3];
    /* 49 */
    gby_Record[wcnt++] = error.By[4];
    /* 50 */
    gby_Record[wcnt++] = error.By[5];
    /* 51 */
    gby_Record[wcnt++] = error.By[6];
    /* 52 */
    gby_Record[wcnt++] = error.By[7];

    error = Err_GetBatSysAllErrLevel(PAGE1);
    /* 53 */
    gby_Record[wcnt++] = error.By[0];
    /* 54 */
    gby_Record[wcnt++] = error.By[1];
    /* 55 */
    gby_Record[wcnt++] = error.By[2];
    /* 56 */
    gby_Record[wcnt++] = error.By[3];
    /* 57 */
    gby_Record[wcnt++] = error.By[4];
    /* 58 */
    gby_Record[wcnt++] = error.By[5];
    /* 59 */
    gby_Record[wcnt++] = error.By[6];
    /* 60 */
    gby_Record[wcnt++] = error.By[7];

    /* max_v and min_v, sum 4 byte */
    wtmp = Bat_GetMaxV();
    /* 61 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 62 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    wtmp = Bat_GetMinV();
    /* 63 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 64 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* max_v¡¢min_v location, sum 4 byte */
    wtmp = Bat_GetMaxVTotalNo();
    /* 65 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 66 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    wtmp = Bat_GetMinVTotalNo();
    /* 67 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 68 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* max_t and min_t, sum 2 byte*/
    /* 69 */
    gby_Record[wcnt++] = Bat_GetMaxT();
    /* 70 */
    gby_Record[wcnt++] = Bat_GetMinT();

    /* max_t¡¢min_t location, sum 4 byte */
    wtmp = Bat_GetMaxTTotalNo();
    /* 71 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /* 72 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    wtmp = Bat_GetMinTTotalNo();
    /*73 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /*74 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* max_r and min_r, sum 2 byte*/
    /*75 */
    gby_Record[wcnt++] = Bat_GetMaxR();
    /*76 */
    gby_Record[wcnt++] = Bat_GetMinR();

    /* max_r¡¢min_r location, sum 4 byte */
    wtmp = Bat_GetMaxRTotalNo();
    /*77 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /*78*/
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    wtmp = Bat_GetMinRTotalNo();
    /*79 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /*80 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* positive resistor  sum 2 byte */
    wtmp = BatSys_GetIsoPosResistor();
    /*81 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /*82 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* negative resistor sum 2 byte */
    wtmp = BatSys_GetIsoNegResistor();
    /*83 */
    gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
    /*84 */
    gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

    /* event type 1 byte */
    /*85 */
    gby_Record[wcnt++] = type;

    /* software version, sum 2 byte */
    (void)Version_GetProjectVer(&version);
    /*86 */
    gby_Record[wcnt++] = version.SW_minor_version & 0xff;
    /*87 */
    gby_Record[wcnt++] = version.SW_revision_version & 0xff;

    /* battery factory, sum 1 byte */
    btmp = (uByte)Bat_GetManufacturer();
    /*88 */
    gby_Record[wcnt++] = btmp;

    /* battery material, sum 1 byte */
    btmp = (uByte)Bat_GetMaterialType();
    /*89 */
    gby_Record[wcnt++] = btmp;

    /* battery pack number, sum 4 byte */
    lwtmp = Bat_GetAllPackNo();
    /*90 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0xff000000) >> 24);
    /*91 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x00ff0000) >> 16);
    /*92 */
    gby_Record[wcnt++] = (uByte)((lwtmp & 0x0000ff00) >> 8);
    /*93 */
    gby_Record[wcnt++] = (uByte)(lwtmp & 0x000000ff);

    /*reserve for test, sum 4 byte */
    /*94 */
    gby_Record[wcnt++] = gby_RsvData[0];
    /*95 */
    gby_Record[wcnt++] = gby_RsvData[1];
    /*96 */
    gby_Record[wcnt++] = gby_RsvData[2];
    /*97 */
    gby_Record[wcnt++] = gby_RsvData[3];

    /* BMU number sum 1 byte */
#ifdef __DEBUG_DATASTORE
    bmu_num = gby_test_bmu_num;
#else
//    (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &btmp);
    bmu_num = Bat_GetBmuNum();
#endif
    /*98 */
    gby_Record[wcnt++] = bmu_num;

    /* battery number, battery voltage, temperature number, temperature */

    /* 99 bcu type */
    gby_Record[wcnt++] = M_BCU_D; 
    
    for(i = 1; i <= bmu_num; i++)

    {
#ifdef __DEBUG_DATASTORE
        bmu_bat_num = gwd_test_bmu_bat_num;
#else
/*
        if(0 == i)
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, &data[0], &btmp);
            bmu_bat_num = (data[0] << 8) + data[1];
        }
        else if(i <= 15)
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G1, &data[0], &btmp);
            bmu_bat_num = (data[2 * (i - 1)] << 8) + data[2 * (i - 1) + 1];
        }
        else
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G2, &data[0], &btmp);
            bmu_bat_num = (data[2 * (i - 16)] << 8) + data[2 * (i - 16) + 1];
        }
*/        
#endif
        /* bmu battery num */
        wtmp = Bat_GetPackSerialNum(i);
        gby_Record[wcnt++] = (uByte)(wtmp >> 8);//(uByte)((bmu_bat_num & 0xff00) >> 8);
        gby_Record[wcnt++] = (uByte)(wtmp >> 0);//(uByte)(bmu_bat_num & 0x00ff);

//        for(j = 0; j < bmu_bat_num; j++)
        for(j = 0; j < wtmp; j++)
        {
            lwtmp = Bat_GetCellV(i, (j + 1));

            /* cell volt */
            gby_Record[wcnt++] = (uByte)((lwtmp & 0xff00) >> 8);
            gby_Record[wcnt++] = (uByte)(lwtmp & 0x00ff);
        }

#ifdef __DEBUG_DATASTORE
        bmu_temper_num = gby_test_temper_num;
#else
/*
        if(0 == i)
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU0_TEMPERNUM, &data[0], &btmp);
            bmu_temper_num = (data[0] << 8) + data[1];
        }
        else if(i <= 15)
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G1, &data[0], &btmp);
            bmu_temper_num = (data[2 * (i - 1)] << 8) + data[2 * (i - 1) + 1];
        }
        else
        {
            (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G2, &data[0], &btmp);
            bmu_temper_num = (data[2 * (i - 16)] << 8) + data[2 * (i - 16) + 1];
        }
*/        
#endif
        /* bmu temperature num */
        wtmp = Bat_GetPackTemperNum(i);
        gby_Record[wcnt++] = (uByte)((wtmp & 0xff00) >> 8);
        gby_Record[wcnt++] = (uByte)(wtmp & 0x00ff);

        for(j = 0; j < wtmp; j++)
        {
            /* temper */
            gby_Record[wcnt++] = Bat_GetTemper(i, (j + 1));
        }
    }

    /* total byte calculate */
    gby_Record[2] = (uByte)((wcnt & 0xff00) >> 8);
    gby_Record[3] = (uByte)(wcnt & 0x00ff);

    /* checksum , sum 2 byte */
    checksum = 0;
    for(j = 2; j < wcnt; j++)
    {
        checksum += gby_Record[j];
    }

    gby_Record[0] =  (uByte)((checksum & 0xff00) >> 8);
    gby_Record[1] =  (uByte)((checksum & 0x00ff) >> 0);
    return wcnt;
}

//************************************************************/
// Function: process of data save
// Trans para: void
//************************************************************/
void DataStore_ProcSaveData()
{
    uByte i;
    uByte status;
    
#ifdef ECU_M_ENABLE_XGATE
    Xgate_ExtFlashType Xgate_ExtFlash;
#endif     
    
    if((4 * Bat_GetBmuNum() + 2 * Bat_GetBatNum() + Bat_GetTemperNum() + 99) > MAX_RECORD_NUM)
    {
        //return; 
    }

#ifdef ECU_M_ENABLE_XGATE
    /* check if data ok last saved page */
    if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.CheckData)
            && (DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.WrData) )
    {

        if(DataStore_CtrlInfo.CheckOnceFlag == DATASTORE_FLAG_OK)
        {
            Xgate_CpuGetExtFlah(&Xgate_ExtFlash);
            if(1 == Xgate_ExtFlash.CpltFlag)
            {
                DataStore_CtrlInfo.WrData = DATASTORE_FLAG_ENABLE;
                DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_DISABLE;

                DataStore_CtrlInfo.CheckOnceFlag = DATASTORE_FLAG_FAIL;

                // when check fail ,want to rewrite data ,wait 30mS
                if(1 == Xgate_ExtFlash.Rslt)
                {
                    DataStore_TmCnt.cnt = 0;
                }
                if((0 == (DataStore_PageInfo.ErrorPage % 16)) && (1 != DataStore_PageInfo.ErrorPage))
                {
                    DataStore_EraseSector(DataStore_PageInfo.ErrorPage / 16);
                    DataStore_TmCnt.cnt = 0;
                }
                else if((0 == (DataStore_PageInfo.HistoryPage % 16)) && (5041 != DataStore_PageInfo.HistoryPage))
                {
                    DataStore_EraseSector(DataStore_PageInfo.HistoryPage / 16);
                    DataStore_TmCnt.cnt = 0;
                } 
                return;
            }
        }
        else
        {
            Xgate_ExtFlash.DataLen = DataStore_CtrlInfo.CheckDataEndAddr - DataStore_CtrlInfo.CheckDataStartAddr;
            Xgate_ExtFlash.DataAddr = (uLWord)&gby_Record[DataStore_CtrlInfo.CheckDataStartAddr];

            Xgate_ExtFlash.Page = DataStore_CtrlInfo.CheckDataPage;
            Xgate_ExtFlash.ChkOrWr = CHECK_TYPE;  // check data
            Xgate_ExtFlash.CpltFlag = 0;
            Xgate_ExtFlash.Rslt = 0;
            for(i = 0; i < 5; i++)
            {
                status = (*NvmIf_FlashOperation.IndleCheck)();

                if(1 == status)
                {
                    Xgate_CpuSetExtFlash(Xgate_ExtFlash); 
                    DataStore_CtrlInfo.CheckOnceFlag = DATASTORE_FLAG_OK;            
                    Xgate_CpuSetStart();
                    break;
                }
            }
            
        }

        return;
    }

    if((DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrData)
            && (DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData))
    {
        /* begin to save record */
        if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrHisData)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrEventData) )
        {

            if(0 == DataStore_WriteRecord())
            {
                DataStore_CtrlInfo.WrData = DATASTORE_FLAG_DISABLE;
            }
        }
    }

#else
    /* check if data ok last saved page */ 
    if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.CheckData)
            && (DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.WrData) )
    {
        for(i = 0; i < 5; i++)
        {
            status = (*NvmIf_FlashOperation.IndleCheck)();
            //status = (*NvmIf_FlashOperation.ReadStatusReg)();
            if(1 == status)
            {
               if(DATASTORE_ERR_OK == (DataStore_CheckRecord()))
               {
                  
                  DataStore_CtrlInfo.WrData = DATASTORE_FLAG_ENABLE;
                  DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_DISABLE;
                  break;
               }
               else
               {
                  DataStore_CtrlInfo.WrData = DATASTORE_FLAG_DISABLE;
               }
            }
            else
            { 
                DataStore_CtrlInfo.WrData = DATASTORE_FLAG_DISABLE;
            }
        }
        if(i == 5)
        {
             DataStore_CtrlInfo.WrData = DATASTORE_FLAG_ENABLE; 
             DataStore_CtrlInfo.CheckData = DATASTORE_FLAG_DISABLE; 
        }

        if((1 == (DataStore_PageInfo.ErrorPage % 16)) && (1 != DataStore_PageInfo.ErrorPage))
        {
            DataStore_EraseSector(DataStore_PageInfo.ErrorPage / 16);
            DataStore_TmCnt.cnt = 0;
        }
        else if((1 == (DataStore_PageInfo.HistoryPage % 16)) && (5041 != DataStore_PageInfo.HistoryPage))
        {
            DataStore_EraseSector(DataStore_PageInfo.HistoryPage / 16);
            DataStore_TmCnt.cnt = 0;
        } 
        return;
    }

    if((DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrData) 
                && (DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData))
    {
        /* begin to save record */
        if( (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrTwoHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrThdHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourErrPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrFourHisPage)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrHisData)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrErrData)
                || (DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.WrEventData) )
        {
            if(0 == DataStore_WriteRecord())
            {
                DataStore_CtrlInfo.WrData = DATASTORE_FLAG_DISABLE;
            } 
        }
    }
    else
    {
    
    }
#endif    
}


uWord DataStore_DetAwake(uByte AiIf_ChannelId)
{
        GeneralReturnType return_rslt;
        uByte i;
        uWord max,min;
        uLWord result = 0;
        uWord adc_value[5];

        for(i = 0; i < 5; i++)
        {
            return_rslt = AiIf_MeasureChannel(AiIf_ChannelId, &adc_value[i]);
            result += adc_value[i];
        }
        max = adc_value[0];
        min = adc_value[0];

        for(i = 1; i < 5; i++)
        {
            if(max < adc_value[i])
            {
                max = adc_value[i];
            }
            if(min > adc_value[i])
            {
                min = adc_value[i];
            }
        }
        result = result - max - min;
        result /= 3;
        result *= REF_VOLT;
        result *= VOLT_GAIN;
        result /= CPU_ADC_ACCURACY;
        result /= 100;
        
        return (uWord)result;
}

GeneralReturnType DataStore_ReadAndTxData(sCanIf_MsgType msg)
{
    uByte tmp;
    uByte type;
    uByte count, j, k;
    uWord rslt, i;    
    uWord total_byte_num;
    uWord HistoryPage;
    uWord ErrorPage;
    uWord need_read_pagenum;
    uWord read_page_now;
    uWord read_page_cnt;
    uLWord llwd_cnt;
    uWord k15volt = 0;
    uWord offbdvolt = 0;
    uWord onbdvolt =0 ;

    /* check record whther has saved */
    if(DATASTORE_FLAG_ENABLE == DataStore_CtrlInfo.CheckData)
    {
        return (GeneralReturnType)DATASTORE_ERR_READ_RECORD;
    }

    (void)EE_LoadVar(SYSTEMST_DATA_RECORD_PAGE);

    tmp = msg.data[0];
    
    SaveStatusReg();
        
    msg.data[1] = 0;

    if(DATASTORE_READ_ERR_RECORD == tmp)
    {
        if(DATASTORE_FLAG_OK == DataStore_PageInfo.ErrorFull)
        {
            ErrorPage = DataStore_PageInfo.ErrorPage;
            if(FOUR_PAGE == DataStore_PageInfo.PageMode)
            {
                if(ErrorPage % 4 != 0)
                {
                    if((ErrorPage % 4) == 1)
                    {
                        ErrorPage += 3;
                    }
                    else if((ErrorPage % 4) == 2)
                    {
                        ErrorPage += 2;
                    }
                    else if((ErrorPage % 4) == 3)
                    {
                        ErrorPage += 1;
                    }
                }
            
            }
            else if(THREE_PAGE == DataStore_PageInfo.PageMode)
            {
                if(ErrorPage % 3 != 0)
                {
                    if((ErrorPage % 3) == 1)
                    {
                        ErrorPage += 2;
                    }
                    else if((ErrorPage % 3) == 2)
                    {
                        ErrorPage += 1;
                    }
                    else
                    {
                    }
                }
            }
            else if(TWO_PAGE == DataStore_PageInfo.PageMode)
            {
                if(ErrorPage % 2 != 0)
                {
                    ErrorPage += 1;
                }
            }
            else
            {
            }

            read_page_now = ErrorPage;
            need_read_pagenum = gwd_MaxErrPage - gwd_MinErrPage + 1;
        }
        else
        {
            read_page_now = gwd_MinErrPage;
            need_read_pagenum = DataStore_PageInfo.ErrorPage - gwd_MinErrPage;
        }

        msg.data[1] |= ((DataStore_PageInfo.ErrorFull & 0x03) << 6);
    }
    else
    {
        if(DATASTORE_FLAG_OK == DataStore_PageInfo.HistoryFull)
        {
            HistoryPage = DataStore_PageInfo.HistoryPage;
            
            if(FOUR_PAGE == DataStore_PageInfo.PageMode)
            {
               
                if((HistoryPage % 4) != 0)
                {
                    if((HistoryPage % 4) == 1)
                    {
                        HistoryPage += 3;
                    }
                    else if((HistoryPage % 4) == 2)
                    {
                        HistoryPage += 2;
                    }
                    else if((HistoryPage % 4) == 3)
                    {
                        HistoryPage += 1;
                    }
                }
            
            }
            else if(THREE_PAGE == DataStore_PageInfo.PageMode)
            {
                if(HistoryPage % 3 != 0)
                {
                    if((HistoryPage % 3) == 1)
                    {
                        HistoryPage += 2;
                    }
                    else if((HistoryPage % 3) == 2)
                    {
                        HistoryPage += 1;
                    }
                    else
                    {
                    }
                }
            }
            else if(TWO_PAGE == DataStore_PageInfo.PageMode)
            {
                if(HistoryPage % 2 != 0)
                {
                    HistoryPage += 1;
                }
            }
            else
            {
            }

            read_page_now = HistoryPage;
            need_read_pagenum = gwd_MaxHisPage - gwd_MinHisPage + 1;
        }
        else
        {
            read_page_now = gwd_MinHisPage;
            need_read_pagenum = DataStore_PageInfo.HistoryPage - gwd_MinHisPage;
        }

        msg.data[1] |= ((DataStore_PageInfo.HistoryFull & 0x03) << 6);
    }

    if(DataStore_CtrlInfo.FlashType == FLASH_SMALL_CAP)
    {
        type = 1;
    }
    else
    {
        type = 2;
    }

    msg.data[0] = 0xAB;
    msg.data[1] |= ((type & 0x03) << 4); /* chip type */
    msg.data[1] |= (DataStore_PageInfo.PageMode & 0x0F); /* page mode */

    msg.data[2] = (uByte)((need_read_pagenum & 0xFF00) >> 8); /* need read page num */
    msg.data[3] = (uByte)(need_read_pagenum & 0x00FF);

    msg.data[4] = 0; /* read protocol version */
    msg.data[5] = 0; /* read protocol version */
    msg.data[6] = 0; /* rsv */
    msg.data[7] = 0; /* rsv */

    msg.id = 0x18889100;
    msg.length = 8;
    rslt = 0;
    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &msg, rslt);

    ECU_DELAYMS(20);
    
    count = 0;
    read_page_cnt = 0;
    llwd_cnt = 0;

    for(;;)
    { 
        k15volt = DataStore_DetAwake(AIIF_CPUAI_K15_NO);
        onbdvolt = DataStore_DetAwake(AIIF_CPUAI_ONBD_CHG_P_NO);
        offbdvolt = DataStore_DetAwake(AIIF_CPUAI_OFFBD_CHG_P_NO);
        if((k15volt < 50) && (onbdvolt < 50) && (offbdvolt < 50))
        {
           count++;
        }
        
        if(count >= 3)
        {
            count = 0;
            break;
        }
        if((*NvmIf_FlashOperation.IndleCheck)())
        {
            /* judge whether read complete */
            if(read_page_cnt >= need_read_pagenum)	
            {
                break;
            }
            else
            {
                if(DATASTORE_READ_ERR_RECORD == tmp)
                {
                    if(read_page_now > gwd_MaxErrPage)
                    {
                        read_page_now = gwd_MinErrPage;
                    }
                }
                else
                {
                    if(read_page_now > gwd_MaxHisPage)
                    {
                        read_page_now = gwd_MinHisPage;
                    }
                }
            }

            /* begin to read record and send to CAN */
            for(i = 0; i < MAX_RECORD_NUM; i++)
            {
                gby_Record[i] = 0;
            }

            if(FLASH_LAGRGE_CAP == DataStore_CtrlInfo.FlashType)
            {
                if(ONE_PAGE == DataStore_PageInfo.PageMode)
                {
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    read_page_cnt++;
                }
                else if(TWO_PAGE == DataStore_PageInfo.PageMode)
                {
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    DataStore_ReadPage((read_page_now++), &gby_Record[gwd_MaxRecordNumPerPage]);
                    read_page_cnt += 2;
                }
                else
                {
                }
            }
            else
            {
                if(ONE_PAGE == DataStore_PageInfo.PageMode)
                {
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    read_page_cnt++;
                }
                else if(TWO_PAGE == DataStore_PageInfo.PageMode)
                {
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    DataStore_ReadPage((read_page_now++), &gby_Record[gwd_MaxRecordNumPerPage]);
                    read_page_cnt += 2;
                }
                else if(THREE_PAGE == DataStore_PageInfo.PageMode)
                {
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    DataStore_ReadPage((read_page_now++), &gby_Record[gwd_MaxRecordNumPerPage]);
                    DataStore_ReadPage((read_page_now++), &gby_Record[2 * gwd_MaxRecordNumPerPage]);
                    read_page_cnt += 3;
                }
                else
                {
                   
                    DataStore_ReadPage((read_page_now++), gby_Record);
                    DataStore_ReadPage((read_page_now++), &gby_Record[gwd_MaxRecordNumPerPage]);
                    DataStore_ReadPage((read_page_now++), &gby_Record[2 * gwd_MaxRecordNumPerPage]);
                    DataStore_ReadPage((read_page_now++), &gby_Record[3 * gwd_MaxRecordNumPerPage]);
                    read_page_cnt += 4;
                
                }
            }

            total_byte_num = (gby_Record[2] << 8) + gby_Record[3];
            for(i = 0; i < 160; i++)
            {
                msg.data[0] = (uByte)i;

                for(j = 0; j < 7; j++)
                {
                    if((i * 7 + j) >= total_byte_num)
                    {
                        for(k = 1; k <= (7 - j); k++)
                        {
                            msg.data[j + k] = 0xAA;
                        }
                        break;
                    }

                    if((i * 7 + j) < MAX_RECORD_NUM)
                    {
                        msg.data[j + 1] = gby_Record[i * 7 + j];
                    }
                }
                msg.id = 0x18889100;
                msg.length = 8;
                (void)CanIf_RequestSend(INT_CAN_CHANNEL, &msg, rslt);

                ECU_DELAYMS(5);
                llwd_cnt++;
                WdgIf_Trig();
            }
            
            if(llwd_cnt >= 170000)
            {
                break;
            }
        }
        WdgIf_Trig(); 
    }
    RestoreStatusReg();
    __EI();
    
    return ERR_OK;
}

void DataStore_EraseFlash(sCanIf_MsgType msg)
{
    uByte i;
    uWord rslt;
    uWord Sector;
    uWord EraseSectorMax;

    DataStore_CtrlInfo.WrErrData = 0;
    DataStore_CtrlInfo.WrEventData = 0;
    DataStore_CtrlInfo.WrHisData = 0;
    DataStore_CtrlInfo.WrFourErrPage = 0;
    DataStore_CtrlInfo.WrFourHisPage = 0;
    DataStore_CtrlInfo.WrOnlyThreeErrPage = 0;
    DataStore_CtrlInfo.WrOnlyThreeHisPage = 0;
    DataStore_CtrlInfo.WrOnlyTwoErrPage = 0;
    DataStore_CtrlInfo.WrOnlyTwoHisPage = 0;
    DataStore_CtrlInfo.WrThdErrPage = 0;
    DataStore_CtrlInfo.WrThdHisPage = 0;
    DataStore_CtrlInfo.WrTwoErrPage = 0;
    DataStore_CtrlInfo.WrTwoHisPage = 0;

    if(1 == msg.data[0])	 // erase error data
    {
       /* Sector = 1;
        EraseSectorMax = MAX_ERR_PAGE_CAP / 16;
        for(i = 0;i < EraseSectorMax;i++)
        {
            while(!(*NvmIf_FlashOperation.IndleCheck)())
            {
                WdgIf_Trig();
            }
            DataStore_EraseSector(Sector++);
        } */
        while(!(*NvmIf_FlashOperation.IndleCheck)())
        {
            WdgIf_Trig();
        }
        DataStore_EraseSector(0); 
        DataStore_PageInfo.ErrorPage = gwd_MinErrPage;
        DataStore_PageInfo.ErrorFull = DATASTORE_FLAG_FAIL;
        
        rslt = EE_SaveVar(BOTHEE, SYSTEMST_DATA_RECORD_PAGE);
    }
    else			// erase history data
    {
      /*  Sector = MAX_ERR_PAGE_CAP / 16;
        Sector++;
        EraseSectorMax = (MAX_HIS_PAGE_CAP - MAX_ERR_PAGE_CAP)/ 16;
        for(i = 0;i < EraseSectorMax;i++)
        {
            while(!(*NvmIf_FlashOperation.IndleCheck)())
            {
                WdgIf_Trig();
            }
            DataStore_EraseSector(Sector++);
        }  */
        while(!(*NvmIf_FlashOperation.IndleCheck)())
        {
            WdgIf_Trig();
        }
        DataStore_EraseSector(315); 
        DataStore_PageInfo.HistoryPage = gwd_MinHisPage;
        DataStore_PageInfo.HistoryFull = DATASTORE_FLAG_FAIL;
        rslt = EE_SaveVar(BOTHEE, SYSTEMST_DATA_RECORD_PAGE);
    }

    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;

    if(Err_GetHwErrDetail(ERR_HW_FLASH_EXT))
    {
        msg.data[2] = 0;
    }
    else if(FLASH_SMALL_CAP == DataStore_CtrlInfo.FlashType)
    {
        msg.data[2] = 1;
    }
    else
    {
        msg.data[2] = 2;
    }
    
    msg.data[3] = 0;
    msg.data[4] = 0;

    msg.id = 0x18879100;
    msg.length = 5;
    rslt = 0;
    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &msg, rslt);
}

#ifdef DATASTORE_DEBUG
void DataStore_Test(void)
{
    uWord i;
    volatile uByte test_rslt;

    for(i = 0; i < MAX_RECORD_NUM; i++)
    {
        gby_test_Record[i] = 0;
        gby_Record[i] = 0;
    }

    // test one page mode
    gby_test_bmu_num = 2;
    gwd_test_bmu_bat_num = 30;
    gby_test_temper_num = 8;
    // 97 fixed byte + (3 + 2*bat_num + temper_num) * bmu_num = 239 byte

    DataStore_EnableEventDataStore(ERROR_DATA);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();

    if(DataStore_PageInfo.PageMode == ONE_PAGE)
    {
        test_rslt = DATASTORE_ERR_OK;
    }
    else
    {
        test_rslt = 0xff;
    }

    if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData)
    {
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 1, &gby_test_Record[0]);

        for(i = 0; i < gwd_RecordNum; i++)
        {
            if(gby_test_Record[i] != gby_Record[i])
            {
                test_rslt = 0xff;
                break;
            }
            else
            {
                test_rslt = DATASTORE_ERR_OK;
            }
        }
    }
    else
    {
        test_rslt = 0xff;
    }

    // test two page mode
    gby_test_bmu_num = 4;
    gwd_test_bmu_bat_num = 48;
    gby_test_temper_num = 8;
    // 97 fixed byte + (3 + 2*bat_num + temper_num) * bmu_num = 525 byte

    DataStore_EnableEventDataStore(ERROR_DATA);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();

    if(DataStore_PageInfo.PageMode == TWO_PAGE)
    {
        test_rslt = DATASTORE_ERR_OK;
    }
    else
    {
        test_rslt = 0xff;
    }

    if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData)
    {
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 2, &gby_test_Record[0]);
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 1, &gby_test_Record[gwd_MaxRecordNumPerPage]);

        for(i = 0; i < gwd_RecordNum; i++)
        {
            if(gby_test_Record[i] != gby_Record[i])
            {
                test_rslt = 0xff;
                break;
            }
            else
            {
                test_rslt = DATASTORE_ERR_OK;
            }
        }
    }
    else
    {
        test_rslt = 0xff;
    }

    // test three page mode
    gby_test_bmu_num = 4;
    gwd_test_bmu_bat_num = 60;
    gby_test_temper_num = 8;
    // 97 fixed byte + (3 + 2*bat_num + temper_num) * bmu_num = 621 byte

    DataStore_EnableEventDataStore(ERROR_DATA);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();

    if(DataStore_PageInfo.PageMode == THREE_PAGE)
    {
        test_rslt = DATASTORE_ERR_OK;
    }
    else
    {
        test_rslt = 0xff;
    }

    if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData)
    {
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 3, &gby_test_Record[0]);
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 2, &gby_test_Record[gwd_MaxRecordNumPerPage]);
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 1, &gby_test_Record[2*gwd_MaxRecordNumPerPage]);

        for(i = 0; i < gwd_RecordNum; i++)
        {
            if(gby_test_Record[i] != gby_Record[i])
            {
                test_rslt = 0xff;
                break;
            }
            else
            {
                test_rslt = DATASTORE_ERR_OK;
            }
        }
    }
    else
    {
        test_rslt = 0xff;
    }

    // test three page and many bmu mode
    gby_test_bmu_num = 20;
    gwd_test_bmu_bat_num = 10;
    gby_test_temper_num = 8;
    // 97 fixed byte + (3 + 2*bat_num + temper_num) * bmu_num = 717 byte

    DataStore_EnableEventDataStore(ERROR_DATA);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();
    ECU_DELAYMS(30);
    DataStore_ProcSaveData();

    if(DataStore_PageInfo.PageMode == THREE_PAGE)
    {
        test_rslt = DATASTORE_ERR_OK;
    }
    else
    {
        test_rslt = 0xff;
    }

    if(DATASTORE_FLAG_DISABLE == DataStore_CtrlInfo.CheckData)
    {
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 3, &gby_test_Record[0]);
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 2, &gby_test_Record[gwd_MaxRecordNumPerPage]);
        DataStore_ReadPage(DataStore_PageInfo.ErrorPage - 1, &gby_test_Record[2*gwd_MaxRecordNumPerPage]);

        for(i = 0; i < gwd_RecordNum; i++)
        {
            if(gby_test_Record[i] != gby_Record[i])
            {
                test_rslt = 0xff;
                break;
            }
            else
            {
                test_rslt = DATASTORE_ERR_OK;
            }
        }
    }
    else
    {
        test_rslt = 0xff;
    }
}
#endif
#define DATASTORE_PAGE_START0		0
#define DATASTORE_PAGE_END0		2048	// 512K
#define DATASTORE_PAGE_START1		2048
#define DATASTORE_PAGE_END1		4096	// 512K

#define DATASTORE_PAGE_NUMB	16		
#define DATASTORE_SECTOR_LEN	16
#define DATASTORE_PAGE_LEN		256

typedef enum
{
    DATASTORE_W_IDLE,
    DATASTORE_W_WRITE_START,
    DATASTORE_W_WRITE,
    DATASTORE_W_ERASE,
};
typedef struct
{
	uByte Count;
	uByte Step;
	uByte * RAMPTR pData;
	uWord StartPage;
	uWord CurPage;
	uWord PrePage;
	uWord dlen;
}sDataStore_UdsWrite;

sDataStore_UdsWrite DataStore_UdsWriteSt[2];
  asd
void DataStore_UdsWriteCbk(void)
{
	uByte step;
	uByte status;
	uWord nextSector;
	uWord pagestart;
	uWord pagestop;
	sDataStore_UdsWrite *DataStore_UdsWritePtr;

	if(DataStore_UdsWriteSt[0].Count || DataStore_UdsWriteSt[0].Count)
	{
		if(DataStore_UdsWriteSt[0].Count)
		{
			DataStore_UdsWritePtr = &(DataStore_UdsWriteSt[0]);
			pagestart = DATASTORE_PAGE_START0;
			pagestop = DATASTORE_PAGE_END0;
		}
		else
		{
			DataStore_UdsWritePtr = &(DataStore_UdsWriteSt[1]);
			pagestart = DATASTORE_PAGE_START1;
			pagestop = DATASTORE_PAGE_END1;
		}
		switch(DataStore_UdsWritePtr->Step)
		{
			case DATASTORE_W_WRITE_START:
			{
				DataStore_UdsWritePtr->PrePage = DataStore_UdsWritePtr->CurPage;
				DataStore_UdsWritePtr->Step = DATASTORE_W_WRITE;
				break;
			}

			case DATASTORE_W_WRITE:
			{
				status = (*NvmIf_FlashOperation.IndleCheck)();

				if(1 == status)
				{
					if(DataStore_UdsWritePtr->CurPage > DataStore_UdsWritePtr->StartPage + DATASTORE_PAGE_NUMB)
					{
						(*NvmIf_FlashOperation.WritePage)(DataStore_UdsWritePtr->CurPage, DataStore_UdsWritePtr->pData + (DataStore_UdsWritePtr->CurPage -DataStore_UdsWritePtr->StartPage)*DATASTORE_PAGE_LEN, DATASTORE_PAGE_LEN);
						DataStore_UdsWritePtr->CurPage++;
					}
					else
					{
						DataStore_UdsWritePtr->Step = DATASTORE_W_ERASE;
					}
				}

				break;
			}

			case DATASTORE_W_ERASE:
			{
				status = (*NvmIf_FlashOperation.IndleCheck)();

				if(1 == status)
				{
					if(pagestop <= DataStore_UdsWritePtr->CurPage + DATASTORE_PAGE_NUMB)
					{
						DataStore_UdsWritePtr->CurPage = pagestart;
						DataStore_EraseSector(pagestart);
					}
					else
					{
						nextSector = ((DataStore_UdsWritePtr->CurPage+DATASTORE_PAGE_NUMB)/DATASTORE_SECTOR_LEN);
						if((DataStore_UdsWritePtr->CurPage/DATASTORE_SECTOR_LEN) != nextSector)
						{
							DataStore_EraseSector(nextSector);
						}
					}
					DataStore_UdsWritePtr->StartPage = DataStore_UdsWritePtr->CurPage;
					if(DataStore_UdsWritePtr->Count)
					{
						DataStore_UdsWritePtr->Count--;
					}
					if(DataStore_UdsWritePtr->Count)
					{
						DataStore_UdsWritePtr->Step = DATASTORE_W_WRITE_START;
					}
					else
					{
						DataStore_UdsWritePtr->Step = DATASTORE_W_IDLE;
					}
				}
				break;
			}

			default:
			{
				break;
			}
		}
	}
	
}

void DataStore_UdsWrite(uByte * RAMPTR p_data, uByte index)
{
	DataStore_UdsWriteSt[index].pData = p_data;
	DataStore_UdsWriteSt[index].Count++;
}
void DataStore_UdsRead(uLWord addr, uByte * RAMPTR p_data, uWord dlen, uByte index)
{
	uByte status;
	uLWord startaddr;
	uWord offsetlen;
	
	startaddr = DataStore_UdsWriteSt[index].PrePage*DATASTORE_PAGE_LEN + addr;
	status = (*NvmIf_FlashOperation.IndleCheck)();
	if(1 == status)
	{
		if(addr/DATASTORE_PAGE_LEN != (addr+dlen)/DATASTORE_PAGE_LEN)
		{
			offsetlen = DATASTORE_PAGE_LEN - (startaddr%DATASTORE_PAGE_LEN);
			(*NvmIf_FlashOperation.NormalRead)(startaddr, p_data, offsetlen);
			(*NvmIf_FlashOperation.NormalRead)(startaddr+offsetlen, p_data+offsetlen, dlen-offsetlen);
		}

		else
		{
			(*NvmIf_FlashOperation.NormalRead)(startaddr, p_data, dlen);
		}

	}
}
#pragma DATA_SEG DEFAULT
