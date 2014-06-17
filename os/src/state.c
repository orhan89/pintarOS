p#include "config.h"
#include "types.h"
#include "crypt.h"
#include "state.h"
#include "fs.h"

static struct state_struct     state_mng;              ///< state_manager

int State_Init()
{
  state_mng.current = 0;
  state_mng.currentKey = 0;
  state_mng.currentRecord = 0;
  state_mng.securityState = 0;

  return STATE_OK; 
}

void State_GetChallenge( uint8_t * buffer )
{
  iu32 block[2], key[4];

  Memory_ReadBlock( (uint16_t) SERNUM_ADDR, (uint16_t) SERNUM_LEN, (uint8_t *) block);
  /* hal_eeprom_read( (iu8*)block, SERNUM_ADDR, SERNUM_LEN ); */

  Memory_ReadBlock( (uint16_t) RAND_STATE_ADDR, (uint16_t) sizeof(key), (uint8_t *) key);
  /* hal_eeprom_read( (iu8*)key, RAND_STATE_ADDR, sizeof(key) ) */

  key[2]=key[1];
  key[3]=key[0];

  crypt_enc( block, key );

  Memory_WriteBlock( (uint16_t) RAND_STATE_ADDR, (uint16_t) RAND_STATE_LEN, (uint8_t *) block);

  crypt_enc( block, key );

  memcpy( state_mng.challenge, block, sizeof(block) );

  memcpy( buffer, block, CRYPT_BLOCK_LEN );
}

int State_Verify(uint8_t *pin)
{
  state_mng.securityState &= 0xfe;

  uint8_t retries;

  retries = Memory_ReadByte(PIN_RETRIES_ADDR);

  if (retries > PIN_MAX_RETRIES)
    {
      return STATE_BLOCKED;
    }

  uint8_t i, temp, diff=0;
  for( i=0; i<PIN_LEN; i++ )
    {
      temp = Memory_ReadByte(PIN_ADDR+i);
      diff |= temp^pin[i];
    }

  if( diff>0 )
    {
      retries++;
    }
  else
    {
      retries=0;
    }

  Memory_WriteByte(PIN_RETRIES_ADDR, retries);
  
  if( diff>0 )
    {
      return STATE_WRONG;
    }

  state_mng.securityState |= 0x01;

  return STATE_OK;
}

uint8_t State_VerifyAuth( uint8_t * encrypted )
{
  uint8_t retries;
  retries = Memory_ReadByte(EXT_AUTH_RETRIES_ADDR);

  if (retries > EXT_AUTH_MAX_RETRIES)
    {
      return STATE_BLOCKED;
    }

  uint32_t key[4];
  Memory_ReadBlock( (uint16_t) EXT_AUTH_KEY_ADDR, (uint16_t) EXT_AUTH_KEY_LEN, (uint8_t *) key);

  crypt_enc( state_mng.challenge, key );

  /* Compare result */
  if( memcmp( encrypted, state_mng.challenge, CRYPT_BLOCK_LEN ) ) {
    retries++;
    Memory_WriteByte(EXT_AUTH_RETRIES_ADDR, retries);

    return STATE_WRONG;
  }

  if(retries > 0)
    {
      retries = 0;
      Memory_WriteByte(EXT_AUTH_RETRIES_ADDR, retries);
    }

  state_mng.securityState |= 0x02;

  return STATE_OK;
}

int State_SetCurrent(uint16_t newFile)
{
  state_mng.current = newFile;
  state_mng.currentRecord = 0;
  state_mng.securityState = 0;

  return STATE_OK;
}

int State_SetCurrentKey(uint16_t newEFKey)
{
  state_mng.currentKey = newEFKey;

  return STATE_OK;
}

uint16_t State_GetCurrent()
{
  return state_mng.current;
}

uint8_t State_GetCurrentSecurity()
{
  return state_mng.securityState;
}

void State_GetCurrentChallenge(uint8_t * buffer)
{
  memcpy( buffer, state_mng.challenge, CRYPT_BLOCK_LEN );
}

