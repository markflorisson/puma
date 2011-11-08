#include <puma.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "log.h"

/* Unfortunately CU_FAIL /fails/ to be useful, it doesn't print the message you pass it... */
#define fail_on_error(expr) \
                        do {\
                            int puma_errno = expr; \
                            if (puma_errno) {\
                                fprintf(stderr, "\n    !!! %s !!!\n", puma_strerror(puma_errno));\
                                CU_FAIL(#expr);\
                                return;\
                            }\
                        } while (0)

#define assert_int_equals(a, b) do {\
        if (a != b) {\
            CU_ASSERT_EQUAL(a, b);\
            fprintf(stderr, "\n    !!! val1=%d val2=%d", a, b);\
        }\
    } while (0)

#define PUMA_ASSERT_DOUBLE_EQUAL(a, b) if (fabs(a - b) > 0.1e-5) {\
    fprintf(stderr, "\n    !!! %lf != %lf !!!\n", (double) a, (double) b);\
    CU_FAIL(#a " " #b);\
    return;\
}
     

int map[NX][NY] = {{0}};
REAL solution[NX][NY] = {{0}};
REAL hare[NX][NY] = {{0}};
REAL puma[NX][NY] = {{0}};

static int
puma_open(const char *filename, const char *mode, FILE **file)
{
    *file = fopen(filename, mode);
    if (!*file)
        return PUMA_OSERROR;

    return PUMA_NOERR;
}

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}


/* Simple test of fread().
 * Reads the data previously written by testFPRINTF()
 * and checks whether the expected characters are present.
 * Must be run after testFPRINTF().
 */
void test_readmap(void)
{
    int nx, ny;
    int i, j;

    fail_on_error(readmap("test/test.dat", map, &nx, &ny));
    assert_int_equals(nx, 10);
    assert_int_equals(ny, 10);

    for(i = 0; i < nx; i++) {
        for(j = 0; j < ny; j++) {
            if (i == j)
                assert_int_equals(map[i][j], 1);
            else
                assert_int_equals(map[i][j], 0);
        }
    }

    memset(map, 0, sizeof(map));

}


void test_kernel(void)
{
    int nx, ny;
    int i, j;
    int value_m; 
    float value_p, value_h, value_s;
    int max_iter=100;
    EquationVariables eqn_obj;

    FILE *file_solution, *file_hares, *file_puma, *file_map;

    fail_on_error(puma_open("test/solution.dat", "r", &file_solution));
    fail_on_error(puma_open("test/Hare.dat", "r", &file_hares));
    fail_on_error(puma_open("test/Puma.dat", "r", &file_puma));
    fail_on_error(puma_open("test/Map.dat", "r", &file_map));

    eqn_obj.time_interval= 0.004;
    eqn_obj.prey_pop_inc_rate = 0.08;
    eqn_obj.pred_rate_coeff = 0.04;
    eqn_obj.rep_rate_pred = 0.02;
    eqn_obj.pred_mort_rate = 0.06;
    eqn_obj.diff_rate_hares = 0.2;
    eqn_obj.diff_rate_pumas = 0.2;

    fscanf(file_map, "%d %d", &ny, &nx);

    /* Scan in all element of the array */
    for (i = 0; i < NX; i++){
        for (j = 0; j < NY; j++){
            fscanf(file_solution, "%f", &value_s);
            fscanf(file_hares, "%f", &value_h);
            fscanf(file_puma, "%f", &value_p);
            fscanf(file_map, "%d", &value_m);
            solution[i][j] = value_s;
            hare[i][j] = value_h;
            puma[i][j] = value_p;
            map[i][j] = value_m;
        }
    }

    for (i = 0; i < max_iter; i++)
       compute(map, puma, hare, nx, ny, &eqn_obj);

    for(i = 0; i < nx; i++){
        for(j =0; j < ny; j++) {
            PUMA_ASSERT_DOUBLE_EQUAL(solution[i][j], hare[i][j]);
           // printf("%d ",hare[i][j]);
        }
    }

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */


int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (!CU_add_test(pSuite, "test of function readmap in I/O", test_readmap) ||
       !CU_add_test(pSuite, "test of kernel code", test_kernel))
   {
      CU_cleanup_registry();
      fputs(CU_get_error_msg(), stderr);
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();

   if (CU_get_error())
       fputs("Some tests FAILED", stderr);
   return CU_get_error();
}

