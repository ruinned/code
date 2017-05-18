/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <Can_Irq.c>
 *  @brief      <Can Irq Isr Source code>
 *
 * <Compiler: CodeWarrior V5.1    MCU:MC9S12XEP100>
 *
 *  @author     <bo.zeng>
 *  @date       <04-05-2014>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140504   bo.zeng     Initial version
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CAN_IRQ_C_AR_MAJOR_VERSION      2
#define CAN_IRQ_C_AR_MINOR_VERSION      4
#define CAN_IRQ_C_AR_PATCH_VERSION      0
#define CAN_IRQ_C_SW_MAJOR_VERSION      1
#define CAN_IRQ_C_SW_MINOR_VERSION      0
#define CAN_IRQ_C_SW_PATCH_VERSION      0

/*=======[I N C L U D E S]====================================================*/
#include "Can.h"
#include "Can_Irq.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (CAN_IRQ_C_AR_MAJOR_VERSION != CAN_IRQ_H_AR_MAJOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Major Version"
#endif 
#if (CAN_IRQ_C_AR_MINOR_VERSION != CAN_IRQ_H_AR_MINOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Minor Version"
#endif
#if (CAN_IRQ_C_AR_PATCH_VERSION != CAN_IRQ_H_AR_PATCH_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Patch Version"
#endif
#if (CAN_IRQ_C_SW_MAJOR_VERSION != CAN_IRQ_H_SW_MAJOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Major Version"
#endif
#if (CAN_IRQ_C_SW_MINOR_VERSION != CAN_IRQ_H_SW_MINOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Minor Version"
#endif

#if (2 != CAN_H_AR_MAJOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Major Version"
#endif
#if (4 != CAN_H_AR_MINOR_VERSION)
    #error "Can_Irq.c:Mismatch in Specification Minor Version"
#endif

#define CAN_START_SEC_CODE_FAST
#include "Can_MemMap.h"
#pragma CODE_SEG __NEAR_SEG NON_BANKED
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_TxISR(void)
{
    Can_TxProcess(CAN_CONTROLLER_0);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_RxISR(void)
{
    Can_RxProcess(CAN_CONTROLLER_0);
    return;
}  
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_ErrTwRwBusoffISR(void)
{
    Can_ErrTwRwBusOff_Handler(CAN_CONTROLLER_0);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_0_WakeupISR(void)
{
    Can_Wakeup_Handler(CAN_CONTROLLER_0);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_TxISR(void)
{
    Can_TxProcess(CAN_CONTROLLER_1);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_RxISR(void)
{
    Can_RxProcess(CAN_CONTROLLER_1);
    return;
}  
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_ErrTwRwBusoffISR(void)
{
    Can_ErrTwRwBusOff_Handler(CAN_CONTROLLER_1);
    return;
}

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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_1_WakeupISR(void)
{
    Can_Wakeup_Handler(CAN_CONTROLLER_1);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_TxISR(void)
{
    Can_TxProcess(CAN_CONTROLLER_2);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_RxISR(void)
{
    Can_RxProcess(CAN_CONTROLLER_2);
    return;
}  
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_ErrTwRwBusoffISR(void)
{
    Can_ErrTwRwBusOff_Handler(CAN_CONTROLLER_2);
    return;
}

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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_2_WakeupISR(void)
{
    Can_Wakeup_Handler(CAN_CONTROLLER_2);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_TxISR(void)
{
    Can_TxProcess(CAN_CONTROLLER_3);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_RxISR(void)
{
    Can_RxProcess(CAN_CONTROLLER_3);
    return;
}  
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_ErrTwRwBusoffISR(void)
{
    Can_ErrTwRwBusOff_Handler(CAN_CONTROLLER_3);
    return;
}

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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_3_WakeupISR(void)
{
    Can_Wakeup_Handler(CAN_CONTROLLER_3);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_TxISR(void)
{
    Can_TxProcess(CAN_CONTROLLER_4);
    return;
}
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_RxISR(void)
{
    Can_RxProcess(CAN_CONTROLLER_4);
    return;
}  
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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_ErrTwRwBusoffISR(void)
{
    Can_ErrTwRwBusOff_Handler(CAN_CONTROLLER_4);
    return;
}

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
INTERRUPT FUNC(void, CAN_CODE_FAST) Can_4_WakeupISR(void)
{
    Can_Wakeup_Handler(CAN_CONTROLLER_4);
    return;
}
#endif /* STD_ON == CAN_CONTROLLER4_WAKEUP_INTERRUPT */
#endif /* STD_ON == CAN_WAKEUP_SUPPORT */
#define CAN_STOP_SEC_CODE_FAST
#include "Can_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/

