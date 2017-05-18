/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_IntErrId.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:41:01>
 */
/*============================================================================*/

/*============================================================================*/
#ifndef  DEM_INTERRID_H
#define  DEM_INTERRID_H
/*============================================================================*/
#define DEM_INTERRID_H_AR_MAJOR_VERSION   3
#define DEM_INTERRID_H_AR_MINOR_VERSION   1
#define DEM_INTERRID_H_AR_PATCH_VERSION   0
#define DEM_INTERRID_H_SW_MAJOR_VERSION  1
#define DEM_INTERRID_H_SW_MINOR_VERSION  0
#define DEM_INTERRID_H_SW_PATCH_VERSION  0

/*=======[M A C R O S]========================================================*/

/*
*Definition of Event IDs
*/

#define soc_too_low_level_1_ID 1
#define charge_current_too_large_level_1_ID 2
#define charge_current_too_large_level_2_ID 3
#define discharge_current_too_large_level_1_ID 4
#define discharge_current_too_large_level_2_ID 5
#define cell_voltage_too_high_level_1_ID 6
#define cell_voltage_too_high_level_2_ID 7
#define cell_voltage_too_high_level_3_ID 8
#define cell_voltage_too_low_level_1_ID 9
#define cell_voltage_too_low_level_2_ID 10
#define cell_voltage_too_low_level_3_ID 11
#define pack_voltage_too_high_level_1_ID 12
#define pack_voltage_too_high_level_2_ID 13
#define pack_voltage_too_low_level_1_ID 14
#define pack_voltage_too_low_level_2_ID 15
#define battery_temperature_too_high_level_1_ID 16
#define battery_temperature_too_high_level_2_ID 17
#define battery_temperature_too_high_level_3_ID 18
#define battery_tempreture_unbalance_level_2_ID 19
#define battery_cell_voltage_unbalance_level_2_ID 20
#define cell_voltage_detection_error_ID 21
#define tempreture_sensor_fault_ID 22
#define current_sensor_fault_ID 23
#define internal_sumvoltage_sensor_fault_ID 24
#define outer__sumvoltage_sensor_fault_ID 25
#define BMS_lost_internal_communication_for_1Second_ID 26
#define BMS_initialization_error_ID 27
#define positive_relay_stuck_or_cannot_close_ID 28
#define negtive_relay_stuck_or_cannot_close_ID 29
#define BMS_lost_car_communication_for_1Second_ID 30
#define isulation_resistance_less_than_100_Ohm_V_ID 31
#define HVIL_open_ID 32
#define precharge_fail_ID 33
#define BMS_reports_charger_charge_error_ID 34
#define Charger_charge_current_is_too_large_ID 35
#define precharge_relay_stuck_or_cannot_close_ID 36
#define charge_relay_stuck_or_cannot_close_ID 37
#define lost_communication_with_charger_for_1_Sec_ID 38
#define HCU_key_signal_match_fail_with_hardware_for_3__Sec_ID 39
#define emergency_line_status_mismatch_with_HVIL_status_ID 40
#define HV_system_isolation_fault_inside_of_BMS_ID 41
#define HV_system_isolation_fault_outside_of_BMS_ID 42
#define BMS_12V_battery_supply_voltage_is_abnormal_ID 43
#define BMS_cannot_receive_HCU_relay_open_request_after_key_off_ID 44
#define HV_circuit_failure_ID 45
#define Crash_detected_from_SRS_ID 46
#define BMS_main_fuse_open_ID 47
#define Liquid_level_sensor_error_ID 48
#define Battery_leakage_occurs_ID 49
#define internal_CAN_bus_off_error_ID 50
#define Car_communication_CAN_bus_off_error_ID 51
#define HVH_Fuse__failure_ID 52
#define AC_Charge_fuse__failure_ID 53
#define HVH_Relay__failure_ID 54
#define BMS_balance_system_failure_ID 55
#define CANTP_E_COM 56
#define CAN_E_TIMEOUT 57
#define CANIF_E_INVALID_DLC 58
#define CANIF_E_STOPPED 59
#define CANIF_E_FULL_TX_BUFFER 60
#define COMM_E_LOWER_LIMIT 61

/* CANSM_DEM_ERRID_0 + networkPtr->netId */
#define CANSM_DEM_ERRID_0  62




#endif  /* end of DEM_INTERRID_H */

/*=======[E N D   O F   F I L E]==============================================*/
