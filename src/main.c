#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "puma.h"

int logFd = 0; /* File descriptor for log file */
int map[NX][NY]; /* Matrix with land and water bitmask */

static void initLogFile();

int 
main(void)
{
	int nx = 0, ny = 0, puma_errno = 0;

	initLogFile();

	/* TODO:
		1. Get command line input. use getopt to parse it
		2. fill the map matrix from i=1 and j=1, to add the halo region
		3. write the logic to print ppm file
		4. Integrate the computational kernel
	*/

	if (puma_errno = readmap(map, "small.dat", &nx, &ny)) {
		error_msg("[%s:%d]: Error reading file: %s\n",__FILE__,__LINE__,puma_strerror(puma_errno));
	}

	debug_msg("%d %d %d\n", map[0][0], map[49][49], map[17][17]);

	

	/* TODO: close log file here  */

	return 0;
}

void initLogFile()
{
	const char *logPath = getenv("PROJ_LOG");
	const char *logFile = "puma.log";

	char buf[128] = {'\0'};

	if(logPath == NULL) {
		fprintf(stderr,"[%s:%d]: Environment not setup\n",__FILE__,__LINE__);
		exit(1);
	}
	
	strcpy(buf,logPath);
	strcat(buf,"/");	
	strcat(buf,logFile);	
	
	logFd = open(buf,O_CREAT|O_RDWR|O_APPEND,S_IRUSR|S_IWUSR);
	if(-1 == logFd) {
		fprintf(stderr,"%s\n",puma_strerror(PUMA_OSERROR));	
	}
	
	/* Redirect stdout and stderr to the log file */	

	dup2(logFd,STDOUT_FILENO);
	dup2(logFd,STDERR_FILENO);

}
