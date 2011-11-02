#ifndef _LOG_H

#include <stdio.h>
#include <stdarg.h>

static void
debug_msg(const char* msg, ...)
{
    va_list list;

    va_start(list,msg);
    #ifdef DEBUG
    vfprintf(stdout,msg,list);
    fflush(stdout);
    #endif

	va_end(list);
}

static void
error_msg(const char* msg, ...)
{
    va_list list;

    va_start(list,msg);
    vfprintf(stderr,msg,list);
    va_end(list);
}

#endif
