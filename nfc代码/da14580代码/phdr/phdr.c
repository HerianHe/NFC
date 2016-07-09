/**
 ****************************************************************************************
 *
 * @file phdr.c
 *
 * @brief peripheral driver Implementation.
 *
 * Copyright (C) AIRBAND 2014-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PHDR
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#include "attm_util.h"
#include "atts_util.h"
#include "phdr.h"
#include "phdr_task.h"
#include "prf_utils.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * DIS ATTRIBUTES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct phdr_env_tag phdr_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY


/// Device Information Service task descriptor
static const struct ke_task_desc TASK_DESC_PHDR = {phdr_state_handler, &phdr_default_handler, phdr_state, PHDR_STATE_MAX, PHDR_IDX_MAX};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void phdr_init(void)
{
		// Reset environment
    memset(&phdr_env, 0, sizeof(phdr_env));
	
    // Create PHDR task
    ke_task_create(TASK_PHDR, &TASK_DESC_PHDR);

    // Set task in idle state
    ke_state_set(TASK_PHDR, PHDR_IDLE);
		
		// Set env blink state
		phdr_env.blink_state=BLINK_OVER;
	
	flash_info_init();
	
}





/// @} PHDR
