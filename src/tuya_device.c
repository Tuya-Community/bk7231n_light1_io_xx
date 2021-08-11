/*
 * @Author: shiliu
 * @email: 
 * @LastEditors: shiliu
 * @file name: tuya_device.c
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-07-29 15:10:00
 */

#define _TUYA_DEVICE_GLOBAL

/* Private includes ----------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_iot_wifi_api.h"
#include "tuya_hal_system.h"
#include "tuya_iot_com_api.h"
#include "tuya_error_code.h"
#include "gw_intf.h"
#include "tuya_gpio.h"
#include "tuya_key.h"
#include "base_event_info.h"


#include "tuya_device.h"
#include "light_system.h"
#include "dp_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* wifi config */
#define WIFI_WORK_MODE_SEL          GWCM_OLD_PROD   /* select Wi-Fi work mode */
#define WIFI_CONNECT_OVERTIME_S     180             /* connect network timeout time, uint: s */

/* reset key config */
#define WIFI_KEY_PIN                TY_GPIOA_9  /* reset button pin */
#define WIFI_KEY_TIMER_MS           100         /* key scan poll time, default 100ms */
#define WIFI_KEY_LONG_PRESS_MS      3000        /* long press time */
#define WIFI_KEY_SEQ_PRESS_MS       400
#define WIFI_KEY_LOW_LEVEL_ENABLE   TRUE

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @Function: wifi_key_process
 * @Description: button is pressed, call the function to process
 * @Input: port: button pin, type: button press type, cnt: press count
 * @Output: none
 * @Return: none
 * @Others: long press enter connect network mode, normal press toggle led 
 */
STATIC VOID_T wifi_key_process(TY_GPIO_PORT_E port, PUSH_KEY_TYPE_E type, INT_T cnt)
{
    PR_DEBUG("port:%d, type:%d, cnt:%d", port, type, cnt);
    OPERATE_RET op_ret = OPRT_OK;

    if (port = WIFI_KEY_PIN) {
        if (LONG_KEY == type) { /* long press enter connect network mode */
            op_ret = tuya_iot_wf_gw_unactive();
            if (op_ret != OPRT_OK) {
                PR_ERR("long press tuya_iot_wf_gw_unactive error, %d", op_ret);
                return;
            }
        } else if (NORMAL_KEY == type) {
#if 1
            if (get_light_status() == LIGHT_OFF) { 
                op_ret = set_light_status(LIGHT_ON); /* light turn on */
                if (op_ret != OPRT_OK) {
                    PR_ERR("dp process set light status error, %d", op_ret);
                    return;
                }
            } else {
                op_ret = set_light_status(LIGHT_OFF); /* light turn off */
                if (op_ret != OPRT_OK) {
                    PR_ERR("dp process set light status error, %d", op_ret);
                    return;
                }
            }
            /* update device current status to cloud */
            update_all_dp();
#endif
        } else {
            PR_NOTICE("key type is no deal");
        }
    }
}

/**
 * @Function: wifi_key_init
 * @Description: initiation reset key
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others:
 */
STATIC VOID_T wifi_key_init(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    KEY_USER_DEF_S key_def;

    op_ret = key_init(NULL, 0, WIFI_KEY_TIMER_MS);
    if (op_ret != OPRT_OK) {
        PR_ERR("key_init err:%d", op_ret);
        return;
    }

    /* config key parameter */
    memset(&key_def, 0, SIZEOF(key_def));
    key_def.port = WIFI_KEY_PIN;
    key_def.long_key_time = WIFI_KEY_LONG_PRESS_MS;
    key_def.low_level_detect = WIFI_KEY_LOW_LEVEL_ENABLE;
    key_def.lp_tp = LP_ONCE_TRIG;
    key_def.call_back = wifi_key_process;
    key_def.seq_key_detect_time = WIFI_KEY_SEQ_PRESS_MS;

    /* register key */
    op_ret = reg_proc_key(&key_def);
    if (op_ret != OPRT_OK) {
        PR_ERR("reg_proc_key err:%d", op_ret);
    }
}

/**
 * @Function:mf_user_pre_gpio_test_cb 
 * @Description: gpio test pre-callback function
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T mf_user_pre_gpio_test_cb(VOID_T) 
{

}

/**
 * @Function: hw_reset_flash_data
 * @Description: erase application data from flash
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
STATIC VOID_T hw_reset_flash_data(VOID_T)
{
    /* erase application data from flash */
}

/**
 * @Function:mf_user_enter_callback 
 * @Description: tell application has entered the production test
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T mf_user_enter_callback(VOID_T) 
{
    hw_reset_flash_data();
}



/**
 * @Function: mf_user_callback
 * @Description: authorization callback function
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others:
 */
VOID_T mf_user_callback(VOID_T) 
{

}

/**
 * @Function:mf_user_product_test_cb 
 * @Description: Finished Product test callbacks
 * @Input: 
 * @Output: none
 * @Return: none
 * @Others: none
 */
OPERATE_RET mf_user_product_test_cb(USHORT_T cmd,UCHAR_T *data, UINT_T len, OUT UCHAR_T **ret_data,OUT USHORT_T *ret_len) 
{
    return OPRT_OK;
}

/**
 * @Function: hw_report_all_dp_status
 * @Description: report all dp status
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T hw_report_all_dp_status(VOID_T)
{
    /* report all dp status */
    update_all_dp();
}

/**
 * @Function:pre_app_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T pre_app_init(VOID_T) 
{
    ty_subscribe_event(EVENT_SDK_EARLY_INIT_OK, "early_init", fast_boot, FALSE);
}

/**
 * @Function:pre_device_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T pre_device_init(VOID_T) 
{
    /* reset key init */
    wifi_key_init();

    PR_DEBUG("%s",tuya_iot_get_sdk_info()); /* print SDK information */
    PR_DEBUG("%s:%s", APP_BIN_NAME, DEV_SW_VERSION); /* print the firmware name and version */
    PR_NOTICE("firmware compiled at %s %s", __DATE__, __TIME__); /* print firmware compilation time */
    PR_NOTICE("system reset reason:[%s]",tuya_hal_system_get_rst_info()); /* print system reboot causes */

    SetLogManageAttr(TY_LOG_LEVEL_NOTICE); /* set the log level */
}

/**
 * @Function:app_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T app_init(VOID_T) 
{

}

/**
 * @Function: status_changed_cb
 * @Description: call this function after the network connection status changes
 * @Input: status: current status
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T status_changed_cb(IN CONST GW_STATUS_E status)
{
    PR_NOTICE("status_changed_cb is status:%d",status);
}

/**
 * @Function: get_file_data_cb
 * @Description: firmware download content process callback
 * @Input: fw: firmware info
 * @Input: total_len: firmware total size
 * @Input: offset: offset of this download package
 * @Input: data && len: this download package
 * @Input: pri_data: private data
 * @Output: remain_len: the size left to process in next cb
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
STATIC OPERATE_RET get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                     IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T 
pri_data)
{
    PR_DEBUG("Rev File Data");
    PR_DEBUG("Total_len:%d ", total_len);
    PR_DEBUG("Offset:%d Len:%d", offset, len);

    return OPRT_OK;
}

/**
 * @Function: upgrade_notify_cb
 * @Description: firmware download finish result callback
 * @Input: fw: firmware info
 * @Input: download_result: 0 means download succes. other means fail
 * @Input: pri_data: private data
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    PR_DEBUG("download  Finish");
    PR_DEBUG("download_result:%d", download_result);
}

/**
 * @Function: gw_ug_inform_cb
 * @Description: ota inform callback
 * @Input: fw: firmware info
 * @Output: none
 * @Return: int:
 * @Others: 
 */
STATIC INT_T gw_ug_inform_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev GW Upgrade Info");
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%d", fw->file_size);

    return tuya_iot_upgrade_gw(fw, get_file_data_cb, upgrade_notify_cb, NULL);
}

/**
 * @Function: gw_reset_cb
 * @Description: called after reset device or app remove device 
 * @Input: type:gateway reset type
 * @Output: none
 * @Return: none
 * @Others: reset factory clear flash data
 */
STATIC VOID_T gw_reset_cb(IN CONST GW_RESET_TYPE_E type)
{
    PR_DEBUG("gw_reset_cb type:%d",type);
    if(GW_REMOTE_RESET_FACTORY != type) {
        PR_DEBUG("type is GW_REMOTE_RESET_FACTORY");
        return;
    }

    hw_reset_flash_data();
}

/**
 * @Function: dev_obj_dp_cb
 * @Description: called after the cloud sends data of type bool, value, enum, string or fault
 * @Input: dp:obj dp info
 * @Output: none
 * @Return: none
 * @Others: app send data by dpid  control device stat
 */
STATIC VOID_T dev_obj_dp_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_DEBUG("dp->cid:%s dp->dps_cnt:%d",dp->cid,dp->dps_cnt);
    UCHAR_T i = 0;

    for(i = 0;i < dp->dps_cnt;i++) {
        deal_dp_proc(&(dp->dps[i]));
    }
}

/**
 * @Function: dev_raw_dp_cb
 * @Description: called after the cloud sends data of type raw
 * @Input: dp: raw dp info
 * @Output: none
 * @Return: none
 * @Others: none
 */
STATIC VOID_T dev_raw_dp_cb(IN CONST TY_RECV_RAW_DP_S *dp)
{
    PR_DEBUG("raw data dpid:%d",dp->dpid);
    PR_DEBUG("recv len:%d",dp->len);
#if 1 
    INT_T i = 0;
    for(i = 0;i < dp->len;i++) {
        PR_DEBUG_RAW("%02X ",dp->data[i]);
    }
#endif
    PR_DEBUG_RAW("\n");
    PR_DEBUG("end");
}

/**
 * @Function: dev_dp_query_cb
 * @Description: click app panel report all data point status
 * @Input: dp_qry: query info
 * @Output: none
 * @Return: none
 * @Others: none
 */
STATIC VOID_T dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry) 
{
    PR_NOTICE("Recv DP Query Cmd");

    hw_report_all_dp_status();
}

/**
 * @Function: wf_nw_status_cb
 * @Description: This function is called when the state of the device connection has changed
 * @Input: stat: curr network status
 * @Output: none
 * @Return: none
 * @Others: none
 */
STATIC VOID_T wf_nw_status_cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    /* print current Wi-Fi status */
    PR_NOTICE("wf_nw_status_cb, wifi_status:%d", stat);

    /* report all DP status when connected to the cloud */
    if (stat == STAT_CLOUD_CONN || stat == STAT_AP_CLOUD_CONN) {
        hw_report_all_dp_status();
    }
}

/**
 * @Function: device_init
 * @Description:
 * @Input: none
 * @Output: none
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
OPERATE_RET device_init(VOID_T) 
{
    OPERATE_RET op_ret = OPRT_OK;

    TY_IOT_CBS_S wf_cbs = {
        status_changed_cb, \
        gw_ug_inform_cb, \
        gw_reset_cb, \
        dev_obj_dp_cb, \
        dev_raw_dp_cb, \
        dev_dp_query_cb, \
        NULL,
    };

    /* tuya IoT framework initialization */
    op_ret = tuya_iot_wf_soc_dev_init_param(WIFI_WORK_MODE_SEL, WF_START_SMART_FIRST, &wf_cbs, NULL, PRODECT_ID, DEV_SW_VERSION);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_wf_soc_dev_init_param error,err_num:%d",op_ret);
        return op_ret;
    }

    /* register Wi-Fi connection status change callback function */
    op_ret = tuya_iot_reg_get_wf_nw_stat_cb(wf_nw_status_cb);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_wf_nw_stat_cb is error,err_num:%d",op_ret);
        return op_ret;
    }

    return op_ret;
}

