/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Can_Irq.h>
 *  @brief      <Can Irq Isr header file>
 *  
 *  <Compiler: CodeWarrior V5.1    MCU:MC9S12XEP100>
 *  
 *  @author     <bo.zeng>
 *  @date       <04-05-2014>
 */
/*============================================================================*/
#ifndef  CAN_IRQ_H
#define  CAN_IRQ_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140504  bo.zeng    Initial version
 * 
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Can_Cfg.h"

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
/* @req <CAN085> Source File Version Information  */
#define CAN_IRQ_H_AR_MAJOR_VERSION  2
#define CAN_IRQ_H_AR_MINOR_VERSION  4
#define CAN_IRQ_H_AR_PATCH_VERSION  0

#define CAN_IRQ_H_SW_MAJOR_VERSION  1
#define CAN_IRQ_H_SW_MINOR_VERSION  0
#define CAN_IRQ_H_SW_PATCH_VERSION  0

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CAN_START_SEC_CODE_FAST
#include "Can_MemMap.h"
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#if (STD_ON == CAN_CONTROLLER0_TX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Tx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_TxISR(void);
#endif /* STD_ON == CAN_CONTROLLER0_TX_INTERRUPT */

#if (STD_ON == CAN_CONTROLLER0_RX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Rx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_RxISR(void);
#endif /* STD_ON == CAN_CONTROLLER0_RX_INTERRUPT */
/******************************************************************************/
/*
 * Brief               <CAN Error/Bus-off Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_ErrTwRwBusoffISR(void);
#pragma CODE_SEG DEFAULT
#if (STD_ON == CAN_WAKEUP_SUPPORT)
#if (STD_ON == CAN_CONTROLLER0_WAKEUP_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Wakeup Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_WakeupISR(void);
#endif /* STD_ON == CAN_CONTROLLER0_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */

#if (STD_ON == CAN_CONTROLLER1_TX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Tx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_TxISR(void);
#endif /* STD_ON == CAN_CONTROLLER1_TX_INTERRUPT */

#if (STD_ON == CAN_CONTROLLER1_RX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Rx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_RxISR(void);
#endif /* STD_ON == CAN_CONTROLLER1_RX_INTERRUPT */

/******************************************************************************/
/*
 * Brief               <CAN Error/Bus-off Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_ErrTwRwBusoffISR(void);

#if (STD_ON == CAN_WAKEUP_SUPPORT)
#if (STD_ON == CAN_CONTROLLER1_WAKEUP_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Wakeup Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_WakeupISR(void);
#endif /* STD_ON == CAN_CONTROLLER1_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */

#if (STD_ON == CAN_CONTROLLER2_TX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Tx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_TxISR(void);
#endif /* STD_ON == CAN_CONTROLLER2_TX_INTERRUPT */

#if (STD_ON == CAN_CONTROLLER2_RX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Rx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_RxISR(void);
#endif /* STD_ON == CAN_CONTROLLER2_RX_INTERRUPT */

/******************************************************************************/
/*
 * Brief               <CAN Error/Bus-off Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_ErrTwRwBusoffISR(void);

#if (STD_ON == CAN_WAKEUP_SUPPORT)
#if (STD_ON == CAN_CONTROLLER2_WAKEUP_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Wakeup Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_WakeupISR(void);
#endif /* STD_ON == CAN_CONTROLLER2_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */

#if (STD_ON == CAN_CONTROLLER3_TX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Tx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_TxISR(void);
#endif /* STD_ON == CAN_CONTROLLER3_TX_INTERRUPT */

#if (STD_ON == CAN_CONTROLLER3_RX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Rx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_RxISR(void);
#endif /* STD_ON == CAN_CONTROLLER3_RX_INTERRUPT */

/******************************************************************************/
/*
 * Brief               <CAN Error/Bus-off Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_ErrTwRwBusoffISR(void);

#if (STD_ON == CAN_WAKEUP_SUPPORT)
#if (STD_ON == CAN_CONTROLLER3_WAKEUP_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Wakeup Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_WakeupISR(void);
#endif /* STD_ON == CAN_CONTROLLER3_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */

#if (STD_ON == CAN_CONTROLLER4_TX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Tx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_TxISR(void);
#endif /* STD_ON == CAN_CONTROLLER4_TX_INTERRUPT */

#if (STD_ON == CAN_CONTROLLER4_RX_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Rx Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_RxISR(void);
#endif /* STD_ON == CAN_CONTROLLER4_RX_INTERRUPT */

/******************************************************************************/
/*
 * Brief               <CAN Error/Bus-off Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_ErrTwRwBusoffISR(void);

#if (STD_ON == CAN_WAKEUP_SUPPORT)
#if (STD_ON == CAN_CONTROLLER4_WAKEUP_INTERRUPT)
/******************************************************************************/
/*
 * Brief               <CAN Wakeup Interrupt Service Routine process>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_WakeupISR(void);
#endif /* STD_ON == CAN_CONTROLLER4_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */

#define CAN_STOP_SEC_CODE_FAST
#include "Can_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CAN_START_SEC_CODE
#include "Can_MemMap.h"
/******************************************************************************/
/*
 * Brief               <This function performs Tx Interrupt Handle>
 * Param-Name[in]      <Controller - CAN controller to be error process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, CAN_CODE) Can_TxProcess(uint8 Controller);

/******************************************************************************/
/*
 * Brief               <This function performs Rx Interrupt Handle>
 * Param-Name[in]      <Controller - CAN controller to be error process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, CAN_CODE) Can_RxProcess(uint8 Controller);

/******************************************************************************/
/*
 * Brief               <This function performs error/tw/rw bus-off  process>
 * Param-Name[in]      <Controller- CAN controller to be Tw/Rw/bus-off process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, CAN_CODE) Can_ErrTwRwBusOff_Handler(uint8 Controller);
 
/******************************************************************************/
/*
 * Brief               <This function performs wake up  process>
 * Param-Name[in]      <Controller- CAN controller to be wake up process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, CAN_CODE) Can_Wakeup_Handler(uint8 Controller);
#define CAN_STOP_SEC_CODE
#include "Can_MemMap.h"

#endif  /* #ifndef  CAN_IRQ_H */

/*=======[E N D   O F   F I L E]==============================================*/



