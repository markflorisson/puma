#include <puma.h>
#include <stdlib.h>

#define BUFSIZE 128

#define XPIXELS 12
#define YPIXELS 12

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
	fclose(file);

	*nxp = nx;
	*nyp = ny;

	return PUMA_NOERR;
}

void
write_pixel_row(FILE *file, int *pixel_row, const int PIXBUFSIZE)
{
	int i = 0;
	int j = 0;

	/* copy the pixel rows XPIXELS number of times */
	for(j = 0; j < XPIXELS; j++)
	{
		for(i = 0; i < PIXBUFSIZE; i++)
		{
			fprintf(file, "%d ",pixel_row[i]);
		}
		fprintf(file,"\n");
	}
}

void
copy_to_buf(int *pixel_buffer, int *pixel, int *pixel_counter)
{

	int i = 0;
	int j = *pixel_counter;

	for(i = 0; i < YPIXELS; i++)
	{
		pixel_buffer[j++] = pixel[RED];
		pixel_buffer[j++] = pixel[GREEN];
		pixel_buffer[j++] = pixel[BLUE];
	}

	*pixel_counter = j;
}

int
write_ppm_file(int map[NX][NX], REAL hare[NX][NY], REAL puma[NX][NY], const int nx, const int ny, const int iter_num)
{
	int i = 0, j = 0;
	char filename[64] = {'\0'};
	int pixel[3] = {0};
	int red_val = 0, green_val = 0;
	int pixel_counter = 0;
	int *pixel_buffer = NULL;
	const int PIXBUFSIZE = YPIXELS * ny * 3; /* 3 since each pixel has RGB */

	sprintf(filename,"pumaHare_%d.ppm",iter_num);

	FILE *file = fopen(filename, "w");

	if (!file) return PUMA_OSERROR;

	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n",YPIXELS * ny,XPIXELS * nx);
	fprintf(file, "%d\n",MAX_COLOR_VAL);

	pixel_buffer = (int*)(malloc(sizeof(int) * PIXBUFSIZE));

	for (i = 1; i <= nx; i++)
	{
		for (j = 1; j <= ny; j++)
		{
			if (map[i][j] == 0)
			{
				/* color water grids with blue */
				pixel[BLUE] = MAX_COLOR_VAL / 2;
				copy_to_buf(pixel_buffer,pixel, &pixel_counter);
				memset(pixel,0,sizeof(int)*3);
				continue;
			}

			/* set puma color value propotional to puma density in the cell */
			red_val =  ( (puma[i][j] * MAX_COLOR_VAL) / MAX_DENSITY );

			/* set hare color value propotional to hare density in the cell */
			green_val = (hare[i][j] * MAX_COLOR_VAL) / MAX_DENSITY;

			pixel[RED] = red_val;
			pixel[GREEN] = green_val;
			pixel[BLUE] = 0;

			copy_to_buf(pixel_buffer, pixel, &pixel_counter);
			memset(pixel,0,sizeof(int)*3);
		}
		pixel_counter = 0;

		write_pixel_row(file,pixel_buffer,PIXBUFSIZE);
		memset(pixel_buffer,0,sizeof(int)*PIXBUFSIZE);
	}
	fclose(file);
	free(pixel_buffer);

	return PUMA_NOERR;

}

