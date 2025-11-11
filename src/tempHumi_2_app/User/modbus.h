#ifndef MODBUS_H
#define MODBUS_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MODBUS_PARSE_TIEOUT		300

#define MODBUS_BOARD_ADDR			0x00
#define MODBUS_DEFAULT_ADDR		0x01

#define MODBUS_WL_TEMP_ID_START_REG			0x0050
#define MODBUS_WL_TEMP_ID_END_REG				0x013F

#define MODBUS_WL_HUMI_ID_START_REG			0x0230
#define MODBUS_WL_HUMI_ID_END_REG				0x031F

#define MODBUS_WL_TEMP_VAL_START_REG			0x0140
#define MODBUS_WL_TEMP_VAL_END_REG				0x022F

#define MODBUS_WL_HUMI_VAL_START_REG			0x0320
#define MODBUS_WL_HUMI_VAL_END_REG				0x040F

typedef uint16_t (*Func)(void);
typedef void (*Wfunc)(uint16_t);

typedef void (*WfuncInt16)(int16_t);

typedef enum{
	REG_SLAVE_ADDR = 0x0000,
	REG_BAUD_RATE_H,
	REG_BAUD_RATE_L,
	REG_RTC_TIME_H,
	REG_RTC_TIME_L,
	REG_TEM_INTERVAL,
	REG_CLOSE_ALARM,
	REG_OPEN_ALARM,
	REG_CLOSE_ABNORMAL,
	REG_OPEN_ABNORMAL,
	REG_TEM_ABNORMAL,
	REG_HUMI_ABNORMAL,
	REG_TEM_DIFF_ALARM,
	REG_HW_VERSION,
	REG_INTERL_TEM,
	REG_EXTERL_TEM,
	REG_HUMIDUTY,
	REG_CHANNEL_4_ALARM,
	REG_OPEN_FLAG,
	REG_OPEN_TIME,
	REG_OPEN_UNIX_H,
	REG_OPEN_UNIX_L,
	REG_UNIQUE_IDHLLL,
	REG_UNIQUE_IDLHLL,
	REG_UNIQUE_IDLLHL,
	REG_UNIQUE_IDLLLH,
	REG_CLOSE_FLAG,
	REG_CLOSE_TIME,
	REG_CLOSE_UNIX_H,
	REG_CLOSE_UNIX_L,
	REG_WATER_TEMP_DIFF,
	REG_WATER_TEMP,
	REG_WL_TEMP_MAX,
	REG_WL_TEMP_MIN,
	REG_WL_TEMP_MAX_MIN_DIFF,
	REG_DEVICE_STATUS,
	REG_CH3_SET,
	REG_TOTAL,
	REG_SELF_TEMP1	= 0x0410,
	REG_SELF_TEMP2,
	REG_SELF_TEMP3,
	REG_SELF_TEMP4,
	REG_SELF_TEMP5,
	REG_SELF_TEMP6,
	REG_SELF_HUMI,
//FOR TEST
	//REG_CLOSE_TIME,
	//REG_OPEN_TIME,
//FOR TEST
	REG_NUM_SAMPLE = 0x1000,
	REG_READ_DATA1_SIZE = 0x1001,
	REG_READ_DATA2_SIZE = 0x1003,
	REG_READ_DATA1 = 0x2000,
	REG_READ_DATA2 = 0x4000,
}ModbusRegEnum;


typedef struct{
	ModbusRegEnum reg;
	Func fun;
}FunTable;

typedef struct{
	ModbusRegEnum reg;
	Wfunc fun;
}WriteFunTable;

typedef struct {
	ModbusRegEnum reg;
	WfuncInt16 fun;
}SelfWriteFunTable;

typedef enum{
	MODBUS_SLAVE_ADDR,
	MODBUS_FUNCTION_CODE,
	MODBUS_DATA,
	MODBUS_CRC_L,
	MODBUS_CRC_H,
}ParseStep;

typedef enum{
	MODBUS_READ_MUL_REG = 0x03,
	MODBUS_WRITE_MUL_REG = 0x10,
}ModbusCommand;

typedef struct{
	ParseStep step;
	uint8_t addr;
	uint8_t function;
	uint8_t data[250];
	uint16_t dataIndex;
	uint16_t crc;
}ModbusStr;

typedef enum{
	TIMER_DISABLE,
	TIMER_ENABLE,
}TimerState;

typedef struct{
	TimerState state;
	uint16_t timerCount;
}ModbusTimer;

typedef struct{
	uint16_t tempID[90];
	uint16_t humiID[15];
	uint16_t tempVal[90];
	uint16_t humiVal[15];
	int16_t selfTemp[6];
	uint32_t selfTempUnix[6];
	int16_t selfHumi;
	uint32_t selfHumiUnix;
}ModbusTempHumiStr;


extern ModbusTempHumiStr TempHumiData;
uint16_t Modbus_Crc_Compute(const uint8_t *buf, uint16_t bufLen);
uint16_t Modbus_Crc_Compute_cont(const uint8_t *buf, uint16_t bufLen, uint16_t val);
void CheckFun(void);
void modbusValInit(void);
void ModbusTimerAct(void);
void ModbusProtocol(void);
#endif
