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

char *puma_strerror(int);
int map[NX][NY]; /* Matrix with land and water bitmask */

typedef struct EquationVariables
{
        float prey_pop_inc_rate;
        float pred_rate_coeff;
        float rep_rate_pred;
        float pred_mort_rate;
        float diff_rate_hares;
        float diff_rate_pumas;

}EquationVariables;

int readmap(int map[NX][NY], const char *filename, int *nxp, int *nyp);

#if __STDC_VERSION__ >= 199901L
    #define PUMA_INLINE inline
#elif __GNUC__
    #define PUMA_INLINE __inline__
#endif

#endif /* for _PUMA_H */
