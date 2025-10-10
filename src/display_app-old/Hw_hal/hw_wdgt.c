#include "hw_wdgt.h"

void WdgtInit(void)
{
	fwdgt_config(2000, FWDGT_PSC_DIV128);//2000 * 1 / (40000 / 128) = 6.4s
	/* after 6.4 seconds to generate a reset */
    fwdgt_enable();
}

void WdgtFeed(void)
{
	static uint16_t count = 0;
	
	count++;
	if(count >= (2 * 1000))
	{
		count = 0;
		fwdgt_counter_reload();
	}
}

