#include "hw_i2c.h"


//i2c interface init, communicate with RTC chip
void I2C_Init(void)
{
	/* enable GPIOB clock */
    //rcu_periph_clock_enable(RCU_GPIOB);
	
	/* enable I2C clock */
    //rcu_periph_clock_enable(RCU_I2C0);
	//gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);//REMAP Pb8 and pb9 to connect I2C
	/* connect PB8 to I2C0_SCL */
    /* connect PB9 to I2C0_SDA */
    gpio_init(I2C0_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, I2C0_SCL_PIN | I2C0_SDA_PIN);
	gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);//REMAP Pb8 and pb9 to connect I2C
	
    /* configure I2C clock */
    i2c_clock_config(I2C0, I2C0_SPEED_400k, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    //i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}
