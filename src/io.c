#include <puma.h>
#include <log.h>
#include <stdlib.h>
#include <math.h>

#define XPIXELS 1 
#define YPIXELS 1

#define MAX_SIZE 1000
#define MAX_COLOR_VAL 50

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
readmap(const char *filename, int map[NX][NY], int *nxp, int *nyp)
{
	int i, j;
	int nx, ny; /* bounds of the map */
	int result;
	FILE *file = fopen(filename, "r");

	if (!file)
		return PUMA_OSERROR;

	/* Scan the size of the X and Y axes */
	if ((result = fscanf(file, "%d %d", &ny, &nx)) != 2)
		return fscanf_error(file, result);

	/* Validate the axes */
	if (nx < 0 || ny < 0 || nx > NX || ny > NY)
		return PUMA_ERROR_OOB;

	/* Scan in all element of the array */
	for (i = 0; i < nx; i++) {
		for (j = 0; j < ny; j++) {
			int value = 0;

			if ((result = fscanf(file, "%d", &value)) != 1)
				return fscanf_error(file, result);

			map[i][j] = value;
		}
	}
	fclose(file);

	*nxp = nx;
	*nyp = ny;

	return PUMA_NOERR;
}

int
write_pixel_row(FILE *file, int *pixel_row, const int PIXBUFSIZE, const int scale_factor)
{
	int i = 0;
	int j = 0;

	/* copy the pixel rows XPIXELS number of times */
	for(j = 0; j < XPIXELS * scale_factor; j++)
	{
		for(i = 0; i < PIXBUFSIZE; i++)
		{
			if(fprintf(file, "%d ",pixel_row[i]) < 0)
				return PUMA_OSERROR;
		}
		if(fprintf(file,"\n") < 0)
			return PUMA_OSERROR;
	}

	return PUMA_NOERR;
}

void
copy_to_buf(int *pixel_buffer, int *pixel, int *pixel_counter, const int scale_factor)
{

	int i = 0;
	int j = *pixel_counter;

	for(i = 0; i < YPIXELS * scale_factor; i++)
	{
		pixel_buffer[j++] = pixel[RED];
		pixel_buffer[j++] = pixel[GREEN];
		pixel_buffer[j++] = pixel[BLUE];
	}

	*pixel_counter = j;
}

void writeMatrix(REAL matrix[NX][NY], const char* filename, const int iter)
{
	char name[128] = {'\0'};
	int i = 0, j = 0;
	FILE *file = NULL;

	sprintf(name,"%s_%d.out",filename,iter);
	file = fopen(name, "w");
	
	for(i = 0; i < NX; i++)
	{
		for(j = 0; j < NX; j++)
		{
			fprintf(file, "%f ", matrix[i][j]);
		}
		fprintf(file,"\n");
	}
	fclose(file);
	
}

int
write_ppm_file(int map[NX][NY], REAL hare[NX][NY], REAL puma[NX][NY], const int nx, const int ny, const float delta_t)
{
	int i = 0, j = 0;
	char filename[64] = {'\0'};
	int pixel[3] = {0};
	int red_val = 0, green_val = 0;
	int ret = PUMA_NOERR;

	int pixel_counter = 0;
	int *pixel_buffer = NULL;
	int PIXBUFSIZE = 0; 

	FILE *file = NULL;
	int scale_factor = 0;
	scale_factor = MAX_SIZE/ny;

	//writeMatrix(hare,"hare",delta_t);
	//writeMatrix(puma,"puma",delta_t);
	//writeMatrix(puma,"map",delta_t);

	if(scale_factor <= 0) scale_factor = 1; /* In case the array is bigger than MAX_SIZE */

	PIXBUFSIZE = YPIXELS * scale_factor * ny * 3; /* 3 since each pixel has RGB */

	debug_msg("[%s:%d]: scale_factor: %d\n",__FILE__,__LINE__,scale_factor);
	debug_msg("[%s:%d]: PIXBUFSIZE: %d\n",__FILE__,__LINE__,PIXBUFSIZE);

	sprintf(filename,"pumaHare_%03d.ppm",(int)delta_t);

	file = fopen(filename, "w");

	ret = PUMA_OSERROR;
	if (!file) 
		goto errexit;

	if (fprintf(file, "P3\n") < 0)
		goto errexit;

	if (fprintf(file, "%d %d\n", YPIXELS * scale_factor * ny, XPIXELS * scale_factor * nx) < 0)
		goto errexit;
		
	if (fprintf(file, "%d\n", MAX_COLOR_VAL) < 0)
		goto errexit;

	ret = PUMA_NOERR;

	pixel_buffer = (int*) malloc(sizeof(int) * PIXBUFSIZE);

	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < ny; j++)
		{
			if (map[i][j] == 0)
			{
				/* color water grids with blue */
				pixel[BLUE] = MAX_COLOR_VAL / 2;
				copy_to_buf(pixel_buffer, pixel, &pixel_counter, scale_factor);
				memset(pixel, 0, sizeof(int) * 3);
				continue;
			}

			/* set puma color value propotional to puma density in the cell */
			red_val = (puma[i][j] * MAX_COLOR_VAL) / MAX_DENSITY;

			/* set hare color value propotional to hare density in the cell */
			green_val = hare[i][j] * MAX_COLOR_VAL / MAX_DENSITY;

			pixel[RED] = red_val;
			pixel[GREEN] = green_val;
			pixel[BLUE] = 0;

			copy_to_buf(pixel_buffer, pixel, &pixel_counter, scale_factor);
			memset(pixel, 0, sizeof(int) * 3);
		}
		pixel_counter = 0;

		ret = write_pixel_row(file, pixel_buffer, PIXBUFSIZE, scale_factor);
		if (ret != PUMA_NOERR) goto errexit;

		memset(pixel_buffer, 0, sizeof(int) * PIXBUFSIZE);
	}

errexit:
	if (file) fclose(file);

	free(pixel_buffer);
	return ret;
}

