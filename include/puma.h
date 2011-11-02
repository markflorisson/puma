#define NX 2000
#define NY NX

#include <stdio.h>
#include <log.h>
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
int readmap(int map[NX][NY], const char *filename, int *nxp, int *nyp);

#if __STDC_VERSION__ >= 199901L
    #define PUMA_INLINE inline
#elif __GNUC__
    #define PUMA_INLINE __inline__
#endif
