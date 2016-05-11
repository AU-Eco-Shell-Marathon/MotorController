/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "CAN.h"
#include "CAN_func.h"

void CAN_func(uint8 mailbox,enum CAN_flag);

struct BMSData BMS;

struct BMSData * CAN_init()
{
    CAN_1_Start();
    return &BMS;
}

void CAN_func(uint8 mailbox,enum CAN_flag flag)
{
    uint16 ID = CAN_1_GET_RX_ID(mailbox);
    uint8 DLC = CAN_1_GET_DLC(mailbox);
    switch (flag){
        case CAN_STATUS :
            if(DLC < 6) break; 
            BMS.state = CAN_1_RX_DATA_BYTE(mailbox,0);
            BMS.RunTime = ((uint16)CAN_1_RX_DATA_BYTE(mailbox,1)<<8) || ((uint16)CAN_1_RX_DATA_BYTE(mailbox,2));
            BMS.NewData |= (1<<0);
            break;
        case BATTERY_V :
            if(DLC < 6) break; 
            BMS.battery_V = ((uint16)CAN_1_RX_DATA_BYTE(mailbox,0)<<8) || ((uint16)CAN_1_RX_DATA_BYTE(mailbox,1));
            BMS.battery_cell_V_min = CAN_1_RX_DATA_BYTE(mailbox,2);
            BMS.battery_cell_no_min = CAN_1_RX_DATA_BYTE(mailbox,3);
            BMS.battery_cell_V_max = CAN_1_RX_DATA_BYTE(mailbox,4);
            BMS.battery_cell_no_max = CAN_1_RX_DATA_BYTE(mailbox,5);
            BMS.NewData |= (1<<1);
            break;
        case BATTERY_A :
            if(DLC < 6) break; 
            BMS.battery_A = ((uint16)CAN_1_RX_DATA_BYTE(mailbox,0)<<8) || ((uint16)CAN_1_RX_DATA_BYTE(mailbox,1));
            BMS.NewData |= (1<<2);
            break;
        case DOD :
            if(DLC < 7) break; 
            BMS.SOC = CAN_1_RX_DATA_BYTE(mailbox,0);
            BMS.DOD = ((uint16)CAN_1_RX_DATA_BYTE(mailbox,1)<<8) || ((uint16)CAN_1_RX_DATA_BYTE(mailbox,2));
            BMS.capacity = ((uint16)CAN_1_RX_DATA_BYTE(mailbox,3)<<8) || ((uint16)CAN_1_RX_DATA_BYTE(mailbox,4));
            BMS.NewData |= (1<<3);
            break;
        case T_BATTERY :
            if(DLC < 6) break; 
            BMS.Tbatt = CAN_1_RX_DATA_BYTE(mailbox,0);
            BMS.NewData |= (1<<4);
            break;
        case IR :
            if(DLC < 6) break;
            BMS.IR_min = CAN_1_RX_DATA_BYTE(mailbox,2); 
            BMS.IR_no_min = CAN_1_RX_DATA_BYTE(mailbox,3);
            BMS.IR_max = CAN_1_RX_DATA_BYTE(mailbox,4);
            BMS.IR_no_max = CAN_1_RX_DATA_BYTE(mailbox,5);
            BMS.NewData |= (1<<5);
        default :
            break;
    }
}

/* [] END OF FILE */
