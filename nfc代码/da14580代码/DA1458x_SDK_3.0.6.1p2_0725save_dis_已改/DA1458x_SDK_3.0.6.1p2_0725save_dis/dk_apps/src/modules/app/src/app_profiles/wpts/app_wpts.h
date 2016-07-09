/**
 ****************************************************************************************
 *
 * @file app_dis.h
 *
 * @brief Device Information Service Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_WPTS_H_
#define APP_WTPS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 *
 * @brief Device Information Service Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW Configuration

#if (BLE_WPTS)

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>
#include "ble_580_sw_version.h"
#include "app_sw_version.h"
/*
 * DEFINES
 ****************************************************************************************
 */


#if (BLE_APP_PRESENT)


#define APP_WPTS_FEATURES                (  WPTS_FEATURE_ALERT_NTF_ACTIVE | WPTS_FEATURE_ALERT_IND_ACTIVE )

#else

#define APP_WPTS_FEATURES                (0)

#endif //(BLE_APP_PRESENT)

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Device Information Service Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Device Information Service Application
 ****************************************************************************************
 */
void app_wpts_init(void);

/**
 ****************************************************************************************
 * @brief Add a Device Information Service instance in the DB
 ****************************************************************************************
 */
void app_wpts_create_db_send(void);

/**
 ****************************************************************************************
 * @brief Enable the Device Information Service
 ****************************************************************************************
 */
void app_wpts_enable_prf(uint16_t);

#endif //BLE_DIS_SERVER

/// @} APP

#endif //APP_DIS_H_
