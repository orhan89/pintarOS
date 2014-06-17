/**
 * @file state.h
 * @brief Main header file, contain all global definition, data structure, and function
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 7/10/2013
 */

#ifndef STATE_H

#define STATE_H

#define STATE_OK                0
#define STATE_ERROR             1
#define STATE_WRONG             2
#define STATE_BLOCKED           3

/********************************/
/*        Data Structure        */
/********************************/

/**
 * @brief structure of Card State Manager
 */
struct state_struct
{
  uint16_t        current;     ///< pointer to current DF header
  uint16_t        currentKey;    ///< pointer to current Key EF header
  uint16_t        currentRecord;  ///< Record number of currently selected EF
  uint8_t         securityState;  ///< security state currently active
  uint8_t         challenge[CRYPT_BLOCK_LEN];
};

/**************************/
/*        Variable        */
/**************************/


/**************************/
/*        Function        */
/**************************/

/**
 * @brief Initialize State Manager
 *
 * @return Result\n
 *          Success     = STATE_OK\n
 *          Not Success = STATE_ERROR
 */
int State_Init();

/**
 * @brief verify security state with PIN
 *
 * Verify security state using PIN
 *
 * @param newState the number of state to be activated
 * @return Result
 */
int State_ChangeState(int newState);

/**
 * @brief verify security state with PIN
 *
 * Verify security state using PIN
 *
 * @param PIN PIN Number
 * @return Result
 */
int State_Verify(uint8_t *PIN);

void State_GetChallenge( uint8_t * buffer );

/**
 * @brief verify security state with External Authenticate
 *
 * Verify terminal identity
 *
 * @param encrypted encrypted challenge from terminal
 * @return Result
 */
uint8_t State_VerifyAuth( uint8_t * encrypted );

/**
 * @brief Set current file
 *
 * Set state of current file to a new file
 *
 * @param newDF index of current DF in file table
 * @return Result
 */
int State_SetCurrent(uint16_t newfile);

/**
 * @brief Set current EFKey
 *
 * Set state of current EFKey to a new EFKey
 *
 * @param newEFKey index of current EFKey in file table
 * @return Result
 */
int State_SetCurrentKey(uint16_t newKey);

/**
 * @brief Get current file
 *
 * Get index of current selected file
 *
 * @return current DF index
 */
uint16_t State_GetCurrent();

/**
 * @brief Get current security state
 *
 * Get current security state
 *
 * @return current security state
 */
uint8_t State_GetCurrentSecurity();

/**
 * @brief Get current challenge
 *
 * Get current challenge
 *
 * @param buffer address to save challenge
 * @return none
 */
void State_GetCurrentChallenge(uint8_t * buffer);

#endif
