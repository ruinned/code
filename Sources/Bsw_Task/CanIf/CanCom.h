 // CanCom.h
#ifndef CANCOM_H
#define CANCOM_H

#include "GeneralTypeDef.h"
#include "CanCom_Type.h"

/*
 *  initial Can channel , and initial COM control parameter
 */
void CCom_Init(void);
/*
 * Copy msg data to pdu, and set pdu contrl parameter,and will start send by pdu mode.
 * PduInfoPtr max length is 8 for CAN
 */
uByte CCom_CopyTxDataToPdu(CCom_PduIdType Pdu_Id, sCCOM_PduInfoType * PduInfoPtr);

/*
 * Update period of pdu in cycle periodic mode.
 * This interface support modify cycle by node number.
 * Need config "TxModePeriodChangeEnable" to PERIOD_CHANGE_ENABLE 
 * in file CanCom_Pbcfg.c
 */
uByte CCom_UpdateTxCycle(CCom_PduIdType Pdu_Id,uLWord Cycle);

/*
 * Set Pdu StartFlag.
 * These interfaces use for Direct mode pdu
 * Usecase:
   CCom_TxEnablePdu(Pdu_Id);
   CCom_CopyTxDataToPdu(Pdu_Id,PduInfoPtr);   
 * This will send this pdu immediate.
 * When need stop 
   CCom_TxDisablePdu(Pdu_Id);  
 */
uByte CCom_TxEnablePdu(CCom_PduIdType Pdu_Id);
uByte CCom_TxDisablePdu(CCom_PduIdType Pdu_Id);

void CCom_MainTx(void);
void CCom_MainRx(void);


#endif  /* CAN_COM_H */
 