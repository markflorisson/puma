#include <puma.h>

#define BUFSIZE 128

#define XPIXELS 12
#define YPIXELS 12

#define MAX_COLOR_VAL 50
#define MAX_DENSITY 5

enum { RED, GREEN, BLUE };


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

void
paint_cell(FILE *file, int *pixel, const int nx, const int ny)
{
	int numRows = XPIXELS * nx;
	int numCols = YPIXELS * ny;

	int i = 0, j = 0;
	
	for(i = 0; i < numRows; i++)
	{
		for(j = 0; j < numCols; j++)
		{
			fprintf(file, "%d %d %d",*pixel,*(pixel+1),*(pixel+2));
		}
	}

}

int 
write_ppm_file(int map[NX][NX], Real hare[NX][NY], Real puma[NX][NY], 
			const int nx, const int ny, const int delta_t,
			const int hare_density, const int puma_density)
{
	int i = 0, j = 0;
	char filename[64] = {'\0'};
	int pixel[3] = {0};
	int red_val = 0, green_val = 0;
	
	sprintf(filename,"pumaHare_%d.ppm",delta_t);
    
	FILE *file = fopen(filename, "w");

    	if (!file) return PUMA_OSERROR;

	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n",YPIXELS*ny,XPIXELS*nx);
	fprintf(file, "%d\n",MAX_COLOR_VAL);

	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < ny; j++)
		{
			if(map[i][j] == 0) 
			{
				/* color water grids with blue */
				pixel[BLUE] = MAX_COLOR_VAL / 2;
				paint_cell(file,pixel,nx,ny);

				continue;
			}

			/* Red color represents puma */
			red_val = (puma_density * MAX_COLOR_VAL) / MAX_DENSITY;

			/* Green color represents hare */
			green_val = (hare_density * MAX_COLOR_VAL) / MAX_DENSITY;

			pixel[RED] = red_val;
			pixel[GREEN] = green_val;
			pixel[BLUE] = 0;

			paint_cell(file,pixel,nx,ny);

		}
		fprintf(file,"\n");
	}

	fclose(file);

}

