#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "puma.h"

extern float random_uniform(float max_val);
extern void rinit(int ijkl);
static int parse_command_line(int argc, char *argv[], EquationVariables *eqn_obj, int *time_step_size, char *filename,
    int max_iter);

int map[NX][NY] = {{0}}; /* Matrix with land and water bitmask */
REAL hare[NX][NY] = {{0}}, puma[NX][NY] = {{0}}; /* Matrices of hare and puma densities */

int
main(int argc, char *argv[])
{
	int nx = 0, ny = 0, puma_errno = 0;
	int i = 0, j = 0;
	const int max_iter = 500;
	char filename[PUMA_FILENAME_SIZE] = {'\0'};

	int time_step_size = 0;
	int write_interval = 0;
	float time_interval = 0.0;
	float delta_t = 0.0;
	
	EquationVariables eqn_obj = { 0 };

	/*
		TODO: check if values are within certain boundaries,
		For eg delta T cannot be 1 million.
	 */
    /* Parse command line inputs. Check coefficients are not negative, and delta_t is not larger than 1 */ 
	if (puma_errno = parse_command_line(argc, argv, &eqn_obj, &time_step_size, filename, max_iter))
    {
		error_msg("[%s:%d]: Error reading command line input: %s\n",__FILE__,__LINE__,puma_strerror(puma_errno));
        	exit(EXIT_FAILURE);
    }

	if ((puma_errno = readmap(filename, map, &nx, &ny)))
	{
		error_msg("[%s:%d]: Error reading file: %s\n",__FILE__,__LINE__,puma_strerror(puma_errno));
        	exit(EXIT_FAILURE);
	}

	debug_msg("[%s:%d]: nx: %d, ny: %d\n",__FILE__,__LINE__,nx,ny);

	rinit(time(NULL)/2);

	/* Init with random values for the densities of hare and puma within 0 and 5 */
	for (i = 1; i <= nx; i++){ 
		for (j = 1; j <= ny; j++){

			if (map[i][j] == 0) continue;

			hare[i][j] = random_uniform(MAX_DENSITY);
			puma[i][j] = random_uniform(MAX_DENSITY);
		}
	}

	delta_t = eqn_obj.delta_t;
	for (time_interval = delta_t; time_interval < max_iter; time_interval += delta_t)
	{
		++write_interval;
		compute(map, puma, hare, nx, ny, &eqn_obj);

		if( (write_interval % time_step_size) != 0) continue;
		
		debug_msg("[%s:%d]: Writing ppm for time_interval %f\n",__FILE__,__LINE__,time_interval);
		if ((puma_errno = write_ppm_file(map, hare, puma, nx, ny, write_interval)))
		{
			error_msg("[%s:%d]: Error writing ppm file for iter %d: %s\n",__FILE__,__LINE__,1,puma_strerror(puma_errno));
            		exit(EXIT_FAILURE);
		}
	}

	return 0;
}

void
print_usage(char *argv[])
{
	fprintf(stdout,"Usage e.g.: %s -f file.dat <-r 0.08> <-a 0.04> <-b 0.02> <-m 0.06> <-k 0.2> <-l 0.2> <-d 0.04> <-t 20>\n",argv[0]);
	fprintf(stdout,"               -f : Input file for land/water bitmask\n");
	fprintf(stdout,"               -r : rate of prey population increase\n");
	fprintf(stdout,"               -a : predation rate coefficient\n");
	fprintf(stdout,"               -b : reproduction rate of predators\n");
	fprintf(stdout,"               -m : predator mortality rate\n");
	fprintf(stdout,"               -k : diffusion rate of hares\n");
	fprintf(stdout,"               -l : diffusion rate of predators\n");
	fprintf(stdout,"               -d : Delta T\n");
	fprintf(stdout,"               -t : Timestep size\n");
}

int
parse_command_line(int argc, char *argv[], EquationVariables *eqn_obj, int *time_step_size, char *filename, int max_iter)
{
	char ch = '\0';

	/* Set the defaults */
	*time_step_size = 20;
	eqn_obj->delta_t = 0.004;
        eqn_obj->prey_pop_inc_rate = 0.08;
        eqn_obj->pred_rate_coeff = 0.04;
        eqn_obj->rep_rate_pred = 0.02;
        eqn_obj->pred_mort_rate = 0.06;
        eqn_obj->diff_rate_hares = 0.2;
        eqn_obj->diff_rate_pumas = 0.2;


	while((ch = (char) getopt(argc, argv, "hr:a:b:m:k:l:t:f:d:")) != -1 )
	{
		switch(ch)
		{
			case 'h': /* help */
				print_usage(argv);
				exit(0);
				break;

			case 'r':
				eqn_obj->prey_pop_inc_rate = atof(optarg);
                if (eqn_obj->prey_pop_inc_rate < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'a':
				eqn_obj->pred_rate_coeff = atof(optarg);
                if (eqn_obj->pred_rate_coeff < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'b':
				eqn_obj->rep_rate_pred = atof(optarg);
                if (eqn_obj->rep_rate_pred < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'm':
				eqn_obj->pred_mort_rate = atof(optarg);
                if (eqn_obj->pred_mort_rate < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'k':
				eqn_obj->diff_rate_hares = atof(optarg);
                if (eqn_obj->diff_rate_hares < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'l':
				eqn_obj->diff_rate_pumas = atof(optarg);
                if (eqn_obj->diff_rate_pumas < 0)
                    return PUMA_ERROR_BAD_CMD_ARG_NEG;
				break;

			case 'd':
				eqn_obj->delta_t = atof(optarg);
                if (eqn_obj->delta_t < 0 || eqn_obj->delta_t > 1)
                    return PUMA_ERROR_BAD_CMD_ARG_DEL_T;
				break;

			case 'f':
				strncpy(filename, optarg, PUMA_FILENAME_SIZE);
				break;

			case 't':
				*time_step_size = atoi(optarg);
                if (*time_step_size <= 0 || *time_step_size > max_iter)
                    return PUMA_ERROR_BAD_CMD_ARG_WO_IN;
				break;

			default:
				debug_msg("[%s:%d]: Setting defaults for equations params\n",__FILE__,__LINE__);
				break;
		}
	}

	if (strlen(filename) <= 0)
	{
		error_msg("[%s:%d]: ERROR!! Missing mandatory input file field\n",__FILE__,__LINE__);
		error_msg("[%s:%d]: Usage: puma -f file.dat\n",__FILE__,__LINE__);
		exit(1);
	}
    return 0;

}
