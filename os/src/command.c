#include "types.h"
#include "config.h"
#include "command.h"
#include "response.h"
#include "transmission.h"
#include "hal.h"
#include "crypt.h"
#include "state.h"
#include "fs.h"

uint8_t response[8];
uint8_t resplen;

void Command_Interpreter()
{
      if ( (header[0]&0xFC)==0x80 )
        {
          switch( header[1]&0xFE )
            {
            case DEBUG_WRITE:
              Command_Write();
              break;
            case DEBUG_READ:
              Command_Read();
              break;
            case DEBUG_GETCURRENT:
              Command_GetCurrent();
              break;
            case DEBUG_GETSECURITY:
              Command_GetSecurity();
              break;
            case DEBUG_FORMAT:
              Command_Format();
              break;
            case DEBUG_ENCRYPT:
              Command_Encrypt();
              break;
            case DEBUG_GETCHALLENGE:
              Command_GetCurrentChallenge();
              break;
            case ISO_SELECT:
              Command_Select();
              break;
            case ISO_READ_BINARY:
              Command_ReadBinary();
              break;
            case ISO_UPDATE_BINARY:
              Command_UpdateBinary();
              break;
            case ISO_CREATE_FILE:
              Command_CreateFile();
              break;
            case ISO_DELETE_FILE:
              Command_DeleteFile();
              break;
            case ISO_VERIFY:
              Command_Verify();
              break;
            case ISO_GET_CHALLENGE:
              Command_GetChallenge();
              break;
            case ISO_EXT_AUTH:
              Command_ExternalAuth();
              break;
            case ISO_GET_RESPONSE:
              Command_GetResponse();
              break;
            default:
              Response_SetSW( Response_INSNotSupported, 0 );
              break;
            }
        }
      else
        {
          Response_SetSW( Response_CLANotSupported, 0);
        }
}

void Command_Write()
{
  uint16_t i;
  uint8_t data;

  for(i=0; i<header[4]; i++)
    {
      Transmission_SendByte( ~header[1] );

      data = Transmission_GetByte();

      Memory_WriteByte( ((uint16_t)header[2]<<8)+header[3]+i, data );
    }

  Response_SetSW( Response_OK, 0);
}

void Command_Read()
{
  uint16_t i;
  uint8_t data;

  Transmission_SendByte( header[1] );

  for(i=0; i<header[4]; i++)
    {
      data = Memory_ReadByte( ((uint16_t)header[2]<<8)+header[3]+i );
      Transmission_SendByte(data);
    }

  Response_SetSW( Response_OK, 0);
}

void Command_GetCurrent()
{
  uint16_t current;

  Transmission_SendByte( header[1] );

  current = State_GetCurrent();

  Transmission_SendByte( (uint8_t)(current>>8) );
  Transmission_SendByte( (uint8_t)current );

  Response_SetSW( Response_OK, 0);
}

void Command_GetSecurity()
{
  uint8_t security, acread, acupdate;

  Transmission_SendByte( header[1] );

  security = State_GetCurrentSecurity();
  Transmission_SendByte( security );

  if( header[4]>1 )
    {
      acread = FS_GetAC(FS_OP_READ);
      Transmission_SendByte( acread );
      acupdate = FS_GetAC(FS_OP_UPDATE);
      Transmission_SendByte( acupdate );
    }  

  Response_SetSW(Response_OK, 0);
}

void Command_Format()
{
  if (!(FSFormat() == FS_OK))
    {
      Response_SetSW( Response_FatalError , 0);
      return;
    }

  Response_SetSW( Response_Normal , 0);
}

void Command_Encrypt()
{
  uint16_t i;
  uint8_t data[8];

  for(i=0; i<header[4]; i++)
    {
      Transmission_SendByte( ~header[1] );

      data[i] = Transmission_GetByte();
    }

  memcpy( response, data, 8 );

  uint32_t key[4];
  Memory_ReadBlock( (uint16_t) EXT_AUTH_KEY_ADDR, (uint16_t) EXT_AUTH_KEY_LEN, (uint8_t *) key);

  crypt_enc( response, key );
  resplen = 8;

  Response_SetSW(Response_OK, 0);
}

void Command_GetCurrentChallenge()
{
	uint8_t i, buf[CRYPT_BLOCK_LEN];

	if( !(header[4] == CRYPT_BLOCK_LEN ) )
          {
            Response_SetSW( Response_WrongLength , 0);
            return;
          }

	/* ACK */
        Transmission_SendByte( header[1] );

	State_GetCurrentChallenge( buf );

	for( i=0; i<CRYPT_BLOCK_LEN; i++ )
          {
            Transmission_SendByte( buf[i] );
          }

        Response_SetSW( Response_OK , 0);
}

void Command_Select()
{
  uint16_t fid, file;
  uint8_t data;

  if((header[2] == 0x00) && (header[3]  == 0x00))
    {
      /* Select MF */
      if(header[4] == 0)
        {
          FSSelectMF();
          Response_SetSW(Response_OK, 0);
        }
      else
        {
          /* Transmission_GetData(); */
          Transmission_SendByte( ~header[1] );
          fid = (uint16_t)(Transmission_GetByte()) << 8;

          Transmission_SendByte( ~header[1] );
          fid |= Transmission_GetByte();

          /* Transmission_SendByte( (uint8_t)(fid>>8) ); */
          /* Transmission_SendByte( (uint8_t)fid ); */

          switch( FS_SelectFID(fid) )
            {
            case FS_OK:
              Response_SetSW(Response_OK, 0);
              break;
            case FS_ERROR_NOT_FOUND:
              Response_SetSW( Response_WrongP1P2_FileNotFound, 0);
              break;
              
            }
        }
    }
}

void Command_ReadBinary()
{
  uint16_t offset, length;
  uint8_t data;

  Transmission_SendByte( header[1] );

  if( !(FS_CheckAC(FS_OP_READ) == FS_OK))
    {
      Response_SetSW( Response_CmdNotAllowed_SecurityStatus , 0);
      return;
    }

  offset = ((uint16_t)header[2] << 8) | header[3];

  uint8_t i;
  for( i=0; i<header[4]; i++ ) 
    {
      FSAccessBinary(FS_OP_READ,offset+i,1,&data);
      Transmission_SendByte(data);
    }

  Response_SetSW( Response_Normal, (uint8_t)i );
}

void Command_UpdateBinary()
{
  uint16_t offset, length;
  uint8_t data;

  if( !(FS_CheckAC(FS_OP_UPDATE) == FS_OK))
    {
      Response_SetSW( Response_CmdNotAllowed_SecurityStatus , 0);
      return;
    }

  offset = ((uint16_t)(header[2]) << 8) | header[3];
  length = (uint16_t) header[4];

  uint8_t i;
  for( i=0; i<length; i++)
    {
      Transmission_SendByte( ~header[1] );

      data = Transmission_GetByte();

      FSAccessBinary(FS_OP_UPDATE,offset+i,1,&data);
    }

  Response_SetSW( Response_Normal , 0);
}


void Command_CreateFile()
{
  uint8_t tag;
  uint16_t size, fid;
  uint8_t acread, acupdate;
  uint8_t result;

  Transmission_SendByte( ~header[1] );
  tag = Transmission_GetByte();

  switch( tag )
    {
    case FS_TAG_DF:
      Transmission_SendByte( ~header[1] );
      fid = (uint16_t) (Transmission_GetByte()) << 8;

      Transmission_SendByte( ~header[1] );
      fid |= (uint16_t) (Transmission_GetByte());

      struct DF_st df;
      df.FID = fid;

      result = FSCreateFile(tag,&df);
      break;
    case FS_TAG_EF:

      Transmission_SendByte( ~header[1] );
      fid = (uint16_t) (Transmission_GetByte()) << 8;

      Transmission_SendByte( ~header[1] );
      fid |= (uint16_t) (Transmission_GetByte());

      Transmission_SendByte( ~header[1] );
      size = (uint16_t) (Transmission_GetByte()) << 8;

      Transmission_SendByte( ~header[1] );
      size |= (uint16_t) (Transmission_GetByte());

      Transmission_SendByte( ~header[1] );
      acread = Transmission_GetByte();

      Transmission_SendByte( ~header[1] );
      acupdate = Transmission_GetByte();

      struct EF_st ef;

      ef.FID = fid;
      ef.structure = FS_EF_STRUCTURE_TRANSPARENT;
      ef.type = FS_EF_TYPE_WORKING;
      ef.ACRead = acread; //(ac && 0xf0) >> 4;
      ef.ACUpdate = acupdate; //ac && 0x0f;
      ef.size = size;

      result = FSCreateFile(tag,&ef);

      break;
    }

  switch ( result )
    {
    case FS_OK:
      Response_SetSW( Response_Normal , 0);
      break;
    default:
      Response_SetSW( Response_FatalError , 0);
      break;
    }
}


void Command_DeleteFile()
{
  uint16_t fid;

  Transmission_SendByte( header[1] );
  fid = (uint16_t) (Transmission_GetByte()) << 8;

  Transmission_SendByte( header[1] );
  fid |= (uint16_t) (Transmission_GetByte());

  switch ( FSDeleteFile(fid) )
    {
    case FS_OK:
      Response_SetSW( Response_Normal , 0);
      break;
    default:
      Response_SetSW( Response_FatalError , 0);
      break;
    }
}


void Command_Verify()
{
  uint8_t pin[PIN_LEN];
  uint8_t i;

  if( header[4] != PIN_LEN )
    {
      Response_SetSW( Response_WrongLength , 0);
      return;
    }

  for( i=0; i<PIN_LEN; i++ )
    {
      Transmission_SendByte( ~header[1] );
      pin[i] = Transmission_GetByte();
    }

  uint8_t retries;
  switch( State_Verify(pin) )
    {
    case STATE_OK:
      Response_SetSW( Response_OK , 0);
      break;
    case STATE_BLOCKED:
      Response_SetSW( Response_CmdNotAllowed_AuthBlocked , 0);
      break;
    case STATE_WRONG:
      retries = Memory_ReadByte(PIN_RETRIES_ADDR);
      Response_SetSW( Response_Warning_Counter | (retries & 0x0f) , 0);
    }
}

void Command_ExternalAuth()
{
  uint8_t encrypted[CRYPT_BLOCK_LEN];
  uint8_t i;

  if( !(header[4] == CRYPT_BLOCK_LEN) )
    {
      Response_SetSW( Response_WrongLength , 0);
      return;
    }

  for( i=0; i<CRYPT_BLOCK_LEN; i++ )
    {
      Transmission_SendByte( ~header[1] );
      encrypted[i] = Transmission_GetByte();
    }

  uint8_t retries;
  switch( State_VerifyAuth( &encrypted ) )
    {
    case STATE_OK:
      Response_SetSW( Response_OK , 0);
      break;
    case STATE_BLOCKED:
      Response_SetSW( Response_CmdNotAllowed_AuthBlocked , 0);
      break;
    case STATE_WRONG:
      retries = Memory_ReadByte(EXT_AUTH_RETRIES_ADDR);
      Response_SetSW( Response_Warning_Counter | (retries & 0x0f) , 0);
    }

}

void Command_GetChallenge()
{
	uint8_t i, buf[CRYPT_BLOCK_LEN];

	if( !(header[4] == CRYPT_BLOCK_LEN ) )
          {
            Response_SetSW( Response_WrongLength , 0);
            return;
          }

	/* ACK */
        Transmission_SendByte( header[1] );

	State_GetChallenge( buf );

	for( i=0; i<CRYPT_BLOCK_LEN; i++ )
          {
            Transmission_SendByte( buf[i] );
          }

        Response_SetSW( Response_OK , 0);
}

void Command_GetResponse()
{
  uint8_t i;

  if( resplen==0 ) {
    Response_SetSW( Response_CmdNotAllowed_ConditionNotSatisfied , 0);
    return;
  }

  if( (header[4]>resplen) || (!header[4]) ) {
    Response_SetSW( Response_WrongLength , 0);
    return;
  }
  resplen=header[4];

  Transmission_SendByte( header[1] );

  for( i=0; i<resplen; i++ )
    {
      Transmission_SendByte(response[i]);
    }

  Response_SetSW( Response_OK , 0);
}
