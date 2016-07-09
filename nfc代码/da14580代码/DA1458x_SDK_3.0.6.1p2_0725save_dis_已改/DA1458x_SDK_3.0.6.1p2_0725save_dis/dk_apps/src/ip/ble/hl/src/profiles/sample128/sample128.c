/**
 ****************************************************************************************
 *
 * @file sample128.c
 *
 * @brief 128 UUID service. Sample code
 *
 * Copyright (C) 2013 Dialog Semiconductor GmbH and its Affiliates, unpublished work
 * This computer program includes Confidential, Proprietary Information and is a Trade Secret 
 * of Dialog Semiconductor GmbH and its Affiliates. All use, disclosure, and/or 
 * reproduction is prohibited unless authorized in writing. All Rights Reserved.
 *
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_SAMPLE128)

#include "sample128.h"
#include "sample128_task.h"
#include "attm_db.h"
#include "gapc.h"
/*
 *  SAMPLE128 PROFILE ATTRIBUTES VALUES DEFINTION
 ****************************************************************************************
 */

/// sample128_1 Service
const struct att_uuid_128 sample128_svc     = {{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}};

 
/// sample128_1 value attribute UUID
const struct att_uuid_128 sample128_1_val     = {{0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F}};

struct att_char128_desc sample128_1_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                                                    {0,0},
                                                                    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F}}; 

const struct att_uuid_128 sample128_2_val     = {{0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F}};

struct att_char128_desc sample128_2_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                                                    {0,0},
                                                                    {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F}}; 

const struct att_uuid_128 sample128_3_val = {{0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,0x3E, 0x3F}};

struct att_char128_desc sample128_3_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 ,0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F}};

const struct att_uuid_128 sample128_4_val = {{0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,0x4E, 0x4F}};


struct att_char128_desc sample128_4_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_NTF,
                                           {0,0},
                                           {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,0x4E, 0x4F}};
const struct att_uuid_128 sample128_5_val = {{0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F}};


struct att_char128_desc sample128_5_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F}};

const struct att_uuid_128 sample128_6_val = {{0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F}};


struct att_char128_desc sample128_6_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F}};

const struct att_uuid_128 sample128_7_val = {{0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F}};


struct att_char128_desc sample128_7_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F}};

const struct att_uuid_128 sample128_8_val = {{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F}};


struct att_char128_desc sample128_8_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F}};

const struct att_uuid_128 sample128_9_val = {{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}};


struct att_char128_desc sample128_9_char = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                           {0,0},
                                           {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}};
																					 
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct sample128_env_tag sample128_env __attribute__((section("retention_mem_area0"),zero_init));

static const struct ke_task_desc TASK_DESC_SAMPLE128 = {sample128_state_handler, &sample128_default_handler, sample128_state, SAMPLE128_STATE_MAX, SAMPLE128_IDX_MAX};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void sample128_init(void)
{
    // Reset Environment
    memset(&sample128_env, 0, sizeof(sample128_env));
    
    // Create SAMPLE128 task
    ke_task_create(TASK_SAMPLE128, &TASK_DESC_SAMPLE128);

    ke_state_set(TASK_SAMPLE128, SAMPLE128_DISABLED);
}

void sample128_send_val(uint8_t val)
{
    // Allocate the alert value change indication
   struct sample128_val_ind *ind = KE_MSG_ALLOC(SAMPLE128_VAL_IND,
                                              sample128_env.con_info.appid, TASK_SAMPLE128,
                                              sample128_val_ind);
   // Fill in the parameter structure
   ind->conhdl = gapc_get_conhdl(sample128_env.con_info.conidx);
	 ind->val = val;
   
   // Send the message
   ke_msg_send(ind);
}

void sample128_disable(void)
{
    att_size_t length;
    uint8_t *alert_lvl;

    // Disable service in database
    attmdb_svc_set_permission(sample128_env.sample128_shdl, PERM_RIGHT_DISABLE);

    struct sample128_disable_ind *ind = KE_MSG_ALLOC(SAMPLE128_DISABLE_IND,
                                                 sample128_env.con_info.appid, TASK_SAMPLE128,
                                                 sample128_disable_ind);

    //Get value stored in DB
    attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL,
                         &length, &alert_lvl);

    // Fill in the parameter structure
    ind->conhdl     = gapc_get_conhdl(sample128_env.con_info.conidx);

    // Send the message
    ke_msg_send(ind);

    // Go to idle state
    ke_state_set(TASK_SAMPLE128, SAMPLE128_IDLE);
}


void sample128_upd_char4_cfm_send(uint8_t status)
{
    struct sample128_upd_char4_cfm *cfm = KE_MSG_ALLOC(SAMPLE128_UPD_CHAR4_CFM,
                                                 sample128_env.con_info.appid, TASK_SAMPLE128,
                                                 sample128_upd_char4_cfm);

    cfm->status = status;
    
    // Send the message
    ke_msg_send(cfm);

}
#endif //BLE_SAMPLE128
