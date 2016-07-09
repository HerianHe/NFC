/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#include "nfc.h"
#include "nfc_task.h"

#include "arch.h"                      // Platform Definitions
#include "gpio.h"
#include "ke_timer.h"
#include "ke_task.h"
#include "uart.h"
/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Environment Structure
struct nfc_env_tag nfc_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Task Descriptor
static const struct ke_task_desc TASK_DESC_NFC = {nfc_state_handler, &nfc_default_handler,
                                                  nfc_state, NFC_STATE_MAX, NFC_IDX_MAX};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void nfc_init(void)
{   
	//Reset environment
	memset(&nfc_env, 0, sizeof(nfc_env));
	
	// Create APP task
	ke_task_create(TASK_NFC, &TASK_DESC_NFC);

	// Initialize Task state
	ke_state_set(TASK_NFC, NFC_DISABLED);
	
	//init uart&gpio
	
	//ke_msg_send_basic(NFC_START_SOCIAL, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
}
