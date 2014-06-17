/**
 * @file response.h
 * @brief Header file for response manager
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 7/10/2013
 */

#ifndef RESPONSE_H

#define RESPONSE_H

/*****************************/
/*        Enumeration        */
/*****************************/

/**
 * @brief response type enumeration
 */
typedef enum
{
  Response_OK,
  Response_Normal,
  Response_Warning_Unchanged,
  Response_Warning_DataCorrupt,
  Response_Warning_EndOfFile,
  Response_Warning_FileDeactivated,
  Response_Warning_Changed,
  Response_Warning_FilledUp,
  Response_Warning_Counter,
  Response_Error_Unchanged,
  Response_Error_Changed,
  Response_WrongLength,
  Response_NotSupported,
  Response_NotSupported_LogicalChannel,
  Response_NotSupported_SecureMessaging,
  Response_NotSupported_LastCommandExpected,
  Response_NotSupported_CommandChain,
  Response_CmdNotAllowed,
  Response_CmdNotAllowed_Incompatible_FS,
  Response_CmdNotAllowed_SecurityStatus,
  Response_CmdNotAllowed_AuthBlocked,
  Response_CmdNotAllowed_RefDataNotUsable,
  Response_CmdNotAllowed_ConditionNotSatisfied,
  Response_CmdNotAllowed_NoCurrentEF,
  Response_CmdNotAllowed_ExpectSecureMsg,
  Response_CmdNotAllowed_IncorrectSecureMsg,
  Response_WrongP1P2,
  Response_WrongP1P2_IncorrectData,
  Response_WrongP1P2_FuncNotSupported,
  Response_WrongP1P2_FileNotFound,
  Response_WrongP1P2_RecordNotFound,
  Response_WrongP1P2_NotEnoughMem,
  Response_WrongP1P2_NCInconsistentTLV,
  Response_WrongP1P2_IncorrectP1P2,
  Response_WrongP1P2_NCInconsistentP1P2,
  Response_WrongP1P2_RefDataNotFound,
  Response_WrongP1P2_FileExist,
  Response_WrongP1P2_DFNameExist,
  Response_INSNotSupported,
  Response_CLANotSupported,
  Response_FatalError,
} rspn_type;

/**************************/
/*        Function        */
/**************************/

/**
 * @brief set up an appropriate response APDU
 *
 * Call by CommandInterpreter() when finish execute the command.
 * Interpret response type from command handler to Return Code (SW1 SW2),
 * then transmit response APDU (Return Code plus Return data) over transTx()
 *
 * @param *apdu_res     pointer to apdu resources
 * @return none
 */

void Response_SetSW(uint8_t response, uint8_t xtra);

#endif
