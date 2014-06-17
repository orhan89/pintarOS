#include "types.h"
#include "config.h"
#include "command.h"
#include "response.h"
#include "transmission.h"


void Response_SetSW( uint8_t response, uint8_t extra )
{
  uint16_t temp;

  switch( response ) 
    {
    case Response_OK:
      sw = 0x9000;
    case Response_Normal:
      sw = 0x6100;
    case Response_Warning_Unchanged:
      sw = 0x6200;
    case Response_Warning_DataCorrupt:
      sw = 0x6281;
    case Response_Warning_EndOfFile:
      sw = 0x6282;
    case Response_Warning_FileDeactivated:
      sw = 0x6283;
    case Response_Warning_Changed:
      sw = 0x6300;
    case Response_Warning_FilledUp:
      sw = 0x6381;
    case Response_Warning_Counter:
      sw = 0x63C0;
    case Response_Error_Unchanged:
      sw = 0x6400;
    case Response_Error_Changed:
      sw = 0x6500;
    case Response_WrongLength:
      sw = 0x6700;
    case Response_NotSupported:
      sw = 0x6800;
    case Response_NotSupported_LogicalChannel:
      sw = 0x6881;
    case Response_NotSupported_SecureMessaging:
      sw = 0x6882;
    case Response_NotSupported_LastCommandExpected:
      sw = 0x6883;
    case Response_NotSupported_CommandChain:
      sw = 0x6884;
    case Response_CmdNotAllowed:
      sw = 0x6900;
    case Response_CmdNotAllowed_Incompatible_FS:
      sw = 0x6981;
    case Response_CmdNotAllowed_SecurityStatus:
      sw = 0x6982;
    case Response_CmdNotAllowed_AuthBlocked:
      sw = 0x6983;
    case Response_CmdNotAllowed_RefDataNotUsable:
      sw = 0x6984;
    case Response_CmdNotAllowed_ConditionNotSatisfied:
      sw = 0x6985;
    case Response_CmdNotAllowed_NoCurrentEF:
      sw = 0x6986;
    case Response_CmdNotAllowed_ExpectSecureMsg:
      sw = 0x6981;
    case Response_CmdNotAllowed_IncorrectSecureMsg:
      sw = 0x6981;
    case Response_WrongP1P2:
      sw = 0x6A00;
    case Response_WrongP1P2_IncorrectData:
      sw = 0x6A80;
    case Response_WrongP1P2_FuncNotSupported:
      sw = 0x6A81;
    case Response_WrongP1P2_FileNotFound:
      sw = 0x6A82;
    case Response_WrongP1P2_RecordNotFound:
      sw = 0x6A83;
    case Response_WrongP1P2_NotEnoughMem:
      sw = 0x6A84;
    case Response_WrongP1P2_NCInconsistentTLV:
      sw = 0x6A85;
    case Response_WrongP1P2_IncorrectP1P2:
      sw = 0x6A86;
    case Response_WrongP1P2_NCInconsistentP1P2:
      sw = 0x6A87;
    case Response_WrongP1P2_RefDataNotFound:
      sw = 0x6A88;
    case Response_WrongP1P2_FileExist:
      sw = 0x6A89;
    case Response_WrongP1P2_DFNameExist:
      sw = 0x6A8A;
    case Response_INSNotSupported:
      sw = 0x6D00;
    case Response_CLANotSupported:
      sw = 0x6E00;
    case Response_FatalError:
      sw = 0x6F00;
    }

  sw |= (uint16_t) extra;

}
