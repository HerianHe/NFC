/**
 ****************************************************************************************
 *
 * @file app_batt.c
 *
 * @brief Battery server application.
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if BLE_BATT_SERVER
#include "app_task.h"                // application task definitions
#include "bass_task.h"
#include "app_batt.h"
#include "gpio.h"
#include "battery.h" 

uint16_t bat_poll_timeout __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
uint8_t cur_batt_level __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
uint8_t batt_alert_en  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
uint8_t bat_led_state __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
uint8_t bat_lvl_alert_used __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
GPIO_PORT bat_led_port __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
GPIO_PIN bat_led_pin __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Create Battery Service's Database.
 *
 * @return void
 ****************************************************************************************
 */

void app_batt_create_db(void)
{
    // Add BASS in the database
    struct bass_create_db_req * req = KE_MSG_ALLOC(BASS_CREATE_DB_REQ, TASK_BASS,
                                                   TASK_APP, bass_create_db_req);

	req->bas_nb = 1;
	req->features[0] = BAS_BATT_LVL_NTF_SUP;
	req->features[1] = BAS_BATT_LVL_NTF_NOT_SUP;

    // Send the message
	//puts("Send BASS_CREATE_DB_REQ\r\n");
    ke_msg_send(req);
}

/**
 ****************************************************************************************
 * @brief Initialize battery level
 *
 * @return void
 ****************************************************************************************
 */

void app_batt_init(void)
{
	// called from app.c::app_init() when chip boots
    cur_batt_level = battery_get_lvl(BATT_CR2032);    
}

/**
 ****************************************************************************************
 * @brief Enable Battery Service.
 *
 * @param[in] batt_lvl          Initial battery level
 * @param[in] lvl_alert_used    Enables/Disables battery level alert
 * @param[in] led_port          Battery level alert LED port
 * @param[in] led_pin           Battery level alert LED pin
 *
 * @return void
 ****************************************************************************************
 */

void app_batt_enable( uint8_t batt_lvl, uint8_t lvl_alert_used, GPIO_PORT led_port, GPIO_PIN led_pin)
{
    
    bat_lvl_alert_used = lvl_alert_used;
    bat_led_port = led_port;
    bat_led_pin = led_pin;

	// Allocate the message
	struct bass_enable_req * req = KE_MSG_ALLOC(BASS_ENABLE_REQ, TASK_BASS, TASK_APP,
											 bass_enable_req);
	// Fill in the parameter structure
	req->conhdl             = app_env.conhdl;
	req->sec_lvl            = PERM(SVC, ENABLE);
	req->con_type           = PRF_CON_NORMAL; // PRF_CON_DISCOVERY;
	req->batt_level_ntf_cfg[0] = 0; // Notifiacation OFF by default.
	req->batt_level_ntf_cfg[1] = 0;
	req->old_batt_lvl[0] = batt_lvl;
	req->old_batt_lvl[1] = 0;
	req->current_batt_lvl[0] = batt_lvl;
	req->current_batt_lvl[1] = 0;

	struct prf_char_pres_fmt *batt_level_pres_format = req->batt_level_pres_format;

	batt_level_pres_format[0].unit = 0x27AD;
	batt_level_pres_format[0].description = 0; // FIXME CORRECT?
	batt_level_pres_format[0].format = 4;
	batt_level_pres_format[0].exponent = 0;
	batt_level_pres_format[0].name_space = 1;
		
	// Send the message
	ke_msg_send(req);
	
}

/**
 ****************************************************************************************
 * @brief Reads devices current battery level and sends battery level update request to Battery Service.
 *
 * @return void
 ****************************************************************************************
 */

void app_batt_lvl(void)
{
    
	uint8_t batt_lvl;
	
	batt_lvl = battery_get_lvl(BATT_CR2032);
	
	if (batt_lvl != cur_batt_level)
		app_batt_set_level(batt_lvl);
	
	//update old_batt_lvl for the next use
	cur_batt_level = batt_lvl;
	
	//if battery level is bellow 5% then start a battery alert to notify user
	
    if(bat_lvl_alert_used && batt_lvl <5)
		app_batt_alert_start();
}	
	
/**
 ****************************************************************************************
 * @brief Sends battery level update request to Battery Service.
 *
 * @param[in] batt_lvl     battery level to set
 *
 * @return void
 ****************************************************************************************
 */

void app_batt_set_level(uint8_t batt_lvl)
{	
	// Allocate the message
	struct bass_batt_level_upd_req * req = KE_MSG_ALLOC(BASS_BATT_LEVEL_UPD_REQ, TASK_BASS, TASK_APP,
                                                 bass_batt_level_upd_req);
		
    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
    req->bas_instance = 0;
	req->batt_level = batt_lvl;
	
  // Send the message
	//puts("Send BASS_BATT_LEVEL_UPD_REQ");
	ke_msg_send(req);
		
}

/**
 ****************************************************************************************
 * @brief Starts battery level polling.
*
 * @param[in] poll_timeout     Battery Polling frequency
 *
 * @return void.
 ****************************************************************************************
 */

void app_batt_poll_start(uint16_t poll_timeout)
{
    
    bat_poll_timeout = poll_timeout;
    
	app_timer_set(APP_BATT_TIMER, TASK_APP, 10);	//first poll in 100 ms

}

/**
 ****************************************************************************************
 * @brief Stops battery level polling.
 *
 * @return void.
 ****************************************************************************************
 */

void app_batt_poll_stop(void)
{
  ke_timer_clear(APP_BATT_TIMER, TASK_APP);
}

/**
 ****************************************************************************************
 * @brief Starts battery alert. Battery Low.
 *
 * @return void.
 ****************************************************************************************
*/

void app_batt_alert_start(void)
{
	batt_alert_en = 1;

    if (bat_lvl_alert_used )
        GPIO_SetActive( bat_led_port, bat_led_pin);

	bat_led_state = 1;
	app_timer_set(APP_BATT_ALERT_TIMER, TASK_APP, 200); //60000); //10 mins timeout
}

/**
 ****************************************************************************************
 * @brief Stops battery alert. Battery Low.
 *
 * @return void.
 ****************************************************************************************
 */

void app_batt_alert_stop(void)
{	
	batt_alert_en = 0;

    if (bat_lvl_alert_used)
        GPIO_SetInactive( bat_led_port, bat_led_pin);
	
	bat_led_state = 0;
	ke_timer_clear(APP_BATT_ALERT_TIMER, TASK_APP);
}

/**
 ****************************************************************************************
 * @brief Reinit battery alert LED port. Must be called in periph_init()
 *
 * @return void.
 ****************************************************************************************
 */

void app_batt_port_reinit(void)
{
    
    if (bat_lvl_alert_used)
    {
        if(bat_led_state == 1){
            GPIO_SetActive( bat_led_port, bat_led_pin);
        }
        else{
            GPIO_SetInactive( bat_led_port, bat_led_pin);
        }
    }


}

#endif //BLE_BATT_SERVER
/// @} APP
