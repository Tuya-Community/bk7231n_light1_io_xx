/* Private includes ----------------------------------------------------------*/
#include "light_system.h"
#include "tuya_gpio.h"
#include "uni_log.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STATIC LED_STATUS_E cur_light_status = LIGHT_ON;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @Function: fast_boot
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID_T fast_boot(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = light_init();
    if (op_ret != OPRT_OK) {
        PR_ERR("fast boot light init error, %d", op_ret);
        return;
    }
}


/**
 * @Function: light_init
 * @Description: light gpio init
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
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
 * @Function: light_on
 * @Description:
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
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
 * @Function: light_off
 * @Description:
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
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
 * @Function: set_light_status
 * @Description:
 * @Input: status: LIGHT_ON or LIGHT_OFF
 * @Output: none
 * @Return: none
 * @Others: none
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
 * @Function: get_light_status
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: LIGHT_ON
 *          LIGHT_OFF
 * @Others: none
 */
LED_STATUS_E get_light_status(VOID_T)
{
    return cur_light_status;
}


