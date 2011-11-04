#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "puma.h"

static void parseCommandLine(int argc, char *argv[], EquationVariables *eqn_obj);

int 
main(int argc, char *argv[])
{
	int nx = 0, ny = 0, puma_errno = 0;
	int i, j, max_iter = 100;
	EquationVariables eqn_obj;

	memset(&eqn_obj,0,sizeof(eqn_obj));

	parseCommandLine(argc, argv, &eqn_obj);

	if (puma_errno = readmap(map, "small.dat", &nx, &ny)) 
	{
		error_msg("[%s:%d]: Error reading file: %s\n",__FILE__,__LINE__,puma_strerror(puma_errno));
	}

	printf("nx: %d, ny: %d\n",nx,ny);

	for(i=0;i<NX;i++)
	{
		for(j=0;j<NY;j++)
		{
			hare[i][j] = 2.0;
			puma[i][j] = 4.0;
		}
	}

	/* Invoke computational kernel */
	for (i = 0; i < max_iter; i++)
	{
		compute(hare, puma, map, nx, ny, &eqn_obj);

		printf("Writing ppm for iter %d\n",i);
		if (puma_errno = write_ppm_file(map, hare, puma, nx, ny, i) )
		{
			error_msg("[%s:%d]: Error writing ppm file for iter %d: %s\n",__FILE__,__LINE__,1,puma_strerror(puma_errno));
		}
	}

	return 0;
}

void
printUsage(char *argv[])
{
	fprintf(stdout,"Usage e.g.: %s <-r 0.08> <-a 0.04> <-b 0.02> <-m 0.06> <-k 0.2> <-l 0.2>\n",argv[0]); 
	fprintf(stdout,"               -r : rate of prey population increase\n");
	fprintf(stdout,"               -a : predation rate coefficient\n");
	fprintf(stdout,"               -b : reproduction rate of predators\n");
	fprintf(stdout,"               -m : predator mortality rate\n");
	fprintf(stdout,"               -k : diffusion rate of hares\n");
	fprintf(stdout,"               -l : diffusion rate of predators\n");
}

void
parseCommandLine(int argc, char *argv[], EquationVariables *eqn_obj)
{
	char ch = '\0';
	
	/* Set the defaults */
	eqn_obj->time_interval= 0.4;
        eqn_obj->prey_pop_inc_rate = 0.08;
        eqn_obj->pred_rate_coeff = 0.04;
        eqn_obj->rep_rate_pred = 0.02;
        eqn_obj->pred_mort_rate = 0.06;
        eqn_obj->diff_rate_hares = 0.2;
        eqn_obj->diff_rate_pumas = 0.2;


	while((ch = (char) getopt(argc, argv, "hr:a:b:m:k:l:t:")) != -1 )
	{
		switch(ch)
		{
			case 'h': /* help */
				printUsage(argv);
				exit(0);
				break;

			case 'r':
				eqn_obj->prey_pop_inc_rate = atof(optarg);
				break;

			case 'a':
				eqn_obj->pred_rate_coeff = atof(optarg);
				break;

			case 'b':
				eqn_obj->rep_rate_pred = atof(optarg);
				break;

			case 'm':
				eqn_obj->pred_mort_rate = atof(optarg);
				break;

			case 'k':
				eqn_obj->diff_rate_hares = atof(optarg);
				break;

			case 'l':
				eqn_obj->diff_rate_pumas = atof(optarg);
				break;

			case 't':
				eqn_obj->time_interval = atof(optarg);	
				break;

			default:
				printf("Setting defaults for equations params\n");
				break;
		}
	}

}
