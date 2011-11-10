#ifndef _LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>


/* 
  Function that returns the current
  timestamp in string format.
*/
static char*
get_time_stamp(char *time_stamp)
{
    time_t current_time;

    current_time = time(NULL);
    strftime(time_stamp, 9, "%H:%M:%S", localtime(&current_time));

    return time_stamp;
}

/*
  Function to print the debug
  messages to standard output.
*/
static void
debug_msg(const char* msg, ...)
{
    va_list list;
    char time_stamp[16] = {'\0'};
    char line[4096] = {'\0'};

    get_time_stamp(time_stamp);

    va_start(list,msg);
#ifdef DEBUG
    snprintf(line, 4096, "%s:%s", time_stamp, msg);
    vfprintf(stdout, line, list);
#endif
    va_end(list);
}

/*
  Function to print the error
  messages to standard output.
*/
static void
error_msg(const char* msg, ...)
{
    va_list list;
    char time_stamp[16] = {'\0'};
    char line[4096] = {'\0'};

    get_time_stamp(time_stamp);

    va_start(list,msg);

    snprintf(line, 4096, "%s:%s",time_stamp,msg);
    vfprintf(stderr,line,list);

    va_end(list);
}

#endif
