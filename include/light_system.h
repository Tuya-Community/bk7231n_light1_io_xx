/**
* @file light_system.h
* @author www.tuya.com
* @brief 
* @version 0.1
* @date 2021-08-19
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __LIGHT_SYSTEM_H__
#define __LIGHT_SYSTEM_H__

#include "tuya_cloud_types.h"
#include "tuya_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define LIGHT_PIN   TY_GPIOA_16

/***********************************************************
***********************typedef define***********************
***********************************************************/
typedef UCHAR_T LED_STATUS_E;
#define LIGHT_OFF   0
#define LIGHT_ON    1

/***********************************************************
***********************variable define**********************
***********************************************************/

/***********************************************************
***********************function define**********************
***********************************************************/

/**
* @brief need quick start tasks

* @return none
*/
VOID_T fast_boot(VOID_T);

/**
* @brief light gpio init
*
* @return none
*/
OPERATE_RET light_init(VOID_T);

/**
* @brief set light status
*
* @param[in] status: LIGHT_ON or LIGHT_OFF
* @return none
*/
OPERATE_RET set_light_status(LED_STATUS_E status);

/**
* @brief get light status
*
* @return light status: LIGHT_ON or LIGHT_OFF
*/
LED_STATUS_E get_light_status(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LIGHT_SYSTEM_H__ */
