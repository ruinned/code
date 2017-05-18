// CanCom_PbCfg.c
#include "CanCom_Type.h"
#include "Ecu.h"

/*
 * Msg buffer define for BCU_C&D
 */
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)) 
sCanIf_MsgType CCom_TxMsg[] =
{
   /* MainInfo 0 */
    {  
        0x4008000,          
        8,
        {
            0
        }

    },
    /* MainInfo 1 */
    {
        0x4018000,
        8,
        {
            0
        }
    },
    /* MainInfo 2 */
    {
        0x4028000,
        8,
        {
            0
        }
    },
     /* MainInfo 3 */
    {
        0x4038000,
        8,
        {
            0
        }
    },
     /* MainInfo 4 */
    {
        0x4048000,
        8,
        {
            0
        }
    },
     /* MainInfo 5 */
    {
        0x4058000,
        8,
        {
            0
        }
    },
    /* MainInfo 6 */
    {
        0x4068000,
        8,
        {
            0
        }
    },
    /* MainInfo 7 */
    {
        0x4078000,
        8,
        {
            0
        }
    },
    /* MainInfo 8 */
    {
        0x4088000,
        8,
        {
            0
        }
    },
    /* MainInfo 9 */
    {
        0x4098000,
        8,
        {
            0
        }
    },
    /* MainInfo 10 */
    {
        0x40A8000,
        8,
        {
            0
        }
    },
    /* MainInfo 11 */
    {
        0x40B8000,
        8,
        {
            0
        }
    },
    /* MainInfo 12 */
    {
        0x40C8000,
        8,
        {
            0
        }
    },
    /* MainInfo 13 ,rsv*/
    /*{
        0x40D8000,
        8,
        {
            0
        }
    }, */
     /* MainInfo 14 */
    {
        0x40E8000,
        8,
        {
            0
        }
    },
     /* MainInfo 15 */
    {
        0x40F8000,
        8,
        {
            0
        }
    },
     /* MainInfo 16 */
    {
        0x4108000,
        8,
        {
            0
        }
    },
    /* Event and cycle Info 1, not here ,deal in other place */
    /* Event and cycle Info 2 */
    {
        0x4818000,
        8,
        {
            0
        }
    },
    /* Event and cycle Info 3 */
    {
        0x4828000,
        8,
        {
            0
        }
    },
    /* Event and cycle Info 4 */
    {
        0x4838000,
        8,
        {
            0
        }
    },
    /* Event and cycle Info 5 */
    {
        0x4848000,
        8,
        {
            0
        }
    },
    /* Main Broadcast */
    {
        0x4FF3F00,
        8,
        {
            0
        }
    }    
    
};
/*
 * PDU definitions
 */
const sCCom_PduType CCom_Pdu[] =
{
    /* MainInfo 0 */
    {
        0, //FlgEndOfList
        0, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[0]) // PduMsgPtr
    },

  /* MainInfo 1 */
    {
        0, //FlgEndOfList
        1, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[1]) // PduMsgPtr
    },
    
  /* MainInfo 2 */
     {
        0, //FlgEndOfList
        2, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[2]) // PduMsgPtr
    },
    /* MainInfo 3 */
    {
        0, //FlgEndOfList
        3, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[3]) // PduMsgPtr
    },
    
    /* MainInfo 4 */
    {
        0, //FlgEndOfList
        4, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[4]) // PduMsgPtr
    },
    
    /* MainInfo 5 */
    {
        0, //FlgEndOfList
        5, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[5]) // PduMsgPtr
    },
    /* MainInfo 6 */
    {
        0, //FlgEndOfList
        6, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[6]) // PduMsgPtr
    },
    /* MainInfo 7 */
    {
        0, //FlgEndOfList
        7, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[7]) // PduMsgPtr
    },
     /* MainInfo 8 */
    {
        0, //FlgEndOfList
        7, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[8]) // PduMsgPtr
    },
     /* MainInfo 9 */
    {
        0, //FlgEndOfList
        9, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[9]) // PduMsgPtr
    },
     /* MainInfo 10 */
    {
        0, //FlgEndOfList
        10, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[10]) // PduMsgPtr
    },
      /* MainInfo 11 */
    {
        0, //FlgEndOfList
        11, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[11]) // PduMsgPtr
    },
      /* MainInfo 12 */
    {
        0, //FlgEndOfList
        12, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[12]) // PduMsgPtr
    },
    /* MainInfo 13 ,rsv*/
     /* MainInfo 14 */
    {
        0, //FlgEndOfList
        13, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[13]) // PduMsgPtr
    },
     /* MainInfo 15 */
    {
        0, //FlgEndOfList
        14, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[14]) // PduMsgPtr
    },
     /* MainInfo 16 */
    {
        0, //FlgEndOfList
        15, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[15]) // PduMsgPtr
    },
    
    /* Event and cycle Info 1, not here ,deal in other place */
    /* Event and cycle Info 2 */
    {
        0, //FlgEndOfList
        16, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_MIXED,//eTxModeMode
                255, //TxModeNumberOfRepetitions
                500, //TxModeRepetitionPeriod
                0, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[16]) // PduMsgPtr
    },
    /* Event and cycle Info 3 */
    {
        0, //FlgEndOfList
        17, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_MIXED,//eTxModeMode
                255, //TxModeNumberOfRepetitions
                1000, //TxModeRepetitionPeriod
                0, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[17]) // PduMsgPtr
    },
    /* Event and cycle Info 4 */
    {
        0, //FlgEndOfList
        18, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_MIXED,//eTxModeMode
                255, //TxModeNumberOfRepetitions
                1000, //TxModeRepetitionPeriod
                0, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[18]) // PduMsgPtr
    },
    /* Event and cycle Info 5 */     
    {
        0, //FlgEndOfList
        19, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_MIXED,//eTxModeMode
                255, //TxModeNumberOfRepetitions
                1000, //TxModeRepetitionPeriod
                0, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[19]) // PduMsgPtr
    },
    /* Main Broadcast */
    {
        0, //FlgEndOfList
        20, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[20]) // PduMsgPtr
    },
    
// END
/////////////////////////////////////////////////////////////////////////
    /* donnot add pdu below */
    {
        1,//FlgEndOfList, 1--- indicate list end
        0xFFFF, //PduHandleId, not care
        8, //PduSize, not care
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        NULL // PduMsgPtr

    }
};

sCCom_PduTxCtrlType CCom_PduTxCtrl[] =
{
    {
        0,
        0,
        0,
        0,
        STARTED,  // direct start
        {
            1U, // is first
            5L // TxFirstTimePeriodFactor, 5ms
        }         
    } ,
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STOPED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STOPED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STOPED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STOPED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    
}; 
#endif // #if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)) 

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
/*
 * Msg buffer define for BMU and BMU_R, default SA = 1,
 * And this can be modify in CCom_Init();
 */
sCanIf_MsgType CCom_TxMsg[] =
{
   /* MainInfo 0 */
    {  
        0x4008001,           
        8,
        {
            0
        }

    },
    /* MainInfo 1 */
    {
        0x4018001,
        8,
        {
            0
        }
    },
    /* MainInfo 2 */
    {
        0x4028001,
        8,
        {
            0
        }
    },
     /* MainInfo 3 */
    {
        0x4038001,
        8,
        {
            0
        }
    },
    /* No Msg in bmu from 4 to 11 */
    /* MainInfo 4 */      
    /* MainInfo 5 */    
    /* MainInfo 6 */   
    /* MainInfo 7 */    
    /* MainInfo 8 */   
    /* MainInfo 9 */     
    /* MainInfo 10 */    
    /* MainInfo 11 */ 
         
    /* MainInfo 12 */
    {
        0x40C8001,
        8,
        {
            0
        }
    },
    /* MainInfo 13 ,rsv*/
    /*{
        0x40D8000,
        8,
        {
            0
        }
    }, */
     /* MainInfo 14 */
    {
        0x40E8001,
        8,
        {
            0
        }
    },     
     /* 15~79 NO msg */    
     /* MainInfo 80 */ 
    {
        0x4508001, // 0x4500001
        8,
        {
            0
        }
    },
    //#if (ECU_CATEGORY == M_BMU_R)
    /* This msg CAN be trig when ECU is BMU_R.
     * Trig will be place in CCom_Init(); 
     */    
    /* Event and cycle Info 1 */
    {
        0x4838000,
        8,
        {
            0
        }
    },
   //#endif
    
    /* Main Broadcast */
    {
        0x4FF3F01,
        8,
        {
            0
        }
    }, 
       
    
};
/*
 * PDU definitions
 */
const sCCom_PduType CCom_Pdu[] =
{
    /* MainInfo 0 */
    {
        0, //FlgEndOfList
        0, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[0]) // PduMsgPtr
    },

  /* MainInfo 1 */
    {
        0, //FlgEndOfList
        1, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[1]) // PduMsgPtr
    },
    
  /* MainInfo 2 */
     {
        0, //FlgEndOfList
        2, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[2]) // PduMsgPtr
    },
    /* MainInfo 3 */
    {
        0, //FlgEndOfList
        3, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[3]) // PduMsgPtr
    },
    
    /* MainInfo 4 */    
    /* MainInfo 5 */     
    /* MainInfo 6 */    
    /* MainInfo 7 */  
    /* MainInfo 8 */    
    /* MainInfo 9 */    
    /* MainInfo 10 */
    /* MainInfo 11 */ 
        
    /* MainInfo 12 */
    {     
        0, //FlgEndOfList
        4, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                500, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[4]) // PduMsgPtr
    
    },
         
    /* MainInfo 13 ,rsv*/
     /* MainInfo 14 */
    {
        0, //FlgEndOfList
        5, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[5]) // PduMsgPtr
    },
     /* 15~79 NO msg */    
     /* MainInfo 80 */ 
     {
        0, //FlgEndOfList
        6, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                1000, //TxModeTimePeriodFactor
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[6]) // PduMsgPtr
    },     
    
    //#if (ECU_CATEGORY == M_BMU_R)
    /* This msg CAN be trig when ECU is BMU_R.
     * Trig will be place in CCom_Init(); 
     */    
    /* Event and cycle Info 1 */
    {
        0, //FlgEndOfList
        7, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_MIXED,//eTxModeMode
                255, //TxModeNumberOfRepetitions
                500, //TxModeRepetitionPeriod
                0, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[7]) // PduMsgPtr
    },
    
    /* Main Broadcast */
    {
        0, //FlgEndOfList
        8, //PduHandleId
        8,//PduSize
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_ENABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        &(CCom_TxMsg[8]) // PduMsgPtr
    },
    
// END
/////////////////////////////////////////////////////////////////////////
    /* donnot add pdu below */
    {
        1,//FlgEndOfList, 1--- indicate list end
        0xFFFF, //PduHandleId, not care
        8, //PduSize, not care
        SEND,
        {
            //PduTxPara
            {
                //sTxMode
                TX_PERIODIC,//eTxModeMode
                0, //TxModeNumberOfRepetitions
                0, //TxModeRepetitionPeriod
                DEFAULT_CYCYLE, //TxModeTimePeriodFactor,default period
                PERIOD_CHANGE_DISABLE//TxModePeriodChangeEnable
            }
        },
        INT_CAN_CHANNEL, //PduCanIfChn
        NULL // PduMsgPtr

    }
};

sCCom_PduTxCtrlType CCom_PduTxCtrl[] =
{
    {
        0,
        0,
        0,
        0,
        STARTED,  // direct start
        {
            1U, // is first
            5L // TxFirstTimePeriodFactor, 5ms
        }         
    } ,
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STOPED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },
    {
        0,
        0,
        0,
        0,
        STARTED,
        {
            1U, // is first
            5L  // TxFirstTimePeriodFactor, 5ms
        }
    },     
    
};

#endif // #if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))

const sCCom_ConfigType  CCom_Config =
{
    CCom_Pdu,
    CCom_PduTxCtrl    
};