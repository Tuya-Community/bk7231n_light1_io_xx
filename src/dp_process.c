/**
* @file dp_process.c
* @author www.tuya.com
* @brief 
* @version 0.1
* @date 2021-08-19
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#include "uni_log.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_cloud_com_defs.h"


#include "light_system.h"
#include "dp_process.h"

/***********************************************************
*************************micro define***********************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/

/***********************************************************
***********************function define**********************
***********************************************************/

/**
* @brief upload all dp data
*
* @return none
*/
VOID_T update_all_dp(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 1; /* update DP number */

    /* no connect router, return */
    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    /* initialize requested memory space */
    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_LIGHT_SWITCH; /* DP ID */
    dp_arr[0].type = PROP_BOOL; /* DP type */
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = get_light_status(); /* DP data */

    /* report DP */
    op_ret = dev_report_dp_json_async(NULL ,dp_arr, dp_cnt);

    /* free requested memory space */
    Free(dp_arr);
    dp_arr = NULL;
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }
}

/**
* @brief handle dp commands from the cloud
*
* @param[in] root: pointer header for dp data
* @return none
*/
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    OPERATE_RET op_ret = OPRT_OK;

    UCHAR_T dpid;
    dpid = root->dpid;
    PR_DEBUG("dpid:%d", dpid);

    switch(dpid) {
        case DPID_LIGHT_SWITCH:
            if (root->value.dp_bool == TRUE) {
                op_ret = set_light_status(LIGHT_ON);
                if (op_ret != OPRT_OK) {
                    PR_ERR("dp process set light status error, %d", op_ret);
                    return;
                }
            } else {
                op_ret = set_light_status(LIGHT_OFF);
                if (op_ret != OPRT_OK) {
                    PR_ERR("dp process set light status error, %d", op_ret);
                    return;
                }
            }
            
            /* update device current status to cloud */
            update_all_dp();
            break;

        default :
            break;
    }
}
