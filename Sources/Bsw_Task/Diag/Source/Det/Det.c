/*============================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       Det.c
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Detailed description of the file(multi-line)>
 *
 *  @author     liuyp
 *  @date       <2012-8-10>
 */
/*============================================================================*/
/*===========================================================================*/
/*					Include Files											 */
/*===========================================================================*/

#if 1 
#include "Det.h"/*@req DET-INFO-001 DET004*/
void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
	/*if the DET state is not DET_START ,return*/
	uint16 uModuleId = 0;
	uint8 uErrorId = 0;
	uint8 uInstanceId = 0;
	uint8 uApiId = 0;
	
	uModuleId = ModuleId, 
	uInstanceId = InstanceId;
	uApiId = ApiId;
	uErrorId = ErrorId;

    return;
}
#else
#include "Det.h"/*@req DET-INFO-001 DET004*/
#include "MemMap.h" /*@req DET-INFO-002 DET006*/
#include "serial.h"


/*********************internal data types**********************/

/*the state of the DET module */
typedef enum
{
    /*module initialized*/
    DET_INIT,
    /*module not initialized*/
    DET_UNINIT,
    /*module started*/
    DET_START
}Det_StateType;

/*define a circular queue*/
typedef struct
{
  /*the head pointer*/
    uint8 front;
    /*the rear pointer*/
    uint8 rear;
    /*the counter for recording the number of the development error*/
    uint8 count;
    /*the array for storing the development error information*/
    uint16 Det_Error_Array[DET_ARRAY_LENGTH][4];
}Det_ErrorCirQueueType;

/*===========================================================================*/
/*					Local Data Definitions									 */
/*===========================================================================*/
/*module state of DET*/
STATIC Det_StateType Det_ModuleState = DET_UNINIT;
/*the queue for dealing with the Error Information*/
STATIC Det_ErrorCirQueueType Det_ErrorCirQueue;

/****************************** implementations *********************************/

/*******************************public functions*********************************/

#if (DET_VERSION_INFO_API == STD_ON)
/**
  * @brief
  *		get the version information of the DET
  * @param versioninfo  version information of DET
  *  @req DET-APIR-007[DET011]
 */
void Det_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	/*@req DET-APIR-008[DET012]*/
	if (NULL_PTR != versioninfo)
    {
		versioninfo->moduleID = DET_MODULE_ID;//MODULE_ID_DET;
    	versioninfo->vendorID = DET_VENDOR_ID;
    	versioninfo->instanceID = DET_INSTANCED_ID;
    	versioninfo->sw_major_version = DET_SW_MAJOR_VERSION;
    	versioninfo->sw_minor_version = DET_SW_MINOR_VERSION;
    	versioninfo->sw_patch_version = DET_SW_PATCH_VERSION;
	}
    return;
}
#endif /* end of #if (DET_VERSION_INFO_API == STD_ON) */

/**
 * @brief
 *  initialize the  DET ,including the initialization of the circular queue.
 *@req DET-APIR-001[DET008]
 */
void Det_Init(void)
{
	uint8 i = 0;
	uint8 j = 0;

    /*initialize the queue*/
    /*@req DET-APIR-002[DET100]*/
    Det_ErrorCirQueue.front = 0;
    Det_ErrorCirQueue.rear = 0;
    Det_ErrorCirQueue.count = 0;
    for (i = 0; i < DET_ARRAY_LENGTH; i++)
    {
        for (j = 0; j < 4u; j++)
        {
            Det_ErrorCirQueue.Det_Error_Array[i][j] = 0;
        }
    }

    /*initialize the serial port*/
    BSP_InitializeDisplay();

    /*set the state DET_INIT*/
    Det_ModuleState = DET_INIT;
    return;
}

/**
 * @brief
 *  start the DET module
 *req DET-APIR-007[DET010]
 *DET-APIR-008[DET002]
 */
void Det_Start(void)
{
	if(DET_INIT == Det_ModuleState)
	{
      Det_ModuleState = DET_START;
	}
	return;
}

/**
 * @brief
 *  the main function of DET,print the development error information by the  serial port.
     this interface not depend on Autosar,but only for App to debug
 */
void Det_MainFunction(void)
{
	uint8 i = 0;
	uint8 numberOfError = 0;

	/*if the DET state is not DET_START ,return*/
	if(DET_START != Det_ModuleState)
	{
		return;
	}

	/*if the queue is empty, return*/
	if(0 == Det_ErrorCirQueue.count)
	{
		return;
	}

	/*print the development error information, which is in the queue*/
	numberOfError = Det_ErrorCirQueue.count;
	for(i=0; i < numberOfError; i++)
	{
		printk("%s\n", "The development error is:");
		printk("0X%x\n", Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.front][0]);
		printk("0X%x\n", Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.front][1]);
		printk("0X%x\n", Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.front][2]);
		printk("0X%x\n", Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.front][3]);

		/*change the queue*/
		Det_ErrorCirQueue.count--;
		Det_ErrorCirQueue.front = (Det_ErrorCirQueue.front + (uint8)1) % DET_ARRAY_LENGTH;
	}
	return;
}


/**
 * @brief
 *  reprot the development error information,store the error information by the  array.
 * @param  ModuleId Module ID of calling module
 * @param  InstanceId identifier of the index based instance of a module
 * @param  ApiId ID of API service in which error is detected
 * @param  ErrorId ID of detected development error
 * @req DET-APIR-003[DET009]
 */
void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
	/*if the DET state is not DET_START ,return*/
    if (DET_START != Det_ModuleState)
    {
        return;
    }

    /*if the queue is full, return*/
    if (DET_ARRAY_LENGTH == Det_ErrorCirQueue.count)
    {
        return;
    }

    /*save the development error information into ram
     but we must make sure the queue is enough else the
     newerrorinformation will be missed
     @req DET-APIR-004[DET001]*/
    Det_ErrorCirQueue.count++;
    Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][0] = ModuleId;
    Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][1] = (uint16) InstanceId;
    Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][2] = (uint16) ApiId;
    Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][3] = (uint16) ErrorId;

    /*print the current error information,modified by liuyp*/
    printk("%s\n", "The development error is:");
    printk("0X%x\n",Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][0]);
    printk("0X%x\n",Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][1]);
    printk("0X%x\n",Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][2]);
    printk("0X%x\n",Det_ErrorCirQueue.Det_Error_Array[Det_ErrorCirQueue.rear][3]);
    /*change the rear of the queue*/
    Det_ErrorCirQueue.rear = (Det_ErrorCirQueue.rear + (uint8)1)% DET_ARRAY_LENGTH;

    return;
}
#endif
/*===========================================================================*/
/*					End of File												 */
/*===========================================================================*/
