#ifndef PROJECT_H
#define PROJECT_H

// define for ready flag
#define DEFAULT_READY         0x00
#define HARDWARE_READY        0x01
#define ALL_BMU_READY         0x02
#define MAXMIN_TEMPER_READY   0x04
#define MAXMIN_CELLV_READY    0x08
#define SUMV_READY            0x10
#define CUR_READY             0x20
#define ALL_CELLV_READY       0x40
#define ALL_TEMPER_READY      0x80

void Prj_PreMainInit(void);
void Prj_PosMainInit(void);
void Prj_UpdateRecvFirFrame(uByte bmu_no);
void Prj_MainCounterCbk(void);
void Prj_MainProCbk(void);

uByte Prj_GetReadyFlag(void);

extern uByte flag_all_err_send;
extern uWord Bat_Err_Detail;

extern uByte Fault_Sts;
extern uByte Fault_Lock_Sts;

#endif