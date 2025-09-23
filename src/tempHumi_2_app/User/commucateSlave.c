#include "commucateSlave.h"
#include "hw_uart.h"

PakStruct PrtocolStr;
PakTimerStr PakTimer;
//static uint32_t writeLen = 0;
//static uint16_t writeCacheAddr = 0;

void PakStuctInit(void)
{
		PrtocolStr.step = PAK_HEAD;
		PrtocolStr.command = COM_FIND;
		PrtocolStr.len = 0;
		memset(PrtocolStr.buf, 0, sizeof(PrtocolStr.buf));
		PrtocolStr.bufLen = 0;
		PrtocolStr.crc = 0;		
	
		PakTimer.TimerEnable = TIMER_DISABLE_COMM;
		PakTimer.count = 0;
}

static void PakTimerStart(void)
{
		PakTimer.TimerEnable = TIMER_ENABLE_COMM;
		PakTimer.count = 0;
}

static void PakTimerStop(void)
{
		PakTimer.TimerEnable = TIMER_DISABLE_COMM;
		PakTimer.count = 0;
}

static void ResponsePak(uint8_t *buf, uint8_t bufLen)
{
		uint8_t resBuf[150] = {0};
		uint8_t i = 0;
		uint8_t j = 0;
		
		resBuf[i++] = PAK_HEAD_VAL;
		resBuf[i++] = PrtocolStr.command | PAK_RES_COM_MASK;
		resBuf[i++] = bufLen + PAK_RES_NO_DATA_LEN;
		if(bufLen > 0)
		{
				for(j = 0; j < bufLen; j++)
				{
						resBuf[i++] = buf[j];
				}
		}
		resBuf[i++] = 0xFF;
		resBuf[i++] = 0xFF;
		resBuf[i++] = PAK_TAIL_VAL;
		HostUartSendBuf(resBuf, i);
}
#if 0
static void FindAction(void)
{
		uint8_t buf[1] = {0x00};
		ResponsePak(buf, 1);
}

static void EraseAction(void)
{
#if 0
		uint32_t addr = 0;
	
		addr += (uint32_t)(PrtocolStr.buf[0]) << 24;
		addr += (uint32_t)(PrtocolStr.buf[1]) << 16;
		addr += (uint32_t)(PrtocolStr.buf[2]) << 8;
		addr += (uint32_t)(PrtocolStr.buf[3]);
		FlashEarsePage(addr);
#endif
		ResponsePak(NULL, 0);
}

static void WriteAction(void)
{
		uint8_t dataLen = 0;
	
		dataLen = PrtocolStr.bufLen - 4;
		WritePageLoad(&PrtocolStr.buf[4], dataLen, writeCacheAddr);
		WritePageExe(FILE_STORE_START_PAGE + writeLen / FLASH_CACHE_PAGE_SIZE);
		writeCacheAddr += dataLen;
		if(writeCacheAddr >= FLASH_CACHE_PAGE_SIZE)
		{
				writeCacheAddr = 0;
		}
		writeLen += dataLen;
#if 0
		uint32_t addr = 0;
		uint8_t dataLen = 0;
		uint8_t i = 0;
		uint8_t j = 4;
		uint32_t TempData = 0;
		
		addr += (uint32_t)(PrtocolStr.buf[0]) << 24;
		addr += (uint32_t)(PrtocolStr.buf[1]) << 16;
		addr += (uint32_t)(PrtocolStr.buf[2]) << 8;
		addr += (uint32_t)(PrtocolStr.buf[3]);
		dataLen = (PrtocolStr.bufLen - 4) / 4;//word length
	
		fmc_unlock();
		for(i = 0; i < dataLen; i++)
		{
				TempData += (uint32_t)(PrtocolStr.buf[j++]);
				TempData += (uint32_t)(PrtocolStr.buf[j++]) << 8;
				TempData += (uint32_t)(PrtocolStr.buf[j++]) << 16;
				TempData += (uint32_t)(PrtocolStr.buf[j++]) << 24;
				fmc_word_program(addr + i * 4, TempData);
				fmc_flag_clear(FMC_FLAG_BANK0_END);
				fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
				fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
				TempData = 0;
		}
		fmc_lock();//lock
#endif
		ResponsePak(NULL, 0);
}

static void ReadAction(void)
{
		//ResponsePak(NULL, 0);
}

static void ExitAction(void)
{
		ResponsePak(NULL, 0);
		//RunApp(APP_ADDRESS);
}
#endif
static void EnterUpgradeAction(void)
{
		uint32_t *bootFlag = (uint32_t *)APP_BOOT_FLAG_ADDR;
	
		ResponsePak(NULL, 0);
		delay_1ms(5);
		__disable_irq();
		*bootFlag = STAY_BOOT_FLAG_VAL;
		NVIC_SystemReset();
}

static void EnterBypass(void)
{
		systemFlag.bit.BypassMode = 1;
		ResponsePak(NULL, 0);
}

static void PakAction(void)
{
		switch(PrtocolStr.command)
		{
#if 0
			case COM_FIND:
				FindAction();
				break;
			case COM_ERASE:
				EraseAction();
				break;
			case COM_WRITE:
				WriteAction();
				break;
			case COM_READ:
				ReadAction();
				break;
			case COM_EXIT:
				ExitAction();
				break;
#endif
			case COM_ENTER_UPGRADE:
				EnterUpgradeAction();
				break;
			case COM_ENTER_BYPASS:
				EnterBypass();
			break;
			default:
				
				break;
		}
}

void ReceParse(uint8_t ch)
{
		static uint8_t tempLen = 0;
	
		PakTimerStart();
		switch(PrtocolStr.step)
		{
			case PAK_HEAD:
				PrtocolStr.len = 0;
				PrtocolStr.bufLen = 0;
				if(ch == PAK_HEAD_VAL)
				{
						PrtocolStr.step = PAK_ADDR;
				}
				break;
			case PAK_ADDR:
				if(ch == StoreConf.SlaveAddr)
				{
						PrtocolStr.step = PAK_COMMAND;
				}
				else
				{
						PrtocolStr.step = PAK_HEAD;
				}
			break;
			case PAK_COMMAND:
				if(ch <= COM_ENTER_BYPASS)
				{
						PrtocolStr.command = (CommandEnum)ch;
						PrtocolStr.step = PAK_LENGTH;
				}
				else
				{
						PrtocolStr.step = PAK_HEAD;
				}
				break;
			case PAK_LENGTH:
				PrtocolStr.len = ch;
				tempLen = ch - 3;
				if(tempLen > 0)
				{
						PrtocolStr.step = PAK_DATA;
				}
				else					
				{
						PrtocolStr.step = PAK_CRC_H;
				}
				break;
			case PAK_DATA:
				if(tempLen > 0)
				{
						PrtocolStr.buf[PrtocolStr.bufLen++] = ch;
						tempLen--;
						if(tempLen == 0)
						{
								PrtocolStr.step = PAK_CRC_H;
						}
				}
				break;
			case PAK_CRC_H:
				if(ch == 0xFF)
				{
						PrtocolStr.step = PAK_CRC_L;
				}
				else
				{
						PrtocolStr.step = PAK_HEAD;
				}
				break;
			case PAK_CRC_L:
				if(ch == 0xFF)
				{
						PrtocolStr.step = PAK_TAIL;
				}
				else
				{
						PrtocolStr.step = PAK_HEAD;
				}
				break;
			case PAK_TAIL:
				if(ch == PAK_TAIL_VAL)
				{
						//action
						PakAction();
						PakTimerStop();
				}
				PrtocolStr.step = PAK_HEAD;
				break;
			default:
				PrtocolStr.step = PAK_HEAD;
				break;
		}
}

void CommunSlaveFun(void)
{
		uint8_t ch = 0;
	
		if(CommunCheck())
		{
				ch = CommunGetData();
				ReceParse(ch);
		}
}
