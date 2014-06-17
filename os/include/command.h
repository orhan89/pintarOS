/**
 * @file command.h
 * @brief Header file for command interpreter and ISO command handler
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 7/10/2013
 */

#ifndef COMMAND_H

#define COMMAND_H

/* Command for Debug */
#define DEBUG_WRITE        0x02
#define DEBUG_READ         0x04
#define DEBUG_GETCURRENT   0x22
#define DEBUG_GETSECURITY  0x24
#define DEBUG_GETCHALLENGE 0x28
#define DEBUG_ENCRYPT      0x26
#define DEBUG_FORMAT       0x0a

/* File Operation */
#define	ISO_SELECT              0xA4      ///< ISO 7816-4 SELECT Instruction code
#define	ISO_READ_BINARY         0xB0      ///< ISO 7816-4 READ BINARY Instruction code
#define	ISO_UPDATE_BINARY       0xD6      ///< ISO 7816-4 UPDATE BINARY Instruction code
#define	ISO_READ_RECORD         0xB2      ///< ISO 7816-4 READ RECORD Instruction code
#define	ISO_UPDATE_RECORD       0xDC      ///< ISO 7816-4 UPDATE RECORD Instruction code
#define	ISO_APPEND_RECORD       0xE2      ///< ISO 7816-4 APPEND RECORD Instruction code

/* File Management */
#define	ISO_CREATE_FILE         0xE0      ///< ISO 7816-4 CREATE FILE Instruction code
#define	ISO_DELETE_FILE         0xE4      ///< ISO 7816-4 DELETE FILE Instruction code

/* Security */
#define	ISO_VERIFY              0x20      ///< ISO 7816-4 VERIFY Instruction code
#define	ISO_EXT_AUTH            0x82      ///< ISO 7816-4 EXTERNAL_AUTH Instruction code
#define	ISO_INT_AUTH            0x88      ///< ISO 7816-4 INTERNAL_AUTH Instruction code
#define ISO_GET_CHALLENGE	0x84	//!< INS byte: Get Challenge

/* Program Code Management */
#define	ISO_LOAD                0xDC      ///< ISO 7816-4 LOAD Instruction code
#define	ISO_INSTALL             0xDC      ///< ISO 7816-4 INSTALL Instruction code
#define	ISO_DELETE              0xDC      ///< ISO 7816-4 DELETE Instruction code

/* Data Transmission */
#define	ISO_GET_RESPONSE        0xC0  	///< ISO 7816-4 GET RESPONSE Instruction code

/**************************/
/*        Function        */
/**************************/

/**
 * @brief Interpret command APDU and call appropriate command handler.
 *
 * Call by main loop when finish receiving command APDU header,
 *
 * @return none
 */
void Command_Interpreter();

/********************************/
/* Standard ISO Command Handler */
/********************************/

/**
 * @brief ISO 7816-4 SELECT command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and set corresponding response (status word)
 *
 * @return none
 */
void Command_Select();

/**
 * @brief ISO 7816-4 READ BINARY command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_ReadBinary();

/**
 * @brief ISO 7816-4 UPDATE BINARY command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_UpdateBinary();

/**
 * @brief ISO 7816-4 READ RECORD command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_ReadRecord();

/**
 * @brief ISO 7816-4 UPDATE RECORD command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_UpdateRecord();

/**
 * @brief ISO 7816-4 APPEND RECORD command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_AppendRecord();

/**
 * @brief ISO 7816-4 CREATE FILE command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_CreateFile();

/**
 * @brief ISO 7816-4 DELETE FILE command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_DeleteFile();

/**
 * @brief ISO 7816-4 VERIFY command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_Verify();

/**
 * @brief ISO 7816-4 INTERNAL_AUTH command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_InternalAuth();

/**
 * @brief ISO 7816-4 INTERNAL_AUTH command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_ExternalAuth();

/**
 * @brief ISO 7816-4 GET RESPONSE command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_GetChallenge();

/**
 * @brief ISO 7816-4 LOAD command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_Load();

/**
 * @brief ISO 7816-4 INSTALL command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_Install();

/**
 * @brief ISO 7816-4 DELETE command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_Delete();

/**
 * @brief ISO 7816-4 GET RESPONSE command handler.
 *
 * Call by CommandInterpreter().
 * Executed the selected command and return the response type and data back to apdu_res 
 *
 * @return none
 */
void Command_GetResponse();

#endif
