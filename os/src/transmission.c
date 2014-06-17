#include "types.h"
#include "config.h"
#include "transmission.h"
#include "hal.h"

uint8_t Transmission_GetByte()
{
  return HAL_IO_RxByte();
}

void Transmission_SendByte(uint8_t databyte)
{
  return HAL_IO_TxByte(databyte);
}

void Transmission_GetHeader()
{
  uint8_t i;

  for (i = 0; i < 5; ++i)
    {
      header[i] = Transmission_GetByte();
    }
}

void Transmission_SendACK()
{
  HAL_IO_TxByte(header[1]);
}

void Transmission_SendNACK()
{
  HAL_IO_TxByte(~header[1]);
}

void Transmission_GetData(uint8_t * dst, uint8_t len)
{
  while ( len>0 )
    {
      Transmission_SendNACK();
      *(dst++) = Transmission_GetByte();
      len--;
    }
}

void Transmission_SendData(uint8_t * src, uint8_t len)
{
  while ( len>0 )
    {
      Transmission_SendByte( *(src++) );
      len--;
    }
}

void Transmission_SendSW()
{
  Transmission_SendByte( ( (uint8_t)(sw>>8) & 0xFF) );
  Transmission_SendByte( (uint8_t)(sw & 0xFF) );
}
