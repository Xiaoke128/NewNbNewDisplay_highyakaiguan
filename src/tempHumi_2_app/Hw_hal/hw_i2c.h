#ifndef HW_I2C_H
#define HW_I2C_H

#include "gd32f10x.h"
#include "Init.h"
#include <stdio.h>

#define I2C0_SPEED_400k              	400000//400k
#define I2C0_SLAVE_ADDRESS7     		0xDE

void I2C_Init(void);

#endif
