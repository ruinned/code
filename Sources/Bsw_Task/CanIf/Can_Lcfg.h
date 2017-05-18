// Can_Lcfg.h
#ifndef CAN_LCFG_H
#define CAN_LCFG_H

#include "GeneralTypeDef.h"

/*typedef struct {
	void (*CancelTxConfirmation)( const Can_PduType *);
	void (*RxIndication)( uint8 ,Can_IdType ,uint8 , const uint8 * );
	void (*ControllerBusOff)(uint8);
	void (*TxConfirmation)(PduIdType);
	void (*ControllerWakeup)(uint8);
	void (*Arc_Error)(uint8,Can_Arc_ErrorType);
} Can_CallbackType;  */

typedef struct
{   
  uByte (* ReadFramePtr)(uLWord *MessageID,uByte *FrameType,uByte *FrameFormat,uByte *Length,uByte *Data);
  uByte (* SendFramePtr)(uByte BufferNum,uLWord MessageID,uByte FrameType,uByte Length,const uByte *Data);
} sCan_RxTxHandleType;  

typedef struct
{
  uByte (* CanEnable)(void);
  uByte (* CanDisable)(void);
  void (* CanEnableEvent)(void);
  void (* CanDisableEvent)(void);
  uByte (* SetAcceptanceCode)(uLWord AccCode1, uLWord AccCode2);
  uByte (* SetAcceptanceMask)(uLWord AccMask1, uLWord AccMask2);
  void (* SetCanBaudrate)(uByte Baudrate);
} sCan_CtrlHandleType;

typedef struct
{
    void(* WakeAndSleepCtrl)(uByte mode);
}sCan_NmCtrlType;

typedef struct
{
  const sCan_RxTxHandleType * Can_RxTxHandlePtr;
  const sCan_CtrlHandleType * Can_CtrlHandleTypePtr;
  const sCan_NmCtrlType *Can_NmCtrlPtr;
} sCan_HandleType;


typedef enum
{
  CAN_HWBUF_EMPTY,
  CAN_HWBUF_HAD_DATE, //had data but not full
  CAN_HWBUF_FULL,
}eCan_HWBufStatusType;

typedef struct
{
  eCan_HWBufStatusType Can_RxStatus;
  eCan_HWBufStatusType Can_TxStatus;
}sCan_RxTxStatusType;

typedef struct
{  
   eCan_HWBufStatusType (* CanGetRxStatus)(void);
   eCan_HWBufStatusType (* CanGetTxStatus)(void);
} sCan_RxTxStatusHandleType;


extern const sCan_HandleType Can_Handle[];

#endif /* CAN_LCFG_H */