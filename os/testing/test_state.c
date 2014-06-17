#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "pintaros.h"
#include "state.h"

#include <stdio.h>

/* Test Suite setup and cleanup functions: */

int init_suite(void) { 
  active_state = 0;
  state_mng[0].currentDF = 0;
  state_mng[0].currentEF = 0;
  state_mng[0].currentKey = 1;
  state_mng[0].securityState = 0;
  return 0; 
}
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

/**
 * @brief Test to change state manager
 *
 * change current state manager then check the active state
 *
 * @return None
 */
void state_test_1(void) {
  CU_ASSERT_EQUAL( stateChangeState(1), 0);
  CU_ASSERT_EQUAL( active_state, 1);
}

/**
 * @brief Test to change current DF
 *
 * change current DF then check the state manager
 *
 * @return None
 */
void state_test_2(void) {
  CU_ASSERT_EQUAL( stateSetCurrentDF(1), 0);
  CU_ASSERT_EQUAL( state_mng[0].currentDF, 1);
  CU_ASSERT_EQUAL( state_mng[0].currentEF, NULL);
  CU_ASSERT_EQUAL( state_mng[0].currentKey, NULL);
  CU_ASSERT_EQUAL( state_mng[0].securityState, 0);
}

/**
 * @brief Test to change current EF
 *
 * change current EF then check the state manager
 *
 * @return None
 */
void state_test_3(void) {
  CU_ASSERT_EQUAL( stateSetCurrentEF(1), 0);
  CU_ASSERT_EQUAL( state_mng[0].currentEF, 1);
}

/**
 * @brief Test to get current DF
 *
 * set currentDF from state manager then check stateGetCurrentDF function output
 *
 * @return None
 */
void state_test_4(void) {
  state_mng[0].currentDF = 2;
  CU_ASSERT_EQUAL( stateGetCurrentDF(), 2);
}

/**
 * @brief Test to get current EF
 *
 * set currentEF from state manager then check stateGetCurrentEF function output
 *
 * @return None
 */
void state_test_5(void) {
  state_mng[0].currentEF = 2;
  CU_ASSERT_EQUAL( stateGetCurrentEF(), 2);
}

/**
 * @brief Test to get current current security state
 *
 * set securityStae from state manager then check stateGetCurrentSecurity function output
 *
 * @return None
 */
void state_test_6(void) {
  state_mng[0].securityState = 2;
  CU_ASSERT_EQUAL( stateGetCurrentSecurity(), 2);
}

/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "State Manager Test Suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "State Manager Change State", state_test_1)) ||
	(NULL == CU_add_test(pSuite, "State Manager Set Current DF", state_test_2)) ||
	(NULL == CU_add_test(pSuite, "State Manager Set Current EF", state_test_3)) ||
	(NULL == CU_add_test(pSuite, "State Manager Get Current DF", state_test_4)) ||
	(NULL == CU_add_test(pSuite, "State Manager Get Current EF", state_test_5)) ||
	(NULL == CU_add_test(pSuite, "State Manager Get Security State", state_test_6))
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
