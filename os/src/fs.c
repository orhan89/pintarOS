/**
 * @file fs.c
 * @brief Implementation for file system module
 *
 * @author Ricky Hariady (ricky.hariady@enter.web.id)
 *
 * @date 9/7/2013
 */

#include "config.h"
#include "types.h"
#include "hal.h"
#include "crypt.h"
#include "state.h"
#include "fs.h"

/** 
 *  @defgroup fs File System
 *  File System Module
 */

/**
 *  @def FS_SIZE
 *  @ingroup fs
 *  Define the file system size in bytes.
 *  Get the value from CONFIG_FS_SIZE
 */
#define FS_SIZE         CONFIG_FS_SIZE

/**
 *  @def FS_START
 *  @ingroup fs
 *  Define the start address given to file system
 *  Get the value from CONFIG_FS_START
 */
#define FS_START        CONFIG_FS_START

/**
 *  @def FS_BLOCK_SIZE
 *  @ingroup fs
 *  Define the size of block to be used
 *  Get the value from CONFIG_FS_BLOCK_SIZE
 */
#define FS_BLOCK_SIZE   CONFIG_FS_BLOCK_SIZE

/**
 *  @def FS_BLOCKS
 *  @ingroup fs
 *  Define the number of blocks available
 *  Value obtained from FS_SIZE and FS_BLOCK_SIZE
 */
#define FS_BLOCKS       FS_SIZE/FS_BLOCK_SIZE

/**
 *  @defgroup fs_structure File System Structure
 *  @ingroup fs
 *  File System Structure
 *
 *  pintarOS file system consist of three section : Block Allocation Table, File Table and File Body
 *  All value given as block
 */

/**
 *  @def FS_ALLOC_TABLE
 *  @ingroup fs_structure
 *  Start block of Allocation Table Section
 */
#define FS_ALLOC_TABLE_OFFSET   0

/**
 *  @def FS_ALLOC_TABLE_SIZE
 *  @ingroup fs_structure
 *  size of Allocation Table (in blocks)
 */
#define FS_ALLOC_TABLE_SIZE     (FS_BLOCKS/8)/FS_BLOCK_SIZE

/**
 *  @def FS_FILE_TABLE
 *  @ingroup fs_structure
 *  Start block of File Table Section
 */
#define FS_FILE_TABLE_OFFSET    FS_ALLOC_TABLE_OFFSET + FS_ALLOC_TABLE_SIZE

/**
 *  @def FS_FILE_TABLE_SIZE
 *  @ingroup fs_structure
 *  size of File Table (in blocks)
 */
#define FS_FILE_TABLE_SIZE      CONFIG_FS_FILE_TABLE_SIZE/FS_BLOCK_SIZE

/**
 *  @def FS_FILE_BODY
 *  @ingroup fs_structure
 *  Start block of File Body Section
 */
#define FS_FILE_BODY_OFFSET     FS_FILE_TABLE_OFFSET + FS_FILE_TABLE_SIZE

/**
 *  @def FS_FILE_BODY_SIZE
 *  @ingroup fs_structure
 *  size of File Body (in blocks)
 */
#define FS_FILE_BODY_SIZE       FS_BLOCKS - FS_FILE_BODY_OFFSET


/**
 *  @defgroup fs_header File Header Structure
 *  File Header Structure
 *
 *  pintarOS file system consist of three section : Block Allocation Table, File Table and File Body
 *  All value given as byte
 */

/**
 *  @def FS_HEADER_TAG
 *  @ingroup fs_header
 *  Start byte of tag in header
 */
#define FS_HEADER_TAG_OFFSET    0

/**
 *  @def FS_HEADER_TAG_SIZE
 *  @ingroup fs_header
 *  size of tag section (in byte)
 */
#define FS_HEADER_TAG_SIZE      1

/**
 *  @def FS_HEADER_FID
 *  @ingroup fs_header
 *  Start byte of FID in header
 */
#define FS_HEADER_FID_OFFSET    FS_HEADER_TAG_OFFSET + FS_HEADER_TAG_SIZE

/**
 *  @def FS_HEADER_FID_SIZE
 *  @ingroup fs_header
 *  size of FID section (in byte)
 */
#define FS_HEADER_FID_SIZE      2

/**
 *  @def FSHEADER_PARENT
 *  @ingroup fs_header
 *  Start byte of parent in header
 */
#define FS_HEADER_PARENT_OFFSET FS_HEADER_FID_OFFSET + FS_HEADER_FID_SIZE

/**
 *  @def FS_HEADER_PARENT_SIZE
 *  @ingroup fs_header
 *  size of parent section (in byte)
 */
#define FS_HEADER_PARENT_SIZE   2

/**
 *  @def FS_HEADER_CHILD
 *  @ingroup fs_header
 *  Start byte of child in header
 */
#define FS_HEADER_CHILD_OFFSET  FS_HEADER_PARENT_OFFSET + FS_HEADER_PARENT_SIZE

/**
 *  @def FS_HEADER_CHILD_SIZE
 *  @ingroup fs_header
 *  size of child section (in byte)
 */
#define FS_HEADER_CHILD_SIZE    2

/**
 *  @def FS_HEADER_SIBLING
 *  @ingroup fs_header
 *  Start byte of sibling in header
 */
#define FS_HEADER_SIBLING_OFFSET        FS_HEADER_CHILD_OFFSET + FS_HEADER_CHILD_SIZE

/**
 *  @def FS_HEADER_SIBLING_SIZE
 *  @ingroup fs_header
 *  size of sibling section (in byte)
 */
#define FS_HEADER_SIBLING_SIZE  2

/**
 *  @def FS_HEADER_BODY
 *  @ingroup fs_header
 *  Start byte of pointer to body in header
 */
#define FS_HEADER_BODY_OFFSET   FS_HEADER_SIBLING_OFFSET + FS_HEADER_SIBLING_SIZE

/**
 *  @def FS_HEADER_BODY_SIZE
 *  @ingroup fs_header
 *  size of pointer to body (in byte)
 */
#define FS_HEADER_BODY_SIZE     2

/**
 *  @def FS_HEADER_SIZE
 *  @ingroup fs_header
 *  total size of file header
 */
#define FS_HEADER_SIZE     FS_HEADER_TAG_SIZE + \
                           FS_HEADER_FID_SIZE + \
                           FS_HEADER_PARENT_SIZE + \
                           FS_HEADER_CHILD_SIZE + \
                           FS_HEADER_SIBLING_SIZE + \
                           FS_HEADER_BODY_SIZE


#define FS_BODY_STRUCTURE_OFFSET        0

#define FS_BODY_STRUCTURE_SIZE          1

#define FS_BODY_TYPE_OFFSET             FS_BODY_STRUCTURE_OFFSET + FS_BODY_STRUCTURE_SIZE

#define FS_BODY_TYPE_SIZE               1

#define FS_BODY_ACREAD_OFFSET           FS_BODY_TYPE_OFFSET + FS_BODY_TYPE_SIZE

#define FS_BODY_ACREAD_SIZE             1

#define FS_BODY_ACUPDATE_OFFSET         FS_BODY_ACREAD_OFFSET + FS_BODY_ACREAD_SIZE

#define FS_BODY_ACUPDATE_SIZE           1

#define FS_BODY_SIZE_OFFSET             FS_BODY_ACUPDATE_OFFSET + FS_BODY_ACUPDATE_SIZE

#define FS_BODY_SIZE_SIZE               2

#define FS_BODY_HEADER_SIZE             FS_BODY_STRUCTURE_SIZE + \
					FS_BODY_TYPE_SIZE + \
					FS_BODY_ACREAD_SIZE + \
					FS_BODY_ACUPDATE_SIZE + \
					FS_BODY_SIZE_SIZE

#define FS_BODY_BODY_OFFSET             FS_BODY_SIZE_OFFSET + FS_BODY_SIZE_SIZE


#define FS_ALLOC_HEADER(address) FSAlloc(CEIL((FS_HEADER_SIZE),FS_BLOCK_SIZE), FS_FILE_TABLE_OFFSET, FS_FILE_BODY_OFFSET, address)

#define FS_ALLOC_BODY(address, length) FSAlloc(CEIL((FS_BODY_HEADER_SIZE + length),FS_BLOCK_SIZE), FS_FILE_BODY_OFFSET, FS_BLOCKS, address);



#define FS_SET_HEADER_TAG(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_TAG_OFFSET, FS_HEADER_TAG_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_TAG(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_TAG_OFFSET, FS_HEADER_TAG_SIZE, (uint8_t *)dest)

#define FS_SET_HEADER_FID(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_FID_OFFSET, FS_HEADER_FID_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_FID(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_FID_OFFSET, FS_HEADER_FID_SIZE, (uint8_t *)dest)

#define FS_SET_HEADER_PARENT(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_PARENT_OFFSET, FS_HEADER_PARENT_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_PARENT(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_PARENT_OFFSET, FS_HEADER_PARENT_SIZE, (uint8_t *)dest)

#define FS_SET_HEADER_CHILD(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_CHILD_OFFSET, FS_HEADER_CHILD_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_CHILD(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_CHILD_OFFSET, FS_HEADER_CHILD_SIZE, (uint8_t *)dest)

#define FS_SET_HEADER_SIBLING(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_SIBLING_OFFSET, FS_HEADER_SIBLING_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_SIBLING(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_SIBLING_OFFSET, FS_HEADER_SIBLING_SIZE, (uint8_t *)dest)

#define FS_SET_HEADER_BODY(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_BODY_OFFSET, FS_HEADER_BODY_SIZE, (uint8_t *)src)

#define FS_GET_HEADER_BODY(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_HEADER_BODY_OFFSET, FS_HEADER_BODY_SIZE, (uint8_t *)dest)



#define FS_SET_BODY_STRUCTURE(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_STRUCTURE_OFFSET, FS_BODY_STRUCTURE_SIZE, (uint8_t *)src)

#define FS_GET_BODY_STRUCTURE(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_STRUCTURE_OFFSET, FS_BODY_STRUCTURE_SIZE, (uint8_t *)dest)

#define FS_SET_BODY_TYPE(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_TYPE_OFFSET, FS_BODY_TYPE_SIZE, (uint8_t *)src)

#define FS_GET_BODY_TYPE(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_TYPE_OFFSET, FS_BODY_TYPE_SIZE, (uint8_t *)dest)

#define FS_SET_BODY_ACREAD(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_ACREAD_OFFSET, FS_BODY_ACREAD_SIZE, (uint8_t *)src)

#define FS_GET_BODY_ACREAD(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_ACREAD_OFFSET, FS_BODY_ACREAD_SIZE, (uint8_t *)dest)

#define FS_SET_BODY_ACUPDATE(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_ACUPDATE_OFFSET, FS_BODY_ACUPDATE_SIZE, (uint8_t *)src)

#define FS_GET_BODY_ACUPDATE(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_ACUPDATE_OFFSET, FS_BODY_ACUPDATE_SIZE, (uint8_t *)dest)

#define FS_SET_BODY_SIZE(block, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_SIZE_OFFSET, FS_BODY_SIZE_SIZE, (uint8_t *)src)

#define FS_GET_BODY_SIZE(block, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_SIZE_OFFSET, FS_BODY_SIZE_SIZE, (uint8_t *)dest)

#define FS_SET_BODY_BODY(block, length, src) Memory_WriteBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_BODY_OFFSET + offset, length, (uint8_t *)src)

#define FS_GET_BODY_BODY(block, length, dest) Memory_ReadBlock(FS_START + (block * FS_BLOCK_SIZE) + FS_BODY_BODY_OFFSET + offset, length, (uint8_t *)dest)

#define CEIL(A,B)       ((A%B)==0 ? (A/B) : (A/B + 1))

int FS_Init()
{
  FSSelectMF();

  return FS_OK;
}

/* int FSGetHeader(uint16_t block_addr, uint8_t offset, uint8_t * dest) */
/* { */

/* } */

int FSSelectMF()
{
  State_SetCurrent((uint16_t) FS_FILE_TABLE_OFFSET);

  return 0;
}

/* int FSSelectFID(uint16_t fid) */
/* { */

/* } */

/* int FSSelectPath(uint16_t* path, int length) */
/* { */

/* } */

/* int FSSelectSFID(uint8_t sfid) */
/* { */

/* } */

/* int FSSelectName(char* DFname, uint8_t length) */
/* { */

/* } */

int FSAccessBinary(int op, int offset, int length, uint8_t *databyte)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSAccessBinary(): Entering FSAccessBinary()\n",debugLevel);
#endif

  uint16_t header, body;

  header = State_GetCurrent();
  /* HAL_IO_TxByte( (uint8_t)(header>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)header ); */

  FS_GET_HEADER_BODY(header,&body);
  /* Memory_ReadBlock((header * FS_BLOCK_SIZE) + FS_HEADER_BODY_OFFSET, FS_HEADER_BODY_SIZE, (uint8_t *)&body); */

  if(op == FS_OP_READ){
    if(length == 0)
      FS_GET_BODY_SIZE(body, &length);

    if(length > 256)
      length = 256;

    FS_GET_BODY_BODY(body, length, databyte);

#if DEBUG
    int i;

    printf ("<%u>FSAccessBinary(): Read Binary\n",debugLevel);
    printf ("<%u>FSAccessBinary(): Header = %u\n",debugLevel, header);
    printf ("<%u>FSAccessBinary(): Body   = %u\n",debugLevel, body);
    printf ("<%u>FSAccessBinary(): Offset = %u Byte(s)\n",debugLevel, offset);
    printf ("<%u>FSAccessBinary(): Length = %u Byte(s)\n",debugLevel, length);
    printf ("<%u>FSAccessBinary(): Data :",debugLevel);
    for(i=0;i<length;i++){
      if((i%16) == 0) printf ("\n");
      printf ("%x ",data[i]);
    }
    printf ("\n");
#endif

  }
  else if(op = FS_OP_UPDATE){
    FS_SET_BODY_BODY(body, length, databyte);
  }

#if DEBUG
  printf ("<%u>FSAccessBinary(): SUCCESS : Data Readed\n",debugLevel);
  printf ("<%u>FSAccessBinary(): Exiting FSAccessBinary()\n",debugLevel);
  debugLevel--;
#endif

  return length;

}

/* int FSAccessRecord(int op, int recordNum, int length, uint8_t *data) */
/* { */

/* } */

int FSFormat()
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSFormat(): Entering FSFormat()\n",debugLevel);
#endif

  uint16_t i;
  uint16_t headerBlock;
  uint16_t none16 = FS_NONE;
  uint16_t fidMF  = 0x3F00;
  uint8_t  tagMF  = FS_TAG_MF;
  int status;

  //Initialize Block Allocation Table
  /* for(i=32; i<32+32; i++) */
  for(i=FS_ALLOC_TABLE_OFFSET; i<(FS_FILE_TABLE_OFFSET*FS_BLOCK_SIZE); i++)
    Memory_WriteByte(FS_START + i, 0);

  status = FS_ALLOC_HEADER(&headerBlock);
  /* HAL_IO_TxByte( (uint8_t)(headerBlock>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)headerBlock ); */
  /* HAL_IO_TxByte( (uint8_t)(FS_ALLOC_TABLE_SIZE>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)FS_ALLOC_TABLE_SIZE ); */
  /* HAL_IO_TxByte( (uint8_t)status ); */

  if(status == FS_OK && headerBlock == FS_FILE_TABLE_OFFSET){
    FS_SET_HEADER_TAG(headerBlock, &tagMF);
    FS_SET_HEADER_FID(headerBlock, &fidMF);
    FS_SET_HEADER_PARENT(headerBlock, &headerBlock);
    FS_SET_HEADER_CHILD(headerBlock, &none16);
    FS_SET_HEADER_SIBLING(headerBlock, &none16);

#if DEBUG
    int i;
    uint8_t temp, tempTag;
    uint16_t tempFID, tempParent, tempChild, tempSibling;

    printf ("<%u>FSFormat(): MF Created\n",debugLevel);
    printf ("<%u>FSFormat(): Address = %u\n",debugLevel,headerBlock);
    printf ("<%u>FSFormat(): Header : ",debugLevel);
    for(i=0;i<FS_HEADER_SIZE;i++){
      temp = Memory_ReadByte((headerBlock * FS_BLOCK_SIZE) + i);
      printf ("%x ",temp);
    }
    printf ("\n");
    FS_GET_HEADER_TAG(headerBlock, &tempTag);
    FS_GET_HEADER_FID(headerBlock, &tempFID);
    FS_GET_HEADER_PARENT(headerBlock, &tempParent);
    FS_GET_HEADER_CHILD(headerBlock, &tempChild);
    FS_GET_HEADER_SIBLING(headerBlock, &tempSibling);
    printf ("<%u>FSFormat(): Tag          = 0x%x\n",debugLevel,tempTag);
    printf ("<%u>FSFormat(): FID          = 0x%x\n",debugLevel,tempFID);
    printf ("<%u>FSFormat(): Parent       = %u\n",debugLevel,tempParent);
    printf ("<%u>FSFormat(): Child        = %u\n",debugLevel,tempChild);
    printf ("<%u>FSFormat(): Sibling      = %u\n",debugLevel,tempSibling);

    printf ("<%u>FSFormat(): Exiting FSFormat()\n",debugLevel);
    debugLevel--;
#endif

    return FS_OK;
  }
  else {

#if DEBUG
    printf ("<%u>FSFormat(): Error while Formatting File System\n",debugLevel);

    printf ("<%u>FSFormat(): Exiting FSFormat()\n",debugLevel);
    debugLevel--;
#endif

    return FS_ERROR;
  }
}

int FSCreateHeader(uint8_t tag, uint16_t fid, uint16_t * addr)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSCreateHeader(): Entering FSCreateHeader()\n",debugLevel);
#endif

  uint16_t      current, currentChild, currentChildSibling, currentChildTemp;
  uint16_t      headerBlock;
  uint16_t      none16 = (uint16_t)FS_NONE;
  int           status;

  current = State_GetCurrent();

  //alloc some space for header
  status = FS_ALLOC_HEADER(&headerBlock);
  /* HAL_IO_TxByte( (uint8_t)(headerBlock>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)headerBlock ); */

  if(status == FS_ERROR_INSUFFICIENT_SPACE){

#if DEBUG
  printf ("<%u>FSCreateHeader(): ERROR : Insufficient Space\n",debugLevel);
  printf ("<%u>FSCreateHeader(): Exiting FSCreateHeader()\n",debugLevel);
  debugLevel--;
#endif

    return status;
  }

  //update header
  FS_SET_HEADER_TAG(headerBlock, &tag);
  FS_SET_HEADER_FID(headerBlock, &fid);
  FS_SET_HEADER_PARENT(headerBlock, &current);
  FS_SET_HEADER_CHILD(headerBlock, &none16);
  FS_SET_HEADER_SIBLING(headerBlock, &none16);

  //update parent/sibling DF header
  FS_GET_HEADER_CHILD(current, &currentChild);

  if (currentChild == none16) {
    FS_SET_HEADER_CHILD(current, &headerBlock);
  }
  else {

    while (currentChild != FS_NONE){

      currentChildTemp = currentChild;

      FS_GET_HEADER_SIBLING(currentChild, &currentChild);

    }

    currentChild = currentChildTemp;

    FS_SET_HEADER_SIBLING(currentChild, &headerBlock);

  }

  *addr = headerBlock;

#if DEBUG
  int i;
  uint8_t temp, tempTag;
  uint16_t tempFID, tempParent, tempChild, tempSibling;

  printf ("<%u>FSCreateHeader(): Header Created\n",debugLevel);
  printf ("<%u>FSCreateHeader(): Address = %u\n",debugLevel,headerBlock);
  printf ("<%u>FSCreateHeader(): Header : ",debugLevel);
  for(i=0;i<FS_HEADER_SIZE;i++){
    temp = Memory_ReadByte((headerBlock * FS_BLOCK_SIZE) + i);
    printf ("%x ",temp);
  }
  printf ("\n");

  FS_GET_HEADER_TAG(headerBlock, &tempTag);
  FS_GET_HEADER_FID(headerBlock, &tempFID);
  FS_GET_HEADER_PARENT(headerBlock, &tempParent);
  FS_GET_HEADER_CHILD(headerBlock, &tempChild);
  FS_GET_HEADER_SIBLING(headerBlock, &tempSibling);

  printf ("<%u>FSCreateHeader(): Tag          = 0x%x\n",debugLevel,tempTag);
  printf ("<%u>FSCreateHeader(): FID          = 0x%x\n",debugLevel,tempFID);
  printf ("<%u>FSCreateHeader(): Parent       = %u\n",debugLevel,tempParent);
  printf ("<%u>FSCreateHeader(): Child        = %u\n",debugLevel,tempChild);
  printf ("<%u>FSCreateHeader(): Sibling      = %u\n",debugLevel,tempSibling);

  FS_GET_HEADER_TAG(current, &tempTag);
  FS_GET_HEADER_FID(current, &tempFID);
  FS_GET_HEADER_PARENT(current, &tempParent);
  FS_GET_HEADER_CHILD(current, &tempChild);
  FS_GET_HEADER_SIBLING(current, &tempSibling);

  printf ("<%u>FSCreateHeader(): Parent Address = %u\n",debugLevel,current);
  printf ("<%u>FSCreateHeader(): Parent Tag     = 0x%x\n",debugLevel,tempTag);
  printf ("<%u>FSCreateHeader(): Parent FID     = 0x%x\n",debugLevel,tempFID);
  printf ("<%u>FSCreateHeader(): Parent Parent  = %u\n",debugLevel,tempParent);
  printf ("<%u>FSCreateHeader(): Parent Child   = %u\n",debugLevel,tempChild);
  printf ("<%u>FSCreateHeader(): Parent Sibling = %u\n",debugLevel,tempSibling);

  currentChild = tempChild;
  i = 1;

  while(currentChild != headerBlock) {

    FS_GET_HEADER_TAG(currentChild, &tempTag);
    FS_GET_HEADER_FID(currentChild, &tempFID);
    FS_GET_HEADER_PARENT(currentChild, &tempParent);
    FS_GET_HEADER_CHILD(currentChild, &tempChild);
    FS_GET_HEADER_SIBLING(currentChild, &tempSibling);

    printf ("<%u>FSCreateHeader(): Sibling (%d) Address = %u\n",debugLevel,i,currentChild);
    printf ("<%u>FSCreateHeader(): Sibling (%d) Tag     = 0x%x\n",debugLevel,i,tempTag);
    printf ("<%u>FSCreateHeader(): Sibling (%d) FID     = 0x%x\n",debugLevel,i,tempFID);
    printf ("<%u>FSCreateHeader(): Sibling (%d) Parent  = %u\n",debugLevel,i,tempParent);
    printf ("<%u>FSCreateHeader(): Sibling (%d) Child   = %u\n",debugLevel,i,tempChild);
    printf ("<%u>FSCreateHeader(): Sibling (%d) Sibling = %u\n",debugLevel,i,tempSibling);

    currentChild = tempSibling;
    i++;
  }

  printf ("<%u>FSCreateHeader(): SUCCESS : Header Created\n",debugLevel);
  printf ("<%u>FSCreateHeader(): Exiting FSCreateHeader()\n",debugLevel);
  debugLevel--;
#endif

  return FS_OK;

}

int FSCreateBodyEF(struct EF_st * desc, uint16_t * addr)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSCreateBodyEF(): Entering FSCreateBodyEF()\n",debugLevel);
#endif

  uint16_t bodyBlock;

  //alloc body
  FS_ALLOC_BODY(&bodyBlock, desc->size);

  FS_SET_BODY_STRUCTURE(bodyBlock, &(desc->structure));
  FS_SET_BODY_TYPE(bodyBlock, &(desc->type));
  FS_SET_BODY_ACREAD(bodyBlock, &(desc->ACRead));
  FS_SET_BODY_ACUPDATE(bodyBlock, &(desc->ACUpdate));
  FS_SET_BODY_SIZE(bodyBlock, &(desc->size));

  *addr = bodyBlock;


#if DEBUG
  int i;
  uint8_t temp, tempStructure, tempType, tempACRead, tempACUpdate;
  uint16_t tempSize;

  printf ("<%u>FSCreateBodyEF(): File Body Created\n",debugLevel);
  printf ("<%u>FSCreateBodyEF(): Block Address = %u\n",debugLevel,bodyBlock);
  printf ("<%u>FSCreateBodyEF(): Header : ",debugLevel);
  for(i=0;i<FS_BODY_HEADER_SIZE;i++){
    temp = Memory_ReadByte((bodyBlock * FS_BLOCK_SIZE) + i);
    printf ("%x ",temp);
  }
  printf ("\n");

  FS_GET_BODY_STRUCTURE(bodyBlock, &tempStructure);
  FS_GET_BODY_TYPE(bodyBlock, &tempType);
  FS_GET_BODY_ACREAD(bodyBlock, &tempACRead);
  FS_GET_BODY_ACUPDATE(bodyBlock, &tempACUpdate);
  FS_GET_BODY_SIZE(bodyBlock, &tempSize);

  printf ("<%u>FSCreateBodyEF(): Structure  = %u\n",debugLevel,tempStructure);
  printf ("<%u>FSCreateBodyEF(): Type       = %u\n",debugLevel,tempType);
  printf ("<%u>FSCreateBodyEF(): ACRead     = %u\n",debugLevel,tempACRead);
  printf ("<%u>FSCreateBodyEF(): ACUpdate   = %u\n",debugLevel,tempACUpdate);
  printf ("<%u>FSCreateBodyEF(): Size       = %u\n",debugLevel,tempSize);

  printf ("<%u>FSCreateBodyEF(): SUCCESS : Body Created\n",debugLevel);
  printf ("<%u>FSCreateBodyEF(): Exiting FSCreateBodyEF()\n",debugLevel);
  debugLevel--;
#endif

  return FS_OK;

}

uint16_t FSSearchFID(uint16_t fid)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSSearchFID(): Entering FSSearchFID()\n",debugLevel);
#endif

  uint16_t current, parent, sibling, child;
  uint16_t tempFID;
  uint8_t  tempTag;

  /* HAL_IO_TxByte( (uint8_t)(fid >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)fid & 0xff ); */

  current = State_GetCurrent();
  /* HAL_IO_TxByte( (uint8_t)(FS_FILE_TABLE_OFFSET >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)FS_FILE_TABLE_OFFSET & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)(current >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)current & 0xff ); */

  //check parent
  FS_GET_HEADER_PARENT(current, &parent);
  /* HAL_IO_TxByte( (uint8_t)(parent >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)parent & 0xff ); */

  FS_GET_HEADER_FID(parent, &tempFID);
  /* HAL_IO_TxByte( (uint8_t)(tempFID >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)tempFID & 0xff ); */

  if(tempFID == fid){

#ifdef DEBUG
    printf ("<%u>FSSearchFID(): File with FID 0x%x found at %u\n",debugLevel, fid, parent);

    printf ("<%u>FSSearchFID(): Exiting FSSearchFID()\n",debugLevel);
    debugLevel--;
#endif

    return parent;
  }

  //check sibling
  FS_GET_HEADER_CHILD(parent, &sibling);
  /* HAL_IO_TxByte( (uint8_t)(sibling >> 8) & 0xff ); */
  /* HAL_IO_TxByte( (uint8_t)sibling & 0xff ); */

  while(sibling != FS_NONE) {

    FS_GET_HEADER_FID(sibling, &tempFID);
    /* HAL_IO_TxByte( (uint8_t)(tempFID >> 8) & 0xff ); */
    /* HAL_IO_TxByte( (uint8_t)tempFID & 0xff ); */

    if(tempFID == fid){

#ifdef DEBUG
      printf ("<%u>FSSearchFID(): File with FID 0x%x found at %d\n",debugLevel, fid, sibling);

      printf ("<%u>FSSearchFID(): Exiting FSSearchFID()\n",debugLevel);
      debugLevel--;
#endif
      /* HAL_IO_TxByte( (uint8_t)0x69 ); */
      /* HAL_IO_TxByte( (uint8_t)(sibling >> 8) & 0xff ); */
      /* HAL_IO_TxByte( (uint8_t)sibling & 0xff ); */

      return sibling;
    }

    FS_GET_HEADER_SIBLING(sibling, &sibling);

  }

  //check child

  FS_GET_HEADER_TAG(current, &tempTag);

  if(tempTag == FS_TAG_DF || tempTag == FS_TAG_MF) {

    FS_GET_HEADER_CHILD(current, &child);
    /* HAL_IO_TxByte( (uint8_t)(child >> 8) & 0xff ); */
    /* HAL_IO_TxByte( (uint8_t)child & 0xff ); */

    while (child != FS_NONE)
    {

      FS_GET_HEADER_FID(child, &tempFID);
      /* HAL_IO_TxByte( (uint8_t)(tempFID >> 8) & 0xff ); */
      /* HAL_IO_TxByte( (uint8_t)tempFID & 0xff ); */

      if(tempFID == fid){

#ifdef DEBUG
        printf ("<%u>FSSearchFID(): File with FID 0x%x found at %d\n",debugLevel, fid, child);

        printf ("<%u>FSSearchFID(): Exiting FSSearchFID()\n",debugLevel);
        debugLevel--;
#endif
      /* HAL_IO_TxByte( (uint8_t)0x96 ); */
      /* HAL_IO_TxByte( (uint8_t)(child >> 8) & 0xff ); */
      /* HAL_IO_TxByte( (uint8_t)child & 0xff ); */

        return child;
      }

      FS_GET_HEADER_SIBLING(child, &child);

    }
  }

#ifdef DEBUG
  printf ("<%u>FSSearchFID(): File with FID 0x%x not found\n",debugLevel, fid);

  printf ("<%u>FSSearchFID(): Exiting FSSearchFID()\n",debugLevel);
  debugLevel--;
#endif
  /* HAL_IO_TxByte( (uint8_t)0x99 ); */
  return FS_NONE;

}


uint16_t FS_SelectFID(uint16_t fid)
{
  uint16_t file;

  /* HAL_IO_TxByte( (uint8_t)0x96 ); */

  file = FSSearchFID(fid);

  if( file == FS_NONE )
    return FS_ERROR_NOT_FOUND;

  /* HAL_IO_TxByte( (uint8_t)(file>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)file ); */

  State_SetCurrent(file);

  return FS_OK;
}


int FSCreateFile(int tag, void * desc)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSCreateFile(): Entering FSCreateFile()\n",debugLevel);
#endif

  uint16_t header, body;
  uint16_t current;
  uint16_t fid;
  uint8_t  currentTag;
  int status;

  if(tag == FS_TAG_DF){
    fid = ((struct DF_st *)desc)->FID;
  }
  else if (tag == FS_TAG_EF) {
    fid = ((struct EF_st *)desc)->FID;
  }

  /* HAL_IO_TxByte( (uint8_t)(fid>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)fid ); */

  //check if current is DF
  current = State_GetCurrent();
  /* HAL_IO_TxByte( (uint8_t)(current>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)current ); */
  
  FS_GET_HEADER_TAG(current, (uint8_t*)&currentTag);
  /* HAL_IO_TxByte( (uint8_t)currentTag ); */
  /* HAL_IO_TxByte( 0x90 ); */
  
  if(currentTag == FS_TAG_EF){

#if DEBUG
    printf ("<%u>FSCreateFile(): ERROR : Creating file under EF\n",debugLevel);
    printf ("<%u>FSCreateFile(): Exiting FSCreateFile()\n",debugLevel);
    debugLevel--;
#endif

    return FS_ERROR;
  }

  //check consistency

  //check all FID of current DF
  status = FSSearchFID(fid);

  if(status != FS_NONE){

#if DEBUG
    printf ("<%u>FSCreateFile(): ERROR : There is a file with same FID within current file()\n",debugLevel);
    printf ("<%u>FSCreateFile(): Exiting FSCreateFile()\n",debugLevel);
    debugLevel--;
#endif

    return FS_ERROR_DUPLICATE_FID;
  }

  //create file header
  status = FSCreateHeader((uint8_t)tag, fid, &header);

  /* HAL_IO_TxByte( (uint8_t)(header>>8) ); */
  /* HAL_IO_TxByte( (uint8_t)header ); */

  if (status != FS_OK){

#if DEBUG
    printf ("<%u>FSCreateFile(): ERROR : Failed to create File Header\n",debugLevel);
    printf ("<%u>FSCreateFile(): Exiting FSCreateFile()\n",debugLevel);
    debugLevel--;
#endif

    return status;
  }

  //create file body
  if(tag == FS_TAG_EF) {
    status = FSCreateBodyEF((struct EF_st *)desc, &body);

    if (status != FS_OK){

#if DEBUG
      printf ("<%u>FSCreateFile(): ERROR : Failed to create File Body\n",debugLevel);
      printf ("<%u>FSCreateFile(): Exiting FSCreateFile()\n",debugLevel);
      debugLevel--;
#endif

      return status;
    }

    FS_SET_HEADER_BODY(header, &body);

  }

#if DEBUG
  printf ("<%u>FSCreateFile(): SUCCESS: File Created\n",debugLevel);
  printf ("<%u>FSCreateFile(): Exiting FSCreateFile()\n",debugLevel);
  debugLevel--;
#endif

  return 0;
}

int FSDeleteFile(uint16_t fid)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSDeleteFile(): Entering FSDeleteFile()\n",debugLevel);
#endif

  uint16_t header, parent, sibling, child, siblingBefore, siblingNext;
  uint16_t body, bodySize;
  uint8_t  tag;

  //check FID
  header = FSSearchFID(fid);

  if(header == FS_NONE){

#if DEBUG
    printf ("<%u>FSDeleteFile(): ERROR : File is not exist\n",debugLevel);
    printf ("<%u>FSDeleteFile(): Exiting FSDeleteFile()\n",debugLevel);
    debugLevel--;
#endif

    return FS_ERROR_NOT_FOUND;
  }

  //if DF, make sure it doesn't have any child

  //if first child, untie from parent, and change parent's child to sibling (if any)
  FS_GET_HEADER_PARENT(header, &parent);

  FS_GET_HEADER_CHILD(parent, &sibling);

  if(sibling == header) {
    FS_GET_HEADER_SIBLING(header, &sibling);
    FS_SET_HEADER_CHILD(parent, &sibling);
  }
  //else, change the sibling before to chain to sibling after
  else {

    while(sibling != header) {
      siblingBefore = sibling;
      FS_GET_HEADER_SIBLING(sibling, &sibling);
    }

    FS_GET_HEADER_SIBLING(header, &siblingNext);
    FS_SET_HEADER_SIBLING(siblingBefore, &siblingNext);
  }

  //free body
  FS_GET_HEADER_TAG(header, &tag);

  if(tag == FS_TAG_EF){
    FS_GET_HEADER_BODY(header, &body);
    FS_GET_BODY_SIZE(body, &bodySize);

#if DEBUG
    printf ("<%u>FSDeleteFile(): Free File Body\n",debugLevel);
    printf ("<%u>FSDeleteFile(): Body Address = %u\n",debugLevel, body);
    printf ("<%u>FSDeleteFile(): Body Size    = %u\n",debugLevel, bodySize);
#endif

    FSFree(body, CEIL(((FS_BODY_HEADER_SIZE)+bodySize),FS_BLOCK_SIZE));
  }

  //free header
#if DEBUG
  printf ("<%u>FSDeleteFile(): Free File Body\n",debugLevel);
  printf ("<%u>FSDeleteFile(): Header Address = %u\n",debugLevel, header);

#endif
  FSFree(header, CEIL((FS_HEADER_SIZE),FS_BLOCK_SIZE));

#if DEBUG
  printf ("<%u>FSDeleteFile(): SUCCESS : File Deleted\n",debugLevel);
  printf ("<%u>FSDeleteFile(): Exiting FSDeleteFile()\n",debugLevel);
  debugLevel--;
#endif

  return FS_OK;
}

int FSAlloc(uint16_t size, uint16_t startBlock, uint16_t endBlock, uint16_t * address)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSAlloc(): Entering FSAlloc()\n",debugLevel);
#endif

  uint16_t i, j, k, m, n;
  uint16_t startbyte, startbit, startSearchBit;
  uint16_t stopbyte, stopbit, stopSearchBit;
  uint16_t tempAddress;
  uint16_t free = 0;
  uint8_t temp = 0;
  bool cont = 0;

  for(i = (startBlock/8); i < (endBlock/8); i++) {

    //read allocation table (byte -> 8 block)
    temp = Memory_ReadByte(FS_START + FS_ALLOC_TABLE_OFFSET + i);

    startSearchBit = (i == startBlock/8) ? startBlock%8 : 0;
    stopSearchBit  = (i == (endBlock/8)-1) ? endBlock%8 : 8;

    // bit per bit operation
    for(j = startSearchBit; j < stopSearchBit; j++) {

      //check if MSB = 0
      if((temp & 0x80) == 0) {

        //check if this the first 0
        if(cont == 0) {

          //make a mark and save the address
          cont = 1;
          tempAddress = (i*8)+j;
          startbyte = i;
          startbit = j;

        }

        //increment free block
        free ++;

        //if free block is sufficient
        if(free == size) {

          stopbyte = i;
          stopbit = j;

          k = startbyte;
          while(k <= stopbyte) {

            m = 0;
            n = 8;

            if(k == startbyte) m = startbit;
            if(k == stopbyte) n = stopbit;

            for(m;m<=n;m++){
              temp = Memory_ReadByte(FS_START + FS_ALLOC_TABLE_OFFSET + k);
              temp = temp | ((uint8_t)128>>m);
              Memory_WriteByte(FS_START + FS_ALLOC_TABLE_OFFSET + k, temp);
            }

            k++;

          }

          //return the first address of free block
          *address = tempAddress;

#if DEBUG
          printf ("<%u>FSAlloc(): Block(s) Allocated\n",debugLevel);
          printf ("<%u>FSAlloc(): Block Address = %u\n",debugLevel,tempAddress);
          printf ("<%u>FSAlloc(): size = %u Block(s)\n",debugLevel,size);
          printf ("<%u>FSAlloc(): startbyte = %u\n",debugLevel,startbyte);
          printf ("<%u>FSAlloc(): startbit = %u\n",debugLevel,startbit);
          printf ("<%u>FSAlloc(): stopbyte = %u\n",debugLevel,stopbyte);
          printf ("<%u>FSAlloc(): stopbit = %u\n",debugLevel,stopbit);
          for(i = startbyte; i <= stopbyte; i++){
            temp = Memory_ReadByte(FS_ALLOC_TABLE_OFFSET + i);
            printf("<%u>FSAlloc(): Allocation table (%u) = %x\n",debugLevel, i, temp);
          }

          printf ("<%u>FSAlloc(): SUCCESS : Block(s) Allocated\n",debugLevel);
          printf ("<%u>FSAlloc(): Exiting FSAlloc()\n",debugLevel);
          debugLevel--;
#endif

          return FS_OK;

        }

      }
      else {

        //if the MSB == 1 and continue
        if(cont == 1) {

          cont = 0;
          free = 0;

        }

      }

      temp = temp << 1;

    }

  }

#if DEBUG
  printf ("<%u>FSAlloc(): ERROR : Insufficient Space\n",debugLevel);
  printf ("<%u>FSAlloc(): Exiting FSAlloc()\n",debugLevel);
  debugLevel--;
#endif

  return FS_ERROR_INSUFFICIENT_SPACE;

}

int FSFree(uint16_t address, uint16_t length)
{

#if DEBUG
  debugLevel++;
  printf ("<%u>FSFree(): Entering FSFree()\n",debugLevel);
#endif

  uint16_t startbyte, stopbyte;
  uint8_t startbit, stopbit;
  uint8_t startXOR, stopXOR;
  uint8_t temp, tempXOR;
  int i,j;

  startbyte = address/8;
  startbit = address%8;
  stopbyte = (address+length)/8;
  stopbit = (address+length)%8;

  for(i = startbyte; i <= stopbyte; i++){
    temp = Memory_ReadByte(FS_START + FS_ALLOC_TABLE_OFFSET + i);

    startXOR = (i == startbyte) ? startbit : 0;
    stopXOR  = (i == stopbyte) ? stopbit : 8;

    tempXOR = 0;

    for(j = startXOR; j < stopXOR; j++)
      tempXOR = tempXOR ^ (128 >> j);

    temp = temp ^ tempXOR;

    Memory_WriteByte(FS_START + FS_ALLOC_TABLE_OFFSET + i, temp);
  }

#if DEBUG
          printf ("<%u>FSFree(): Block(s) Freed\n",debugLevel);
          printf ("<%u>FSFree(): Block Address = %u\n",debugLevel,address);
          printf ("<%u>FSFree(): size = %u Block(s)\n",debugLevel,length);
          printf ("<%u>FSFree(): startbyte = %u\n",debugLevel,startbyte);
          printf ("<%u>FSFree(): startbit = %u\n",debugLevel,startbit);
          printf ("<%u>FSFree(): stopbyte = %u\n",debugLevel,stopbyte);
          printf ("<%u>FSFree(): stopbit = %u\n",debugLevel,stopbit);
          for(i = startbyte; i <= stopbyte; i++){
            temp = Memory_ReadByte(FS_ALLOC_TABLE_OFFSET + i);
            printf("<%u>FSFree(): allocation table (%u) = %x\n",debugLevel, i, temp);
          }

          printf ("<%u>FSFree(): SUCCESS : Block Freed()\n",debugLevel);
          printf ("<%u>FSFree(): Exiting FSFree()\n",debugLevel);
          debugLevel--;
#endif

  return FS_OK;

}

uint8_t FS_GetAC(int op)
{
  uint16_t header, body;
  uint8_t ac;

  header = State_GetCurrent();

  FS_GET_HEADER_BODY(header,&body);

  if(op == FS_OP_READ)
    {
      FS_GET_BODY_ACREAD(body, &ac);
    }
  else
    {
      FS_GET_BODY_ACUPDATE(body, &ac);
    }

  return ac;
}


uint8_t FS_CheckAC(int op)
{
  uint16_t header, body;
  uint8_t ac;

  ac = FS_GetAC(op);

  uint8_t current = State_GetCurrentSecurity();

  /* HAL_IO_TxByte( ac ); */
  /* HAL_IO_TxByte( current ); */

  if( current>=ac )
    return FS_OK;
  else
    return FS_ERROR_SECURITY_STATUS;
}

  
