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
 * @Description: 将需要快速启动功能放到该函数中
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
 * @Description: 灯的gpio口初始化
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
OPERATE_RET light_init(VOID_T)
{
     OPERATE_RET op_ret = OPRT_OK;

    /* 设置pin口为上拉输出模式 */
    op_ret = tuya_gpio_inout_set(LIGHT_PIN, FALSE);
    if (op_ret != OPRT_OK) {
        PR_ERR("light init gpio set inout error!");
        return op_ret;
    }
    
    op_ret = tuya_gpio_mode_set(LIGHT_PIN, TY_GPIO_PULLUP);
    if (op_ret != OPRT_OK) {
        PR_ERR("light init gpio mode set error!");
        return op_ret;
    }
    
    /* 打开灯 */
    op_ret = light_on();
    if (op_ret != OPRT_OK) {
        PR_ERR("light init light on error!");
        return op_ret;
    }


    return op_ret;
    
}

/**
 * @Function: light_on
 * @Description: 开灯
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
OPERATE_RET light_on(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* pin口输出低电平，打开灯 */
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
 * @Description: 关灯
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
OPERATE_RET light_off(VOID_T)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* pin口输出高电平，关闭灯 */
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
 * @Description: 设置灯状态
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
 * @Function: get_cur_light_status
 * @Description: 获取灯状态
 * @Input: status: LIGHT_ON or LIGHT_OFF
 * @Output: none
 * @Return: LIGHT_ON: 开灯状态
 *          LIGHT_OFF: 关灯状态
 * @Others: none
 */
LED_STATUS_E get_cur_light_status(VOID_T)
{
    return cur_light_status;
}


