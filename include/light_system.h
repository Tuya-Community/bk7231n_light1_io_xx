#ifndef __LIGHT_SYSTEM_H__
#define __LIGHT_SYSTEM_H__

/* Includes ------------------------------------------------------------------*/
#include "tuya_cloud_types.h"
#include "tuya_gpio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LIGHT_PIN   TY_GPIOA_16

typedef UCHAR_T LED_STATUS_E;
#define LIGHT_OFF   0
#define LIGHT_ON    1

/* Exported functions ------------------------------------------------------- */
VOID_T fast_boot(VOID_T);

OPERATE_RET light_init(VOID_T);

OPERATE_RET set_light_status(LED_STATUS_E status);

LED_STATUS_E get_light_status(VOID_T);

#endif /* __LIGHT_SYSTEM_H__ */

