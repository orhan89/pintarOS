#include "config.h"
#include "types.h"
#include "hal.h"

#define INTERNAL_EEPROM_SIZE    0x200

uint8_t Memory_ReadByte(uint16_t address)
{
  if( address < INTERNAL_EEPROM_SIZE )
    {
      return HAL_InternalReadByte(address);
    }
  else
    {
      return HAL_ExternalReadByte(address - INTERNAL_EEPROM_SIZE);
    }
}

void Memory_WriteByte(uint16_t address, uint8_t databyte)
{
  if( address < INTERNAL_EEPROM_SIZE )
    {
      HAL_InternalWriteByte(address, databyte);
    }
  else
    {
      HAL_ExternalWriteByte(address - INTERNAL_EEPROM_SIZE, databyte);
    }
}

int Memory_ReadBlock(uint16_t address, uint16_t size, uint8_t * databyte)
{
  uint16_t count;

  for( count=0; count < size; count++)
    {
      *(databyte+count) = Memory_ReadByte(address+count);
    }

  return count;
}

int Memory_WriteBlock(uint16_t address, uint16_t size, uint8_t * databyte)
{
  uint16_t count;

  for( count=0; count < size; count++)
    {
      Memory_WriteByte(address+count, *(databyte+count));
    }

  return count;
}
