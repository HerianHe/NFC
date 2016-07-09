// nfc.h
#ifndef _NFC_H_
#define _NFC_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration
#include "ke_msg.h"

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>           // Common BT Definitions

#include "arch.h"            // Platform Definitions
#include "uart.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// Application environment structure
struct nfc_env_tag
{
	int no_use;
};

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

/// Application environment
extern struct nfc_env_tag nfc_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void nfc_init(void);

#endif
