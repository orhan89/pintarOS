#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "pintaros.h"
#include "response.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

/**
 * @brief Test Response Manager (OK)
 *
 * set response type to OK, then check SW1 and SW2
 *
 * @return None
 */
void response_test_1(void) {
  apdu_res.rsp_type = Response_OK;
  apdu_res.rsp_xtra_param = 0x00;
  ResponseManager(&tx_buffer, &apdu_res);
  CU_ASSERT_EQUAL(tx_buffer.buffer[0] , 0x90);
  CU_ASSERT_EQUAL(tx_buffer.buffer[1] , 0x00);
  CU_ASSERT_EQUAL(tx_buffer.length , 2);
}

/**
 * @brief Test Response Manager (Command Not Allowed)
 *
 * set response type to CmdNotAllowed (Condition Not Satisfied), then check SW1 and SW2
 *
 * @return None
 */
void response_test_2(void) {
  apdu_res.rsp_type = Response_CmdNotAllowed_ConditionNotSatisfied;
  apdu_res.rsp_xtra_param = 0x00;
  ResponseManager(&tx_buffer, &apdu_res);
  CU_ASSERT_EQUAL(tx_buffer.buffer[0] , 0x69);
  CU_ASSERT_EQUAL(tx_buffer.buffer[1] , 0x85);
  CU_ASSERT_EQUAL(tx_buffer.length , 2);
}


/**
 * @brief Test Response Manager with Response Data
 *
 * set response type to OK and the data, then check SW1 and SW2 and Response Data
 *
 * @return None
 */
void response_test_3(void) {
  apdu_res.rsp_type = Response_OK;
  apdu_res.rsp_xtra_param = 0x00;
  apdu_res.rsp_data[0] = 0x00;
  apdu_res.rsp_data[1] = 0x01;
  apdu_res.rsp_data[2] = 0x02;
  apdu_res.rsp_data[3] = 0x03;
  apdu_res.rsp_data[4] = 0x04;
  apdu_res.rsp_data_len = 5;

  ResponseManager(&tx_buffer, &apdu_res);
  CU_ASSERT_EQUAL(tx_buffer.buffer[0] , 0x90);
  CU_ASSERT_EQUAL(tx_buffer.buffer[1] , 0x00);
  CU_ASSERT_EQUAL(tx_buffer.buffer[2] , 0x00);
  CU_ASSERT_EQUAL(tx_buffer.buffer[3] , 0x01);
  CU_ASSERT_EQUAL(tx_buffer.buffer[4] , 0x02);
  CU_ASSERT_EQUAL(tx_buffer.buffer[5] , 0x03);
  CU_ASSERT_EQUAL(tx_buffer.buffer[6] , 0x04);
  CU_ASSERT_EQUAL(tx_buffer.length , 7);

}


/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "Response Manager Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "Response Manager (OK)", response_test_1)) ||
        (NULL == CU_add_test(pSuite, "Response Manager (Command Not Found)", response_test_2)) ||
        (NULL == CU_add_test(pSuite, "Response Manager (OK with Data)", response_test_3))
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
