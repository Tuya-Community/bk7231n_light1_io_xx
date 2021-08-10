#ifndef __DP_PROCESS_H__
#define __DP_PROCESS_H__

/* Includes ------------------------------------------------------------------*/
#include "tuya_cloud_types.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DPID_LIGHT_SWITCH   20

/* Exported functions ------------------------------------------------------- */
VOID_T update_all_dp(VOID_T);
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

#endif /* __DP_PROCESS_H__ */

