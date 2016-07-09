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


//序列号**********************************************************************
/**************************************
* uint8_t serialNumberInit(void);
* 序列号的初始化
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
* 获取产生的3位序列号，赋值给数组地址pBuf
**************************************/
uint8_t getSerialNumber(void *pBuf)
{
	((uint8_t *)pBuf)[0]=phdr_env.serial_number[0];
	((uint8_t *)pBuf)[1]=phdr_env.serial_number[1];
	((uint8_t *)pBuf)[2]=phdr_env.serial_number[2];
	return 0;
}
//结束序列号******************************************************************






//余额************************************************************************
/**************************************
* uint8_t cashInit(void);
* 余额的初始化
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
* 存储余额
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
* 获取余额，放到一个pBuf的数组里面
**************************************/
uint8_t get_cash(void *pBuf)
{
	for (uint8_t i = 0; i<CASH_LENGTH; i++)
	{
		((uint8_t *)pBuf)[i] = phdr_env.cash_remain[i];
	}
	return 0;
}
//结束余额********************************************************************




//密码************************************************************************
/*************************************
* uint8_t pwd_init( void )
* 在flash内部密码的初始化函数
* 即在对应位存入相应的初始密码，8个0
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
* 向flash内部写入密码
* 参数pBuf为一个8位uint8_t的数组的地址
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
* 读取flash内部存入的密码
* 参数pBuf为一个8位uint8_t数组的地址
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
* 清空flash内部存入的密码
**************************************/
uint8_t flash_pwd_delete(void)
{
	for (uint8_t i = 0; i<PASSWORD_LENGTH; i++)
	{
		phdr_env.password_number[i]=0;
	}
	return 0;
}
//结束密码*******************************************************************



//TINFO************************************************************************
/**************************************
* uint8_t  Tinfo_init( void )
* 在flash内部初始化发送数据的存储
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
* 在flash内部发送数据的全部读取
* 参数是一个长度INFO_LENGTH的数组地址
* 返回值是osal flash操作的值，具体参见API文档
* 一般使用成功是SUCCESS
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
* 在flash内部发送数据区域的接收数据的长度为s
* 则向s后面写入长度为len的数组，地址是pBuf
* 若超过存储长度的数据不写
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
* 读取flash内部发送数据的存储区域的第seq个位存入的数据
* 若seq未超过存储区域的长度，返回对应值
* 若seq超过存储区域的长度，返回0xFF
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
* 向flash内部发送数据的存储区域的第seq位存入数据value
* 若seq未超过存储区域的长度，返回读写成功对应值
* 若seq超过存储区域的长度，返回0xFF
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
//结束TINFO*******************************************************************




//TINFOLEN*******************************************************************
/**************************************
* uint8_t Tinfo_Length_init(void）
* 在flash内部初始化发送数据的长度存储位
**************************************/
uint8_t Tinfo_Length_init(void)
{
	phdr_env.person_info_length=0;
	return 0;
}

/**************************************
* uint8_t Tinfo_Length_set(uint8_t length)
* 在flash内部写入发送数据的长度存储位
**************************************/
uint8_t Tinfo_Length_set(uint8_t length)
{
	phdr_env.person_info_length=length;
	return 0;
}

/**************************************
* uint8_t Tinfo_Length_get(void)
* 在flash内部读取发送数据的长度存储位
**************************************/
uint8_t Tinfo_Length_get(void)
{
	return phdr_env.person_info_length;
}
//结束TINFOLEN*******************************************************************



//RINFO*******************************************************************
/**************************************
* uint8_t flash_RinfoPageAddress(uint8_t num)
* 获取收到的存储信息第num页的地址
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
* 在flash内部初始化接收数据存储
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
* 在flash内部初始化所有接收数据存储
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
* 在flash内部接收数据的全部写入add位置
* 参数是一个长度INFO_LENGTH的数组地址
* 返回值是osal flash操作的值，具体参见API文档
* 一般使用成功是SUCCESS
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
* 在已有5页flash接收区写入数据，若数组的ID已有或者存储已满
* 则不存储，一个区域的ID全为0代表该区域可以存储
**************************************/
uint8_t Recinfo_Compare_Save(void *pBuf)
{
	uint8_t checkID[MAXPAGE] = { 0 };
	uint8_t availID[MAXPAGE] = { 0 };
	
	uint8_t state = 0;
	//存在性检验
	//某位为0则空闲
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
	//有效性检验
	//某位为1则不重复
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
* 获取收到的存储信息addr个部分的ID值
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
* 在flash内部接收数据区域的第pageID页的第seq处开始为第0位，
* 向后（包括seq）读取长度9的数组
* 若超过存储长度，则在数组后补零
* 赋值给pBuf处
* 注意：这里使用的pageID和内部存储的pages不是一样的值。比如内部存储了5页数据，用flash_Rinfo_get_pages()
* 得到的值是5，但是要输入这里的值是4。也就是说，内部有5页数据，但page的标号是从0到4。
**************************************/
uint8_t Rinfo_short_read(void *pBuf, uint8_t seq, uint8_t pageID)
{
	//分配一个长度为INFO_LENGTH的数组
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

	////释放内存
	//osal_mem_free(inMem);
	return 0;
}
////////APP*******************************************************************




////////BLE*******************************************************************
/**************************************
* uint8_t Rinfo_single_read( uint8_t Addr, uint8_t index )
* 读取flash内部接收数据的存储区域的第Addr第index位存入的数据
* 若seq未超过存储区域的长度，返回对应值
* 若seq超过存储区域的长度，返回0xFF
**************************************/
uint8_t Rinfo_single_read(uint8_t Addr, uint8_t index)
{
	return phdr_env.others_info[Addr].info[index];
}

/**************************************
* uint8_t Rinfo_single_write(uint8_t Addr, uint8_t value)
* 向flash内部接收数据的存储区域的第Addr块第index位存入数据value
* 若seq未超过存储区域的长度，返回读写成功对应值
* 若seq超过存储区域的长度，返回0xFF
**************************************/
uint8_t Rinfo_single_write(uint8_t Addr, uint8_t index, uint8_t value)
{
	phdr_env.others_info[Addr].info[index]=value;
	return 0;
}
////////BLE*******************************************************************
//结束RINFO*******************************************************************


//RPAGES****该值是用于存放当前flash内有多少数据********************************
/**************************************
* uint8_t Rinfo_pages_init(void)
* 在flash内部初始化发送数据的页数存储位
**************************************/
uint8_t Rinfo_pages_init(void)
{
	phdr_env.pages_num=0;
	return 0;
}

/**************************************
* uint8_t Rinfo_add_pages(void)
* 在flash中增加发送数据的页数一页
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
* 在flash中减去发送数据的页数一页
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
* 在flash中获取发送数据的页数
**************************************/
uint8_t Rinfo_get_pages(void)
{
	return phdr_env.pages_num;
}
//结束RPAGES*******************************************************************


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
* 在flash内部初始化所有信息
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
