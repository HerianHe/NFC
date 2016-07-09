/**
 ****************************************************************************************
 *
 * @file sample128_task.h
 *
 * @brief Header file - sample128_task.
 * @brief 128 UUID service. sample code
 *
 * Copyright (C) 2013 Dialog Semiconductor GmbH and its Affiliates, unpublished work
 * This computer program includes Confidential, Proprietary Information and is a Trade Secret 
 * of Dialog Semiconductor GmbH and its Affiliates. All use, disclosure, and/or 
 * reproduction is prohibited unless authorized in writing. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef SAMPLE128_TASK_H_
#define SAMPLE128_TASK_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
*/

#if (BLE_SAMPLE128)

#include <stdint.h>
#include "ke_task.h"
#include "sample128.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximum number of Sample128 task instances
#define SAMPLE128_IDX_MAX                 (1)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Possible states of the SAMPLE128 task
enum
{
    /// Disabled State
    SAMPLE128_DISABLED,
    /// Idle state
    SAMPLE128_IDLE,
    /// Connected state
    SAMPLE128_CONNECTED,

    /// Number of defined states.
    SAMPLE128_STATE_MAX
};

/// Messages for Sample128
enum
{
    /// Start sample128. Device connection
    SAMPLE128_ENABLE_REQ = KE_FIRST_MSG(TASK_SAMPLE128),

    /// Disable confirm.
    SAMPLE128_DISABLE_IND,

    /// Att Value change indication
    SAMPLE128_VAL_IND,

    ///Create DataBase
    SAMPLE128_CREATE_DB_REQ,
    ///Inform APP of database creation status
    SAMPLE128_CREATE_DB_CFM,

    ///Create DataBase
    SAMPLE128_UPD_CHAR2_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR2_CFM,
	
    ///Create DataBase
    SAMPLE128_UPD_CHAR3_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR3_CFM,

    ///Create DataBase
    SAMPLE128_UPD_CHAR4_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR4_CFM,
		
    ///Create DataBase
    SAMPLE128_UPD_CHAR5_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR5_CFM,

    ///Create DataBase
    SAMPLE128_UPD_CHAR6_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR6_CFM,
		
    ///Create DataBase
    SAMPLE128_UPD_CHAR7_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR7_CFM,

    ///Create DataBase
    SAMPLE128_UPD_CHAR8_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR8_CFM,
		
    ///Create DataBase
    SAMPLE128_UPD_CHAR9_REQ,
    ///Inform APP of database creation status
    SAMPLE128_UPD_CHAR9_CFM,		
    
    /// Error Indication
    SAMPLE128_ERROR_IND,
};


/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */

/// Parameters of the @ref SAMPLE128_ENABLE_REQ message
struct sample128_enable_req
{
    /// Connection Handle
    uint16_t conhdl;
    /// Security level
    uint8_t sec_lvl;

    /// characteristic 1 value
    uint8_t sample128_1_val;
    
    /// characteristic 2 value
    my_new_2B sample128_2_val;
    
    /// char 4 Ntf property status
    uint8_t feature;
	 
	  /// characteristic 3 value
    my_new_2B sample128_3_val;
	 	/// characteristic 4 value
    my_new_2B sample128_4_val;
	 	/// characteristic 5 value
    uint8_t sample128_5_val;
	 	/// characteristic 6 value
    my_new_8B sample128_6_val;
	 	/// characteristic 7 value
    my_new_8B sample128_7_val;		
	 	/// characteristic 8 value
    my_new_20B sample128_8_val;
	 	/// characteristic 9 value
    uint8_t sample128_9_val;		
};

/// Parameters of the @ref SAMPLE128_DISABLE_IND message
struct sample128_disable_ind
{
    /// Connection Handle
    uint16_t conhdl;
};

/// Parameters of the @ref SAMPLE128_VAL_IND message
struct sample128_val_ind
{
    /// Connection handle
    uint16_t conhdl;
    /// Value
    uint8_t val;
    
};

/// Parameters of the @ref SAMPLE128_CREATE_DB_REQ message
struct sample128_create_db_req
{
    /// Indicate if TXPS is supported or not
    uint8_t features;
};

/// Parameters of the @ref SAMPLE128_CREATE_DB_CFM message
struct sample128_create_db_cfm
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref SAMPLE128_UPD_CHAR2_REQ message
struct sample128_upd_char2_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_2B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char3_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_2B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char4_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_2B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char5_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    uint8_t val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char6_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_8B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char7_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_8B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR3_REQ message
struct sample128_upd_char8_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    my_new_20B val;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR9_REQ message
struct sample128_upd_char9_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Characteristic Value
    uint8_t val;
};

/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char2_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char3_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char4_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char5_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char6_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char7_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char8_cfm
{
    /// Status
    uint8_t status;
};
/// Parameters of the @ref SAMPLE128_UPD_CHAR2_CFM message
struct sample128_upd_char9_cfm
{
    /// Status
    uint8_t status;
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler sample128_state_handler[SAMPLE128_STATE_MAX];
extern const struct ke_state_handler sample128_default_handler;
extern ke_state_t sample128_state[SAMPLE128_IDX_MAX];

#endif //BLE_SAMPLE128

#endif // SAMPLE128_TASK_H_
