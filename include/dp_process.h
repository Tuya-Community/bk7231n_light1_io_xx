/**
* @file dp_process.h
* @author www.tuya.com
* @brief 
* @version 0.1
* @date 2021-08-19
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __DP_PROCESS_H__
#define __DP_PROCESS_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define DPID_LIGHT_SWITCH   20

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
VOID_T update_all_dp(VOID_T);

/**
* @brief handle dp commands from the cloud
*
* @param[in] root: pointer header for dp data
* @return none
*/
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DP_PROCESS_H__ */

