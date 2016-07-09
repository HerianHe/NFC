/**
 ****************************************************************************************
 *
 * @file phdr_task.c
 *
 * @brief peripheral driver. Task implementation.
 *
 * Copyright (C) AIRBAND 2014-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PHDRTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#include "gap.h"
#include "gattc_task.h"
#include "phdr.h"
#include "phdr_task.h"
#include "prf_utils.h"
#include "attm_util.h"
#include "gpio.h"
#include "ke_timer.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief LED灯blink消息处理函数
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int led_blink_handler(ke_msg_id_t const msgid, 
                           struct phdr_set_blink_req const *param, 
													 ke_task_id_t const dest_id, 
													 ke_task_id_t const src_id)
{
	///flag indicates current led is light(1) or not(0)
	static uint8_t flag = 0;
	///times indicates current times that led have blinked
	static uint8_t times=0;
	
	///counts represent the altogether times that led should blink
	static uint8_t counts=0;
	///ledperiod represent led blink spare
	static uint8_t ledperiod=0;
	
	if(param->start_flag==START_BLINK)
	{
		counts=(param->blink_times);
		ledperiod=(param->period);
		flag=0;
		times=0;
		///setting current state to blinking;
		phdr_env.blink_state=BLINKING;
	}
	
	if(flag)
	{
    GPIO_SetActive( GPIO_PORT_0, GPIO_PIN_7 );
		flag = 0;
		times++;
	}
	else
	{
		GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_7);
		flag = 1;
		times++;
	}
	
	if(param->start_flag==STOP_BLINK)
	{
		times=0;
		flag=0;
		counts=0;
		ledperiod=0;
		GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_7);
		ke_timer_clear(COUNT_OVER, TASK_PHDR);
		///set current state to blink_over;
		phdr_env.blink_state=BLINK_OVER;
	}
	else
	{
		if(times<counts)
		{
			ke_timer_set(COUNT_OVER, TASK_PHDR, ledperiod);
		}else{
			times=0;
			flag=0;
			GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_7);
			///set current state to blink_over;
			phdr_env.blink_state=BLINK_OVER;
		}
	}
	
	
	
	return (KE_MSG_CONSUMED);

}


/**
 ****************************************************************************************
 * @brief 与LED灯blink消息配合的软件定时器任务，用于间断性输入
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int count_over_handler(ke_msg_id_t const msgid, 
                           void const *param, 
													 ke_task_id_t const dest_id, 
													 ke_task_id_t const src_id)
{
	struct phdr_set_blink_req *req=KE_MSG_ALLOC(LED_BLINK,
                                                  TASK_PHDR, TASK_APP,
                                                  phdr_set_blink_req);
	req->blink_times=0;
	req->period=0;
	req->start_flag=CONTINUE_BLINK;
	ke_msg_send(req);
	return (KE_MSG_CONSUMED);
}





/**
 ****************************************************************************************
 * @brief 用于处理震动的消息
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int vibrate_handler(ke_msg_id_t const msgid, 
                           void const *param, 
													 ke_task_id_t const dest_id, 
													 ke_task_id_t const src_id)
{
	/*struct phdr_set_blink_req *req=KE_MSG_ALLOC(LED_BLINK,
                                                  TASK_PHDR, TASK_APP,
                                                  phdr_set_blink_req);
	req->blink_times=0;
	req->period=0;
	req->start_flag=CONTINUE_BLINK;
	ke_msg_send(req);*/
  GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_4);
	ke_timer_clear(VIBRATION_OVER, TASK_PHDR);
	GPIO_SetActive(GPIO_PORT_0, GPIO_PIN_4);
	ke_timer_set(VIBRATION_OVER, TASK_PHDR, 43);
	return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief 用于处理震动结束定时器的消息
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int vibrate_over_handler(ke_msg_id_t const msgid, 
                           void const *param, 
													 ke_task_id_t const dest_id, 
													 ke_task_id_t const src_id)
{
	GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_4);
	return (KE_MSG_CONSUMED);
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


/// Default State handlers definition
const struct ke_msg_handler phdr_default_state[] =
{
		{LED_BLINK,           (ke_msg_func_t) led_blink_handler        },
		{COUNT_OVER,          (ke_msg_func_t) count_over_handler       },
		{VIBRATION,           (ke_msg_func_t) vibrate_handler     		 },
		{VIBRATION_OVER,      (ke_msg_func_t) vibrate_over_handler     },
};

///Specifies the message handler structure for every input state.
const struct ke_state_handler phdr_state_handler[PHDR_STATE_MAX] =
{
    [PHDR_IDLE]   = KE_STATE_HANDLER_NONE,
};

///Specifies the message handlers that are common to all states.
const struct ke_state_handler phdr_default_handler = KE_STATE_HANDLER(phdr_default_state);

///Defines the place holder for the states of all the task instances.
ke_state_t phdr_state[PHDR_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY



/// @} PHDRTASK
