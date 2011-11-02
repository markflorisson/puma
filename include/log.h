#ifndef _LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static char *
getTimeStamp(char *timeStamp)
{
	time_t currentTime;

	currentTime = time(0);
	strftime(timeStamp,9,"%H:%M:%S",localtime(&currentTime));

	return(timeStamp);
}



static void
debug_msg(const char* msg, ...)
{
	va_list list;
	char timeStamp[16] = {'\0'};
	
	getTimeStamp(timeStamp);

	va_start(list,msg);
#ifdef DEBUG
	fprintf(stdout,"%s:",timeStamp);
	vfprintf(stdout,msg,list);
	fflush(stdout);
#endif

	va_end(list);
}

static void
error_msg(const char* msg, ...)
{
	va_list list;
	char timeStamp[16] = {'\0'};
	
	getTimeStamp(timeStamp);

	va_start(list,msg);

	fprintf(stderr,"%s:",timeStamp);
	vfprintf(stderr,msg,list);

	va_end(list);
}

#endif
