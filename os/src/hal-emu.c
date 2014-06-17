#include "config.h"
#include "pintaros.h"
#include "hal.h"

uint8_t * nvm;

int HAL_IO_Config(int baudrate)
{

}

void HAL_IO_ISR()
{

}

uint8_t HAL_IO_RxByte()
{

}

int HAL_IO_TxByte(uint8_t data)
{

}

uint8_t HAL_Mem_ReadByte(int address)
{
  return nvm[address];
}

uint8_t HAL_Mem_WriteByte(int address, uint8_t data)
{
  nvm[address] = data;

  return data;
}

int HAL_Mem_ReadBlock(int address, int size, uint8_t * data)
{
  int i;

  for(i=0; i<size; i++)
    {
      data[i] = HAL_Mem_ReadByte(address+i);
    }

  return i;
}

int HAL_Mem_WriteBlock(int address, int size, uint8_t * data)
{
  int i;

  for(i=0; i<size; i++)
    {
      HAL_Mem_WriteByte(address+i, data[i]);
    }

  return i;
}

int HAL_Mem_Check()
{
  return 0;
}

int HAL_Mem_Initialize()
{
  nvm = (uint8_t *)calloc(CONFIG_FS_SIZE, sizeof(uint8_t));

  return CONFIG_FS_SIZE;
}

int HAL_Mem_DeInitialize()
{
  free(nvm);

  return 0;
}
