/**
 ****************************************************************************************
 *
 * @file app_dis_task.c
 *
 * @brief Device Information Service Application Task
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

#include "rwip_config.h"        // SW Configuration
#include <string.h>             // srtlen()

#if (BLE_WPTS)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "wpts_task.h"          // Device Information Service Server Task API
#include "wpts.h"               // Device Information Service Definitions
#include "app_wpts.h"            // Device Information Service Application Definitions
#include "app_wpts_task.h"       // Device Information Service Application Task API
#include "app_task.h"           // Application Task API



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles DIS Server profile database creation confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int wpts_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct wpts_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8 len;
    
    if (ke_state_get(dest_id) == APP_DB_INIT)
    {
          if (param->status == CO_ERROR_NO_ERROR)
        {
            // Go to the idle state

            {
                // Set Manufacturer Name value in the DB
                struct wpts_set_char_val_req *req_name = KE_MSG_ALLOC_DYN(WPTS_SET_CHAR_VAL_REQ,
                                                                          TASK_DISS, TASK_APP,
                                                                          wpts_set_char_val_req,
                                                                          18);

                // Fill in the parameter structure
                req_name->char_code     = WPTS_CHARGING_PRU_CTRL_IDX_CHAR;
                req_name->val_len       = 18;
                memcpy(&req_name->val[0], 0, 1);

                // Send the message
                ke_msg_send(req_name);
            }
					}
        // Inform the Application Manager
        struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP,
                                                           app_module_init_cmp_evt);

        cfm->status = param->status;

        ke_msg_send(cfm);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles disable indication from the DIS Server profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int wpts_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct wpts_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}




/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

#endif //(BLE_DIS_SERVER)

/// @} APP
