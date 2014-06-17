#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "config.h"
#include "pintaros.h"
#include "hal.h"
#include "fs.h"
#include "state.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void)
{
  HAL_Mem_Initialize();
  return 0;
}

int clean_suite(void)
{
  HAL_Mem_DeInitialize();
  return 0; 
}

/************* Test case functions ****************/

/**
 * @brief Test File System Select MF
 *
 * Select MF, then test current DF and EF from statemanager
 *
 * @return None
 */
void fs_test_1(void)
{
  uint16_t addr1;
  uint16_t addr2;
  uint16_t addr3;
  uint16_t addr4;
  uint16_t addr5;
  uint16_t addr6;
  uint16_t addr7;
  uint16_t addr8;
  uint16_t addr9;
  int status;

  printf ("\n");

  // initial
  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  //                             Free                                   //
  //                            Blocks                                  //
  //                                                                    //
  ////////////////////////////////////////////////////////////////////////

  // alloc some block
  CU_ASSERT_EQUAL(FSAlloc(10, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr1), FS_OK);
  CU_ASSERT_EQUAL(addr1, 0);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xc0);

  ////////////////////////////////////////////////////////////////////////
  //             //                                                     // addr1 = 0
  //    Used     //                     Free                            //
  // (10 Blocks) //                    Blocks                           //
  //             //                                                     //
  ////////////////////////////////////////////////////////////////////////

  // alloc another block
  CU_ASSERT_EQUAL(FSAlloc(20, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr2), FS_OK);
  CU_ASSERT_EQUAL(addr2, 10);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xfc);


  ////////////////////////////////////////////////////////////////////////
  //                               //                                   // addr1 = 0
  //             Used              //              Free                 // addr2 = 10
  //          (30 Blocks)          //             Blocks                //
  //                               //                                   //
  ////////////////////////////////////////////////////////////////////////

  // free last block
  CU_ASSERT_EQUAL(FSFree(addr2, 20), FS_OK);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xc0);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0x00);


  ////////////////////////////////////////////////////////////////////////
  //             //                                                     // addr1 = 0
  //    Used     //                     Free                            //
  // (10 Blocks) //                    Blocks                           //
  //             //                                                     //
  ////////////////////////////////////////////////////////////////////////

  // alloc another block again
  CU_ASSERT_EQUAL(FSAlloc(30, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr3), FS_OK);
  CU_ASSERT_EQUAL(addr3, 10);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0xff);

  ////////////////////////////////////////////////////////////////////////
  //                                       //                           // addr1 = 0
  //                 Used                  //              Free         // addr3 = 10
  //              (40 Blocks)              //             Blocks        //
  //                                       //                           //
  ////////////////////////////////////////////////////////////////////////

  // free first 10 blocks
  CU_ASSERT_EQUAL(FSFree(addr1, 10), FS_OK);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0x3f);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0xff);

  ////////////////////////////////////////////////////////////////////////
  //           //                          //                           //
  //    Free   //           Used           //            Free           // addr3 = 10
  //(10 Blocks)//        (30 Blocks)       //           Blocks          //
  //           //                          //                           //
  ////////////////////////////////////////////////////////////////////////

  // alloc some block (bigger than the free block in the beginning)
  CU_ASSERT_EQUAL(FSAlloc(20, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr4), FS_OK);
  CU_ASSERT_EQUAL(addr4, 40);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0x3f);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(5), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(6), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(7), 0xf0);


  ////////////////////////////////////////////////////////////////////////
  //           //                                //                     //
  //    Free   //              Used              //         Free        // addr3 = 10
  //(10 Blocks)//           (50 Blocks)          //        Blocks       // addr4 = 40
  //           //                                //                     //
  ////////////////////////////////////////////////////////////////////////

  // free another 30 blocks in the beginning
  CU_ASSERT_EQUAL(FSFree(addr3, 30), FS_OK);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(5), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(6), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(7), 0xf0);

  ////////////////////////////////////////////////////////////////////////
  //                            //               //                     //
  //           Free             //       D       //         Free        //
  //        (40 Blocks)         //  (20 Blocks)  //        Blocks       // addr4 = 40
  //                            //               //                     //
  ////////////////////////////////////////////////////////////////////////

  // alloc some block enough to occupy the free space in the beginning
  CU_ASSERT_EQUAL(FSAlloc(30, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr5), FS_OK);
  CU_ASSERT_EQUAL(addr5, 0);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xfc);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0x00);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(5), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(6), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(7), 0xf0);

  ////////////////////////////////////////////////////////////////////////
  //                 //           //             //                     //
  //      Used       //   Free    //    Used     //         Free        //
  //   (30 Blocks)   //(10 Blocks)// (20 Blocks) //        Blocks       // addr4 = 40
  //                 //           //             //                     // addr5 = 0
  ////////////////////////////////////////////////////////////////////////

  // alloc some block to occupy the free space
  CU_ASSERT_EQUAL(FSAlloc(10, CONFIG_FS_START, CONFIG_FS_SIZE/CONFIG_FS_BLOCK_SIZE, &addr6), FS_OK);
  CU_ASSERT_EQUAL(addr6, 30);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(5), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(6), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(7), 0xf0);

  ////////////////////////////////////////////////////////////////////////
  //                                             //                     //
  //                    Used                     //         Free        //
  //                 (60 Blocks)                 //        Blocks       // addr4 = 40
  //                                             //                     // addr5 = 0
  //////////////////////////////////////////////////////////////////////// addr6 = 30

  // alloc some block to occupy the free space
  CU_ASSERT_EQUAL(FSAlloc(50, CONFIG_FS_START, CONFIG_FS_START+100, &addr7), FS_ERROR_INSUFFICIENT_SPACE);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(0), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(2), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(3), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(4), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(5), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(6), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(7), 0xf0);

  // alloc some block to occupy the free space
  CU_ASSERT_EQUAL(FSAlloc(200, 100, 200, &addr7), FS_ERROR_INSUFFICIENT_SPACE);

  // alloc some block start at block 100
  CU_ASSERT_EQUAL(FSAlloc(10, 100, 200, &addr8), FS_OK);
  CU_ASSERT_EQUAL(addr8, 100);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(100/8 + 0), 0x0f);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(100/8 + 1), 0xfc);

  // alloc some block start at block 100
  CU_ASSERT_EQUAL(FSAlloc(10, 100, 200, &addr9), FS_OK);
  CU_ASSERT_EQUAL(addr9, 110);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(100/8 + 0), 0x0f);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(100/8 + 1), 0xff);
  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(100/8 + 2), 0xff);

}

/**
 * @brief Test File System Format
 *
 * Format File System
 *
 * @return None
 */
void fs_test_2(void) {

  uint8_t  tag;
  uint16_t fid;
  uint16_t parent;
  uint16_t child;
  uint16_t sibling;
  int i;

  printf ("\n");

  FSFormat();

  CU_ASSERT_EQUAL(HAL_Mem_ReadByte(1024/8 + 0), 0xe0);

  for(i=(1024/8) + 1; i<1024*4;i++)
    CU_ASSERT_EQUAL(HAL_Mem_ReadByte(i), 0x00);

  HAL_Mem_ReadBlock(1024 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1024 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1024 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1024 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1024 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x3f);
  CU_ASSERT_EQUAL(fid, 0x3f00);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 0);

  FSInitialize();
  CU_ASSERT_EQUAL(stateGetCurrent(), 1024);

}

/**
 * @brief Test File System Create Header
 *
 * @return None
 */
void fs_test_3(void) {

  uint16_t header;
  uint8_t  tag;
  uint16_t fid;
  uint16_t parent;
  uint16_t child;
  uint16_t sibling;

  stateSetCurrent(1024);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_EF, (uint16_t) 0x1234, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1027);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_DF, (uint16_t) 0x1235, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1030);

  stateSetCurrent(1030);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_EF, (uint16_t) 0x1236, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1033);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_DF, (uint16_t) 0x1237, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1036);

  stateSetCurrent(1024);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_EF, (uint16_t) 0x1238, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1039);

  stateSetCurrent(1030);

  CU_ASSERT_EQUAL(FSCreateHeader(FS_TAG_EF, (uint16_t) 0x1239, &header), FS_OK);
  CU_ASSERT_EQUAL(header, 1042);

  HAL_Mem_ReadBlock(1024 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1024 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1024 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1024 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1024 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x3f);
  CU_ASSERT_EQUAL(fid, 0x3f00);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 1027);
  CU_ASSERT_EQUAL(sibling, 0);

  HAL_Mem_ReadBlock(1027 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1027 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1027 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1027 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1027 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1234);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1030);

  HAL_Mem_ReadBlock(1030 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1030 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1030 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1030 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1030 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x4f);
  CU_ASSERT_EQUAL(fid, 0x1235);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 1033);
  CU_ASSERT_EQUAL(sibling, 1039);

  HAL_Mem_ReadBlock(1033 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1033 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1033 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1033 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1033 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1236);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1036);

  HAL_Mem_ReadBlock(1036 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1036 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1036 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1036 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1036 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x4f);
  CU_ASSERT_EQUAL(fid, 0x1237);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1042);

  HAL_Mem_ReadBlock(1039 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1039 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1039 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1039 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1039 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1238);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 0);

  HAL_Mem_ReadBlock(1042 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1042 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1042 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1042 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1042 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1239);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 0);

}

/**
 * @brief Test File System Create File
 *
 * @return None
 */
void fs_test_4(void) {
  struct DF_st df;
  struct EF_st ef;

  uint8_t  tag;
  uint16_t fid;
  uint16_t parent;
  uint16_t child;
  uint16_t sibling;

  printf ("\n");

  FSFormat();

  FSInitialize();
  
  ef.FID = 0x1234;
  ef.structure = FS_EF_STRUCTURE_TRANSPARENT;
  ef.type = FS_EF_TYPE_INTERNAL;
  ef.ACRead = 1;
  ef.ACUpdate = 2;
  ef.size = 100;
  FSCreateFile(FS_TAG_EF,&ef);

  df.FID = 0x1235;
  FSCreateFile(FS_TAG_DF,&df);

  stateSetCurrent((uint16_t) 1030);

  ef.FID = 0x1236;
  ef.structure = FS_EF_STRUCTURE_TRANSPARENT;
  ef.type = FS_EF_TYPE_INTERNAL;
  ef.ACRead = 1;
  ef.ACUpdate = 2;
  ef.size = 100;
  FSCreateFile(FS_TAG_EF,&ef);

  df.FID = 0x1237;
  FSCreateFile(FS_TAG_DF,&df);

  stateSetCurrent((uint16_t) 1024);

  ef.FID = 0x1238;
  ef.structure = FS_EF_STRUCTURE_RECORD;
  ef.type = FS_EF_TYPE_WORKING;
  ef.ACRead = 3;
  ef.ACUpdate = 4;
  ef.size = 200;
  FSCreateFile(FS_TAG_EF,&ef);

  stateSetCurrent((uint16_t) 1030);

  ef.FID = 0x1239;
  ef.structure = FS_EF_STRUCTURE_RECORD;
  ef.type = FS_EF_TYPE_WORKING;
  ef.ACRead = 3;
  ef.ACUpdate = 4;
  ef.size = 200;
  FSCreateFile(FS_TAG_EF,&ef);

  HAL_Mem_ReadBlock(1027 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1027 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1027 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1027 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1027 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1234);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1030);

  HAL_Mem_ReadBlock(1030 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1030 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1030 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1030 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1030 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x4f);
  CU_ASSERT_EQUAL(fid, 0x1235);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 1033);
  CU_ASSERT_EQUAL(sibling, 1039);

  HAL_Mem_ReadBlock(1033 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1033 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1033 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1033 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1033 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1236);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1036);

  HAL_Mem_ReadBlock(1036 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1036 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1036 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1036 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1036 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x4f);
  CU_ASSERT_EQUAL(fid, 0x1237);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 1042);

  HAL_Mem_ReadBlock(1039 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1039 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1039 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1039 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1039 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1238);
  CU_ASSERT_EQUAL(parent, 1024);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 0);

  HAL_Mem_ReadBlock(1042 * 4 + 0, 1, (uint8_t *)&tag);
  HAL_Mem_ReadBlock(1042 * 4 + 1, 2, (uint8_t *)&fid);
  HAL_Mem_ReadBlock(1042 * 4 + 3, 2, (uint8_t *)&parent);
  HAL_Mem_ReadBlock(1042 * 4 + 5, 2, (uint8_t *)&child);
  HAL_Mem_ReadBlock(1042 * 4 + 7, 2, (uint8_t *)&sibling);

  CU_ASSERT_EQUAL(tag, 0x5f);
  CU_ASSERT_EQUAL(fid, 0x1239);
  CU_ASSERT_EQUAL(parent, 1030);
  CU_ASSERT_EQUAL(child, 0);
  CU_ASSERT_EQUAL(sibling, 0);

}

/**
 * @brief Test File System Access Binary Data
 *
 * @return None
 */
void fs_test_5(void) {

  uint8_t data1[10];
  uint8_t data2[10];
  int i;

  printf ("\n");

  for(i = 0; i < 10; i++)
    data1[i] = i;

  stateSetCurrent((uint16_t)1039);

  FSAccessBinary(FS_OP_UPDATE,0,10,data1);

  FSAccessBinary(FS_OP_READ,0,10,data2);
}


/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "File System Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "Block Allocation", fs_test_1)) ||
	(NULL == CU_add_test(pSuite, "Format File System", fs_test_2)) ||
	(NULL == CU_add_test(pSuite, "Create File", fs_test_3)) ||
	(NULL == CU_add_test(pSuite, "File System Select with Name", fs_test_4)) ||
	(NULL == CU_add_test(pSuite, "File System Update & Read Binary", fs_test_5))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   //printf("\n");
   //CU_basic_show_failures(CU_get_failure_list());
   //printf("\n\n");

   /*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
   */

   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();

}
