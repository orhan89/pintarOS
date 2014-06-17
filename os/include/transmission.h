/**
 * @file transmission.h
 * @brief Header file for transmission handler
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 7/10/2013
 */

#ifndef TRANSMISSION_H

#define TRANSMISSION_H

#define TRANSMISSION_OK         0
#define TRANSMISSION_ERROR      1

/*****************************/
/*        Enumeration        */
/*****************************/

/**
 * @brief Transmission protocol enumeration
 */
typedef enum
{
        T0,     ///< Using T0 Protocol
        T1,     ///< Using T1 Protocol
} t_proto;

/**
 * @brief Transmission Baudrate enumeration
 */
typedef enum
{
        B9600,  ///< Baudrate 9.600 bit/s
        B19200, ///< Baudrate 19.200 bit/s
        B38400, ///< Baudrate 38.400 bit/s
        B111600 ///< Baudrate 111.600 bit/s
} t_baudrate;

/********************************/
/*        Data Structure        */
/********************************/

/**
 * @brief structure of transmission configuration
 */
struct t_config_struct
{
        t_proto         protocol;       ///< transmission protocol to use : T0 (0) or T1 (1)
        t_baudrate      baudrate;       ///< speed (baudrate) of the transmission
};


/**************************/
/*        Variable        */
/**************************/

uint8_t header[5];
uint16_t sw;

struct  t_config_struct tconfig;                ///< transmission configuration

/**************************/
/*        Function        */
/**************************/

/**
 * @brief Initialize the transmission handler
 *
 * @param config The initialization structure
 * @return Result\n
 *          Success     = TRANSMISSION_OK\n
 *          Not Success = TRANSMISSION_ERROR
 */
uint8_t Transmission_Init(struct t_config_struct config);

/**
 * @brief Receive the command APDU header.
 *
 * Call by main loop, then read 5 byte of data by HAL_IO_RxByte(). The command APDU header received then saved in header variable
 *
 * @return none
 */
void Transmission_GetHeader(); 

/**
 * @brief Acknowledge command
 *
 * Send back INS from header
 *
 * @return none
 */
void Transmission_SendACK();

/**
 * @brief NAcknowledge command
 *
 * Send back negation of INS from header
 *
 * @return none
 */
void Transmission_SendNACK();

/**
 * @brief Get Command Data
 *
 * Receive and save data from terminal
 *
 * @param dst address where to save data received
 * @param data len indicate how much data would be received (in byte)
 * @return none
 */
void Transmission_GetData(uint8_t * dst, uint8_t len);

/**
 * @brief Sent Response Data
 *
 * Sent Response Data to terminal
 *
 * @param src address where data to be sent are saved
 * @param data len indicate how much data would be sent (in byte)
 * @return none
 */
void Transmission_SendData(uint8_t * src, uint8_t len);

/**
 * @brief Sent Response Status Word
 *
 * Sent Response status word to terminal
 *
 * @param dst address where to save data received
 * @param data len indicate how much data would be received (in byte)
 * @return none
 */
void Transmission_SendSW();

#endif
