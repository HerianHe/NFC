/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"               // SW configuration
#include <string.h> 

#include "nfc.h"
#include "nfc_task.h"

#include "arch.h"                      // Platform Definitions
#include <gpio.h>
#include "ke_timer.h"
#include "ke_task.h"
#include "uart.h"
#include "co_math.h"
#include "gpio.h"

#include "phdr.h"
#include "storage_operate.h"
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void uart_flush_rx_fifo(){
    SetWord16(UART2_IIR_FCR_REG,3); 
}
void uart_flush_tx_fifo(){
    SetWord16(UART2_IIR_FCR_REG,5); 
}
void uart_flush_rx_tx_fifo(){
    SetWord16(UART2_IIR_FCR_REG,7); 
}

uint8_t rec_buf[110] = {0};
uint8_t snd_buf[108] = {0};
uint8_t time_5_min = 0;

//social handlers --------------------------------------------------------------------------------
int nfc_start_social_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//reset the 15 min timer
	time_5_min = 0;
	//if social is on then close social mode
	if(nfc_state[0] == NFC_SOCIAL){
		ke_msg_send_basic(NFC_SOCIAL_END, TASK_NFC, TASK_NFC);
		return 0;
	}
	//set the state of nfc to social
	ke_state_set(TASK_NFC, NFC_SOCIAL);
	//init uart
	uart_init(UART_BAUDRATE_115K2, 0x03);//115k2, 8bit, no parity, 1 stop bit
	//PULL UP RSTPDN -- START PN532
	GPIO_SetActive(GPIO_PORT_0, GPIO_PIN_7);
	//read the serial num and generate random number
	uint8_t serial_num[3] = {0};
	getSerialNumber(serial_num);
	co_random_init(0);
	
	//start timer for the ending of the social phase
	ke_timer_set(NFC_SOCIAL_15_MIN_TIMER, TASK_NFC, 30000); //5 mins
													 
	//init social
	ke_msg_send_basic(NFC_SOCIAL_INIT, TASK_NFC, TASK_NFC);
	return 0;
}

int nfc_social_15_min_timer_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	time_5_min++;
	if(time_5_min == 3){
		time_5_min = 0;
		ke_msg_send_basic(NFC_SOCIAL_END, TASK_NFC, TASK_NFC);
	}else{
		ke_timer_set(NFC_SOCIAL_15_MIN_TIMER, TASK_NFC, 30000); //5 mins
	}
	return 0;
}

int nfc_social_init_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//wakeup&SAMconfiguration
	uint8_t buf[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
	memcpy(snd_buf, buf, 24);
	uart2_write(snd_buf, 24, NULL);
	uart2_finish_transfers();
	
	//generate random number
	uint8_t num = co_rand_byte()%2;
	
	//init as initiator/target
	//set a timer to wait for sam to send before init as in/tg
	if(num == 1){	//initiator
		ke_timer_set(NFC_SOCIAL_INIT_AS_INITIATOR, TASK_NFC, 5);
	}else{	//target
		ke_timer_set(NFC_SOCIAL_INIT_AS_TARGET, TASK_NFC, 5);
	}
	return 0;
}

//initiator
void nfc_social_init_as_initial_rec_ack_callback(uint8_t UART_STATUS);
int nfc_social_init_as_initiator_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){	
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	//send InJumpForDEP
	uint8_t InJumpForDEP_Buf[12] = {0x00, 0x00, 0xFF, 0x05, 0xFB, 0xD4, 0x56, 0x01, 0x00, 0x00, 0xD5, 0x00};
	memcpy(snd_buf, InJumpForDEP_Buf, 12);
	uart2_write(InJumpForDEP_Buf, 12, NULL);
	uart2_finish_transfers();
	//receive ACK
	uart2_read(rec_buf, 6, nfc_social_init_as_initial_rec_ack_callback);
	return 0;
}
void nfc_social_init_as_initial_rec_ack_callback(uint8_t UART_STATUS){
	uart2_read(rec_buf, 26, NULL);
	ke_timer_set(NFC_SOCIAL_CHK_INIT_AS_INITIATOR, TASK_NFC, 100); //1s
}

int nfc_social_chk_init_as_initiator_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	if((rec_buf[5] == 0xD5) && (rec_buf[6] == 0x57) && (rec_buf[7] == 0x00)){
		//init succeed
		ke_msg_send_basic(NFC_SOCIAL_EXCHANGE_DATA_INITIATOR, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}else{
		//init failed
		ke_msg_send_basic(NFC_SOCIAL_INIT, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}
	return 0;
}

void nfc_social_exchange_data_initiator_rec_ack_callback(uint8_t UART_STATUS);
void nfc_social_exchange_data_initiator_rec_info_callback(uint8_t UART_STATUS);
int nfc_social_exchange_data_initiator_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	//read local personal info
	uint8_t PData[100] = {0};
	Tinfo_all_read(PData);
	//build send frame
	uint8_t DCS = 0;
	snd_buf[0] = 0x00;
	snd_buf[1] = 0xFF;
	snd_buf[2] = 0x67;
	snd_buf[3] = 0x99;
	snd_buf[4] = 0xD4;
	DCS = DCS - snd_buf[4];
	snd_buf[5] = 0x40;
	DCS = DCS - snd_buf[5];
	snd_buf[6] = 0x01;
	DCS = DCS - snd_buf[6];
	uint8_t i;
	for(i = 0;i < 100;i++){
		snd_buf[7+i] = PData[i];
		DCS = DCS - PData[i];
	}
	snd_buf[107] = DCS;
	//send the frame
	uart2_write(snd_buf, 108, NULL);
	uart2_finish_transfers();
	//receive ack
	uart2_read(rec_buf, 6, nfc_social_exchange_data_initiator_rec_ack_callback);
	return 0;
}
void nfc_social_exchange_data_initiator_rec_ack_callback(uint8_t UART_STATUS){
	uart2_read(rec_buf, 110, nfc_social_exchange_data_initiator_rec_info_callback);
}
void nfc_social_exchange_data_initiator_rec_info_callback(uint8_t UART_STATUS){
	//transfer succeed, store the data
	Recinfo_Compare_Save(&rec_buf[8]);
}

//target
void nfc_social_init_as_target_rec_ack_callback(uint8_t UART_STATUS);
int nfc_social_init_as_target_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	//send TgInitAsTarget
	uint8_t TgInitAsTarget_Buf[46] = {0x00, 0x00, 0xFF, 0x27, 0xD9, 0xD4, 0x8C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5E, 0x00};
	memcpy(snd_buf, TgInitAsTarget_Buf, 46);
	uart2_write(TgInitAsTarget_Buf, 46, NULL);
	uart2_finish_transfers();
	//receive ACK
	uart2_read(rec_buf, 6, nfc_social_init_as_target_rec_ack_callback);
	return 0;
}
void nfc_social_init_as_target_rec_ack_callback(uint8_t UART_STATUS){
	//determine the receive length
	uart2_read(rec_buf, 27, NULL);
	ke_timer_set(NFC_SOCIAL_CHK_INIT_AS_TARGET, TASK_NFC, 100); //1s
}

int nfc_social_chk_init_as_target_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	if((rec_buf[5] == 0xD5) && (rec_buf[6] == 0x8D)){
		//init succeed
		ke_msg_send_basic(NFC_SOCIAL_GET_DATA_TARGET, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}else{
		//init failed, restart initialization
		ke_msg_send_basic(NFC_SOCIAL_INIT, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}
	return 0;
}

void nfc_social_get_data_target_rec_ack_callback(uint8_t UART_STATUS);
void nfc_social_get_data_target_rec_info_callback(uint8_t UART_STATUS);
int nfc_social_get_data_target_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	//send TgInitAsTarget
	uint8_t TgGetData_Buf[9] = {0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD4, 0x86, 0xA6, 0x00};
	memcpy(snd_buf, TgGetData_Buf, 9);
	uart2_write(TgGetData_Buf, 9, NULL);
	uart2_finish_transfers();
	//receive ACK
	uart2_read(rec_buf, 6, nfc_social_get_data_target_rec_ack_callback);
	return 0;
}
void nfc_social_get_data_target_rec_ack_callback(uint8_t UART_STATUS){
	//receive info frame
	uart2_read(rec_buf, 110, nfc_social_get_data_target_rec_info_callback);
}
void nfc_social_get_data_target_rec_info_callback(uint8_t UART_STATUS){
	//get succeed, store the data
	Recinfo_Compare_Save(&rec_buf[8]);
	//send data
	ke_msg_send_basic(NFC_SOCIAL_SET_DATA_TARGET, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
}

void nfc_social_set_data_target_rec_ack_callback(uint8_t UART_STATUS);
void nfc_social_set_data_target_rec_info_callback(uint8_t UART_STATUS);
int nfc_social_set_data_target_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	//read local personal info
	uint8_t PData[100] = {0};
	Tinfo_all_read(PData);
	//build send frame
	uint8_t DCS = 0;
	snd_buf[0] = 0x00;
	snd_buf[1] = 0xFF;
	snd_buf[2] = 0x66;
	snd_buf[3] = 0x9A;
	snd_buf[4] = 0xD4;
	DCS = DCS - snd_buf[4];
	snd_buf[5] = 0x8E;
	DCS = DCS - snd_buf[5];
	uint8_t i;
	for(i = 0;i < 100;i++){
		snd_buf[6+i] = PData[i];
		DCS = DCS - PData[i];
	}
	snd_buf[106] = DCS;
	//send the frame
	uart2_write(snd_buf, 107, NULL);
	uart2_finish_transfers();
	//receive ack
	uart2_read(rec_buf, 6, nfc_social_set_data_target_rec_ack_callback);
	return 0;
}
void nfc_social_set_data_target_rec_ack_callback(uint8_t UART_STATUS){
	uart2_read(rec_buf, 10, nfc_social_set_data_target_rec_info_callback);	
}
void nfc_social_set_data_target_rec_info_callback(uint8_t UART_STATUS){
	if(rec_buf[5] == 0xD5 && rec_buf[6] == 0x8F && rec_buf[7] == 0x00){
		//set data succeed
	}
}

int nfc_social_end_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//set the state of nfc to disabled
	ke_state_set(TASK_NFC, NFC_DISABLED);
	//pull down RSTPDN
	GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_7);
	return 0;
}

//reader handlers --------------------------------------------------------------------------------
int nfc_start_reader_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//check the pn532 state
	if(nfc_state[0] == NFC_SOCIAL || nfc_state[0] == NFC_READER)
		return 0;
	//set the state of nfc to READER
	ke_state_set(TASK_NFC, NFC_READER);
	//PULL UP RSTPDN -- START PN532
	GPIO_SetActive(GPIO_PORT_0, GPIO_PIN_7);
	//init card
	//set a timer waiting for samconfiguration
	ke_timer_set(NFC_READER_INIT_CARD, TASK_NFC, 3);
	
	//wakeup&SAMconfiguration
	uint8_t buf[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
	memcpy(snd_buf, buf, 24);
	uart2_write(buf, 24, NULL);
	uart2_finish_transfers();
	return 0;
}

void nfc_reader_in_list_passive_target_rec_ack_callback(uint8_t UART_STATUS);
void nfc_reader_in_list_passive_target_rec_info_callback(uint8_t UART_STATUS);
int nfc_reader_init_card_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	uint8_t InListPassiveTarget[11] = {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};
	uart2_write(InListPassiveTarget, 11, NULL);
	uart2_finish_transfers();
	//receive ack
	uart2_read(rec_buf, 6, nfc_reader_in_list_passive_target_rec_ack_callback);
	return 0;
}
void nfc_reader_in_list_passive_target_rec_ack_callback(uint8_t UART_STATUS){
	//receive info
	uart2_read(rec_buf, 26, nfc_reader_in_list_passive_target_rec_info_callback);
}
void nfc_reader_in_list_passive_target_rec_info_callback(uint8_t UART_STATUS){
	if((rec_buf[9] == 0x00) && (rec_buf[10] == 0x08)){
		ke_msg_send_basic(NFC_READER_SELECT_APP, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}else{
		ke_msg_send_basic(NFC_READER_END, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
	}
}

void nfc_reader_select_app_rec_ack_callback(uint8_t UART_STATUS);
void nfc_reader_select_app_rec_info_callback(uint8_t UART_STATUS);
int nfc_reader_select_app_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	uint8_t SelApp[18] = {0x00, 0x00, 0xFF, 0x0C, 0xF4, 0xD4, 0x40, 0x01, 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x01, 0x00, 0x34, 0x00};
	uart2_write(SelApp, 18, NULL);
	uart2_finish_transfers();
	//receive ack
	uart2_read(rec_buf, 6, nfc_reader_select_app_rec_ack_callback);
	return 0;
}
void nfc_reader_select_app_rec_ack_callback(uint8_t UART_STATUS){
	//receive info
	uart2_read(rec_buf, 20, nfc_reader_select_app_rec_info_callback);
}
void nfc_reader_select_app_rec_info_callback(uint8_t UART_STATUS){
	//receive info
	ke_msg_send_basic(NFC_READER_GET_BALANCE, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
}

void nfc_reader_get_balance_rec_ack_callback(uint8_t UART_STATUS);
void nfc_reader_get_balance_rec_info_callback(uint8_t UART_STATUS);
int nfc_reader_get_balance_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//flush fifos, prepare for transfer
	uart_flush_rx_tx_fifo();
	uint8_t GetBalance[15] = {0x00, 0x00, 0xFF, 0x08, 0xF8, 0xD4, 0x40, 0x01, 0x80, 0x5C, 0x00, 0x02, 0x04, 0x09, 0x00};
	uart2_write(GetBalance, 15, NULL);
	uart2_finish_transfers();
	//receive ack
	uart2_read(rec_buf, 6, nfc_reader_get_balance_rec_ack_callback);
	return 0;
}
void nfc_reader_get_balance_rec_ack_callback(uint8_t UART_STATUS){
	//receive balance
	uart2_read(rec_buf, 16, nfc_reader_get_balance_rec_info_callback);
}
void nfc_reader_get_balance_rec_info_callback(uint8_t UART_STATUS){
	//receive balance info frame -- store data
	save_cash(&rec_buf[8]);
	//end of reader process
	ke_msg_send_basic(NFC_READER_END, KE_BUILD_ID(TASK_NFC, 0), KE_BUILD_ID(TASK_NFC, 0));
}

int nfc_reader_end_handler(ke_msg_id_t const msgid,
									void const *param,
									ke_task_id_t const dest_id,
									ke_task_id_t const src_id){
	//set the state of nfc to DISABLED
	ke_state_set(TASK_NFC, NFC_DISABLED);
	//pull down RSTPDN
	GPIO_SetInactive(GPIO_PORT_0, GPIO_PIN_7);
	return 0;
}

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

/* Specifies the message handlers that are common to all states. */
const struct ke_msg_handler nfc_default_state[] = {
	//SOCIAL
	{NFC_START_SOCIAL,	                    (ke_msg_func_t)nfc_start_social_handler},
	//READER
	{NFC_START_READER,	                    (ke_msg_func_t)nfc_start_reader_handler},
};
const struct ke_msg_handler nfc_social[] = {
	//SOCIAL
	{NFC_SOCIAL_INIT,	                      (ke_msg_func_t)nfc_social_init_handler},
	{NFC_SOCIAL_15_MIN_TIMER,	              (ke_msg_func_t)nfc_social_15_min_timer_handler},
	{NFC_SOCIAL_INIT_AS_INITIATOR,	        (ke_msg_func_t)nfc_social_init_as_initiator_handler},
	{NFC_SOCIAL_CHK_INIT_AS_INITIATOR,	    (ke_msg_func_t)nfc_social_chk_init_as_initiator_handler},
	{NFC_SOCIAL_EXCHANGE_DATA_INITIATOR,	  (ke_msg_func_t)nfc_social_exchange_data_initiator_handler},
	{NFC_SOCIAL_INIT_AS_TARGET,	            (ke_msg_func_t)nfc_social_init_as_target_handler},
	{NFC_SOCIAL_CHK_INIT_AS_TARGET,	        (ke_msg_func_t)nfc_social_chk_init_as_target_handler},
	{NFC_SOCIAL_GET_DATA_TARGET,	          (ke_msg_func_t)nfc_social_get_data_target_handler},
	{NFC_SOCIAL_SET_DATA_TARGET,	          (ke_msg_func_t)nfc_social_set_data_target_handler},
	{NFC_SOCIAL_END,	                      (ke_msg_func_t)nfc_social_end_handler},
};
const struct ke_msg_handler nfc_reader[] = {
	//READER
	{NFC_READER_INIT_CARD,                  (ke_msg_func_t)nfc_reader_init_card_handler},
	{NFC_READER_SELECT_APP,                 (ke_msg_func_t)nfc_reader_select_app_handler},
	{NFC_READER_GET_BALANCE,                (ke_msg_func_t)nfc_reader_get_balance_handler},
	{NFC_READER_END,	                      (ke_msg_func_t)nfc_reader_end_handler},
};
///Specifies the message handler structure for every input state.
const struct ke_state_handler nfc_state_handler[NFC_STATE_MAX] =
{
	[NFC_DISABLED]   = KE_STATE_HANDLER_NONE,
	[NFC_SOCIAL]   = KE_STATE_HANDLER(nfc_social),
	[NFC_READER]   = KE_STATE_HANDLER(nfc_reader),
};

const struct ke_state_handler nfc_default_handler = KE_STATE_HANDLER(nfc_default_state);

/* Defines the place holder for the states of all the task instances. */
ke_state_t nfc_state[NFC_IDX_MAX] __attribute__((section("retention_mem_area0"), zero_init)); //RETENTION MEMORY 
