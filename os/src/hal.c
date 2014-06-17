#include "config.h"
#include "types.h"
#include "hal.h"
#include "crypt.h"

#define INTERNAL_EEPROM_SIZE    0x200

extern void hwinit();

int HAL_Init()
{
  hwinit();
  return HAL_OK;
}

extern uint8_t recbytet0();

uint8_t HAL_IO_RxByte (void)
{
  return recbytet0();
}

extern void sendbytet0(uint8_t byte);

void HAL_IO_TxByte(uint8_t byte)
{
  sendbytet0(byte);
}

extern uint8_t readinternalmem(uint16_t addr);
extern uint8_t xeread(uint16_t addr);

uint8_t HAL_InternalReadByte(uint16_t address)
{
  return readinternalmem(address);
}

uint8_t HAL_ExternalReadByte(uint16_t address)
{
  return xeread(address - INTERNAL_EEPROM_SIZE);
}

extern void writeinternalmem(uint16_t addr, uint8_t byte);
extern void xewrt(uint16_t addr, uint8_t byte);

void HAL_Mem_WriteByte(uint16_t address, uint8_t databyte)
{
      writeinternalmem(address, databyte);
}
void HAL_Mem_WriteByte(uint16_t address, uint8_t databyte)
{
      xewrt(address - INTERNAL_EEPROM_SIZE, databyte);
}

uint8_t HAL_Mem_ReadByte(uint16_t address)
{
  if( address < INTERNAL_EEPROM_SIZE )
    {
      return readinternalmem(address);
    }
  else
    {
      return xeread(address - INTERNAL_EEPROM_SIZE);
    }
}

void HAL_Mem_WriteByte(uint16_t address, uint8_t databyte)
{
  if( address < INTERNAL_EEPROM_SIZE )
    {
      writeinternalmem(address, databyte);
    }
  else
    {
      xewrt(address - INTERNAL_EEPROM_SIZE, databyte);
    }
}

int HAL_Mem_ReadBlock(uint16_t address, uint16_t size, uint8_t * databyte)
{
  uint16_t count;

  for( count=0; count < size; count++)
    {
      *(databyte+count) = HAL_Mem_ReadByte(address+count);
    }

  return count;
}

int HAL_Mem_WriteBlock(uint16_t address, uint16_t size, uint8_t * databyte)
{
  uint16_t count;

  for( count=0; count < size; count++)
    {
      HAL_Mem_WriteByte(address+count, *(databyte+count));
    }

  return count;
}

void HAL_RND_GetBlock( uint8_t * buf )
{
  return;
}
