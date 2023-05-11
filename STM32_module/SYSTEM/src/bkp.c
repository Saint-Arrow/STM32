#include "bkp.h"


void Bkp_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
	BKP_DeInit();
	PWR_DeInit();
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
}

