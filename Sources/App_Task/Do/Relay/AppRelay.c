
#include "AppRelay.h"
#include "DataLayer.h"
#include "SchIf.h"

#include "RelayIf.h"
#include "BatterySysSt.h"

#define RELAY_FEED_BACK_START_NO 2
#define RELAY_ERR_MAX  5
#define ERR_RELAY_ID_NOT_MATCH 0x4000


typedef struct
{
    const uByte RelayId;
    eActor_RelayStatusType RelaySt;
} sActor_RelayType;

typedef struct
{
    uLWord DurationTimeCnt; // 1ms unit
    Bool  DurationTimeEn;
    eActor_RelayStatusType RelaySt_Bak;
} sActor_CtrlRelayParaType;

sActor_RelayType Actor_Relay[] =
{
    {(uByte)CPU_FAN_ID,RELAY_OPENED},
    {(uByte)CPU_HEAT_ID,RELAY_OPENED},
    {(uByte)POS_RELAY_ID,RELAY_OPENED},
    {(uByte)NEG_RELAY_ID,RELAY_OPENED},
    {(uByte)PRE_RELAY_ID,RELAY_OPENED},
    {(uByte)CHG_RELAY_ID,RELAY_OPENED},
    {(uByte)BCU_CTRL_BMU_FAN_ID,RELAY_OPENED},
    {(uByte)BCU_CTRL_BMU_HEAT_ID,RELAY_OPENED},
};


sActor_CtrlRelayParaType Actor_CtrlRelayPara[] =  //the para to control relay
{
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
    {0,FALSE,RELAY_OPENED},
};

#ifndef ECU_M_IOHWAB_PWMIF  //use the pwm to control relay
static eActor_RelayStatusType Actor_GetRelayFeedBackSt(uByte relay_id)  //read the feedback signal of relay
{
    uByte Relay_FeedBackSt;
    eActor_RelayStatusType rslt;
    Relay_FeedBackSt = 0;
    rslt = RELAY_RSV;

    switch(relay_id)
    {
    case POS_RELAY_ID: //control the pos relay
    {
        (void)RelayIf_FeedBack((uByte)RELAYIF_CPU_RLY_POS_NO,&Relay_FeedBackSt);
        break;
    }
    case NEG_RELAY_ID: //control the neg relay
    {
        (void)RelayIf_FeedBack((uByte)RELAYIF_CPU_RLY_NEG_NO,&Relay_FeedBackSt);
        break;
    }
    case PRE_RELAY_ID://control the pre relay
    {
        (void)RelayIf_FeedBack((uByte)RELAYIF_CPU_RLY_PRE_NO,&Relay_FeedBackSt);
        break;
    }
    case CHG_RELAY_ID://control the chg relay
    {
        (void)RelayIf_FeedBack((uByte)RELAYIF_CPU_RLY_CHG_NO,&Relay_FeedBackSt);
        break;
    }
    default:
    {
        //return rslt;
        break;
    }

    }
    if((uByte)OPEN_RELAY == Relay_FeedBackSt)
    {
        rslt = RELAY_OPENED;
    }
    else
    {
        rslt = RELAY_CLOSED;
    }

    return rslt;
}
#endif
GeneralReturnType Actor_RelayInit(void)
{
    uByte i;
    GeneralReturnType rslt;

    rslt = (GeneralReturnType)ERR_OK;

  //  rslt = RelayIf_IcChangeMode(0,(uByte)RELAYIF_IC_IO_MODE); 

    for(i = 0; i < RELAYIF_MAX_NUM; i++)
    {
        rslt = RelayIf_Ctrl(i,(uByte)OPEN_RELAY );  //OPEN_RELAY  init the relay control to open
#ifndef ECU_M_IOHWAB_PWMIF //if not use the PWM then have feedback signal
        Actor_Relay[i].RelaySt = RELAY_OPENED;
        if(i >= RELAY_FEED_BACK_START_NO)
        {
            Actor_Relay[i].RelaySt = RELAY_OPENED;//Actor_GetRelayFeedBackSt(i); //the feedback status
        }
#else
        Actor_Relay[i].RelaySt = RELAY_OPENED;
#endif
        Actor_CtrlRelayPara[i].RelaySt_Bak = Actor_Relay[i].RelaySt;
    }
    return rslt;
}

static GeneralReturnType Relay_Execute(uByte relay_id, sActor_CtrlRelayCmdType relay_cmd)
{
    GeneralReturnType rslt;
    GeneralReturnType relay_rslt;
    static uByte relay_err_cnt = 0;
    rslt = (GeneralReturnType)ERR_OK;
    relay_rslt = (GeneralReturnType)ERR_OK;
    if(Actor_Relay[relay_id].RelayId == relay_id)
    {
#ifndef ECU_M_IOHWAB_PWMIF
       /* if((relay_id >= RELAY_FEED_BACK_START_NO)
                && (relay_cmd.Bcmd == (bool)Actor_GetRelayFeedBackSt(relay_id)))
        {
            return rslt;
        } */
#endif
        if((relay_cmd.CmdType != IMMED_RELAY_CMD) && (relay_cmd.DuratonTime > 0))
        {
            // need keep some time
            relay_err_cnt = 0;
            Actor_CtrlRelayPara[relay_id].DurationTimeCnt = relay_cmd.DuratonTime / MIN_TIMER_MS;
            Actor_CtrlRelayPara[relay_id].DurationTimeEn = TRUE;
            if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
            {
                Actor_CtrlRelayPara[relay_id].RelaySt_Bak = RELAY_OPENED;
            }
            else
            {
                Actor_CtrlRelayPara[relay_id].RelaySt_Bak = RELAY_CLOSED;
            }

            if(TEST_RELAY_CMD == relay_cmd.CmdType)  //for the relay_test
            {
                Actor_CtrlRelayPara[relay_id].RelaySt_Bak = Actor_Relay[relay_id].RelaySt;
                if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                {
                    relay_rslt = RelayIf_Ctrl(relay_id,(uByte)OPEN_RELAY);
                }
                else
                {
                    relay_rslt = RelayIf_Ctrl(relay_id,(uByte)CLOSE_RELAY);
                }

                if(ERR_OK == relay_rslt)
                {
                    if(relay_id < RELAY_FEED_BACK_START_NO)
                    {
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
                    }
                    else
                    {
#ifndef ECU_M_IOHWAB_PWMIF
                        //Actor_Relay[relay_id].RelaySt = Actor_GetRelayFeedBackSt(relay_id);
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
#else
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
#endif
                    }
                    rslt = ERR_OK;
                }
                else
                {
                    Actor_CtrlRelayPara[relay_id].DurationTimeEn = FALSE;
                    rslt = relay_rslt;
                }
            }
        }
        else //immedial control
        {
            if(Actor_CtrlRelayPara[relay_id].DurationTimeEn)
            {
               // need wait complete DurationTime, this function is used for Test relay
            }
            else
            {
                if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                {
                    relay_rslt = RelayIf_Ctrl(relay_id,(uByte)OPEN_RELAY);
                }
                else
                {
                    relay_rslt = RelayIf_Ctrl(relay_id,(uByte)CLOSE_RELAY);
                }
                if(ERR_OK == relay_rslt)
                {
                    if(relay_id < RELAY_FEED_BACK_START_NO)
                    {
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
                    }
                    else
                    {
#ifndef ECU_M_IOHWAB_PWMIF
                       // Actor_Relay[relay_id].RelaySt = Actor_GetRelayFeedBackSt(relay_id);
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
#else
                        if(relay_cmd.Bcmd == (bool)RELAY_OPEN_CMD)
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_OPENED;
                        }
                        else
                        {
                            Actor_Relay[relay_id].RelaySt = RELAY_CLOSED;
                        }
#endif
                    }
                    Actor_CtrlRelayPara[relay_id].RelaySt_Bak = Actor_Relay[relay_id].RelaySt;
                    rslt = ERR_OK;
                }
                else
                {
                    if(relay_cmd.CmdType != IMMED_RELAY_CMD)
                    {
                        relay_err_cnt++;
                        if(relay_err_cnt > RELAY_ERR_MAX)
                        {
                            relay_err_cnt = RELAY_ERR_MAX;
                            Actor_CtrlRelayPara[relay_id].DurationTimeEn = FALSE;
                        }
                    }
                    rslt = relay_rslt;
                }
            }
        }
    }
    else
    {
        rslt = ERR_RELAY_ID_NOT_MATCH;
    }
    return rslt;
}

GeneralReturnType Actor_CtrlRelay(uByte relay_id, sActor_CtrlRelayCmdType relay_cmd)
{
    GeneralReturnType rslt;
    //uByte io_id;
    rslt = ERR_OK;

    switch(relay_id)
    {

    case CPU_FAN_ID: //fan relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_FAN_NO,relay_cmd);
        BatSys_UpdateAuxASt(Actor_GetRelaySt(CPU_FAN_ID));
        break;
    }
    case CPU_HEAT_ID: //heat relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_HEAT_NO,relay_cmd);
        BatSys_UpdateAuxBSt(Actor_GetRelaySt(CPU_HEAT_ID));

        break;
    }
    case POS_RELAY_ID: //pos relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_POS_NO,relay_cmd);
        BatSys_UpdatePosRlySt(Actor_GetRelaySt(POS_RELAY_ID));
        break;
    }
    case NEG_RELAY_ID: //neg relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_NEG_NO,relay_cmd);
        BatSys_UpdateNegRlySt(Actor_GetRelaySt(NEG_RELAY_ID));
        break;
    }
    case PRE_RELAY_ID: //pre relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_PRE_NO,relay_cmd);
        BatSys_UpdatePreRlySt(Actor_GetRelaySt(PRE_RELAY_ID));
        break;
    }
    case CHG_RELAY_ID: //chg relay
    {
        rslt = Relay_Execute((uByte)RELAYIF_CPU_RLY_CHG_NO,relay_cmd);
        BatSys_UpdateChgRlySt(Actor_GetRelaySt(CHG_RELAY_ID));
        break;
    }
    case BCU_CTRL_BMU_FAN_ID:  //bcu control the bmu fan relay
    {
        //rslt = Data_CtrlBmuRelay(CPU_FAN_ID, relay_cmd);
        break;
    }
    case BCU_CTRL_BMU_HEAT_ID:  //bcu control the bmu fan relay
    {
        //rslt = Data_CtrlBmuRelay(CPU_HEAT_ID, relay_cmd);
        break;
    }
    default:
    {
        break;
    }

    }

    return rslt;
}


eActor_RelayStatusType Actor_GetRelaySt(uByte relay_id)
{
    eActor_RelayStatusType relay_st;
    uWord num;
    relay_st = RELAY_RSV;
    num = sizeof(Actor_Relay)/sizeof(Actor_Relay[0]);
    if(relay_id < num)
    {
        if(relay_id == Actor_Relay[relay_id].RelayId)
        {
            relay_st = Actor_Relay[relay_id].RelaySt;
        }
    }

    return relay_st;
}

void Actor_RelayMain(void)  //for delay relay control and test relay
{
    // call by 5ms task
    uByte num;
    uByte i;
    sActor_CtrlRelayCmdType relay_cmd;
    relay_cmd.DuratonTime = 0;
    relay_cmd.Dute = 0;
    relay_cmd.Frequence = 0;

    num = sizeof(Actor_CtrlRelayPara)/sizeof(Actor_CtrlRelayPara[0]);
    for(i = 0; i < num; i++)
    {
        // rd relay status from pin
        //////////////////////

        if(Actor_CtrlRelayPara[i].DurationTimeEn)
        {
            if(Actor_CtrlRelayPara[i].DurationTimeCnt > 0)
            {
                Actor_CtrlRelayPara[i].DurationTimeCnt--;
            }

            if(0 == Actor_CtrlRelayPara[i].DurationTimeCnt)
            {
                // set relay status to last status

                Actor_CtrlRelayPara[i].DurationTimeEn = FALSE;
                if(Actor_CtrlRelayPara[i].RelaySt_Bak != Actor_Relay[i].RelaySt)
                {
                    if(RELAY_CLOSED == Actor_CtrlRelayPara[i].RelaySt_Bak)
                    {
                        relay_cmd.Bcmd = (bool)RELAY_CLOSE_CMD;
                    }
                    else // include error
                    {
                        relay_cmd.Bcmd = (bool)RELAY_OPEN_CMD;
                    }
                    if(!Actor_CtrlRelay(Actor_Relay[i].RelayId,relay_cmd)) //the delay control and test relay
                    {
                        Actor_Relay[i].RelaySt = Actor_CtrlRelayPara[i].RelaySt_Bak;
//                        Actor_CtrlRelayPara[i].DurationTimeEn = FALSE;
                    }
                }

            }
        }
    }
}
