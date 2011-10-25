#include <puma.h>

#define BUFSIZE 128

static PUMA_INLINE int
fscanf_error(FILE *file, int retval)
{
     if (retval == EOF) {
        if (ferror(file))
            return PUMA_OSERROR;
        return PUMA_ERROR_SHORT_FILE;
    }
    return PUMA_ERROR_INVALID_DATA;
}

int
readmap(int map[NX][NY], const char *filename, int *nxp, int *nyp)
{
    int i, j;
    int nx, ny; /* bounds of the map */
    int result;
    FILE *file = fopen(filename, "r");

    if (!file)
        return PUMA_OSERROR;

    /* Scan the size of the X and Y axes */
    if ((result = fscanf(file, "%d %d", &nx, &ny)) != 2)
        return fscanf_error(file, result);

    /* Validate the axes */
    if (nx < 0 || ny < 0 || nx > NX || ny > NY)
        return PUMA_ERROR_OOB;

    /* Scan in all element of the array */
    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
            int value;

            if ((result = fscanf(file, "%d", &value)) != 1)
                return fscanf_error(file, result);

            map[i][j] = value;
        }
    }

    *nxp = nx;
    *nyp = ny;

    return PUMA_NOERR;
}

