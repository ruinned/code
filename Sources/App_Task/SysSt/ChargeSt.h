// ChargeStData.h
#ifndef CHARGEST_H
#define CHARGEST_H

#include "GeneralTypeDef.h"


/* define for charge status */
#define CHG_ST_OFF  0x00
#define CHG_ST_ON   0x01
#define CHG_ST_END  0x02
#define CHG_ST_ERR  0x03


/*define for Charge stage */
#define CHG_STAGE_PRE_HEAT 1
#define CHG_STAGE_PRE_CHG  2
#define CHG_STAGE_CC_MODE  3
#define CHG_STAGE_CV_MODE  4
#define CHG_STAGE_READY_TO_END 5
#define CHG_STAGE_END      6

typedef struct
{
    // for AC charger
    uByte AC_LinkSt;   /* Charge ineterface detect,0:disable 1:enable  2:rsv  3: valid */
    uWord CableCap;    /* for Gb charge, 16A mode or 32A mode */
    uWord CC_Volt;     /* for Gb charge, detect point 4 voltage */
    uByte CC_St;       /* for Gb charge, detect point 4 status*/
    uWord CP_Period;   /* for Gb charge, detect point 3 period */
    uByte CP_DutyCycle;/* for Gb charge, detect point 3 pwm duty */
    uWord CP_PmtCur;

    // for DC charger
    uByte DC_LinkSt;    /* Charge ineterface detect,0:disable 1:enable  2:rsv  3: valid */
    uWord CC2_Volt;     /* for Gb charge, detect point 2 voltage */
    uByte CC2_St;       /* for Gb charge, detect point 2 status*/
} sChg_InterfaceType;

typedef struct
{
    uWord PermitMaxCellV;
    uWord PermitMaxSumV; 
    uWord PermitMaxCurrent;
    uByte PermitMinT;
    uWord PermitMinCurrent;  
    uWord DownCurrentStep;
    uWord PermitMinCellV; 
    uByte PermitMaxT;
    
} sChg_ParameterType;

typedef struct
{
    uByte PmtChgExtSt;
    uByte PmtChgOnSt;
} Chg_PmtChgStType;

typedef struct
{
    uWord AcPermitChgCur;
    uWord DcPermitChgCur;
} Chg_PmtChgCurType;

typedef struct
{
    Chg_PmtChgStType PermitChgSt;
    Chg_PmtChgCurType PermitChgCur;
    uByte ChgSt;       /* 0: handle  1: charging  2: charge full  3 : error*/
    uByte ChgStage;    /* 0: pre heat  1: pre chg 2: cc chg 3: cv chg 4: chg end */
    uWord AccumulatedChgTime;  /*  */
    uWord RemainChgTime;
} sChg_CtrlType;


typedef union
{
    uLWord Lwd;

    struct
    {
        uByte Rsv        : 8;
               
        uByte SumVHigh   : 1;
        uByte CellVLow   : 1;
        uByte CellVHigh  : 1;
        uByte SysIsoErr  : 1;        
        uByte HdErr      : 1;
        uByte ReachSoc   : 1;
        uByte ReachSumV  : 1;
        uByte ReachCellV : 1;
        
        uByte OutputConFault : 1;
        uByte ChgRlyFault : 1;
        uByte ChgFull     : 1;
        uByte BatNotMatch : 1;            
        uByte CurHigh    : 1;
        uByte TemperLow  : 1;
        uByte TemperHigh : 1;
        uByte SumVLow    : 1;
        
       
        uByte Rsv1            : 3;
        uByte OtherFault      : 1;
        
        uByte SmokeAlarmFault : 1;
        uByte AuxPowerFault   : 1;       
        uByte RxDataOverTime  : 1;
        uByte ChgConFault     : 1;
        
    } Bit;
} uBmsStopType;

typedef union
{
    uByte By;
    struct
    {
        uByte ReachCondition  : 2;
        uByte PeopleStop      : 2;
        uByte FaultStop       : 2; 
        uByte Rsv             : 2;
    } Bit;
} uChargerStopType;


typedef union
{
    uWord Wd;
    struct
    {
        uByte TripFault     : 2;
        uByte OtherFault    : 2;      
        uByte CurNotMatch   : 2; 
        uByte SumVFault     : 2;
        
        uByte TemperHigh    : 2;
        uByte ConectorFault : 2;
        uByte IntTemperHigh : 2;
        uByte EnergyFault   : 2;

       
        
    } Bit;

} uChargerFaultType;



typedef union
{
    uByte By;
    struct
    { 
      uByte FaultStop       : 2; 
      uByte PeopleStop      : 2; 
      uByte ReachCondition  : 2; 
      uByte Rsv             : 2;   
    } Bit;
} uChargerStopMotoType;


typedef union
{
    uWord Wd;
    struct
    {  
       uByte EnergyFault   : 2;
       uByte IntTemperHigh : 2;
       uByte ConectorFault : 2;
       uByte TemperHigh    : 2;
    
         
       uByte OtherFault    : 2; 
       uByte CurNotMatch   : 2; 
       uByte SumVFault     : 2; 
       uByte TripFault     : 2;
       
                 
    } Bit;

} uChargerFaultMotoType;



typedef struct
{
    uBmsStopType          BmsStop;
    uChargerStopType      ChargerStop;
    uChargerStopMotoType  ChargerStopMoto;
    
    uChargerFaultType      ChargerFault;
    uChargerFaultMotoType  ChargerFaultMoto;
} sChg_EndReasonType;

typedef struct
{
    uByte ChargerSt;        /* 0:normal 1: unnormal 2: rsv 3: valid */
    uWord OutPutCurrent;
    uWord OutPutVoltage;
    uWord MaxOutPutVoltage;
    uWord MinOutPutVoltage;
    uWord OutPutMaxCurrent;
    uWord OutPutMinCurrent;
    uWord OutPutEnergy;
    uWord ErrorCode;
} sChg_ChargerInfoType;

typedef struct
{
    sChg_InterfaceType  Chg_Interface;
    sChg_ParameterType  Chg_Parameter;
    sChg_CtrlType       Chg_Ctrl;
    sChg_EndReasonType  Chg_EndReason;
    sChg_ChargerInfoType  Chg_ChargerInfo;
} sChg_InfoType;

extern void ChgInfo_Init(void);

extern  uByte Chg_GetAcLinkSt(void);
extern  void Chg_UpdateAcLinkSt(uByte status);

extern  uWord Chg_GetCableCap(void);
extern  void Chg_UpdateCableCap(uWord cap);

extern  uWord Chg_GetCcVolt(void);
extern  void Chg_UpdateCCVolt(uWord volt);

extern  uByte Chg_GetCcSt(void);
extern  void Chg_UpdateCcSt(uByte status);

extern  uWord Chg_GetCpPeriod(void);
extern  void Chg_UpdateCpPeriod(uWord period);

extern  uByte Chg_GetCpDutyCycle(void);
extern  void Chg_UpdateCpDutyCycle(uByte duty);

extern  uWord Chg_GetCpPmtCur(void);
extern  void Chg_UpdateCpPmtCur(uWord cur);

extern  uByte Chg_GetDcLinkSt(void);
extern  void Chg_UpdateDcLinkSt(uByte status);

extern  uWord Chg_GetCc2Volt(void);
extern  void Chg_UpdateCc2Volt(uWord volt);

extern  uByte Chg_GetCc2St(void);
extern  void Chg_UpdateCc2St(uByte status);

extern  uByte Chg_GetPermitMaxT(void);
extern  void Chg_UpdatePermitMaxT(uByte max_t);

extern  uByte Chg_GetPermitMinT(void);
extern  void Chg_UpdatePermitMinT(uByte min_t);

extern  uWord Chg_GetPermitMaxSumV(void);
extern  void Chg_UpdatePermitMaxSumV(uWord sum_v);

extern  uWord Chg_GetPermitMaxCellV(void);
extern  void Chg_UpdatePermitMaxCellV(uWord max_v);

extern uWord Chg_GetPermitMinCellV(void);
extern void Chg_UpdatePermitMinCellV(uWord min_v);

extern  uWord Chg_GetPermitMaxCurrent(void);
extern  void Chg_UpdatePermitMaxCurrent(uWord cur);

extern  uWord Chg_GetPermitMinCurrent(void);
extern  void Chg_UpdatePermitMinCurrent(uWord cur);

extern  uWord Chg_GetDownCurrentStep(void);
extern  void Chg_UpdateDownCurrentStep(uWord cur_step);

extern uByte Chg_GetPermitChgExtSt(void);
extern void Chg_UpdatePermitChgExtSt(uByte status);

extern uByte Chg_GetPermitChgOnSt(void);
extern  void Chg_UpdatePermitChgOnSt(uByte status);

extern uWord Chg_GetPermitChgCurOn(void);
extern void Chg_UpdatePermitChgCurOn(uWord cur);

extern uWord Chg_GetPermitChgCurExt(void);
extern void Chg_UpdatePermitChgCurExt(uWord cur);

extern  uByte Chg_GetChgSt(void);
extern  void Chg_UpdateChgSt(uByte status);

extern  uByte Chg_GetChgStage(void);
extern  void Chg_UpdateChgStage(uByte stage);

extern  uWord Chg_GetAccumulatedChgTime(void);
extern  void Chg_UpdateAccumulatedChgTime(uWord time);

extern  uWord Chg_GetRemainChgTime(void);
extern  void Chg_UpdateRemainChgTime(uWord time);

extern  uLWord Chg_GetBmsStopReason(void);
extern  void Chg_UpdateBmsStopReason(uLWord reason);

extern  uByte Chg_GetChargerStopReason(void);
extern  void Chg_UpdateChargerStopReason(uByte reason);

extern  uWord Chg_GetChargerFault(void);
extern  void Chg_UpdateChargerFault(uWord fault);

extern  uByte Chg_GetChargerSt(void);
extern  void Chg_UpdateChargerSt(uByte status);

extern uWord Chg_GetOutPutVoltage(void);
extern void Chg_UpdateOutPutVoltage(uWord volt);

extern uWord Chg_GetOutPutCurrent(void);
extern void Chg_UpdateOutPutCurrent(uWord cur);

extern  uWord Chg_GetMaxOutPutVoltage(void);
extern  void Chg_UpdateMaxOutPutVoltage(uWord volt);

extern  uWord Chg_GetMinOutPutVoltage(void);
extern  void Chg_UpdateMinOutPutVoltage(uWord volt);

extern  uWord Chg_GetOutPutMinCurrent(void);
extern   void Chg_UpdateOutPutMinCurrent(uWord cur);

extern  uWord Chg_GetOutPutMaxCurrent(void);
extern   void Chg_UpdateOutPutMaxCurrent(uWord cur);

extern  uWord Chg_GetOutPutEnergy(void);
extern  void Chg_UpdateOutPutEnergy(uWord energy);

extern  uWord Chg_GetErrorCode(void);
extern  void Chg_UpdateErrorCode(uWord err_code);

#endif  /* ChargeSt_H */