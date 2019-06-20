#include "headfile.h"
#include "Message.h"
#include "crc.h"
#include "Bluetooth.h"

#define NumCount 3
unsigned char sData[NumCount];
unsigned char rData[NumCount];


void SendData(void)
{
  sData[0] = 0x00;
  sData[0] |= (g_GetMeetingMaster<<0);
  sData[0] |= (g_MasterOutFlag<<1);
  sData[0] |= (g_StartMaster<<2);
  width_t check_code = crcCompute(sData, NumCount - 2);
  sData[NumCount - 2] = (unsigned char)(check_code >> 8);
  sData[NumCount - 1] = (unsigned char)(check_code);
  for (unsigned char i = 0; i < NumCount; i++)
  {
    uart_putchar(uart4, sData[i]);
  }
}

void ReceiveData(void)
{
  static int RcNum = 0;
  uart_getchar(uart4, &rData[RcNum]);
  RcNum++;
  if (RcNum >= NumCount)
  {
    RcNum = 0;
    width_t check_code = crcCompute(rData, NumCount - 2);
    if (1)//rData[NumCount - 2] == (unsigned char)(check_code >> 8)
       // && rData[NumCount - 1] == (unsigned char)(check_code))
    {
      if(g_GetMeetingMaster)
        g_GetMeetingSlave |= (rData[0]&0x01);
      g_SlaveOutFlag = (rData[0]&0x02)>>1;
      g_StartSlave = (rData[0]&0x04)>>2;
    }
  }
}