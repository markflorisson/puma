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

/*Return error information */
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
			return;\
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


int init_suite1(void)
{
    return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}


/*Test input readmap function*/
void test_readmap(void)
{
    int nx, ny;
    int i, j;

    fail_on_error(readmap("test/test.dat", map, &nx, &ny));
    assert_int_equals(nx, 10);
    assert_int_equals(ny, 10);

    for(i = 1; i <= nx; i++) {
        for(j = 1; j <= ny; j++) {
            if (i == j)
                assert_int_equals(map[i][j], 1);
            else
                assert_int_equals(map[i][j], 0);
        }
    }

   

}

/*Test kernal code- compute function*/
void test_kernel_aux(int test_case, EquationVariables *eqn_obj){
    int nx, ny;
    int i, j;
    int value_m; 
    float value_p, value_h, value_s;
    float time_interval = 0.0;
    float delta_t = 0.0;
    double max_iter=1.0;
    
    FILE *file_solution, *file_hares, *file_puma, *file_map;

    switch(test_case){
        case 1:
            fail_on_error(puma_open("test/solution.dat", "r", &file_solution));
            fail_on_error(puma_open("test/Hare.dat", "r", &file_hares));
            fail_on_error(puma_open("test/Puma.dat", "r", &file_puma));
            break;
        case 2:
            fail_on_error(puma_open("test/solution2.dat", "r", &file_solution));
            fail_on_error(puma_open("test/Hare2.dat", "r", &file_hares));
            fail_on_error(puma_open("test/Puma2.dat", "r", &file_puma));
            break;
        case 3:
            fail_on_error(puma_open("test/solution3.dat", "r", &file_solution));
            fail_on_error(puma_open("test/Hare3.dat", "r", &file_hares));
            fail_on_error(puma_open("test/Puma3.dat", "r", &file_puma));
            break;
    }

    fail_on_error(puma_open("test/Map.dat", "r", &file_map));
    
    assert_int_equals(fscanf(file_map, "%d %d", &ny, &nx), 2);

    /* Scan in all element of the array */
    for (i = 1; i <= nx; i++){
        for (j = 1; j <= ny; j++){
            assert_int_equals(fscanf(file_solution, "%f", &value_s), 1);
            assert_int_equals(fscanf(file_hares, "%f", &value_h), 1);
            assert_int_equals(fscanf(file_puma, "%f", &value_p), 1);
            assert_int_equals(fscanf(file_map, "%d", &value_m), 1);

            solution[i][j] = value_s;
            hare[i][j] = value_h;
            puma[i][j] = value_p;
            map[i][j] = value_m;
        }
    }
    
    delta_t = (*eqn_obj).delta_t;
    printf("...",test_case);

    for (time_interval = delta_t; time_interval < max_iter; time_interval += delta_t)
       compute(map, puma, hare, nx, ny, &(*eqn_obj));

    for(i = 1; i <= nx; i++){
        for(j = 1; j <= ny; j++) {
            PUMA_ASSERT_DOUBLE_EQUAL(solution[i][j], hare[i][j]);
        }
    }
}


void test_kernel(void)
{
    int i;

    EquationVariables eqn_obj;
    
    eqn_obj.delta_t= 0.004;
    eqn_obj.prey_pop_inc_rate = 0.08;
    eqn_obj.pred_rate_coeff = 0.04;
    eqn_obj.rep_rate_pred = 0.02;
    eqn_obj.pred_mort_rate = 0.06;
    eqn_obj.diff_rate_hares = 0.2;
    eqn_obj.diff_rate_pumas = 0.2;

    for(i = 1; i < 4; i++){
        test_kernel_aux(i,&eqn_obj);
    }
    printf("...");
    
}




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

