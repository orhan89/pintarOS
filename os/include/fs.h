/**
 * @file fs.h
 * @brief Header file for file system
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 9/7/2013
 */

/** 
 *  @defgroup fs File System
 *  File System Module
 */

#ifndef FS_H

#define FS_H

#define FS_OK                           0

#define FS_ERROR                        30
#define FS_ERROR_INSUFFICIENT_SPACE     31
#define FS_ERROR_NOT_FOUND              32
#define FS_ERROR_DUPLICATE_FID          33
#define FS_ERROR_SECURITY_STATUS        34

#define FS_NONE                         0

#define FS_TAG_MF                       0x3F
#define FS_TAG_DF                       0x4F
#define FS_TAG_EF                       0x5F

#define FS_EF_STRUCTURE_TRANSPARENT     0
#define FS_EF_STRUCTURE_RECORD          1
#define FS_EF_STRUCTURE_CYCLIC          3

#define FS_EF_TYPE_WORKING      0
#define FS_EF_TYPE_INTERNAL     1

#define FS_OP_READ      0
#define FS_OP_UPDATE    1


/*****************************/
/*        Enumeration        */
/*****************************/

/**
 * @brief EF File Structure enumeration
 * @ingroup fs
 */
enum ef_struct
{
        Transparent,    ///< File using transparent structure
        Record,         ///< File using Record structure
	Cyclic          ///< File using Cyclic Record Structure
};

/**
 * @brief EF File Type enumeration
 * @ingroup fs
 */
enum ef_type
{
        Working,        ///< Working type file
        Internal,       ///< Internal type file
};


/****************************/
/*      Data Structure      */
/****************************/

/**
 * @brief structure of EF file header
 * @ingroup fs
 */
struct EF_st
{
        uint16_t        FID;            ///< File identifier
        uint8_t         structure;      ///< file structure : Transparent or Record
        uint8_t         type;           ///< type of file : Working or Internal
        uint8_t         ACRead;         ///< access control for read operation
        uint8_t         ACUpdate;       ///< access control for write operation
        uint8_t         *ptr_body;      ///< pointer to file body
        uint16_t        size;           ///< size of file
} ;

/**
 * @brief structure of DF file descriptor
 * @ingroup fs
 */
struct DF_st
{
        uint16_t                FID;                    ///< File identifier
        char                    DFname[16];             ///< DF name 
        bool                    asc_flag;               ///< indication to application specific code
        int                     (* asc)(int);           ///< pointer to the ASC handler
} ;

/**************************/
/*        Variable        */
/**************************/


/**************************/
/*        Function        */
/**************************/

int FSFormat();

/**
 * @brief Initializer
 * @ingroup fs
 * Initialize file system
 *
 * @return Result
 */
int FSInitialize();

/**
 * @brief FSGetHeader
 * @ingroup fs
 * File system function to retrieve header information of a file
 *
 * @return Result
 */
int FSGetHeader(uint16_t block_addr, uint8_t offset, uint8_t * dest);

int FSCreateHeader(uint8_t tag, uint16_t fid, uint16_t * addr);

uint16_t FSSearchFID(uint16_t fid);

/**
 * @brief select MF
 * @ingroup fs
 * File system function to select MF
 *
 * @return Result
 */
int FSSelectMF();

/**
 * @brief select with full FID
 * @ingroup fs
 * File system function to select a file with full FID
 *
 * @param fid FID of file to select
 * @return Result
 */
int FSSelectFID(uint16_t fid);

/**
 * @brief select with path
 * @ingroup fs
 * File system function to select a file with path
 *
 * @param path pointer to path
 * @param length length of the path
 * @return Result
 */
int FSSelectPath(uint16_t* path, int length);

/**
 * @brief select with short FID
 * @ingroup fs
 * File system function to select a file with short FID
 *
 * @param sfid Short FID of file to select
 * @return Result
 */
int FSSelectSFID(uint8_t sfid);

/**
 * @brief select with name
 * @ingroup fs
 * File system function to select a DF file with name
 *
 * @param DFname pointer to DFname
 * @param length length of DFname
 * @return Result
 */
int FSSelectName(char* DFname, uint8_t length);

/**
 * @brief access a transparent file
 * @ingroup fs
 * File system function to access (read & update) a transparent file
 *
 * @param op operation to perform
 * @param offset offset of the data to read/update
 * @param length length of the data to read/update
 * @param *data pointer to data buffer
 * @return Result
 */
int FSAccessBinary(int op, int offset, int length, uint8_t *databyte);

/**
 * @brief access a record file
 * @ingroup fs
 * File system function to access a record file
 *
 * @param op operation to perform
 * @param recordNum record Number 
 * @param length length of the data
 * @param *data pointer to data buffer
 * @return Result
 */
int FSAccessRecord(int op, int recordNum, int length, uint8_t *databyte);

/**
 * @brief create a new file
 * @ingroup fs
 * File system function to create a file
 *
 * @param desc Descriptor of the file
 * @return Result
 */
int FSCreateFile(int tag, void * desc);

/**
 * @brief delete a file
 * @ingroup fs
 * File system function to delete a file
 *
 * @param *fid FID of the file to delete
 * @return File System Result enum
 */
int FSDeleteFile(uint16_t fid);

int FSAlloc(uint16_t size, uint16_t startBlock, uint16_t endBlock, uint16_t * address);

int FSAllocHeader(uint16_t * address);

int FSAllocBody(uint16_t * address, uint16_t length);

int FSFree(uint16_t address, uint16_t length);

#endif
