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
/* wifi 相关配置 */
#define WIFI_WORK_MODE_SEL          GWCM_OLD_PROD   //wifi 工作模式选择
#define WIFI_CONNECT_OVERTIME_S     180             //wifi 配网超时时间，单位：s

/* 配网按键相关宏,长按进入配网模式 */
#define WIFI_KEY_PIN                TY_GPIOA_9 //按键引脚 
#define WIFI_KEY_TIMER_MS           100          //轮询扫描按键所需的时间，一般默认为 100ms
#define WIFI_KEY_LONG_PRESS_MS      3000        //按键长按时间设置
#define WIFI_KEY_SEQ_PRESS_MS       400         //按键连按时间设置
#define WIFI_KEY_LOW_LEVEL_ENABLE   TRUE        //TRUE：按键按下为低，FALSE：按键按下为高

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @Function: wifi_key_process
 * @Description: 按键被按下后，调用该函数进行处理
 * @Input: port：触发引脚, type：按键触发类型, cnt:按键触发次数
 * @Output: none
 * @Return: none
 * @Others: 长按触发配网模式，短按开关led
 */
STATIC VOID_T wifi_key_process(TY_GPIO_PORT_E port,PUSH_KEY_TYPE_E type,INT_T cnt)
{
    PR_DEBUG("port:%d,type:%d,cnt:%d", port, type, cnt);
    OPERATE_RET op_ret = OPRT_OK;

    if (port = WIFI_KEY_PIN) {
        if (LONG_KEY == type) { //press long enter linking network
            /* 手动移除设备 */
            tuya_iot_wf_gw_unactive();
        } else if (NORMAL_KEY == type) {
#if 1
            if (get_cur_light_status() == LIGHT_OFF) { /* 如果当前 light 是打开的，关闭它 */
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
            /* 设备状态改变后，应将设备状态同步到云端 */
            update_all_dp();
#endif
        } else {
            PR_NOTICE("key type is no deal");
        }
    }

    return;
}

/**
 * @Function: wifi_key_init
 * @Description: 初始化 WiFi 按键
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 该函数需在tuya iot 初始化完成后调用（不能在 pre_app_init() 里面调用，可在其他三个中调用）。
 */
STATIC VOID_T wifi_key_init(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* 按键相关初始化 */
    KEY_USER_DEF_S key_def;

    op_ret = key_init(NULL, 0, WIFI_KEY_TIMER_MS);
    if (op_ret != OPRT_OK) {
        PR_ERR("key_init err:%d", op_ret);
        return;
    }

    /* 初始化 key 相关参数 */
    memset(&key_def, 0, SIZEOF(key_def));
    key_def.port = WIFI_KEY_PIN;                            //按键引脚
    key_def.long_key_time = WIFI_KEY_LONG_PRESS_MS;         //长按时间配置
    key_def.low_level_detect = WIFI_KEY_LOW_LEVEL_ENABLE;   //TRUE:低电平算按下，FALSE：高电平算按下
    key_def.lp_tp = LP_ONCE_TRIG;   //
    key_def.call_back = wifi_key_process;                   //按键按下后回调函数
    key_def.seq_key_detect_time = WIFI_KEY_SEQ_PRESS_MS;    //连按间隔时间配置

    /* 注册按键 */
    op_ret = reg_proc_key(&key_def);
    if (op_ret != OPRT_OK) {
        PR_ERR("reg_proc_key err:%d", op_ret);
    }

    return;
}

/**
 * @Function:mf_user_pre_gpio_test_cb 
 * @Description: gpio测试前置回调函数
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T mf_user_pre_gpio_test_cb(VOID_T) 
{
    return;
}

/**
 * @Function: hw_reset_flash_data
 * @Description: 擦除 flash 中应用数据
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
STATIC VOID_T hw_reset_flash_data(VOID_T)
{
    /* 将存到 flash 中的应用数据全部擦除掉 */
    return;
}

/**
 * @Function:mf_user_enter_callback 
 * @Description: 通知应用已经进入到产测，在回调中对应用数据清除。
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T mf_user_enter_callback(VOID_T) 
{
    hw_reset_flash_data();
    return;
}



/**
 * @Function: mf_user_callback
 * @Description: 授权回调函数
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 清空flash中存储的数据
 */
VOID_T mf_user_callback(VOID_T) 
{
    return;
}

/**
 * @Function:mf_user_product_test_cb 
 * @Description: 成品测试回调
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
 * @Description: 上报所有 dp 点
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
    /* WiFi 按键  初始化 */
    wifi_key_init();

    PR_DEBUG("%s",tuya_iot_get_sdk_info()); /* 打印 SDK 相关信息 */
    PR_DEBUG("%s:%s", APP_BIN_NAME, DEV_SW_VERSION); /* 打印固件版本名称和版本号 */
    PR_NOTICE("firmware compiled at %s %s", __DATE__, __TIME__); /* 打印固件编译时间 */
    PR_NOTICE("system reset reason:[%s]",tuya_hal_system_get_rst_info()); /* 打印系统重启原因 */

    SetLogManageAttr(TY_LOG_LEVEL_NOTICE); /* 设置日志打印等级 */
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
    return;
}

/**
 * @Function: status_changed_cb
 * @Description: wifi 连网状态改变后调用该函数
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
 * @Description: ota升级通知回调
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
 * @Description: 设备重置或 app 上移除设备后调用该函数
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
 * @Description: 云端下发obj(bool, value, enum, string和fault)类型的数据后调用该函数
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
 * @Description: 云端下发raw类型数据后调用该函数
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
    return;
}

/**
 * @Function: dev_dp_query_cb
 * @Description: app进入面板后触发查询dp点状态
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
 * @Description: 设备连网状态发生改变后，调用该函数进行处理
 * @Input: stat: curr network status
 * @Output: none
 * @Return: none
 * @Others: none
 */
STATIC VOID_T wf_nw_status_cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    /* 打印当前WiFi状态 */
    PR_NOTICE("wf_nw_status_cb, wifi_status:%d", stat);

    /* 成功连接到云平台后上报所有DP状态 */
    if (stat == STAT_CLOUD_CONN || stat == STAT_AP_CLOUD_CONN) {
        hw_report_all_dp_status();
    }
}

/**
 * @Function: device_init
 * @Description: tuya iot 所有工作都已完成
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

    /* 涂鸦IoT框架初始化 */
    op_ret = tuya_iot_wf_soc_dev_init_param(WIFI_WORK_MODE_SEL, WF_START_SMART_FIRST, &wf_cbs, NULL, PRODECT_ID, DEV_SW_VERSION);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_wf_soc_dev_init_param error,err_num:%d",op_ret);
        return op_ret;
    }

    /* 注册Wi-Fi模组的状态回调函数，当模组连网状态发生改变时，会调用 wf_nw_status_cb() 函数 */
    op_ret = tuya_iot_reg_get_wf_nw_stat_cb(wf_nw_status_cb);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_wf_nw_stat_cb is error,err_num:%d",op_ret);
        return op_ret;
    }

    return op_ret;
}

