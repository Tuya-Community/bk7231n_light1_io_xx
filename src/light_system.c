/**
* @file light_system.c
* @author www.tuya.com
* @brief 
* @version 0.1
* @date 2021-08-19
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#include "light_system.h"
#include "uni_log.h"

/***********************************************************
*************************micro define***********************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/
STATIC LED_STATUS_E cur_light_status = LIGHT_ON;


/***********************************************************
***********************function define**********************
***********************************************************/


/**
* @brief need quick start tasks

* @return none
*/
VOID_T fast_boot(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = light_init();
    if (op_ret != OPRT_OK) {
        PR_ERR("fast boot light init error, %d", op_ret);
        return;
    }
    return;
}

/**
* @brief light gpio init
*
* @return none
*/
OPERATE_RET light_init(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* light pin set output */
    op_ret = tuya_gpio_inout_set(LIGHT_PIN, FALSE);
    if (op_ret != OPRT_OK) {
        PR_ERR("light init gpio set inout error!");
        return op_ret;
    }

    /* light pin mode set pullup */
    op_ret = tuya_gpio_mode_set(LIGHT_PIN, TY_GPIO_PULLUP);
    if (op_ret != OPRT_OK) {
        PR_ERR("light init gpio mode set error!");
        return op_ret;
    }
    
    /* light on */
    op_ret = set_light_status(LIGHT_ON);
    if (op_ret != OPRT_OK) {
        PR_ERR("light init light on error!");
        return op_ret;
    }
    return op_ret;
}

/**
* @brief light on
*
* @return none
*/
STATIC OPERATE_RET light_on(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* pin set low level, light on */
    op_ret = tuya_gpio_write(LIGHT_PIN, FALSE);
    if (op_ret != OPRT_OK) {
        PR_ERR("light on error!");
        return op_ret;
    }

    cur_light_status = LIGHT_ON;

    return op_ret;
}

/**
* @brief light off
*
* @return none
*/
STATIC OPERATE_RET light_off(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* pin set high level, light off */
    op_ret = tuya_gpio_write(LIGHT_PIN, TRUE);
    if (op_ret != OPRT_OK) {
        PR_ERR("light off error!");
        return op_ret;
    }

    cur_light_status = LIGHT_OFF;

    return op_ret;
}

/**
* @brief set light status
*
* @param[in] status: LIGHT_ON or LIGHT_OFF
* @return none
*/
OPERATE_RET set_light_status(LED_STATUS_E status)
{
    OPERATE_RET op_ret = OPRT_OK;

    if (status == LIGHT_ON) {
        op_ret = light_on();
        if (op_ret != OPRT_OK) {
            return op_ret;
        }
    } else {
        light_off();
        if (op_ret != OPRT_OK) {
            return op_ret;
        }
    }

    return op_ret;
}

/**
* @brief get light status
*
* @return light status: LIGHT_ON or LIGHT_OFF
*/
LED_STATUS_E get_light_status(VOID_T)
{
    return cur_light_status;
}
