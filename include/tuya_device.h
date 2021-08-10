#ifndef __TUYA_DEVICE_H__
#define __TUYA_DEVICE_H__

/* Includes ------------------------------------------------------------------*/
#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */
    
#ifdef _TUYA_DEVICE_GLOBAL
    #define _TUYA_DEVICE_EXT 
#else
    #define _TUYA_DEVICE_EXT extern
#endif /* _TUYA_DEVICE_GLOBAL */ 

// device information define
#define DEV_SW_VERSION USER_SW_VER
#define PRODECT_ID "fnrwpglflmbhjvvh"

/* Exported functions ------------------------------------------------------- */

_TUYA_DEVICE_EXT \
OPERATE_RET device_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TUYA_DEVICE_H__ */

