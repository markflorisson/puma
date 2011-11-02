#include "puma.h"

static char *error_messages[] = {
    #define ERR(errname, msg) msg
    #include "_errors.h"
    #undef ERR
};

/* Return an error message for a puma errno */
char *
puma_strerror(int puma_errno)
{
    if (puma_errno == PUMA_OSERROR)
        return strerror(errno);

    return error_messages[puma_errno];
}
