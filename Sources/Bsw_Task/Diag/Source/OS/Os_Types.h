/*============================================================================*/
/** Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Os_Types.h>
 *  @brief      <>
 *  
 *  <>
 *  
 *  @author     <ISOFT>
 *  @date       <2012-09-14>
 */
/*============================================================================*/
#ifndef OS_TYPES_H
#define OS_TYPES_H

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef uint32 StackDataType;
typedef uint8 * StackPtrType;
typedef uint16 SizeType;

typedef uint8 TaskType;
typedef TaskType* TaskRefType;
typedef uint8 TaskStateType;
typedef TaskStateType* TaskStateRefType;
typedef uint8 ResourceType;
typedef uint32 EventMaskType;
typedef EventMaskType* EventMaskRefType;

typedef uint32 TickType;
typedef TickType* TickRefType;

typedef struct 
{
	TickType maxallowedvalue;
	TickType ticksperbase;
	TickType mincycle;
} AlarmBaseType;
typedef AlarmBaseType* AlarmBaseRefType;
typedef uint8 AlarmType;
typedef AlarmType* AlarmRefType;
typedef uint8 CounterType;
typedef void (*AlarmCallbackType)(void);
typedef uint16 AppModeType; 
typedef uint8 OSServiceIdType;
typedef uint8 PriorityType;
typedef uint8 IPLType;	
typedef uint8 ScheduleType;
typedef uint8 LockerType;
typedef uint8 CallLevelType;
typedef uint8 ApplicationType;
#define INVALID_OSAPPLICATION 0xFF

typedef enum 
{
	APPLICATION_ACCESSIBLE,
	APPLICATION_RESTARTING,
	APPLICATION_TERMINATED
} ApplicationStateType;

typedef ApplicationStateType * ApplicationStateRefType;
typedef uint8 TrustedFunctionIndexType;
typedef void * TrustedFunctionParameterRefType;
typedef sint8 AccessType;

typedef enum 
{
	ACCESS,
	NO_ACCESS
} ObjectAccessType;

typedef enum 
{
	OBJECT_TASK,
	OBJECT_ISR,
	OBJECT_ALARM,
	OBJECT_RESOURCE,
	OBJECT_COUNTER,
	OBJECT_SCHEDULETABLE
} ObjectTypeType;

typedef void * MemoryStartAddressType;

typedef uint32 MemorySizeType;
typedef uint8 ISRType;
typedef uint8 ScheduleTableType;

typedef enum 
{
	SCHEDULETABLE_STOPPED,
	SCHEDULETABLE_NEXT,
	SCHEDULETABLE_WAITING,
	SCHEDULETABLE_RUNNING,
	SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS
} ScheduleTableStatusType;

typedef ScheduleTableStatusType * ScheduleTableStatusRefType;

typedef enum 
{
	PRO_IGNORE,
	PRO_TERMINATETASKISR,
	PRO_TERMINATEAPPL,
	PRO_TERMINATEAPPL_RESTART,
	PRO_SHUTDOWN
} ProtectionReturnType;

typedef enum 
{
	RESTART,
	NO_RESTART
} RestartType;

typedef uint32 PhysicalTimeType;

#endif

 /*=======[R E V I S I O N   H I S T O R Y]====================================*/
/** <VERSION>  <DATE>  <AUTHOR>     <REVISION LOG>
 *    V1.0    20120919  ISOFT       Initial version
 */
/*=======[E N D   O F   F I L E]==============================================*/

