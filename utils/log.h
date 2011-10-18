#ifndef _LOG_H

#include <stdio.h>
#include <stdarg.h>

void 
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

#endif
