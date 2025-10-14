#ifndef MODBUS_H
#define MODBUS_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MODBUS_PARSE_TIEOUT		300

#define MODBUS_BOARD_ADDR			0x00
#define MODBUS_DEFAULT_ADDR		0x01

typedef uint16_t (*Func)(void);
typedef void (*Wfunc)(uint16_t);

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
	REG_OPEN_CLOSE_FLAG,
	REG_OPEN_CLOSE_TIME,
	REG_UNIQUE_IDHLLL,
	REG_UNIQUE_IDLHLL,
	REG_UNIQUE_IDLLHL,
	REG_UNIQUE_IDLLLH,
	REG_TOTAL,
}ModbusRegEnum;


typedef struct{
	ModbusRegEnum reg;
	Func fun;
}FunTable;

typedef struct{
	ModbusRegEnum reg;
	Wfunc fun;
}WriteFunTable;

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
	uint8_t ParseDone;
	uint8_t addr;
	uint8_t function;
	uint8_t data[512];
	uint16_t dataIndex;
	uint16_t crc;
}ModbusStr;

extern ModbusStr modbus;

void CheckFun(void);
uint16_t Modbus_Crc_Compute(const uint8_t *buf, uint16_t bufLen);
void modbusValInit(void);
void modbusMasterParse(uint8_t ch);
void ModbusSend(uint8_t* buf, uint8_t len);
#endif
