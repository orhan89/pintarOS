#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "pintaros.h"
#include "command.h"
#include "response.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void) { 
  return 0; 
}
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

/**
 * @brief Test Command Interpreter (CLA Not Supported)
 *
 * set a wrong CLA and test the response
 *
 * @return None
 */
void command_test_1(void) {
  apdu_res.CLA = 0x12;
  CommandInterpreter(&rx_buffer, &apdu_res);
  CU_ASSERT_EQUAL(apdu_res.rspn , CLANotSupported);
}

/**
 * @brief Test Command Interpreter (INS Not Supported)
 *
 * set a wrong INS and test the response
 *
 * @return None
 */
void command_test_2(void) {
  apdu_res.CLA = 0x00;
  apdu_res.INS = 0xFF;
  CommandInterpreter(&rx_buffer, &apdu_res);
  CU_ASSERT_EQUAL(apdu_res.rspn , INSNotSupported);
}

/**
 * @brief Test Command Interpreter (ISO Select)
 *
 * Set apdu_res to apropriate (true) Select MF APDU, then check the response
 *
 * @return None
 */
void command_test_3(void) {
  apdu_res.CLA = 0x00;
  apdu_res.INS = 0xA4;
  apdu_res.P1  = 0x00;
  apdu_res.P2  = 0x00;
  CU_ASSERT_EQUAL(Cmd_SELECT(&apdu_res) , 0);
  CU_ASSERT_EQUAL(apdu_res.rspn , OK);
}

/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "Command Interpreter and Handler Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "Command Interpreter (CLA NOT SUPPORTED)", command_test_1)) ||
        (NULL == CU_add_test(pSuite, "Command Interpreter (INS NOT SUPPORTED)", command_test_2)) ||
	(NULL == CU_add_test(pSuite, "Command Select (Select MF)", command_test_3))
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
