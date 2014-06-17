#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "pintaros.h"
#include "transmission.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void) { 
  tconfig.protocol = T0;
  tconfig.baudrate = B9600;

  rx_buffer_ptr = &rx_buffer;
  tx_buffer_ptr = &tx_buffer;
  return 0; 
}
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

/**
 * @brief Test to Configure transmission handler
 *
 * @return None
 */
void transmission_test_1(void) {
  CU_ASSERT_EQUAL( transInit(tconfig), 0);
}

/**
 * @brief Test to receive data
 *
 * receive a byte data, then check the rx_buffer
 *
 * @return None
 */
void transmission_test_2(void) {
  transRx();
  CU_ASSERT_EQUAL( rx_buffer_ptr->buffer[0], 61);
  CU_ASSERT_EQUAL( rx_buffer_ptr->length, 1);
}


/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "Transmission Handler Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "Transmission Handler Test Config", transmission_test_1)) ||
	(NULL == CU_add_test(pSuite, "Transmission Handler Rx", transmission_test_2))
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
