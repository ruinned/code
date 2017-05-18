#ifndef PRJ_DEFINE_H
#define PRJ_DEFINE_H

#include "Elock.h"
#include "GeneralTypeDef.h"


/*            define for SOC         */
/*            define for SOC         */
/*            define for SOC         */
#define SOC_FUN_ENABLE   1        //功能开关使能
#define SOC_FUN_DISABLE  0        //功能开关关闭

#define OCV_INIT_ESTMAT   SOC_FUN_DISABLE //开机修正功能开关
#define CHG_END_ESTMAT    SOC_FUN_ENABLE //充满修正功能开关
#define LMT_LOW_ESTMAT    SOC_FUN_DISABLE //极限低端修正功能开关
#define LMT_HIGH_ESTMAT   SOC_FUN_DISABLE  //极限高端修正功能开关
#define SOC_SMOOTH        SOC_FUN_DISABLE  //平滑修正功能开关

#define DELTAV_SOC_LESS_985  200U   // unit 1mV  //设置距离充电恒压点单体电压大于此值时，SOC不大于98.5%
#define DELTAV_SOC_LESS_995  100U   // unit 1mV  //设置距离充电恒压点单体电压大于此值时，SOC不大于99.5%

#define DELTA_MIN_V          1000U  // unit 1mV //平均电压比最低单体电压大于此值时，SOC修正时判断如果以最低电压作为判断条件的要以平均电压作为判断条件

#define OCV_STANDBY_TIME     3600U // unit 1s   //设定开机修正时静置时间，单位1S
#define OCV_LINEAR_RANGE_L1  0     // unit 1mV  //设定开机修正单体电压线性段起始电压
#define OCV_LINEAR_RANGE_H1  4200U // unit 1mV  //设定开机修正单体电压线性段结束电压
#define OCV_LINEAR_RANGE_L2  0     // unit 1mV  //设定开机修正单体电压线性段起始电压
#define OCV_LINEAR_RANGE_H2  4200 // unit 1mV  //设定开机修正单体电压线性段结束电压

#define AVGV_CHG_END         4000//4120U // unit 1mV //设定充满修正时平均电压需要达到的条件

#define MAX_CELLV_LMT        4200U // unit 1mV   //设定极限高端修正的单体电压阈值，此值建议大于单体电压过高三级故障阈值一点
#define MIN_CELLV_LMT        2500U // unit 1mV   //设定极限低端修正的单体电压阈值，此值建议大于单体电压过低三级故障阈值一点

#define SOC_MAX_SOC_POINT  21U //设定开机修正电池数据表的SOC点个数
#define SOC_MAX_T_POINT    4U  //设定开机修正电池数据表的温度点个数


/*            end of define for SOC        */
/*            end of define for SOC        */
/*            end of define for SOC        */



/*            define for SOF         */
/*            define for SOF         */
/*            define for SOF         */
#define SOF_FUN_ENABLE      1        //功能开关使能
#define SOF_FUN_DISABLE     0        //功能开关关闭

#define SOF_DCH_CUR_VOLT_MODIFY   SOF_FUN_DISABLE   //计算最大允许放电电流时考虑根据单体电压做修正的功能开关
#define SOF_DCH_CUR_ERR_MODIFY    SOF_FUN_DISABLE   //计算最大允许放电电流时考虑根据故障条件做修正的功能开关
#define SOF_FD_CUR_VOLT_MODIFY    SOF_FUN_DISABLE   //计算最大允许回馈电流时考虑根据单体电压做修正的功能开关
#define SOF_FD_CUR_ERR_MODIFY     SOF_FUN_DISABLE   //计算最大允许回馈电流时考虑根据故障条件做修正的功能开关
#define SOF_PLUSE_CUR_MODIFY      SOF_FUN_DISABLE   //计算最大允许脉冲电流功能开关
#define SOF_CALC_MAX_POWER        SOF_FUN_ENABLE    //计算最大允许功率（持续功率、脉冲功率）功能开关
#define SOF_USER_FUN              SOF_FUN_ENABLE    //用户自定义计算放电、回馈电流、功率功能

#define SOF_MAX_T_POINT      12    //功率电池数据表的温度点个数
#define SOF_MAX_SOC_POINT    12    //功率电池数据表的SOC点个数
#define SOF_MAX_VOLT_POINT   4     //计算功率时考虑电压修正的电压表电压个数

#define SOF_CONST_DCHCUR_SMOOTH_UNIT       10    // 0.1A/10ms, unit 0.1A   //计算完持续放电电流后从当前电流值平滑修正到目标值的速度
#define SOF_CONST_FEEDBACKCUR_SMOOTH_UNIT  10    // 0.1A/10ms, unit 0.1A   //计算完持续回馈电流后从当前电流值平滑修正到目标值的速度
#define SOF_PULSE_DCHCUR_SMOOTH_UNIT       10    // 0.1A/10ms, unit 0.1A   //计算完脉冲放电电流后从当前电流值平滑修正到目标值的速度
#define SOF_PULSE_FEEDBACKCUR_SMOOTH_UNIT  10    // 0.1A/10ms, unit 0.1A   //计算完脉冲回馈电流后从当前电流值平滑修正到目标值的速度

#define SOF_CONST_DCHPOW_SMOOTH_UNIT       10    // 0.1kw/10ms, unit 0.1kw //计算完持续放电功率后从当前电流值平滑修正到目标值的速度
#define SOF_CONST_FEEDBACKPOW_SMOOTH_UNIT  10    // 0.1kw/10ms, unit 0.1kw //计算完持续回馈功率后从当前电流值平滑修正到目标值的速度
#define SOF_PULSE_DCHPOW_SMOOTH_UNIT       10    // 0.1kw/10ms, unit 0.1kw //计算完脉冲放电功率后从当前电流值平滑修正到目标值的速度
#define SOF_PULSE_FEEDBACKPOW_SMOOTH_UNIT  10    // 0.1kw/10ms, unit 0.1kw //计算完脉冲回馈功率后从当前电流值平滑修正到目标值的速度

#define SOF_MODIFY_CELLV_LOW_L1    50    //单体电压过低一级故障的降放电功率百分比值设定
#define SOF_MODIFY_CELLV_LOW_L2    0     //单体电压过低二级故障的降放电功率百分比值设定
#define SOF_MODIFY_SUMV_LOW_L1     50    //总电压过低一级故障的降放电功率百分比值设定
#define SOF_MODIFY_SUMV_LOW_L2     0     //总电压过低二级故障的降放电功率百分比值设定
#define SOF_MODIFY_DCHCUR_HIGH_L1  90    //放电电流过高一级故障的降放电功率百分比值设定
#define SOF_MODIFY_DCHCUR_HIGH_L2  80    //放电电流过高二级故障的降放电功率百分比值设定

#define SOF_MODIFY_CELLV_HIGH_L1   50   //单体电压过高一级故障的降回馈功率百分比值设定
#define SOF_MODIFY_CELLV_HIGH_L2   0    //单体电压过高二级故障的降回馈功率百分比值设定
#define SOF_MODIFY_SUMV_HIGH_L1    50   //总电压过高一级故障的降回馈功率百分比值设定
#define SOF_MODIFY_SUMV_HIGH_L2    0    //总电压过高二级故障的降回馈功率百分比值设定
#define SOF_MODIFY_FDCUR_HIGH_L1   90   //回馈电流过高一级故障的降回馈功率百分比值设定
#define SOF_MODIFY_FDCUR_HIGH_L2   80   //回馈电流过高二级故障的降回馈功率百分比值设定

#define SOF_MODIFY_DETVOLT_L1      90   //压差过大一级故障降放电、回馈功率百分比值设定
#define SOF_MODIFY_DETVOLT_L2      80   //压差过大二级故障降放电、回馈功率百分比值设定
#define SOF_MODIFY_DETTEMP_L1      90   //温差过大一级故障降放电、回馈功率百分比值设定
#define SOF_MODIFY_DETTEMP_L2      80   //温差过大二级故障降放电、回馈功率百分比值设定
#define SOF_MODIFY_INTCAN_ERR      0    //内部CAN通信故障降放电、回馈功率百分比值设定

/*            end of define for SOF        */
/*            end of define for SOF        */
/*            end of define for SOF        */



/*            define for CHG         */
/*            define for CHG         */
/*            define for CHG         */
#define ELOCK_TEMP_POINT  166     //电子锁NTC温度点个数
#define ELOCK_TEMP_POINT1  166     //电子锁NTC温度点个数
//#define ELOCK_INIT_CMD()  ELOCK_LEFT_UNLOCK_FD_HIGH()  //上电开始控制电子锁解锁
#define CHGM_ERR_MAX_CELLV      4200     /* 3.7V */    //设定允许充电的最高电压阈值，最高单体电压大于此值，充电管理判断不允许充电
#define CHGM_ERR_MIN_CELLV      1000   /* 1V*/   //设定允许充电的最低电压阈值，最低单体电压小于此值，充电管理判断不允许充电

#define CHGM_ERR_MAX_SUMV       4030     /* 390V */ //设定允许充电的最高总电压阈值，总电压大于此值，充电管理判断不允许充电
#define CHGM_ERR_MIN_SUMV       1000     /* 100V */ //设定允许充电的最低总电压阈值，总电压小于此值，充电管理判断不允许充电

#define CHGM_ERR_MAX_TEMPER     105    /* 60'C, *///设定允许充电的最高温度阈值，最高温度大于此值，充电管理判断不允许充电
#define CHGM_ERR_MIN_TEMPER     20     /* -10'C,*/ //设定允许充电的最低温度阈值，最低温度小于此值，充电管理判断不允许充电

#define MINV_DOWN_CC_CUR   2000  /* 2.0V */   //设定当最低单体电压小于此设定值时，充电管理计算充电允许电流在原来基础上减半，也就是低压太低，建议小电流充电
#define MAXV_DOWN_CC_CUR   4030  //3600  /* 3.6V *///设定当充电时间小于3分钟但是最高单体电压已经超过此设定值，判定为充满后反复充电，加快降电流以防出现过充故障

#define CHG_DET_LIMIT_T    //充电管理判断允不允许充电时是否判断温度过高和过低这两个条件的功能开关
#define CHG_DET_LOW_V      //充电管理判断允不允许充电时是否判断单体电压和总压过过低这两个条件的功能开关
#define CHG_DET_GB_PLUG_CUR //计算交流充电允许充电电流时是否考虑CC、CP的功能开关

#define CHG_RELAY_CTRL //判断不允许充电时是否需要断开继电器判断标志功能开关


#define CHGM_JUDGE_CUR          300                  /* 30 A*/ //设定当最大允许充电电流大于此值时，判断充电过流的标准为计算的最大允许充电电流*1.5倍
#define CHGM_ERR_MAX_CUR        (OFFSET_CUR - 450)   /* 45A */ //设定当最大允许充电电流小于上面设定值时，判断充电过流标准统一为当前电流大于此设定值
#define CHGM_DET_START_CHG_CUR  (OFFSET_CUR - 20)    /* 2A */ //设定计算充电时间的最小电流值，也就是只有电流大于此值才认为进行充电过程，计算充电时间

/*            end of define for CHG        */
/*            end of define for CHG        */
/*            end of define for CHG        */

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

#pragma DATA_SEG DEFAULT

#endif //#define PRJ_DEFINE_H