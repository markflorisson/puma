#ifndef _PUMA_H

#define NX 2000
#define NY NX

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>

/* All the puma errnos. A description can be obtained using puma_strerror(myerrno) */
enum puma_errors {
    #define ERR(errname, msg) errname
    #include "_errors.h"
    #undef ERR
    N_PUMA_ERRORS
};

typedef double Real;

char *puma_strerror(int);
int map[NX+2][NY+2]; /* Matrix with land and water bitmask */
Real hare[NX+2][NY+2], puma[NX+2][NY+2]; /* Matrices of hare and puma densities */


typedef struct EquationVariables
{
        float time_interval;
        float prey_pop_inc_rate;
        float pred_rate_coeff;
        float rep_rate_pred;
        float pred_mort_rate;
        float diff_rate_hares;
        float diff_rate_pumas;

}EquationVariables;

//int readmap(int map[NX+2][NY+2], const char *filename, int *nxp, int *nyp);
//int write_ppm_file(int map[NX+2][NX+2], double hare[NX+2][NY+2], double puma[NX+2][NY+2], const int nx, const int ny, const int delta_t);
//void compute(Real hare[NX+2][NY+2], Real puma[NX+2][NY+2], int land[NX+2][NY+2], int nx, int ny, EquationVariables *eq_val);
int readmap(const char *filename, int *nxp, int *nyp);
int write_ppm_file(const int nx, const int ny, const int delta_t);
void compute(int nx, int ny, EquationVariables *eq_val);

#if __STDC_VERSION__ >= 199901L
    #define PUMA_INLINE inline
#elif __GNUC__
    #define PUMA_INLINE __inline__
#endif

#endif /* for _PUMA_H */
