/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals setup and initialization. 
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
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "periph_setup.h"            // peripheral configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization

//debug
#include "ke_timer.h"
#include "phdr_task.h"
#include "nfc_task.h"

/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)
    
i.e.  
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/
#ifdef CFG_PRINTF_UART2
	RESERVE_GPIO( UART2_TX, GPIO_PORT_1,  GPIO_PIN_2, PID_UART2_TX);
	RESERVE_GPIO( UART2_RX, GPIO_PORT_1,  GPIO_PIN_3, PID_UART2_RX);
#endif
	RESERVE_GPIO( UART_TX, GPIO_PORT_2,  GPIO_PIN_8, PID_UART2_TX);
	RESERVE_GPIO( UART_RX, GPIO_PORT_2,  GPIO_PIN_9, PID_UART2_RX);
	RESERVE_GPIO( NFC_RST, GPIO_PORT_0, GPIO_PIN_7, PID_GPIO);
	RESERVE_GPIO( KEY1, GPIO_PORT_2, GPIO_PIN_3, PID_GPIO);
	RESERVE_GPIO( VB, GPIO_PORT_0, GPIO_PIN_4, PID_GPIO);
}
#endif //DEVELOPMENT_DEBUG

/**
 ****************************************************************************************
 * @brief Map port pins
 *
 * The Uart and SPI port pins and GPIO ports are mapped
 ****************************************************************************************
 */
void set_pad_functions(void)        // set gpio port function mode
{

#ifdef CFG_PRINTF_UART2
	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_2, OUTPUT, PID_UART2_TX, false );
	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_3, INPUT, PID_UART2_RX, false );
#endif
/*
* Configure application ports.
i.e.    
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false ); // Set P_01 as Generic purpose Output
*/
	GPIO_ConfigurePin( GPIO_PORT_2, GPIO_PIN_8, OUTPUT, PID_UART2_TX, false );
	GPIO_ConfigurePin( GPIO_PORT_2, GPIO_PIN_9, INPUT, PID_UART2_RX, false );
	GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_7, OUTPUT, PID_GPIO, false );
	GPIO_ConfigurePin( GPIO_PORT_2, GPIO_PIN_3, INPUT, PID_GPIO, false );
	GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_4, OUTPUT, PID_GPIO, false );
}


/**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain is down. The Uart and SPi clocks are set.
 *
 * @return void
 ****************************************************************************************
 */

void key_one_interrupt_handler(void){	
		/*struct phdr_set_blink_req *req=KE_MSG_ALLOC(LED_BLINK,
                                                  TASK_PHDR, TASK_NONE,
                                                  phdr_set_blink_req);
		req->blink_times=50;
		req->period=20;
		req->start_flag=START_BLINK;
		
		ke_msg_send(req);
		ke_msg_send_basic(VIBRATION, KE_BUILD_ID(TASK_PHDR, 0), KE_BUILD_ID(TASK_PHDR, 0));*/
		ke_msg_send_basic(NFC_START_READER, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
		//ke_timer_set(NFC_SOCIAL_INIT_AS_TARGET, TASK_NFC, 10);
		return;
}
void periph_init(void) 
{
	// Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP)) ; 
    
#if ES4_CODE
    SetBits16(CLK_16M_REG,XTAL16_BIAS_SH_DISABLE, 1);
#endif    
	
	//rom patch
	patch_func();
	
	//Init pads
	set_pad_functions();

#if (BLE_APP_PRESENT)
/*
* (Re)Initialize peripherals
i.e.    
    uart_init(UART_BAUDRATE_115K2, 3);
*/        
#endif

#ifdef CFG_PRINTF_UART2
		SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
		uart2_init(UART_BAUDRATE_115K2, 3);
#endif

		//enable the clk of uart2 and init uart2 as 115k2, 1stop, 8bit, no parity
		SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
		uart2_init(UART_BAUDRATE_115K2, 3);
    
   // Enable the pads
	SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
	
	GPIO_EnableIRQ(GPIO_PORT_2, GPIO_PIN_3,GPIO0_IRQn,false,true ,10);
	GPIO_RegisterCallback(GPIO0_IRQn,key_one_interrupt_handler);
}
