// SensorDi.h
#ifndef SENSOR_H
#define SENSOR_H

#include "GeneralTypeDef.h"
#include "Ecu.h"


typedef enum
{
  DI_STATUS_LOW,
  DI_STATUS_HIGH,
  DI_STATUS_ERROR
}eSensor_DiStatusType;

#define CPU_DI1_ID 0
#define CPU_DI2_ID 1
#define CPU_DI3_ID 2
#define CPU_DI4_ID 3


extern eSensor_DiStatusType Sensor_GetDiSt(uByte di_id);//,eSensor_DiStatusType *StPtr);

#endif