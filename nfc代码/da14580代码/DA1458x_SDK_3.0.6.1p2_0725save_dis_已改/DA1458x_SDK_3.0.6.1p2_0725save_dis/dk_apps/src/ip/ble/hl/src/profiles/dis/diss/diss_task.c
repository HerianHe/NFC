/**
 ****************************************************************************************
 *
 * @file diss_task.c
 *
 * @brief Device Information Service Task implementation.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup DISSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_DIS_SERVER)
#include "gap.h"
#include "gattc_task.h"
#include "diss.h"
#include "diss_task.h"
#include "prf_utils.h"
#include "attm_util.h"
#include "atts_util.h"
#include "attm_cfg.h"
#include "prf_utils.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int diss_create_db_req_handler(ke_msg_id_t const msgid,
                                      struct diss_create_db_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    //Service content flag
    uint32_t cfg_flag;
    //DB Creation Statis
    uint8_t status = ATT_ERR_NO_ERROR;

    //Save profile id
    diss_env.con_info.prf_id = TASK_DISS;

    //Compute Attribute Table and save it in environment
    cfg_flag = diss_compute_cfg_flag(param->features);

    status = attm_svc_create_db(&diss_env.shdl, (uint8_t *)&cfg_flag, DIS_IDX_NB, &diss_env.att_tbl[0],
                                dest_id, &diss_att_db[0]);

    if (status == ATT_ERR_NO_ERROR)
    {
        //Disable service
        status = attmdb_svc_set_permission(diss_env.shdl, PERM(SVC, DISABLE));

        //If we are here, database has been fulfilled with success, go to idle test
        ke_state_set(TASK_DISS, DISS_IDLE);
    }

    //Send response to application
    struct diss_create_db_cfm * cfm = KE_MSG_ALLOC(DISS_CREATE_DB_CFM, src_id, TASK_DISS,
                                                   diss_create_db_cfm);
    cfm->status = status;
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref DISS_SET_CHAR_VAL_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int diss_set_char_val_req_handler(ke_msg_id_t const msgid,
                                         struct diss_set_char_val_req const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
    // Request status
    uint8_t status;
    // Characteristic Declaration attribute handle
    uint16_t handle;

    //Save the application task id
    diss_env.con_info.appid = src_id;

    // Check Characteristic Code
    if (param->char_code <= DIS_PNP_ID_CHAR)
    {
        // Get Characteristic Declaration attribute handle
        handle = diss_env.shdl + diss_env.att_tbl[param->char_code];

        // Check if the Characteristic exists in the database
        if (handle != PRF_ERR_INEXISTENT_HDL)
        {
            // Check the value length
            status = diss_check_val_len(param->char_code, param->val_len);

            if (status == PRF_ERR_OK)
            {
                // Set value in the database
                attmdb_att_set_value(handle + 1, param->val_len, (uint8_t *)&param->val[0]);
            }
        }
        else
        {
            status = PRF_ERR_INEXISTENT_HDL;
        }
    }
    else
    {
        status = PRF_ERR_INVALID_PARAM;
    }

    if (status != PRF_ERR_OK)
    {
        // Status is PRF_ERR_INVALID_PARAM or PRF_ERR_UNEXPECTED_LEN or PRF_ERR_INEXISTENT_HDL
        prf_server_error_ind_send((prf_env_struct *)&diss_env, status,
                                  DISS_ERROR_IND, DISS_SET_CHAR_VAL_REQ);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref DISS_ENABLE_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int diss_enable_req_handler(ke_msg_id_t const msgid,
                                   struct diss_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    //Save the connection handle associated to the profile
    diss_env.con_info.conidx = gapc_get_conidx(param->conhdl);
    //Save the application id
    diss_env.con_info.appid = src_id;

    // Check if the provided connection exist
    if (diss_env.con_info.conidx == GAP_INVALID_CONIDX)
    {
        // The connection doesn't exist, request disallowed
        prf_server_error_ind_send((prf_env_struct *)&diss_env, PRF_ERR_REQ_DISALLOWED,
                                  DISS_ERROR_IND, DISS_ENABLE_REQ);
    }
    else
    {
        //Enable Attributes + Set Security Level
        attmdb_svc_set_permission(diss_env.shdl, param->sec_lvl);

        // Go to connected state
        ke_state_set(TASK_DISS, DISS_CONNECTED);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Disconnection indication to HTPT.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_disconnect_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    if (KE_IDX_GET(src_id) == diss_env.con_info.conidx)
    {
        diss_disable(param->conhdl);
    }

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATT_WRITE_CMD_IND message.
 * The handler checks if the stream needs to be turned on.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_write_cmd_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_cmd_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Update the attribute value
	  uint8_t value_buf[20]={0};
    switch (DIS_IDX(param->handle))
    {
      /* case STREAMDATAD_IDX_ENABLE_VAL:
				  attmdb_att_update_value(param->handle, param->length, param->offset,
                                     (uint8_t*)&(param->value[0]));
				  atts_write_rsp_send(streamdatad_env.conhdl, param->handle, PRF_ERR_OK);
				  memcpy(value_buf,&(param->value),param->length);
				  attmdb_att_set_value(STREAMDATAD_DIR_VAL_HANDLE(1), param->length, value_buf);
				  prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_DIR_VAL_HANDLE(0));
			
			break;*/
      
        case DIS_IDX_MANUFACTURER_NAME_VAL:
        //   __nop(); 
				 attmdb_att_update_value(param->handle, param->length, param->offset,
                                     (uint8_t*)&(param->value[0]));
				  atts_write_rsp_send(diss_env.conhdl, param->handle, PRF_ERR_OK);
				  memcpy(value_buf,&(param->value),param->length);
				  attmdb_att_set_value( ATT_CHAR_MANUF_NAME, param->length, value_buf);
				  prf_server_send_event((prf_env_struct *)&(diss_env.con_info), false, ATT_CHAR_MANUF_NAME);
				
        break;
				

    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATT_READ_CMD message.
 * The handler checks if the stream needs to be turned on.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_read_cmd_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_read_cmd_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Update the attribute value
	  uint8_t value_buf[20]={0};
    switch (DIS_IDX(param->handle))
    {
      /* case STREAMDATAD_IDX_ENABLE_VAL:
				  attmdb_att_update_value(param->handle, param->length, param->offset,
                                     (uint8_t*)&(param->value[0]));
				  atts_write_rsp_send(streamdatad_env.conhdl, param->handle, PRF_ERR_OK);
				  memcpy(value_buf,&(param->value),param->length);
				  attmdb_att_set_value(STREAMDATAD_DIR_VAL_HANDLE(1), param->length, value_buf);
				  prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_DIR_VAL_HANDLE(0));
			
			break;*/
      
        case DIS_IDX_MANUFACTURER_NAME_VAL:
        //   __nop(); 
				// attmdb_att_update_value(param->handle, param->length, param->offset,
        //                             (uint8_t*)&(param->value[0]));
				//  atts_write_rsp_send(diss_env.conhdl, param->handle, PRF_ERR_OK);
				//  memcpy(value_buf,&(param->value),param->length);
				//  attmdb_att_set_value( ATT_CHAR_MANUF_NAME, param->length, value_buf);
				  prf_server_send_event((prf_env_struct *)&(diss_env.con_info), false, ATT_CHAR_MANUF_NAME);
				
        break;
				

    }

    return (KE_MSG_CONSUMED);
}
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

///Disabled State handler definition.
const struct ke_msg_handler diss_disabled[] =
{
    {DISS_CREATE_DB_REQ,         (ke_msg_func_t)diss_create_db_req_handler},
};

///Idle State handler definition.
const struct ke_msg_handler diss_idle[] =
{
    {DISS_SET_CHAR_VAL_REQ,      (ke_msg_func_t)diss_set_char_val_req_handler},
    {DISS_ENABLE_REQ,            (ke_msg_func_t)diss_enable_req_handler},
};

/// Default State handlers definition
const struct ke_msg_handler diss_default_state[] =
{
    {GAPC_DISCONNECT_IND,        (ke_msg_func_t) gapc_disconnect_ind_handler},
};
/// Connected State handlers definition
const struct ke_msg_handler diss_connected[] =
{
    {GATTC_WRITE_CMD_IND, (ke_msg_func_t)gattc_write_cmd_ind_handler},
		{GATTC_READ_CMD_IND, (ke_msg_func_t)gattc_read_cmd_ind_handler},
//    {GATTC_CMP_EVT,       (ke_msg_func_t)gattc_cmp_evt_handler},
}; 

///Specifies the message handler structure for every input state.
const struct ke_state_handler diss_state_handler[DISS_STATE_MAX] =
{
    [DISS_DISABLED]    = KE_STATE_HANDLER(diss_disabled),
    [DISS_IDLE]        = KE_STATE_HANDLER(diss_idle),
    [DISS_CONNECTED]   = KE_STATE_HANDLER(diss_connected),
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler diss_default_handler = KE_STATE_HANDLER(diss_default_state);

///Defines the place holder for the states of all the task instances.
ke_state_t diss_state[DISS_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

#endif //BLE_DIS_SERVER

/// @} DISSTASK
