/*********************************************************************
* INCLUDES
*/

#include "storage_operate.h"
#include "prf_types.h"
#include "stdlib.h"
#include "phdr_task.h"
#include "phdr.h"
/*********************************************************************
* FUNCTIONS
*/


//���к�**********************************************************************
/**************************************
* uint8_t serialNumberInit(void);
* ���кŵĳ�ʼ��
**************************************/
uint8_t serialNumberInit()
{
	phdr_env.serial_number[0]=SERIAL_NUMBER0;
	phdr_env.serial_number[1]=SERIAL_NUMBER1;
	phdr_env.serial_number[2]=SERIAL_NUMBER2;
	return 0;
}


/**************************************
* uint8_t flash_getSerialNumber(void *pBuf);
* ��ȡ������3λ���кţ���ֵ�������ַpBuf
**************************************/
uint8_t getSerialNumber(void *pBuf)
{
	((uint8_t *)pBuf)[0]=phdr_env.serial_number[0];
	((uint8_t *)pBuf)[1]=phdr_env.serial_number[1];
	((uint8_t *)pBuf)[2]=phdr_env.serial_number[2];
	return 0;
}
//�������к�******************************************************************






//���************************************************************************
/**************************************
* uint8_t cashInit(void);
* ���ĳ�ʼ��
**************************************/
uint8_t cashInit(void)
{
	phdr_env.cash_remain[0]=0;
	phdr_env.cash_remain[1]=0;
	phdr_env.cash_remain[2]=0;
	phdr_env.cash_remain[3]=0;
	return 0;
}

/**************************************
* uint8_t save_cash(void *pBuf);
* �洢���
**************************************/
uint8_t save_cash(void *pBuf)
{
	for (uint8_t i = 0; i<CASH_LENGTH; i++)
	{
		 phdr_env.cash_remain[i]=((uint8_t *)pBuf)[i];
	}
	return 0;
}

/**************************************
* uint8_t get_cash(void *pBuf);
* ��ȡ���ŵ�һ��pBuf����������
**************************************/
uint8_t get_cash(void *pBuf)
{
	for (uint8_t i = 0; i<CASH_LENGTH; i++)
	{
		((uint8_t *)pBuf)[i] = phdr_env.cash_remain[i];
	}
	return 0;
}
//�������********************************************************************




//����************************************************************************
/*************************************
* uint8_t pwd_init( void )
* ��flash�ڲ�����ĳ�ʼ������
* ���ڶ�Ӧλ������Ӧ�ĳ�ʼ���룬8��0
**************************************/
uint8_t pwd_init(void)
{
	for (uint8_t i = 0; i<PASSWORD_LENGTH; i++)
	{
		phdr_env.password_number[i]=0;
	}
	return 0;
}

/*************************************
* uint8_t pwd_write(void *pBuf)
* ��flash�ڲ�д������
* ����pBufΪһ��8λuint8_t������ĵ�ַ
**************************************/
uint8_t pwd_write(void *pBuf)
{
	for (uint8_t i = 0; i<PASSWORD_LENGTH; i++)
	{
		phdr_env.password_number[i]=((uint8_t *)pBuf)[i];
	}
	return 0;
}

/**************************************
* uint8_t pwd_read(void *pBuf)
* ��ȡflash�ڲ����������
* ����pBufΪһ��8λuint8_t����ĵ�ַ
**************************************/
uint8_t pwd_read(void *pBuf)
{
	for (uint8_t i = 0; i<PASSWORD_LENGTH; i++)
	{
		((uint8_t *)pBuf)[i] = phdr_env.password_number[i];
	}
	return 0;
}

/**************************************
* uint8_t pwd_delete(void *pBuf)
* ���flash�ڲ����������
**************************************/
uint8_t flash_pwd_delete(void)
{
	for (uint8_t i = 0; i<PASSWORD_LENGTH; i++)
	{
		phdr_env.password_number[i]=0;
	}
	return 0;
}
//��������*******************************************************************



//TINFO************************************************************************
/**************************************
* uint8_t  Tinfo_init( void )
* ��flash�ڲ���ʼ���������ݵĴ洢
**************************************/
uint8_t Tinfo_init(void)
{
	//memset(&phdr_env.person_info, 0, INFO_LENGTH);
	for (uint8_t i = 0; i<INFO_LENGTH; i++)
	{
		phdr_env.person_info.info[i]=0;
	}
	return 0;
}

/**************************************
* uint8_t Tinfo_all_read(void *pBuf)
* ��flash�ڲ��������ݵ�ȫ����ȡ
* ������һ������INFO_LENGTH�������ַ
* ����ֵ��osal flash������ֵ������μ�API�ĵ�
* һ��ʹ�óɹ���SUCCESS
**************************************/
uint8_t Tinfo_all_read(void *pBuf)
{
	for (uint8_t i = 0; i<INFO_LENGTH; i++)
	{
		((uint8_t *)pBuf)[i] = phdr_env.person_info.info[i];
	}
	return 0;
}

/**************************************
* uint8_t Tinfo_short_write(void *pBuf, uint8_t len)
* ��flash�ڲ�������������Ľ������ݵĳ���Ϊs
* ����s����д�볤��Ϊlen�����飬��ַ��pBuf
* �������洢���ȵ����ݲ�д
**************************************/
uint8_t Tinfo_short_write(void *pBuf, uint8_t len)
{
	  uint8_t length = 0;
	  length = Tinfo_Length_get();
	
		
		uint8_t i = 0;
		for (i = 0; i<len; i++)
		{
			if ((length + i)<INFO_LENGTH)
			{
				phdr_env.person_info.info[length + i] = ((uint8_t *)pBuf)[i];
			}
			else
			{
				break;
			}
		}
		length = length + len;
		if (length>INFO_LENGTH)
		{
			length = INFO_LENGTH;
		}

  	Tinfo_Length_set(length);
		return 0;
}

/**************************************
* uint8_t Tinfo_single_read( uint8_t seq )
* ��ȡflash�ڲ��������ݵĴ洢����ĵ�seq��λ���������
* ��seqδ�����洢����ĳ��ȣ����ض�Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
uint8_t Tinfo_single_read(uint8_t seq)
{
	if (seq<INFO_LENGTH)
	{
		return phdr_env.person_info.info[seq];
	}
	else{
		return 0xFF;
	}
}

/**************************************
* uint8_t flash_Tinfo_single_write(uint8_t seq, uint8_t value)
* ��flash�ڲ��������ݵĴ洢����ĵ�seqλ��������value
* ��seqδ�����洢����ĳ��ȣ����ض�д�ɹ���Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
uint8_t Tinfo_single_write(uint8_t seq, uint8_t value)
{
	if (seq<INFO_LENGTH)
	{
		phdr_env.person_info.info[seq] = value;
		return 0;
	}
	else
	{
		return 0xFF;
	}
}
//����TINFO*******************************************************************




//TINFOLEN*******************************************************************
/**************************************
* uint8_t Tinfo_Length_init(void��
* ��flash�ڲ���ʼ���������ݵĳ��ȴ洢λ
**************************************/
uint8_t Tinfo_Length_init(void)
{
	phdr_env.person_info_length=0;
	return 0;
}

/**************************************
* uint8_t Tinfo_Length_set(uint8_t length)
* ��flash�ڲ�д�뷢�����ݵĳ��ȴ洢λ
**************************************/
uint8_t Tinfo_Length_set(uint8_t length)
{
	phdr_env.person_info_length=length;
	return 0;
}

/**************************************
* uint8_t Tinfo_Length_get(void)
* ��flash�ڲ���ȡ�������ݵĳ��ȴ洢λ
**************************************/
uint8_t Tinfo_Length_get(void)
{
	return phdr_env.person_info_length;
}
//����TINFOLEN*******************************************************************



//RINFO*******************************************************************
/**************************************
* uint8_t flash_RinfoPageAddress(uint8_t num)
* ��ȡ�յ��Ĵ洢��Ϣ��numҳ�ĵ�ַ
**************************************/
uint8_t flash_RinfoPageAddress(uint8_t num)
{
	/*if ((num >= 0) && (num<MAXPAGES))
	{
		return RINFO_ADDRESS1 + num;
	}
	return RINFO_ADDRESS1;*/
	return 0;
}

/**************************************
* uint8_t Rinfo_init(uint8_t Addr)
* ��flash�ڲ���ʼ���������ݴ洢
**************************************/
uint8_t Rinfo_init(uint8_t Addr)
{
	if(Addr<5)
	{
		for (uint8_t i = 0; i<INFO_LENGTH; i++)
		{
			phdr_env.others_info[Addr].info[i]=0;
		}
	}
	return 0;
}

/**************************************
* uint8_t Rinfo_ALLinit(void)
* ��flash�ڲ���ʼ�����н������ݴ洢
**************************************/
uint8_t Rinfo_ALLinit(void)
{
	for (uint8_t i = 0; i < MAXPAGE; i++)
	{
		Rinfo_init(i);
	}
	return 0;
}

////////NFC*******************************************************************
/**************************************
* uint8_t Rinfo_all_write(void *pBuf, uint8_t add)
* ��flash�ڲ��������ݵ�ȫ��д��addλ��
* ������һ������INFO_LENGTH�������ַ
* ����ֵ��osal flash������ֵ������μ�API�ĵ�
* һ��ʹ�óɹ���SUCCESS
**************************************/
uint8_t Rinfo_all_write(void *pBuf, uint8_t addr)
{
	for (uint8_t i = 0; i<INFO_LENGTH; i++)
	{
		phdr_env.others_info[addr].info[i]=((uint8_t *)pBuf)[i];
	}
	return 0;
}

/**************************************
* uint8_t Recinfo_Compare_Save(void *pBuf)
* ������5ҳflash������д�����ݣ��������ID���л��ߴ洢����
* �򲻴洢��һ�������IDȫΪ0�����������Դ洢
**************************************/
uint8_t Recinfo_Compare_Save(void *pBuf)
{
	uint8_t checkID[MAXPAGE] = { 0 };
	uint8_t availID[MAXPAGE] = { 0 };
	
	uint8_t state = 0;
	//�����Լ���
	//ĳλΪ0�����
	for(uint8_t j = 0; j<MAXPAGE; j++)
	{
		for (uint8_t i = 0; i<10; i++)
		{
			if(phdr_env.others_info[j].info[i]!=0)
			{
				availID[j]=1;
			}
		}
	}
	//��Ч�Լ���
	//ĳλΪ1���ظ�
	for(uint8_t j = 0; j<MAXPAGE; j++)
	{
		for (uint8_t i = 0; i<10; i++)
		{
			if(phdr_env.others_info[j].info[i]!=((uint8_t *)pBuf)[i])
			{
				checkID[j]=1;
			}
		}
	}
	state=checkID[0]+checkID[1]+checkID[2]+checkID[3]+checkID[4];
	
	if(state==5)
	{
		if(availID[4]==0)
			state=4;
		if(availID[3]==0)
			state=3;
		if(availID[2]==0)
			state=2;
		if(availID[1]==0)
			state=1;
		if(availID[0]==0)
			state=0;
		Rinfo_all_write(pBuf,state);
	}
	
	return state;

}
/**************************************
* uint8_t Recinfo_getID(void *pBuf, uint8_t addr)
* ��ȡ�յ��Ĵ洢��Ϣaddr�����ֵ�IDֵ
**************************************/
uint8_t Recinfo_getID(void *pBuf, uint8_t addr)
{
	for (uint8_t i = 0; i<ID_LENGTH; i++)
	{
		((uint8_t *)pBuf)[i] = phdr_env.others_info[addr].info[i];
	}
	return 0;
}
////////NFC*******************************************************************


////////APP*******************************************************************
/**************************************
* uint8_t Rinfo_short_read(void *pBuf, uint8_t seq, uint8_t pageID)
* ��flash�ڲ�������������ĵ�pageIDҳ�ĵ�seq����ʼΪ��0λ��
* ��󣨰���seq����ȡ����9������
* �������洢���ȣ������������
* ��ֵ��pBuf��
* ע�⣺����ʹ�õ�pageID���ڲ��洢��pages����һ����ֵ�������ڲ��洢��5ҳ���ݣ���flash_Rinfo_get_pages()
* �õ���ֵ��5������Ҫ���������ֵ��4��Ҳ����˵���ڲ���5ҳ���ݣ���page�ı���Ǵ�0��4��
**************************************/
uint8_t Rinfo_short_read(void *pBuf, uint8_t seq, uint8_t pageID)
{
	//����һ������ΪINFO_LENGTH������
	//uint16* inMem;
	//inMem = (uint16 *)osal_mem_alloc(INFO_LENGTH);
	//osal_memset(inMem, 0, INFO_LENGTH);

	//osal_snv_read(flash_RinfoPageAddress(pageID), INFO_LENGTH, inMem);

	uint8_t temp[9] = { 0 };


	uint8_t i = 0;
	for (i = 0; i<9; i++)
	{
		if ((seq + i)<INFO_LENGTH)
		{
			temp[i] =  phdr_env.others_info[pageID].info[i];
		}
		else
		{
			break;
		}
	}

	for (i = 0; i<9; i++)
	{
		((uint8_t *)pBuf)[i] = temp[i];
	}

	////�ͷ��ڴ�
	//osal_mem_free(inMem);
	return 0;
}
////////APP*******************************************************************




////////BLE*******************************************************************
/**************************************
* uint8_t Rinfo_single_read( uint8_t Addr, uint8_t index )
* ��ȡflash�ڲ��������ݵĴ洢����ĵ�Addr��indexλ���������
* ��seqδ�����洢����ĳ��ȣ����ض�Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
uint8_t Rinfo_single_read(uint8_t Addr, uint8_t index)
{
	return phdr_env.others_info[Addr].info[index];
}

/**************************************
* uint8_t Rinfo_single_write(uint8_t Addr, uint8_t value)
* ��flash�ڲ��������ݵĴ洢����ĵ�Addr���indexλ��������value
* ��seqδ�����洢����ĳ��ȣ����ض�д�ɹ���Ӧֵ
* ��seq�����洢����ĳ��ȣ�����0xFF
**************************************/
uint8_t Rinfo_single_write(uint8_t Addr, uint8_t index, uint8_t value)
{
	phdr_env.others_info[Addr].info[index]=value;
	return 0;
}
////////BLE*******************************************************************
//����RINFO*******************************************************************


//RPAGES****��ֵ�����ڴ�ŵ�ǰflash���ж�������********************************
/**************************************
* uint8_t Rinfo_pages_init(void)
* ��flash�ڲ���ʼ���������ݵ�ҳ���洢λ
**************************************/
uint8_t Rinfo_pages_init(void)
{
	phdr_env.pages_num=0;
	return 0;
}

/**************************************
* uint8_t Rinfo_add_pages(void)
* ��flash�����ӷ������ݵ�ҳ��һҳ
**************************************/
uint8_t Rinfo_add_pages(void)
{
	phdr_env.pages_num=phdr_env.pages_num+1;
	if(phdr_env.pages_num>5)
	{
		phdr_env.pages_num=5;
	}
	return 0;
}

/**************************************
* uint8_t Rinfo_minus_pages(void)
* ��flash�м�ȥ�������ݵ�ҳ��һҳ
**************************************/
uint8_t Rinfo_minus_pages(void)
{
	if (phdr_env.pages_num>0)
	{
		phdr_env.pages_num = phdr_env.pages_num - 1;
	}
	return phdr_env.pages_num;
}

/**************************************
* uint8_t Rinfo_get_pages(void)
* ��flash�л�ȡ�������ݵ�ҳ��
**************************************/
uint8_t Rinfo_get_pages(void)
{
	return phdr_env.pages_num;
}
//����RPAGES*******************************************************************


uint8_t cmpinfoid(void *pBuf1,void *pBuf2, uint8_t length)
{
	uint8_t temp=0;
	for(uint8_t i =0; i<length;i++)
	{
		if(((uint8_t *)pBuf1)[i]==((uint8_t *)pBuf2)[i])
		{
			temp=1;
		}
	}
	return temp;
}

/**************************************
* uint8_t flash_info_init( void )
* ��flash�ڲ���ʼ��������Ϣ
**************************************/
uint8_t flash_info_init(void)
{
	serialNumberInit();
	cashInit();
	pwd_init();
	Tinfo_init();
	Tinfo_Length_init();
	Rinfo_pages_init();
	Rinfo_ALLinit();
	return 0;
}
