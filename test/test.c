#include <puma.h>
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"

int map[NX][NY] = {{0}};

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}


/* Simple test of fread().
 * Reads the data previously written by testFPRINTF()
 * and checks whether the expected characters are present.
 * Must be run after testFPRINTF().
 */
void test_readmap(void)
{

  int nx,ny;
  char *filename = "small.dat";
  int i,j;
  FILE *file = fopen(filename, "r");
  int value;
  fscanf(file, "%d %d", &nx, &ny);
  int buf[nx][ny];

  /* Scan in all element of the array */
  for (i = 0; i < nx; i++){
    for (j = 0; j < ny; j++){
      fscanf(file, "%d", &value);
      buf[i][j] = value;
    }
  }

  PUMA_FAIL_ON_ERR(readmap(filename, map, &nx, &ny));

  for(i = 0; i < nx; i++){
    for(j =0; j < ny; j++)
      CU_ASSERT_EQUAL(buf[i][j], map[i][j]);
  }

  memset(map, 0, sizeof(map));

}


void test_kernel(void)
{
  int nx,ny;
  int i,j;
  int value_s,value_p,value_h,value_m;
  int max_iter=100;
  char filename_Solution[64] = {"solution.dat"};
  char filename_Hare[64] = {"Hare.dat"};
  char filename_Puma[64] = {"Puma.dat"};
  char filename_Map[64] = {"Map.dat"};
  EquationVariables eqn_obj;

  FILE *file_Solution = fopen(filename_Solution, "r");
  FILE *file_Hares = fopen(filename_Hare, "r");
  FILE *file_Puma = fopen(filename_Puma, "r");
  FILE *file_Map = fopen(filename_Map, "r");

  eqn_obj.time_interval= 0.4;
  eqn_obj.prey_pop_inc_rate = 0.08;
  eqn_obj.pred_rate_coeff = 0.04;
  eqn_obj.rep_rate_pred = 0.02;
  eqn_obj.pred_mort_rate = 0.06;
  eqn_obj.diff_rate_hares = 0.2;
  eqn_obj.diff_rate_pumas = 0.2;

  //fscanf(file_Map, "%d %d", &nx, &ny);
  nx=4;
  ny=4;
  REAL solution[NX][NY];
  REAL hare[NX][NY];
  REAL puma[NX][NY];
  int Map[NX][NY];


  /* Scan in all element of the array */
  for (i = 0; i < NX; i++){
    for (j = 0; j < NY; j++){
      fscanf(file_Solution, "%d", &value_s);
      fscanf(file_Hares, "%d", &value_h);
      fscanf(file_Puma, "%d", &value_p);
      fscanf(file_Map, "%d", &value_m);
      solution[i][j] =2;// value_s;
      hare[i][j] =2;// value_h;
      puma[i][j] = 1;//value_p;
      Map[i][j] = 1;//value_m;


    }
  }




  //for (i = 0; i < max_iter; i++)
  // compute(hare, puma, map, nx, ny, &eqn_obj);

   for(i = 0; i < nx; i++){
     for(j =0; j < ny; j++)
       CU_ASSERT_EQUAL(solution[i][j], hare[i][j]);
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
   if ((NULL == CU_add_test(pSuite, "test of function readmap in I/O", test_readmap)) ||
       (NULL == CU_add_test(pSuite, "test of kernal code", test_kernel )))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

