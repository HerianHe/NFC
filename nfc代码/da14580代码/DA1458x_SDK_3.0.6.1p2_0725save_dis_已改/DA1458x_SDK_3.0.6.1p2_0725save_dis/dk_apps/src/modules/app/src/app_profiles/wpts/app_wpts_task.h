/**
 ****************************************************************************************
 *
 * @file app_dis_task.h
 *
 * @brief Header file - APPDISTASK.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_WPTS_TASK_H_
#define APP_WPTS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APPDISTASK Task
 * @ingroup APPDIS
 * @brief Device Information Service Application Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_WPTS)

#include "wpts_task.h"
#include "ke_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal number of APP DIS Task instances
#define APP_WPTS_IDX_MAX        (1)

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles DIS Server profile database creation confirmation.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int wpts_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct wpts_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles disable indication from the DIS Server profile.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int wpts_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct wpts_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);                                      


#endif //(BLE_DIS_SERVER)

/// @} APPDISTASK

#endif //APP_DIS_TASK_H_
