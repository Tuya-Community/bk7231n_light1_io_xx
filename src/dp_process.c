/* Private includes ----------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_cloud_com_defs.h"


#include "light_system.h"
#include "dp_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @Function: update_all_dp
 * @Description: 上报所有DP点状态
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T update_all_dp(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 1; /* 上传 DP 点个数 */

    /* 没有连接到路由器，退出 */
    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    /* 申请内存空间 */
    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    /* 初始化申请到的内存空间 */
    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    /* 填写要上报的数据 */
    dp_arr[0].dpid = DPID_LIGHT_SWITCH; /* DP ID */
    dp_arr[0].type = PROP_BOOL; /* DP 数据类型 */
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = get_cur_light_status(); /* DP 数据内容 */

    /* 上报DP数据 */
    op_ret = dev_report_dp_json_async(NULL ,dp_arr, dp_cnt);

    /* 使用完成，释放申请到的内存空间 */
    Free(dp_arr);
    dp_arr = NULL;
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }
}

/**
 * @Function: deal_dp_proc
 * @Description: DP下发处理函数
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    OPERATE_RET op_ret = OPRT_OK;

    UCHAR_T dpid;
    dpid = root->dpid;
    PR_DEBUG("dpid:%d",dpid);

     switch(dpid) {
        case DPID_LIGHT_SWITCH:
            if (root->value.dp_bool == TRUE) { /*  */
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
            
            /* 设备状态发生改变后，应上报当前设备状态 */
            update_all_dp();
            break;

        default :
            break;
    }
}


