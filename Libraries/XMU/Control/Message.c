#include "headfile.h"
#include "Message.h"
#include "crc.h"
#include "Bluetooth.h"

unsigned char sData;
unsigned char rData;
void SendData(void)
{
	sData = 0x80;
	sData |= (g_MasterOutFlag << 5);
	sData += g_StateMaster;
	uart_putchar(uart4, sData);
}

void ReceiveData(void)
{
	uart_getchar(uart4, &rData);
	if (0x80 == (rData & 0xC0))
	{
		if (rData & (1 << 5)) g_SlaveOutFlag = 1;
		else g_SlaveOutFlag = 0;
		g_StateSlave = MAX(g_StateSlave, (rData & 0x1F));
		
	}
}