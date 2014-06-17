/**
 * @file hal.h
 * @brief Header file for HAL (Hardware Abstraction Layer)
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 7/10/2013
 */

#ifndef HAL_H

#define HAL_H

#define HAL_OK          0
#define HAL_ERROR       1

/**************************/
/*        Variable        */
/**************************/

/* int      *Rx_callback_ptr;       ///< pointer of Rx transmission handler to call by the interrupt handler */

/**************************/
/*        Function        */
/**************************/

/**
 * @brief Initialize Hardware
 *
 * @return Result\n
 *          Success     = HAL_OK\n
 *          Not Success = HAL_ERROR
 */
int HAL_Init();

/**
 * @brief Receive 1 byte data.
 *
 * Receive 1 byte data from from serial IO
 *
 * @return the data byte received
 */
uint8_t HAL_IO_RxByte();

/**
 * @brief Transmit 1 byte data.
 *
 * Transmit 1 byte data to serial IO
 *
 * @param data data byte to transmit
 * @return none
 */
void HAL_IO_TxByte(uint8_t ch);

/**
 * @brief Read 1 byte data.
 *
 * Read 1 byte data from non-volatile memory (EEPROM/Flash)
 *
 * @param address virtual address of memory want to read
 * @return the data byte readed
 */
uint8_t HAL_Mem_ReadByte(uint16_t address);

/**
 * @brief Write 1 byte data.
 *
 * Write 1 byte data to non-volatile memory (EEPROM/flash)
 *
 * @param address virtual address of memory want to write
 * @param databyte the data to be write
 * @return none
 */
void HAL_Mem_WriteByte(uint16_t address, uint8_t databyte);

/**
 * @brief Read block of data.
 *
 * Read block of data from non-volatile memory (EEPROM/Flash)
 *
 * @param address virtual address of beginning of memory want to read
 * @param size size of data to be read
 * @param databyte address where the readed data to be saved
 * @return data readed
 */
int HAL_Mem_ReadBlock(uint16_t address, uint16_t size, uint8_t * databyte);

/**
 * @brief Write block of data.
 *
 * write block of data to non-volatile memory (EEPROM/Flash)
 *
 * @param address virtual address of beginning of memory want to read
 * @param size size of data to be write
 * @param databyte address where the data to be write are saved
 * @return data wrote
 */
int HAL_Mem_WriteBlock(uint16_t address, uint16_t size, uint8_t * databyte);

/**
 * @brief Generate Pseudo Random Numbers
 *
 * @param buf address where the pseudo random numbers to be saved
 * @return none
 */
void HAL_RND_GetBlock( uint8_t * buf);

#endif
