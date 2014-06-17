#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "config.h"
#include "pintaros.h"
#include "hal.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

/**
 * @brief Test HAL Memory Initialize and Check
 *
 * @return None
 */
void hal_memory_test_1(void) {
  CU_ASSERT_EQUAL( HAL_Mem_Initialize(), CONFIG_FS_SIZE);
  CU_ASSERT_EQUAL( HAL_Mem_Check(), 0);
}

/**
 * @brief Test HAL Memory Write and Read
 *
 * @return None
 */
void hal_memory_test_2(void) {
  int i;

  for(i=0;i<CONFIG_FS_SIZE;i++)
    CU_ASSERT_EQUAL( HAL_Mem_WriteByte(i,(uint8_t)(i%256)), (uint8_t)(i%256));

  for(i=0;i<CONFIG_FS_SIZE;i++)
    CU_ASSERT_EQUAL( HAL_Mem_ReadByte(i), (uint8_t)(i%256));
}

/**
 * @brief Test HAL Memory Write Block and Read Block
 *
 * @return None
 */
void hal_memory_test_3(void) {
  int i;
  uint8_t dataW[CONFIG_FS_SIZE];
  uint8_t dataR[CONFIG_FS_SIZE];
  uint8_t test;

  for(i=0;i<CONFIG_FS_SIZE;i++)
    dataW[i] = (uint8_t)(i%256);
    
  CU_ASSERT_EQUAL( HAL_Mem_WriteBlock(CONFIG_FS_START,CONFIG_FS_SIZE,(uint8_t*)&dataW), CONFIG_FS_SIZE);

  CU_ASSERT_EQUAL( HAL_Mem_ReadBlock(CONFIG_FS_START,CONFIG_FS_SIZE,(uint8_t*)&dataR), CONFIG_FS_SIZE);

  for(i=0;i<100;i++)
    CU_ASSERT_EQUAL( dataW[i], dataR[i]);
}

/**
 * @brief Test HAL IO Configuration
 *
 * @return None
 */
void hal_io_test_1(void) {
  CU_ASSERT_EQUAL( HAL_IO_Config(9600), 0);
}

/**
 * @brief Test HAL IO Tx and Rx
 *
 * @return None
 */
void hal_io_test_2(void) {
  CU_ASSERT_EQUAL( HAL_IO_TxByte(2), 0);
  CU_ASSERT_EQUAL( HAL_IO_RxByte(), 2);
}

/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "HAL Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "HAL Memory Test Init & Check", hal_memory_test_1)) ||
	(NULL == CU_add_test(pSuite, "HAL Memory Test Write & Read (Byte)", hal_memory_test_2)) ||
	(NULL == CU_add_test(pSuite, "HAL Memory Test Write & Read (Block)", hal_memory_test_3)) ||
	(NULL == CU_add_test(pSuite, "HAL IO Test Config", hal_io_test_1)) ||
	(NULL == CU_add_test(pSuite, "HAL IO Tx & Rx", hal_io_test_2))
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
