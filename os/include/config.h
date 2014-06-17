/**
 * @file config.h
 * @brief Common configuration definition
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 9/7/2013
 */

#define CONFIG_FS_SIZE                  512
#define CONFIG_FS_START                 64
#define CONFIG_FS_BLOCK_SIZE            2
#define CONFIG_FS_FILE_TABLE_SIZE       128

#define MAX_BUFFER_SIZE 32

#define ATR_LEN_ADDR    0x0001
#define ATR_ADDR        0x0002
#define ATR_MAXLEN      24

#define PIN_ADDR                ATR_ADDR+ATR_MAXLEN
#define PIN_LEN                 4
#define PIN_RETRIES_ADDR        PIN_ADDR+PIN_LEN 
#define PIN_RETRIES_LEN         1

#define PIN_MAX_RETRIES         3

#define SERNUM_ADDR             PIN_RETRIES_ADDR + PIN_RETRIES_LEN
#define SERNUM_LEN              8

#define RAND_STATE_ADDR         (SERNUM_ADDR + SERNUM_LEN)
#define RAND_STATE_LEN          32

#define EXT_AUTH_KEY_ADDR       (RAND_STATE_ADDR + RAND_STATE_LEN)
#define EXT_AUTH_KEY_LEN        16

#define EXT_AUTH_RETRIES_ADDR   (EXT_AUTH_KEY_ADDR + EXT_AUTH_KEY_LEN)
#define EXT_AUTH_RETRIES_LEN    1

#define EXT_AUTH_MAX_RETRIES         3
