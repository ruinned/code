#ifndef GBCHGCAN_H
#define GBCHGCAN_H


//#include "GeneralTypeDef.h"
//#include "CanIf_Type.h"
#include "ChgM.h"
#include "CanIf.h"

/* define for GB charge protocol */

/**************************Charging handshake stage **********************************/

#define   ID_CHM     0x1826F456       //27930-2015

#define   ID_BHM     0x182756F4

/* MSG of charge */
#define   ID_CRM     0x1801F456
/*MSG of BMS */
#define   ID_BRM     0x180256F4

/******************** Charging parameter configuration stage *****************************/
/*MSG of charge */
#define   ID_CTS     0x1807F456
#define   ID_CML     0x1808F456
#define   ID_CRO     0x100AF456
/* MSG of BMS */
#define   ID_BCP     0x180656F4
#define   ID_BRO     0x100956F4



/******************Charging stage ***************************************************/
/* MSG of charge */
#define   ID_CCS     0x1812F456 
#define   ID_CST     0x101AF456
/* MSG of BMS */
#define   ID_BCL     0x181056F4
#define   ID_BCS     0x181156F4
#define   ID_BSM     0x181356F4
#define   ID_BMV     0x181556F4 
#define   ID_BMT     0x181656F4
#define   ID_BSP     0x181756F4 
#define   ID_BST     0x101956F4



/***************Charging end stage *************************************************/
/* MSG of charge */
#define   ID_CSD     0x181DF456
/* MSG of BMS */
#define   ID_BSD     0x181C56F4



/***************Error message *****************************************************/
/* MSG of charge */
#define   ID_CEM     0x081FF456 

/*MSG of BMS */
#define   ID_BEM      0x081E56F4 

/*MSG of Tpcm */
#define   ID_TPCM    0x1CEC56F4

#define  ID_TPCM_CHG  0x1CECF456

/* MSG of Tpcm_DATA */
#define   ID_TPCM_DATA    0x1CEB56F4

/* MSG PF of BMS */
#define   BRM_PF  0x02
#define   BCP_PF  0x06
/* #define   BRO_PF  0x09 */
/* #define   BCL_PF  0x10 */
#define   BCS_PF  0x11
/* #define   Bsm_PF  0x13 */
#define   BMV_PF  0x15
#define   BMT_PF  0x16
#define   BSP_PF  0x17
/* #define   BST_PF  0x19 */
/* #define   BSD_PF  0x1C */
/* #define   BEM_PF  0x1E */

#define   BHM_PF   0x27

/* MSG PF of CHG */
#define   CRM_PF  0x01
#define   CTS_PF  0x07
#define   CML_PF  0x08
#define   CRO_PF  0x0A
#define   CCS_PF  0x12
#define   CST_PF  0x1A
#define   CSD_PF  0x1D
#define   CEM_PF  0x1F 

#define   CHM_PF  0x26

/* MSG PF of Tpcm */
#define   TPCM_PF  0xEC



/* MASK of receive CHG MSG */
#define   CHM_RX  0x26

#define   CRM_RX  0x01
#define   CTS_RX  0x02
#define   CML_RX  0x04
#define   CRO_RX  0x08
#define   CCS_RX  0x10
#define   CST_RX  0x20
#define   CSD_RX  0x40
#define   CEM_RX  0x80   

/* MASK of error */
#define   RX_CHM_ERR	 0x01
#define   RX_CRM0_ERR	 0x01
#define   RX_CRM1_ERR	 0x04

#define   RX_CTS_CML_ERR	 0x01
#define   RX_CRO_ERR	     0x04

#define   RX_CCS_ERR	 0x01
#define   RX_CST_ERR	 0x04

#define   RX_CSD_ERR	 0x01

/* other */
//#define CHG_RX_BMS      0xAA
//#define CHG_NOT_RX_BMS  0x00    

//#define CHG_READY      0xAA
//#define CHG_NOT_READY  0x00
#define BMS_READY      0xAAU
#define BMS_NOT_READY  0x00U

#define CHG_MODE_CV    0x01U
#define CHG_MODE_CC    0x02U

/* GB charge end reason */
//#define  REACH_SOC     0x01
//#define  REACH_SUMV    0x04
//#define  REACH_CELLV   0x10
//#define  ISO_ERR       0x01
//#define  CON_OVER_T    0x04
//#define  BMS_OVER_T    0x10
//#define  CHG_CON_ERR   0x40
//#define  BAT_OVER_T    0x01
//#define  OTHER_ERR     0x04
//#define  OVER_CUR      0x01
//#define  OVER_VOLT     0x04


/* Cmd of Tpcm */
#define RTS          16
#define CTS          17
#define END_MSG_ACK  19
#define ABORT        255

/* MASK of Tpcm */
#define TPCM_NULL    0x00
#define TPCM_BRM     0x01
#define TPCM_BCP     0x02
#define TPCM_BCS     0x04
#define TPCM_BMV     0x08
#define TPCM_BMT     0x10
#define TPCM_BSP     0x20


/* Byte num of Tpcm */
///#define BYTE_NUM_BRM   49//41
#define BYTE_NUM_BCP   13
#define BYTE_NUM_BCS   9
/*
#define BYTE_NUM_BMV   240  // need to modify
#define BYTE_NUM_BMT   16   // need to modify
*/
#define BYTE_NUM_BSP   12   // need to modify

/* msg num of Tpcm */
//#define MSG_NUM_BRM   7//6
#define MSG_NUM_BCP   2
#define MSG_NUM_BCS   2
/*
#define MSG_NUM_BMV   35 // need to modify
#define MSG_NUM_BMT   3   // need to modify
*/
#define MSG_NUM_BSP   2   // need to modify


#define CNT_LINK    200   //2S, for detect


typedef struct 
{
    
    uByte Brm : 1; 					 //bms recognise
    uByte Bcp : 1; 					 //battery charge data
    uByte Bro : 1;				   //battery Ready to charge
    uByte Bcl : 1; 					 //battery charge requirement
    
    uByte Bcs : 1; 					 //battery charge status
    uByte Bsm : 1;					 //bms send battery status
    uByte Bmv : 1; 					 //battery voltage
    uByte Bmt : 1;					 //battery temperature
    
    uByte Bsp : 1;					 //battery reserved
    uByte Bst : 1; 					 //bms stop charge
    uByte Bsd : 1;				   //bms statistics data message
    uByte Bem : 1; 					 //bms error message

    uByte RxCtsBrm : 1;		 //receive cts of Brm
    uByte RxCtsBcp : 1;		 //receive cts of Bcp
    uByte RxCtsBcs : 1;		 //receive cts of Bcs  
    uByte RxCtsBmv : 1;    //receive cts of Bmv
    
    uByte RxCtsBmt : 1; 	 //receive cts of Bmt
    uByte RxCtsBsp : 1; 	 //receive cts of Bsp
    uByte RxBcsFinish : 1;
    uByte Rsv        : 5;

   // uWord rx_chg_msg;		 //receive charge message
}sGbChgCan_FLG;

typedef struct
{
    uByte MsgNumBrm;	    //request send Brm msg num.
    uByte MsgNumBcp;		  //request send Bcp msg num.
    uByte MsgNumBcs;			//request send Bcs msg num.
    uByte MsgNumBmv;			//request send Bmv msg num.
    uByte MsgNumBmt;		  //request send Bmt msg num.
    uByte MsgNumBsp;			//request send Bsp msg num.

    uByte MsgNoBrm;       //send Brm msg num
    uByte MsgNoBcp;				//send Bcp msg num
    uByte MsgNoBcs;				//send Bcs msg num
    uByte MsgNoBmv;				//send Bmv msg num
    uByte MsgNoBmt;				//send Bmt msg num
    uByte MsgNoBsp;				//send Bsp msg num

  
    uByte Tpcm ; 
    sGbChgCan_FLG  Flg; 

}sGbChgCan_CtrlType;

typedef struct
{
    uByte ChgVer[3];
}sGbChg_Chm;

typedef struct
{
    uByte RxBms;
    uByte No;
    uByte Region[6];
} sGbChg_Crm;

typedef struct
{
    uByte Sec;
    uByte Min;
    uByte Hour;
    uByte Day;
    uByte Month;
    uWord Year;
} sGbChg_Cts;

typedef struct
{
    uWord MaxOutSumv;
    uWord MinOutSumv;
    uWord MaxOutCur;
} sGbChg_Cml;

typedef struct
{
    uByte Ready;
} sGbChg_Cro;

typedef struct
{
    uWord OutputSumv;
    uWord OutputCur;
    uWord ChgTime;
    uByte ChgEn;   //GB27930-2011
} sGbChg_Ccs;

typedef struct
{
    uByte StopReason;
    uWord FaultReason;
    uByte ErrorReason;
} sGbChg_Cct;


typedef struct
{
    uWord ChgTime;
    uWord ChgEng;
    uByte ChgNo;
} sGbChg_Csd;

typedef struct
{
    uByte BmsTimeOut[4];
} sGbChg_Cem;

typedef struct
{
    sGbChg_Chm  Chm;
    sGbChg_Crm  Crm;
    sGbChg_Cts  Cts;
    sGbChg_Cml  Cml;
    sGbChg_Cro  Cro;
    sGbChg_Ccs  Ccs;
    sGbChg_Cct  Cct;
    sGbChg_Csd  Csd;
    sGbChg_Cem  Cem;
    uWord RxFlg;
} sGbChgCan_RxType;

typedef struct
{
    uByte Brm; 									//bms recognise
    uByte Bcp; 									//battery charge data
    uByte Bro;									//battery ready to charge
    uByte Bcl; 									//battery charge requirement
    uByte Bcs; 									//battery charge status
    uByte Bsm;									//bms send battery status
    uByte Bmv; 									//battery voltage
    uByte Bmt;									//battery temperature
    uByte Bsp;									//battery reserved
    uByte Bst; 									//bms stop charge
    uByte Bsd;									//bms statistics data message
    uByte Bem; 								  //bms error message
    
    uByte Tpcm;									//mutipacket msg
    uByte WaitAckBrm;		      //wait for ack of brm          
    uByte WaitAckBcp;			  	//wait for ack of bcp  			
    uByte WaitAckBcs;				  //wait for ack of bcs  
    uByte WaitAckBmv;					//wait for ack of bmv  
    uByte WaitAckBmt;					//wait for ack of bmt  
    uByte WaitAckBsp;					//wait for ack of bsp  
    uByte WaitCtsBrm;					//wait for cts of brm  
    uByte WaitCtsBcp;					//wait for cts of bcp  
    uByte WaitCtsBcs;					//wait for cts of bcs  
    uByte WaitCtsBmv;					//wait for cts of bmv  
    uByte WaitCtsBmt;					//wait for cts of bmt  
    uByte WaitCtsBsp;					//wait for cts of bsp  
    
    uWord RxChgMsg;  				
      
} sGbChgCan_TmCntType;

extern void GbChgCan_TxData(void);
extern void GbChgCan_Init(void);
extern void GbChg_CountCbk(void);

extern void GbChg_RxDataFromCharger(sCanIf_MsgType msg);
extern void GbChgCan_LoadData(void);
extern void GbChgCan_TpcmProc(void);
  


#endif /* GBCHG_H  */