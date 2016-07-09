/**
 ****************************************************************************************
 *
 * @file sample128.h
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

#ifndef SAMPLE128_H_
#define SAMPLE128_H_

typedef unsigned char my_new_2B[2];
typedef unsigned char my_new_8B[8];
typedef unsigned char my_new_20B[20];
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_SAMPLE128)

#include "ke_task.h"
#include "atts.h"
#include "prf_types.h"

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Handles offsets
enum
{
    SAMPLE128_1_IDX_SVC,

    SAMPLE128_1_IDX_CHAR,
    SAMPLE128_1_IDX_VAL,

    SAMPLE128_2_IDX_CHAR,
    SAMPLE128_2_IDX_VAL,
    
	  SAMPLE128_3_IDX_CHAR,
    SAMPLE128_3_IDX_VAL,
	
	  SAMPLE128_4_IDX_CHAR,
    SAMPLE128_4_IDX_VAL,
    SAMPLE128_4_IDX_CFG,
	
	  SAMPLE128_5_IDX_CHAR,
    SAMPLE128_5_IDX_VAL,	
	
	  SAMPLE128_6_IDX_CHAR,
    SAMPLE128_6_IDX_VAL,	
	
	  SAMPLE128_7_IDX_CHAR,
    SAMPLE128_7_IDX_VAL,	

	  SAMPLE128_8_IDX_CHAR,
    SAMPLE128_8_IDX_VAL,	

	  SAMPLE128_9_IDX_CHAR,
    SAMPLE128_9_IDX_VAL,	
	
    SAMPLE128_1_IDX_NB,
};

///Characteristics Code for Write Indications
enum
{
    SAMPLE128_ERR_CHAR,
    SAMPLE128_1_CHAR,
	  SAMPLE128_2_CHAR,
	  SAMPLE128_3_CHAR,
    SAMPLE128_4_CFG,
	  SAMPLE128_5_CHAR,
	  SAMPLE128_6_CHAR,
	  SAMPLE128_7_CHAR,	
	  SAMPLE128_8_CHAR,	
	  SAMPLE128_9_CHAR,	
};

/*
 * STRUCTURES
 ****************************************************************************************
 */

/// sample128 environment variable
struct sample128_env_tag
{
    /// Connection Information
    struct prf_con_info con_info;

    /// Sample128  svc Start Handle
    uint16_t sample128_shdl;
    
    //Notification property status
    uint8_t feature;

};

/*
 *  SAMPLE128 PROFILE ATTRIBUTES VALUES DECLARATION
 ****************************************************************************************
 */

/// sample128 Service
extern const struct att_uuid_128 sample128_svc;
/// sample128_1 - Characteristic
extern struct att_char128_desc sample128_1_char;
/// sample128_1 - Value
extern const struct att_uuid_128 sample128_1_val;
/// sample128_2 - Characteristic
extern struct att_char128_desc sample128_2_char;
/// sample128_2 - Value
extern const struct att_uuid_128 sample128_2_val;
/// sample128_3 - Characteristic
extern struct att_char128_desc sample128_3_char;
/// sample128_3 - Value
extern const struct att_uuid_128 sample128_3_val;
/// sample128_4 - Characteristic
extern struct att_char128_desc sample128_4_char;
/// sample128_4 - Value
extern const struct att_uuid_128 sample128_4_val;
/// sample128_5 - Characteristic
extern struct att_char128_desc sample128_5_char;
/// sample128_5 - Value
extern const struct att_uuid_128 sample128_5_val;
/// sample128_6 - Characteristic
extern struct att_char128_desc sample128_6_char;
/// sample128_6 - Value
extern const struct att_uuid_128 sample128_6_val;
/// sample128_7 - Characteristic
extern struct att_char128_desc sample128_7_char;
/// sample128_7 - Value
extern const struct att_uuid_128 sample128_7_val;
/// sample128_8 - Characteristic
extern struct att_char128_desc sample128_8_char;
/// sample128_8 - Value
extern const struct att_uuid_128 sample128_8_val;
/// sample128_9 - Characteristic
extern struct att_char128_desc sample128_9_char;
/// sample128_9 - Value
extern const struct att_uuid_128 sample128_9_val;
/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern struct sample128_env_tag sample128_env;
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the SAMPLE128 module.
 * This function performs all the initializations of the SAMPLE128 module.
 ****************************************************************************************
 */
void sample128_init(void);

/**
 ****************************************************************************************
 * @brief Send value change to application.
 * @param val Value.
 ****************************************************************************************
 */
 
void sample128_send_val(uint8_t val);

/**
 ****************************************************************************************
 * @brief Disable role.
 ****************************************************************************************
 */
void sample128_disable(void);

/**
 ****************************************************************************************
 * @brief Send update char4 confirmation message
 ****************************************************************************************
 */
void sample128_upd_char4_cfm_send(uint8_t status);
#endif //BLE_SAMPLE128

#endif // SAMPLE128_H_
