
#ifndef PWMIF_TYPE_H
#define PWMIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"
#ifdef ECU_M_IOHWAB_PWMIF

#define PWMIF_MAX_NUM  4
 

typedef enum 
{
   PWMIF_RELAY_POS_NO,
   PWMIF_RELAY_NEG_NO,
   PWMIF_RELAY_PRE_NO,
   PWMIF_RELAY_CHG_NO,
   /* the number of PWM,can be add upon,such as PWM9,PWM10...*/
}ePwmIf_IdNumType;

/*typedef struct 
{
   uWord duty;
}sPwm_DutyType; */

typedef enum
{
   START_LEVEL_LOW,
   START_LEVEL_HIGH
}ePwmIf_StartLevelType;   //the start level

typedef uByte (* PwmIf_EnablePtrtype)(void);   //enable pwm,start pwm function
typedef uByte (* PwmIf_DisablePtrtype)(void);  //disable pwm
typedef uByte (* PwmIf_SetRatio16Ptrtype)(uWord time);  //set the pwm duty
typedef uByte (* PwmIf_SetValuePtrtype)(void);    //set the start level to high 
typedef uByte (* PwmIf_ClrValuePtrtype)(void);   //set the start level to low

typedef struct 
{
    const ePwmIf_IdNumType PwmIf_IdNum;  //the PWM number
    uWord PwmIf_Ratio;     //the pwm duty
    ePwmIf_StartLevelType PwmIf_StartLevel;  //the pwm start level
}sPwmIf_PropertyType;  //pwm property

typedef struct 
{
    PwmIf_EnablePtrtype  PwmIf_EnablePtr;
    PwmIf_DisablePtrtype  PwmIf_DisablePtr;
    PwmIf_SetRatio16Ptrtype  PwmIf_SetRatio16Ptr;
    PwmIf_SetValuePtrtype  PwmIf_SetValuePtr;
    PwmIf_ClrValuePtrtype  PwmIf_ClrValuePtr;
}sPwmIf_OperationType;

typedef struct 
{
    sPwmIf_PropertyType *PwmIf_PropertyPtr;
    const sPwmIf_OperationType *PwmIf_OperationPtr;
}sPwmIf_HandleType;
#endif
#endif 
 

