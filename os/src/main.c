#include "config.h"
#include "types.h"
#include "hal.h"
#include "transmission.h"
#include "crypt.h"
#include "state.h"
#include "fs.h"
#include "command.h"
#include "response.h"

int main()
{
  int i, len;
  uint8_t atr;

  if ( !(HAL_Init() == HAL_OK) )
    {
      for (;;) { }
    }

  Transmission_SendByte( 0x3B );

  if ( !(State_Init() == STATE_OK) )
    {
      for (;;) { }
    }

  if ( !(FS_Init() == FS_OK) )
    {
      for (;;) { }
    }

  len = Memory_ReadByte( ATR_LEN_ADDR );

  if( len>ATR_MAXLEN )
    for(;;) {}
  for( i=1; i<len; i++ ) {
    atr = Memory_ReadByte( ATR_ADDR+i );
    Transmission_SendByte( atr );
  }

  do
    {
      Transmission_GetHeader();
      Command_Interpreter();
      Transmission_SendSW();
    } while (1);
}
