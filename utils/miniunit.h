/* file: miniunit.h
 *
 * Mini unit test macros
 *
 */

#ifndef MINI_UNIT_TEST
#define MINI_UNIT_TEST

#define U_ASSERT(res, test) if (!(test)) { printf("assert failed: %s:%i expression: %s\n",  __FILE__, __LINE__, #test); res++;}
#define U_RUN(test) test() == 0 ? tests_passed++ : tests_failed++;

extern int tests_passed;
extern int tests_failed;

#endif