#ifndef _NFC_TASK_H_
#define _NFC_TASK_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#include "ke_task.h"        // Kernel Task
#include "ke_msg.h"         // Kernel Message

#include <stdint.h>         // Standard Integer
#include "uart.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Number of NFC Task Instances
#define NFC_IDX_MAX                 (1)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// States of NFC task
enum NFC_STATE
{
    /// Disabled State
    NFC_DISABLED,
	
		NFC_SOCIAL,
		NFC_READER,

    /// Number of defined states.
    NFC_STATE_MAX
};

/// Messages for NFC task
enum {
	//default messages
	NFC_START_SOCIAL = KE_FIRST_MSG(TASK_NFC),
	NFC_START_READER,
	//social messages
	NFC_SOCIAL_INIT,
	NFC_SOCIAL_15_MIN_TIMER,
	NFC_SOCIAL_INIT_AS_INITIATOR, 
	NFC_SOCIAL_CHK_INIT_AS_INITIATOR,
	NFC_SOCIAL_EXCHANGE_DATA_INITIATOR,
	NFC_SOCIAL_INIT_AS_TARGET,
	NFC_SOCIAL_CHK_INIT_AS_TARGET,
	NFC_SOCIAL_GET_DATA_TARGET,
	NFC_SOCIAL_SET_DATA_TARGET,
	NFC_SOCIAL_END,
	//reader messages
	NFC_READER_INIT_CARD,
	NFC_READER_SELECT_APP,
	NFC_READER_GET_BALANCE,
	NFC_READER_END,
};
/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler nfc_default_handler;
extern const struct ke_state_handler nfc_state_handler[NFC_STATE_MAX];
extern ke_state_t nfc_state[NFC_IDX_MAX];



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

 
#endif
