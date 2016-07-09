/**
 ****************************************************************************************
 *
 * @file phdr.h
 *
 * @brief Header file - peripheral driver..
 *
 * Copyright (C) AIRBAND 2014-2015
 *
 *
 ****************************************************************************************
 */

#ifndef PHDR_H_
#define PHDR_H_

/**
 ****************************************************************************************
 * @addtogroup PHDR peripheral driver
 * @ingroup PHD
 * @brief peripheral driver
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "atts.h"
#include <stdint.h>
#include "prf_types.h"
#include "stdlib.h"
#include "storage_operate.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Pointer to the connection clean-up function
#define PHDR_CLEANUP_FNCT        (NULL)
#define BLINKING                 1
#define BLINK_OVER               0




struct id_info
{
    uint8_t info[INFO_LENGTH];
};


///peripheral driver Environment Variable
struct phdr_env_tag
{
    ///led blink state
		uint8_t blink_state;
		///serial number information
		uint8_t serial_number[SERIAL_LENGTH];
	  ///cash storage
		uint8_t cash_remain[CASH_LENGTH];
	  ///password storage
		uint8_t password_number[PASSWORD_LENGTH];
    ///social mode
		struct  id_info person_info;
		uint8_t person_info_length;
		
		uint8_t pages_num;
    struct  id_info others_info[MAXPAGE];
};


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern struct phdr_env_tag phdr_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the PHDR module.
 * This function performs all the initializations of the PHDR module.
 ****************************************************************************************
 */
void phdr_init(void);




/// @} PHDR

#endif // PHDR_H_
