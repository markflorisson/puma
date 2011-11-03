#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "puma.h"

static void initLogFile();
static void parseCommandLine(int argc, char *argv[], EquationVariables *eqn_obj);

int 
main(int argc, char *argv[])
{
	int nx = 0, ny = 0, puma_errno = 0;
	EquationVariables eqn_obj;

	initLogFile();

	/* usage: ./puma -r <rate of prey population increase> -a <predation rate coefficient> 
			-b <reproduction rate of predators> -m <predator mortality rate> 
			-k <diffusion rate of hares> -l <diffusion rate of predators> 
	*/
	parseCommandLine(argc, argv, &eqn_obj);
			 

	/* TODO:
		1. Get command line input. use getopt to parse it
		2. fill the map matrix from i=1 and j=1, to add the halo region
		3. write the logic to print ppm file
		4. Integrate the computational kernel
	*/

	if (puma_errno = readmap(map, "small.dat", &nx, &ny)) {
		error_msg("[%s:%d]: Error reading file: %s\n",__FILE__,__LINE__,puma_strerror(puma_errno));
	}


	close(log_fd);
	return 0;
}

void
parseCommandLine(int argc, char *argv[], EquationVariables *eqn_obj)
{


}

void 
initLogFile()  /* TODO: Put this in log.c */
{

	const char *log_path = getenv("PROJ_LOG");
	const char *log_file = "puma.log";

	char buf[128] = {'\0'};

	if(log_path == NULL) {
		fprintf(stderr,"[%s:%d]: Environment not setup\n",__FILE__,__LINE__);
		exit(1);
	}
	
	strcpy(buf,log_path);
	strcat(buf,"/");	
	strcat(buf,log_file);	
	
	log_fd = open(buf,O_CREAT|O_RDWR|O_APPEND,S_IRUSR|S_IWUSR);
	if(-1 == log_fd) {
		fprintf(stderr,"%s\n",puma_strerror(PUMA_OSERROR));	
	}
	
	/* Redirect stdout and stderr to the log file */	

	dup2(log_fd,STDOUT_FILENO);
	dup2(log_fd,STDERR_FILENO);

}
