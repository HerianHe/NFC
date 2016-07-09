/**
 ****************************************************************************************
 *
 * @file sample128_task.c
 *
 * @brief Sample128 Task implementation.
 *
 * @brief 128 UUID service. sample code
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
#include "app_custom_proj.h"
#include "gap.h"
#include "gapc.h"
#include "gattc_task.h"
#include "atts_util.h"
#include "sample128.h"
#include "sample128_task.h"
#include "attm_cfg.h"
#include "attm_db.h"
#include "prf_utils.h"
#include "app.h"
#include "storage_operate.h"
extern uint8_t sample128_placeholder2[2];
int id=0;
uint8_t data_count_w=0;
uint8_t data_count_r=0;
uint8_t data_count_rcard=0;
uint8_t send_amount=3;/*一张名片的包数*/
uint8_t card_amount=5;/*一次发送的名片个数*/
uint8_t send_char1=0x00;
my_new_2B sample128_my_new2B1 = {0x06,0x06};
my_new_20B zeros_20B={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	
my_new_20B send_char8={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	
my_new_20B new_send_char8={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/*
***********Identify Password****************
*/
int identify(uint8_t* a, uint8_t* b)
{
	int i=7,j=0;
	while (i>=0)
	{
	  if (((uint8_t*)a)[i]!=b[i])
	//	if (((uint8_t*)a)[0]==0x00)
			{ 
				j=1; 
				break;
			}
		i--;
	}
	 if (j == 0)
	 {
		 
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_3_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 //attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL, PERM(RD, ENABLE)| PERM(NTF,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL, PERM(NTF,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, PERM(RD, ENABLE)| PERM(WR,ENABLE));
		 return 1;
	 }
	 else 
	 {		 		 
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_3_IDX_VAL, PERM(WR,ENABLE));
		 //attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL, PERM(RD, ENABLE)| PERM(NTF,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL, PERM(NTF,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL, PERM(WR,ENABLE));
		 attmdb_att_set_permission(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, PERM(WR,ENABLE));

      return 0;
   }
}
//设置FFF8的值
void my_gattc_read8_cmd_ind_handler(uint8_t data_cnt,uint8_t seq,uint8_t pageID)
{
			/*设置char8的值*/
			send_char8[0]=data_cnt;
	    Rinfo_short_read(send_char8, seq, pageID);
      attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL, sizeof(my_new_20B), (uint8_t*)&send_char8);	
			
}

//计算名片包数
uint8_t countbag(uint8_t length)
{
	  uint8_t j;
    if(length%19!=0)
    {j=(length/19)+1;}
    else {j=length/19;}
    
		return j;
}
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref SAMPLE128_CREATE_DB_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sample128_create_db_req_handler(ke_msg_id_t const msgid,
                                       struct sample128_create_db_req const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    //Database Creation Status
    uint8_t status;
    uint8_t nb_att_16;
    uint8_t nb_att_128;
    uint8_t nb_att_32;
    uint16_t att_decl_svc = ATT_DECL_PRIMARY_SERVICE;
    uint16_t att_decl_char = ATT_DECL_CHARACTERISTIC;
    uint16_t att_decl_cfg = ATT_DESC_CLIENT_CHAR_CFG;
    uint16_t val_hdl;
    uint16_t char_hdl; 

    //Save Profile ID
    sample128_env.con_info.prf_id = TASK_SAMPLE128;

    /*---------------------------------------------------*
     * Link Loss Service Creation
     *---------------------------------------------------*/

        //Add Service Into Database
        nb_att_16 = 4+7; // 11 UUID16 atts
        nb_att_32 = 0;// No UUID32 att
        nb_att_128 = 2+7; //9 UUID128 att
        status = attmdb_add_service(&(sample128_env.sample128_shdl), TASK_SAMPLE128,
                                             nb_att_16, nb_att_32, nb_att_128, 58+1+(19*7)+2+2+1+8+8+20+1); //total attributte size = 36, 16 (svc)  + 19 (desc_char) + 1 (att)

        if (status == ATT_ERR_NO_ERROR)
        {
            //add svc attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN, //Data size = 16 (ATT_UUID_128_LEN)
                                               ATT_UUID_16_LEN, (uint8_t*)&att_decl_svc, PERM(RD, ENABLE),
                                               &(sample128_env.sample128_shdl));
            
            
            
            status = attmdb_att_set_value(sample128_env.sample128_shdl, ATT_UUID_128_LEN, (uint8_t *)sample128_svc.uuid);
            
            
//char 1     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint8_t), //Data size = 1
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_1_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_1_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_1_char), (uint8_t *)&sample128_1_char);
            

//////char 2
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
                        
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(my_new_2B), //Data size = 1
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_2_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_2_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_2_char), (uint8_t *)&sample128_2_char);

/*            //add cfg attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint16_t), 
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_cfg, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                               &(val_hdl));*/
//Characteristic 3:
//////////////////////////////////////////////////////////////////////////////////
// Add characteristic declaration attribute to database
            status = attmdb_add_attribute( sample128_env.sample128_shdl,
                                           ATT_UUID_128_LEN + 3, //Data size = 19
                                           ATT_UUID_16_LEN,//Size of declaration type ID
                                           (uint8_t*) &att_decl_char, // 0x2803
                                           PERM(RD, ENABLE),// Permissions
                                           &(char_hdl) // Handle to the characteristic declaration
                                          );
// Add characteristic value declaration attribute to database
            status = attmdb_add_attribute( sample128_env.sample128_shdl,
                                           sizeof(my_new_2B), //Data size = 20 Bytes
                                           ATT_UUID_128_LEN,// Size of custom type ID = 128-bit
                                           (uint8_t*)&sample128_3_val.uuid, // UUID
                                           PERM(WR, ENABLE),// Permissions
                                           &(val_hdl) // Handle to the value attribute
                                          );
// Store the value handle for characteristic 3
            memcpy(sample128_3_char.attr_hdl, &val_hdl, sizeof(uint8_t));
// Set initial value of characteristic 3
            status = attmdb_att_set_value( char_hdl,
                                          sizeof(sample128_3_char),
                                         (uint8_t *)&sample128_3_char
                                         );
					
//char 4     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(my_new_2B), //Data size = 20
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_4_val.uuid, PERM(NTF, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_4_char.attr_hdl, &val_hdl, sizeof(uint16_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_4_char), (uint8_t *)&sample128_4_char);
						//add cfg attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint16_t), 
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_cfg, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                               &(val_hdl));

//char 5     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint8_t), //Data size = 8
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_5_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_5_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_5_char), (uint8_t *)&sample128_5_char);

//char 6     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(my_new_8B), //Data size = 8
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_6_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_6_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_6_char), (uint8_t *)&sample128_6_char);
//char 7     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(my_new_8B), //Data size = 8
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_7_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_7_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_7_char), (uint8_t *)&sample128_7_char);			
//char 8     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(my_new_20B), //Data size = 8
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_8_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_8_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_8_char), (uint8_t *)&sample128_8_char);			
//char 9     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint8_t), //Data size = 8
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_9_val.uuid, PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_9_char.attr_hdl, &val_hdl, sizeof(uint8_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_9_char), (uint8_t *)&sample128_9_char);					
						
///////////////////////////////////////////////////////////////////////////////////////////////////////////////						
            //Disable sample128 service
            attmdb_svc_set_permission(sample128_env.sample128_shdl, PERM(SVC, DISABLE));

            //Go to Idle State
    
            //If we are here, database has been fulfilled with success, go to idle state
            ke_state_set(TASK_SAMPLE128, SAMPLE128_IDLE);
        }
        
        //Send CFM to application
        struct sample128_create_db_cfm * cfm = KE_MSG_ALLOC(SAMPLE128_CREATE_DB_CFM, src_id,
                                                    TASK_SAMPLE128, sample128_create_db_cfm);
        cfm->status = status;
        ke_msg_send(cfm);
    

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Enable the Sample128 role, used after connection.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sample128_enable_req_handler(ke_msg_id_t const msgid,
                                    struct sample128_enable_req const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    
    uint16_t temp = 1;
    
    // Keep source of message, to respond to it further on
    sample128_env.con_info.appid = src_id;
    // Store the connection handle for which this profile is enabled
    sample128_env.con_info.conidx = gapc_get_conidx(param->conhdl);

    // Check if the provided connection exist
    if (sample128_env.con_info.conidx == GAP_INVALID_CONIDX)
    {
        // The connection doesn't exist, request disallowed
        prf_server_error_ind_send((prf_env_struct *)&sample128_env, PRF_ERR_REQ_DISALLOWED,
                 SAMPLE128_ERROR_IND, SAMPLE128_ENABLE_REQ);
    }
    else
    {
        // Sample128 1
        attmdb_svc_set_permission(sample128_env.sample128_shdl, param->sec_lvl);

        //set char 1 to specified value
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->sample128_1_val);
        //set char 2 to specified value
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL,
                             sizeof(my_new_2B), (uint8_t *)&param->sample128_2_val);

			  // Set characteristic 3 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_3_IDX_VAL,
                             sizeof(my_new_2B), (uint8_t *)&param->sample128_3_val);
			
			  // Set characteristic 4 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL,
                             sizeof(my_new_2B), (uint8_t *)&param->sample128_4_val);

			  // Set characteristic 5 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->sample128_5_val);

			  // Set characteristic 6 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL,
                             sizeof(my_new_8B), (uint8_t *)&param->sample128_6_val);
			  // Set characteristic 7 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL,
                             sizeof(my_new_8B), (uint8_t *)&param->sample128_7_val);
			  // Set characteristic 8 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL,
                             sizeof(my_new_20B), (uint8_t *)&param->sample128_8_val);
			 // Set characteristic 9 to specified value
       attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->sample128_9_val);

			
        sample128_env.feature = param->feature; 
        
        if (!sample128_env.feature)
        {
               temp = 0;
        }
        
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_4_IDX_CFG,
                             sizeof(uint16_t), (uint8_t *)&temp);
        
        // Go to Connected state
        ke_state_set(TASK_SAMPLE128, SAMPLE128_CONNECTED);
    }

    return (KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Updates characteristic's 2 value. Sends notification to peer if property is enabled.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */


static int sample128_upd_char4_req_handler(ke_msg_id_t const msgid,
                                    struct sample128_upd_char4_req const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_OK;

    // Check provided values
    if(param->conhdl == gapc_get_conhdl(sample128_env.con_info.conidx))
    {
        // Update value in DB
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL,
                             sizeof(my_new_2B), (uint8_t *)&param->val);

        if((sample128_env.feature & PRF_CLI_START_NTF))
            // Send notification through GATT
            prf_server_send_event((prf_env_struct *)&sample128_env, false,
                    sample128_env.sample128_shdl + SAMPLE128_4_IDX_VAL);
        
    }
    else
    {
        status = PRF_ERR_INVALID_PARAM;
    }

    if (status != PRF_ERR_OK)
    {
        sample128_upd_char4_cfm_send(status);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATT_WRITE_CMD_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

static int gattc_write_cmd_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_cmd_ind const *param,/*位数不同 8 16*/
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{    
	  uint8_t value8_0=0x00;
	  uint8_t value8_1=0x01;
	  uint8_t value8_2=0x02;
	  uint8_t value8_3=0x03;
	  uint16_t value16_0=0x0000;
	  uint16_t value16_1=0x0001;
	  uint16_t value16_2=0x0002;
	  uint16_t value16_3=0x0003;
	  uint16_t value16_4=0x0200;
	  uint16_t value16_5=0x0201;
	  uint16_t value16_6=0x0101;
	  uint16_t value16_7=0x0100;
	  uint16_t value16_8=0x0300;
	  uint16_t value16_9=0x0301;
    uint8_t char_code = SAMPLE128_ERR_CHAR;
    uint8_t status = PRF_APP_ERROR;
	  att_size_t length = sizeof(my_new_8B);
	  uint8_t a[8]={0};
    uint8_t *b;
    uint8_t rev9_state;
    if (KE_IDX_GET(src_id) == sample128_env.con_info.conidx)
    {
			/*****每次写之前都需对比输入密码与内置密码是否一致****/ 
		//	if (id==0)/*密码错误*/
		//	{
			//	pwd_read(a);//从机器内部读取密码
			//
		//	  attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL,
    //                         &length, &a);
			 // attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL,
      //                       &length, &b);			
			 // id=identify(a, b);
		//	}
			
        if (param->handle == sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL)
        {
            char_code = SAMPLE128_1_CHAR;
        }
        
        if (param->handle == sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL)
        {
            char_code = SAMPLE128_2_CHAR;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_3_IDX_VAL)
        {
            char_code = SAMPLE128_3_CHAR;
        }
		   	 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_4_IDX_CFG)
        {
            char_code = SAMPLE128_4_CFG;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL)
        {
            char_code = SAMPLE128_5_CHAR;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL)
        {
            char_code = SAMPLE128_6_CHAR;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL)
        {
            char_code = SAMPLE128_7_CHAR;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL)
        {
            char_code = SAMPLE128_8_CHAR;
        }
				 if (param->handle == sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL)
        {
            char_code = SAMPLE128_9_CHAR;
        }				
				
				/***if password corrects***/
        if (id==1)
				{				
					if (char_code == SAMPLE128_1_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)param->value);
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
/*****************************************************/						

/*****************************************************/		
					 if(param->value[0]==0x01)/*写名片*/
					  //if(*param->value == 0x0100)
            {/*写名片模式*/
							data_count_w=0;/*写名片前初始化*/
              data_count_w++;/*包数加1*/				
							/*char5 存放当前data_count_w*/
              attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_w);						
              attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);		
              /*写名片存储区域长度初始化*/
							Tinfo_Length_init();							
						 /*FFF4返回0x0101*/
	           /***********send notification****************************/
            sample128_placeholder2[0]=0x01;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);								
             }								
						else if(param->value[0]==0x02)/*读名片*/
						{
							data_count_r=0;/*读名片前初始化*/
           //   attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);						
							data_count_rcard=0;/*读名片前初始化*/
           //     attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_rcard);						
             /*更新char8*/							
              data_count_r++;/*包数加1*/
      				my_gattc_read8_cmd_ind_handler(data_count_r,(data_count_r-1)*19,data_count_rcard);								
              attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);						
							data_count_rcard++;/*名片数加1*/	
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_rcard);						
						 	/*名片总数*/
							card_amount=Rinfo_get_pages();
							attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&card_amount);						

							/*当前名片包数*/
							uint8_t temp=0;
							temp=Rinfo_single_read(data_count_rcard-1,0);
							send_amount=countbag(temp+18);
							attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&send_amount);						
           if(card_amount>0)
					 {
							/*FFF4返回0x0201*/
            sample128_placeholder2[0]=0x02;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	
					 }
					 else if (card_amount==0)
					 {
						/*FFF4返回0x0200*/
            sample128_placeholder2[0]=0x02;
            sample128_placeholder2[1]=0x00;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	
					 }
						 }              
         }			
					else if (char_code == SAMPLE128_5_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
               
         }				 
					else if (char_code == SAMPLE128_9_CHAR)
         {
            
            //Save value in DB
            rev9_state=attmdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
            if(*param->value==value8_1)
						{/*电话*/}
            else if(*param->value==value8_2)
						{/*短信*/}						
            else if(*param->value==value8_3)
						{/*微信*/}		
						if(rev9_state==ATT_ERR_NO_ERROR)
						{/*用FFF4 RETURN 0x0301*/	
	/***********send notification****************************/
            sample128_placeholder2[0]=0x03;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	

             }
							else if(rev9_state!=ATT_ERR_NO_ERROR)
						{/*用FFF4 RETURN 0x0300*/		
	/***********send notification****************************/
            sample128_placeholder2[0]=0x03;
            sample128_placeholder2[1]=0x00;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	
								 
            }				
					}						
					else if (char_code == SAMPLE128_2_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_2B), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
               
         }			
					else if (char_code == SAMPLE128_3_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_2B), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
						/*****/
/****************************************************************/
						if(param->value[0]==0x02)
						{
							if(param->value[1]==0x01)
						{
							if(data_count_r==send_amount)/*一张名片发送完毕*/
							{
								if(data_count_rcard==card_amount)/*所有名片发送完毕*/
								{	
								/*退出读名片模式*/
								data_count_r=0;
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);						
								data_count_rcard=0;									
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_rcard);						
                /*char8 空包 包头0x00*/
								my_new_20B send_char8={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL, sizeof(my_new_20B), (uint8_t*)&send_char8);
                /*清空内存*/
								Rinfo_ALLinit();
								Rinfo_pages_init();									
                 }		
                else if(data_count_rcard<card_amount)/*还有未发送的名片*/		
								{	
								/*发送新名片*/
								data_count_r=0;
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);						
								data_count_rcard++;									
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_rcard);						
                /*char8 空包 包头0xFF*/
								my_new_20B send_char8={0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_8_IDX_VAL, sizeof(my_new_20B), (uint8_t*)&send_char8);
                 }									
               }
							 else if(data_count_r<send_amount)/*一张名片未发完*/							 
							 {
								 data_count_r++;/*包数加1*/	                  							 
                //attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(my_new_2B), (uint8_t *)&data_count_r);						
                /*更新char8*/							
							  my_gattc_read8_cmd_ind_handler(data_count_r,(data_count_r-1)*19,data_count_rcard-1);																						
							 }							
						}								
             }						
							 
         }				 
					else if (char_code == SAMPLE128_6_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_8B), (uint8_t *)param->value);
					  flash_pwd_delete();//清空密码
					  uint8_t* temp=(uint8_t*)(param->value);		
            pwd_write(temp);//存入新密码
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
               
         }		
					else if (char_code == SAMPLE128_7_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_8B), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
						pwd_read(a);//从机器内部读取密码
					//	attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL,
          //                     &length, &a);
			      attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL,
                             &length, &b);			
			      id=identify(a, b);	//比对密码
            	if(id==0)
				       { /*FFF4返回0x0001*/
	  /***********send notification****************************/
                struct sample128_upd_char4_req *req4 = KE_MSG_ALLOC(
                                                        SAMPLE128_UPD_CHAR4_REQ,
                                                        TASK_SAMPLE128,
                                                        TASK_APP,
                                                        sample128_upd_char4_req
                                                        );
            sample128_placeholder2[0]=0x00;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	

                }
				      else if(id==1)
		         		{ /*FFF4返回0x0000*/
	/***********send notification****************************/
            sample128_placeholder2[0]=0x00;
            sample128_placeholder2[1]=0x00;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);	

                }						
               
         }					 
					else if (char_code == SAMPLE128_8_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_20B), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
						if(*param->value==0)/*空包，写名片结束*/
					{
						/*计数器清零*/
					  data_count_w=0;						
            attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_w);

						/*FFF4返回0x0101*/
	/***********send notification****************************/
            sample128_placeholder2[0]=0x01;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);

						 
					}			
          else if((uint8_t)param->value[0]==data_count_w)/*与计数器相符*/
          {
						/*计数器加1*/
					  data_count_w++;
            attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(my_new_2B), (uint8_t *)&data_count_w);							

						/*FFF4返回0x0101*/
	/***********send notification****************************/
            sample128_placeholder2[0]=0x01;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);

							 
						/*将数据存入内存中*/	 
						uint8_t* temp=(uint8_t*)(param->value);
					  Tinfo_short_write(temp,19);
					//  Tinfo_Length_set(data_count_w*19);
           }	
					else if(param->value[0]!=data_count_w)/*与计数器不符*/
					{/*FFF4返回0x0100*/
	/***********send notification****************************/
            sample128_placeholder2[0]=0x01;
            sample128_placeholder2[1]=0x00;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);

					 }
         }
				 
        else if (char_code == SAMPLE128_4_CFG)
        {
            
            // Written value
            uint16_t ntf_cfg;

            // Extract value before check
            ntf_cfg = co_read16p(&param->value[0]);
        
            // Only update configuration if value for stop or notification enable
            if ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_NTF))
            {
                //Save value in DB
                attmdb_att_set_value(param->handle, sizeof(uint16_t), (uint8_t *)&param->value[0]);
                
                // Conserve information in environment
                if (ntf_cfg == PRF_CLI_START_NTF)
                {
                    // Ntf cfg bit set to 1
                    sample128_env.feature |= PRF_CLI_START_NTF;
                }
                else
                {
                    // Ntf cfg bit set to 0
                    sample128_env.feature &= ~PRF_CLI_START_NTF;
                }                
                
                status = PRF_ERR_OK; 
                
            }
        }
				 
				}
				 
/****************if password incorrect******************/				
				else if(id==0)
				{
         data_count_w=0;
         attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_w);
         data_count_r=0;					
              attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_5_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_r);						
         data_count_rcard=0;					
                attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_9_IDX_VAL, sizeof(uint8_t), (uint8_t *)&data_count_rcard);						
					if (char_code == SAMPLE128_1_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)param->value);
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
/*****************************************************/						
						if(param->value[0]==0x00)/*初始化*/
						{
							/*初始化*/
           //   flash_info_init();
				
							//test
              uint8_t temp[100]={0,100,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
                                27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,
							                  0,50,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
                                27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
							uint8_t temp1[100]={0,100,100,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
                                27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,51,
							                  0,50,100,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
                                27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,51};
              Recinfo_Compare_Save(temp);
							//Recinfo_Compare_Save(temp1);
							/* Rinfo_all_write(temp, 0);
					    Rinfo_add_pages();
              Rinfo_all_write(temp1,1);								
              Rinfo_add_pages();		*/
							
              /*FFF4返回0x0001*/
	  /***********send notification****************************/
            sample128_placeholder2[0]=0x00;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);
								
						}
               
         }							
				 	if (char_code == SAMPLE128_7_CHAR)
         {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(my_new_8B), (uint8_t *)param->value);
            
            if(param->last)
            {
                sample128_send_val((uint8_t)*param->value);
            } 

            status = PRF_ERR_OK;
						pwd_read(a);//从机器内部读取密码
					//	attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_6_IDX_VAL,
          //                    &length, &a);
			      attmdb_att_get_value(sample128_env.sample128_shdl + SAMPLE128_7_IDX_VAL,
                             &length, &b);			
			      id=identify(a, b);	
            	if(id==0)
				       { /*FFF4返回0x0000*/
	  /***********send notification****************************/
            sample128_placeholder2[0]=0x00;
            sample128_placeholder2[1]=0x00;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);

                }
				      else if(id==1)
		         		{ /*FFF4返回0x0001*/
	/***********send notification****************************/
            sample128_placeholder2[0]=0x00;
            sample128_placeholder2[1]=0x01;
						ke_timer_set(APP_SAMPLE128_TIMER,TASK_APP,0);

                }		
							}				
							
        else if (char_code == SAMPLE128_4_CFG)
        {
            
            // Written value
            uint16_t ntf_cfg;

            // Extract value before check
            ntf_cfg = co_read16p(&param->value[0]);
        
            // Only update configuration if value for stop or notification enable
            if ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_NTF))
            {
                //Save value in DB
                attmdb_att_set_value(param->handle, sizeof(uint16_t), (uint8_t *)&param->value[0]);
                
                // Conserve information in environment
                if (ntf_cfg == PRF_CLI_START_NTF)
                {
                    // Ntf cfg bit set to 1
                    sample128_env.feature |= PRF_CLI_START_NTF;
                }
                else
                {
                    // Ntf cfg bit set to 0
                    sample128_env.feature &= ~PRF_CLI_START_NTF;
                }                
                
                status = PRF_ERR_OK; 
                
            }
        }
			}
    }

    // Send Write Response
    atts_write_rsp_send(sample128_env.con_info.conidx, param->handle, status);
    
    return (KE_MSG_CONSUMED);
}




/**
 ****************************************************************************************
 * @brief Disconnection indication to sample128.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gap_disconnnect_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_disconnect_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

    // Check Connection Handle
    if (KE_IDX_GET(src_id) == sample128_env.con_info.conidx)
    {
        
        // In any case, inform APP about disconnection
        sample128_disable();
    }

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Disabled State handler definition.
const struct ke_msg_handler sample128_disabled[] =
{
    {SAMPLE128_CREATE_DB_REQ,   (ke_msg_func_t) sample128_create_db_req_handler },
};

/// Idle State handler definition.
const struct ke_msg_handler sample128_idle[] =
{
    {SAMPLE128_ENABLE_REQ,      (ke_msg_func_t) sample128_enable_req_handler },
};

/// Connected State handler definition.
const struct ke_msg_handler sample128_connected[] =
{
    {GATTC_WRITE_CMD_IND,       (ke_msg_func_t) gattc_write_cmd_ind_handler},
    {SAMPLE128_UPD_CHAR4_REQ,   (ke_msg_func_t) sample128_upd_char4_req_handler},
		
};

/// Default State handlers definition
const struct ke_msg_handler sample128_default_state[] =
{
    {GAPC_DISCONNECT_IND,    (ke_msg_func_t) gap_disconnnect_ind_handler},
};

/// Specifies the message handler structure for every input state.
const struct ke_state_handler sample128_state_handler[SAMPLE128_STATE_MAX] =
{
    [SAMPLE128_DISABLED]    = KE_STATE_HANDLER(sample128_disabled),
    [SAMPLE128_IDLE]        = KE_STATE_HANDLER(sample128_idle),
    [SAMPLE128_CONNECTED]   = KE_STATE_HANDLER(sample128_connected),
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler sample128_default_handler = KE_STATE_HANDLER(sample128_default_state);

/// Defines the place holder for the states of all the task instances.
ke_state_t sample128_state[SAMPLE128_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init));

#endif //BLE_SAMPLE128

