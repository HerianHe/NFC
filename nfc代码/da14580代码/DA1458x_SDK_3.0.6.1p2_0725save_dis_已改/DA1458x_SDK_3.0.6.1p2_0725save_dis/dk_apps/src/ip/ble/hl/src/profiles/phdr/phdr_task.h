/**
 ****************************************************************************************
 *
 * @file phdr_task.h
 *
 * @brief Header file - PHDRTASK.
 *
 * Copyright (C) AIRBAND 2014-2015
 *
 *
 ****************************************************************************************
 */

#ifndef PHDR_TASK_H_
#define PHDR_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup PHDRTASK Task
 * @ingroup PHDR
 * @brief peripheral driver Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "ke_task.h"
#include "phdr.h"
#include "prf_types.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define PHDR_IDX_MAX        (1)

/// some action define that will be performed by the message param
#define START_BLINK        		1
#define CONTINUE_BLINK        0
#define STOP_BLINK        		2

/// Possible states of the PHDR task
enum
{
    ///Idle state
    PHDR_IDLE,

    ///Number of defined states.
    PHDR_STATE_MAX
};

/// Messages for Device Information Service Server
enum
{
    
    LED_BLINK = KE_FIRST_MSG(TASK_PHDR),
		COUNT_OVER,

	  VIBRATION,
	
	  VIBRATION_OVER,
	
    /// Inform the application that the profile service role task has been disabled after a disconnection
    PHDR_DISABLE_IND,

    ///Error indication to Host
    PHDR_ERROR_IND,
};

/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */
/// Parameters of the @ref LED_BLINK message - shall be dynamically allocated
/// 这里的blink_times代表闪灯次数
/// period是周期间隔
/// start_flag是标志位，1代表开始blink，0继续，2代表结束该次blink
struct phdr_set_blink_req
{
    /// blink times
    uint8_t blink_times;
    /// period
    uint8_t period;
		/// start_flag
		uint8_t start_flag;
};


/// Parameters of the @ref VIBRATION message
struct phdr_vibrate
{
    ///Connection handle
    uint16_t conhdl;
};


/// Parameters of the @ref PHDR_DISABLE_IND message
struct phdr_disable_ind
{
    ///Connection handle
    uint16_t conhdl;
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler phdr_state_handler[PHDR_STATE_MAX];
extern const struct ke_state_handler phdr_default_handler;
extern ke_state_t phdr_state[PHDR_IDX_MAX];



/// @} PHDRTASK
#endif // PHDR_TASK_H_
