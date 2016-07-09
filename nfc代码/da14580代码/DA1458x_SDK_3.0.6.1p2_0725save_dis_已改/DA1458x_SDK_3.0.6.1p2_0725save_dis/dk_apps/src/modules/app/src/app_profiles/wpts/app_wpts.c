/**
 ****************************************************************************************
 *
 * @file app_dis.c
 *
 * @brief Device Information Service Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_WPTS)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_wpts.h"                 // Device Information Service Application Definitions
#include "app_wpts_task.h"            // Device Information Service Application Task API
#include "app.h"                     // Application Definitions
#include "app_task.h"                // Application Task Definitions
#include "wpts_task.h"               // Health Thermometer Functions


/*
 * LOCAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_wpts_init(void)
{
    return;
}

void app_wpts_create_db_send(void)
{
    // Add DIS in the database
    struct wpts_create_db_req *req = KE_MSG_ALLOC(WPTS_CREATE_DB_REQ,
                                                  TASK_WPTS, TASK_APP,
                                                  wpts_create_db_req);

    req->features = APP_WPTS_FEATURES;

    // Send the message
    ke_msg_send(req);
}

void app_wpts_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct wpts_enable_req *req = KE_MSG_ALLOC(WPTS_ENABLE_REQ,
                                               TASK_WPTS, TASK_APP,
                                               wpts_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->sec_lvl            = PERM(SVC, ENABLE);
    req->wpts_1_val        = 1;

    // Send the message
    ke_msg_send(req);

}

#endif //BLE_WPTS_SERVER

/// @} APP
