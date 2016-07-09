/*********************************************************************
* INCLUDES
*/
#include "prf_types.h"
#include "stdlib.h"
#include "atts.h"
#include "phdr_task.h"
#include "phdr.h"

/*********************************************************************
* CONSTANT
*/
//#define SERIAL_ADDRESS		0x80
//#define SERIAL_LENGTH		3
//
//#define CASH_ADDRESS		0x81
//#define CASH_LENGTH			4
//
//
//#define PASSWORD_ADDRESS	0x90  
//
//#define INFO_LENGTH			250
//
//#define TINFO_ADDRESS		0x91
//#define TINFO_LEN_ADDRESS	0x92
//
//#define MAXPAGES			5 
//
//#define R_PAGES_ADDRESS		0x93
//
//#define ID_LENGTH			16
//
//#define RINFO_ADDRESS1		0x94
//#define RINFO_ADDRESS2		0x95
//#define RINFO_ADDRESS3		0x96
//#define RINFO_ADDRESS4		0x97
//#define RINFO_ADDRESS5		0x98
//
//
//
//#define EXIST		1
//#define NOEXIST		0
//
//
//
//#define INFO_PAGES_ADDR    0x91
//#define INFO_PAGES_LEN    1

#define SERIAL_LENGTH            3
#define SERIAL_NUMBER0			     0
#define SERIAL_NUMBER1           0
#define SERIAL_NUMBER2           10

#define CASH_LENGTH			4

#define PASSWORD_LENGTH            8

#define INFO_LENGTH              100
#define MAXPAGE             5
#define ID_LENGTH           10
#define EXIST		1
#define NOEXIST		0
/*********************************************************************
* FUNCTIONS
*/
//���к�**********************************************************************
/**************************************
* uint8_t serialNumberInit(void);
* ���кŵĳ�ʼ��
**************************************/
uint8_t serialNumberInit(void);


/**************************************
* uint8_t flash_getSerialNumber(void *pBuf);
* ��ȡ������3λ���кţ���ֵ�������ַpBuf
**************************************/
extern uint8_t getSerialNumber(void *pBuf);
//�������к�******************************************************************




//���************************************************************************
/**************************************
* uint8_t cashInit(void);
* ���ĳ�ʼ��
**************************************/
uint8_t cashInit(void);

/**************************************
* uint8_t save_cash(void *pBuf);
* �洢���
**************************************/
extern uint8_t save_cash(void *pBuf);

/**************************************
* uint8_t get_cash(void *pBuf);
* ��ȡ���ŵ�һ��pBuf����������
**************************************/
extern uint8_t get_cash(void *pBuf);
//�������********************************************************************





//����************************************************************************
/*************************************
* uint8_t pwd_init( void )
* ��flash�ڲ�����ĳ�ʼ������
* ���ڶ�Ӧλ������Ӧ�ĳ�ʼ���룬8��0
**************************************/
extern uint8_t pwd_init(void);

/*************************************
* uint8_t pwd_write(void *pBuf)
* ��flash�ڲ�д������
* ����pBufΪһ��8λuint8_t������ĵ�ַ
**************************************/
extern uint8_t pwd_write(void *pBuf);

/**************************************
* uint8_t pwd_read(void *pBuf)
* ��ȡflash�ڲ����������
* ����pBufΪһ��8λuint8_t����ĵ�ַ
**************************************/
extern uint8_t pwd_read(void *pBuf);

/**************************************
* uint8_t pwd_delete(void *pBuf)
* ���flash�ڲ����������
**************************************/
extern uint8_t pwd_delete(void);
//��������*******************************************************************





//TINFO************************************************************************
/**************************************
* uint8_t Tinfo_init( void )
* ��flash�ڲ���ʼ���������ݵĴ洢
**************************************/
extern uint8_t Tinfo_init(void);

/**************************************
* uint8_t Tinfo_all_read(void *pBuf)
* ��flash�ڲ��������ݵ�ȫ����ȡ
* ������һ������INFO_LENGTH�������ַ
* ����ֵ��osal flash������ֵ������μ�API�ĵ�
* һ��ʹ�óɹ���SUCCESS
**************************************/
extern uint8_t Tinfo_all_read(void *pBuf);

/**************************************
* uint8_t Tinfo_short_write(void *pBuf, uint8_t len)
* ��flash�ڲ�������������Ľ������ݵĳ���Ϊs
* ����s����д�볤��Ϊlen�����飬��ַ��pBuf
* �������洢���ȵ����ݲ�д
**************************************/
extern uint8_t Tinfo_short_write(void *pBuf, uint8_t len);

/**************************************
* uint8_t Tinfo_single_read( uint8_t seq )
* ��ȡflash�ڲ��������ݵĴ洢����ĵ�seq��λ���������
* ��seqδ�����洢����ĳ��ȣ����ض�Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
extern uint8_t Tinfo_single_read(uint8_t seq);

/**************************************
* uint8_t flash_Tinfo_single_write(uint8_t seq, uint8_t value)
* ��flash�ڲ��������ݵĴ洢����ĵ�seqλ��������value
* ��seqδ�����洢����ĳ��ȣ����ض�д�ɹ���Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
extern uint8_t Tinfo_single_write(uint8_t seq, uint8_t value);
//����TINFOLEN*******************************************************************




//TINFOLEN*******************************************************************
/**************************************
* uint8_t flash_Tinfo_Length_init(void��
* ��flash�ڲ���ʼ���������ݵĳ��ȴ洢λ
**************************************/
extern uint8_t Tinfo_Length_init(void);

/**************************************
* uint8_t flash_Tinfo_Length_set(uint8_t length)
* ��flash�ڲ�д�뷢�����ݵĳ��ȴ洢λ
**************************************/
extern uint8_t Tinfo_Length_set(uint8_t length);

/**************************************
* uint8_t flash_Tinfo_Length_get(void)
* ��flash�ڲ���ȡ�������ݵĳ��ȴ洢λ
**************************************/
extern uint8_t Tinfo_Length_get(void);
//����TINFO*******************************************************************

//RINFO*******************************************************************
/**************************************
* uint8_t RinfoPageAddress(uint8_t num)
* ��ȡ�յ��Ĵ洢��Ϣ��seqҳ�ĵ�ַ
**************************************/
extern uint8_t RinfoPageAddress(uint8_t num);

/**************************************
* uint8_t Rinfo_init(uint8_t Addr��
* ��flash�ڲ���ʼ���������ݴ洢
**************************************/
uint8_t Rinfo_init(uint8_t Addr);

/**************************************
* uint8_t Rinfo_ALLinit(void)
* ��flash�ڲ���ʼ�����н������ݴ洢
**************************************/
extern uint8_t Rinfo_ALLinit(void);



////////NFC*******************************************************************
/**************************************
* uint8_t Rinfo_all_write(void *pBuf, uint8_t addr)
* ��flash�ڲ��������ݵ�ȫ��д��addλ��
* ������һ������INFO_LENGTH�������ַ
* ����ֵ��osal flash������ֵ������μ�API�ĵ�
* һ��ʹ�óɹ���SUCCESS
**************************************/
extern uint8_t Rinfo_all_write(void *pBuf, uint8_t addr);

/**************************************
* uint8_t Recinfo_getID(void *pBuf, uint8_t addr)
* ��ȡ�յ��Ĵ洢��Ϣaddr�����ֵ�IDֵ
**************************************/
uint8_t Recinfo_getID(void *pBuf, uint8_t addr);

/**************************************
* uint8_t Recinfo_Compare_Save(void *pBuf)
* ������5ҳflash������д�����ݣ��������ID���л��ߴ洢����
* �򲻴洢��һ�������IDȫΪ0�����������Դ洢
**************************************/
extern uint8_t Recinfo_Compare_Save(void *pBuf);
////////NFC*******************************************************************


////////APP*******************************************************************
/**************************************
* uint8_t Rinfo_short_read(void *pBuf, uint8_t seq, uint8_t pages)
* ��flash�ڲ�������������ĵ�pageҳ�ĵ�seq����ʼΪ��0λ��
* ��󣨰���seq����ȡ����9������
* �������洢���ȣ������������
* ��ֵ��pBuf��
**************************************/
extern uint8_t Rinfo_short_read(void *pBuf, uint8_t seq, uint8_t pages);
////////APP*******************************************************************


////////BLE*******************************************************************
/**************************************
* uint8_t Rinfo_single_read( uint8_t Addr, uint8_t index )
* ��ȡflash�ڲ��������ݵĴ洢����ĵ�Addr��indexλ���������
* ��seqδ�����洢����ĳ��ȣ����ض�Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
extern uint8_t Rinfo_single_read(uint8_t Addr, uint8_t index);

/**************************************
* uint8_t Rinfo_single_write(uint8_t Addr,uint8_t index, uint8_t value)
* ��flash�ڲ��������ݵĴ洢����ĵ�Addr���indexλ��������value
* ��seqδ�����洢����ĳ��ȣ����ض�д�ɹ���Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
extern uint8_t Rinfo_single_write(uint8_t Addr, uint8_t index, uint8_t value);
////////BLE*******************************************************************
//����RINFO*******************************************************************



//RPAGES****��ֵ�����ڴ�ŵ�ǰflash���ж�������********************************
/**************************************
* uint8_t Rinfo_pages_init(void)
* ��flash�ڲ���ʼ���������ݵ�ҳ���洢λ
**************************************/
uint8_t Rinfo_pages_init(void);

/**************************************
* uint8_t Rinfo_add_pages(void)
* ��flash�����ӷ������ݵ�ҳ��һҳ
**************************************/
extern uint8_t Rinfo_add_pages(void);

/**************************************
* uint8_t Rinfo_minus_pages(void)
* ��flash�м�ȥ�������ݵ�ҳ��һҳ
**************************************/
extern uint8_t Rinfo_minus_pages(void);

/**************************************
* uint8_t Rinfo_minus_pages(void)
* ��flash�л�ȡ�������ݵ�ҳ��
**************************************/
extern uint8_t Rinfo_get_pages(void);

/**************************************
* uint8_t Rinfo_get_pages(void)
* ��flash�л�ȡ�������ݵ�ҳ��
**************************************/
extern uint8_t Rinfo_get_pages(void);
//����RPAGES*******************************************************************

uint8_t cmpinfoid(void *pBuf1,void *pBuf2, uint8_t length);

/**************************************
* uint8_t flash_info_init( void )
* ��flash�ڲ���ʼ��������Ϣ
**************************************/
extern uint8_t flash_info_init(void);
