#include <puma.h>

int land[NX][NY];
double hare[NX][NY], puma[NX][NY];
double hare_new[NX][NY], puma_new[NX][NY];

/* To reduce access to memory, the compute function assumes the population densities on water cells are initialised to -1 */
void
compute(double hare[NX][NY], double puma[NX][NY], int land[NX][NY], int nx, int ny, double delta_t, double r, 
    double a, double k, double b, double m, double l) 
{
    int n, i, j;
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            hare_new[i][j] = puma_new[i][j] = 0;
        }
    }
    for (i = 1; i < nx - 1; i++)
    {
        for (j = 1; j < ny - 1; j++)
	{
		/* If cell[i][j] is land, compute the new value of hare and puma.*/
		if (land[i][j] == 0) continue;

		int n = land[i - 1][j] + land[i + 1][j] + land[i][j - 1] + land[i][j + 1];

		hare_new[i][j] = hare[i][j] + delta_t * (r * hare[i][j] - a * hare[i][j] * puma[i][j] + 
				k * (hare[i - 1][j] + hare[i + 1][j] + hare[i][j - 1] + hare[i][j + 1] - n * hare[i][j]));

		puma_new[i][j] = puma[i][j] + delta_t * (b * hare[i][j] * puma[i][j] - m * puma[i][j] + 
				l * (puma[i - 1][j] + puma[i + 1][j] + puma[i][j - 1] + puma[i][j + 1] - n * puma[i][j]));


		/* If cell[i][j] is water, do nothing. */

	}
    }
    /*` Replace the old hare and puma matrices by the new two. */
    for (i = 1; i < nx - 1; i++)
    {
        for (j = 1; j < ny - 1; j++)
        {
            hare[i][j] = hare_new[i][j];
            puma[i][j] = puma_new[i][j];
        }
    }
}

/*int main(void)
{

    double r, a, k, b, m, l, delta_t;
    int i, j, nx, ny;
    delta_t = 0.4;
    nx = ny = 10;
    r = 0.08;
    a = 0.04;
    b = 0.02;
    m = 0.06;
    k = l = 0.2;
    printf("NX: %d, NY: %d\n", NX, NY);
    
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            if (i == 0 || i == nx - 1 || j == 0 || j == ny -1)
            {
                land[i][j] = 0;
                hare[i][j] = .0;
                puma[i][j] = .0;
            }
            else
            {
                land[i][j] = 1;
                hare[i][j] = 0;
                puma[i][j] = 0;
            }
        }
    }
    hare[5][5] = 1;
    puma[5][5] = 0;

    printf("Hare matrix before: \n");
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            printf("%f " , hare[i][j]);
        }
        printf("\n");
    }

        compute(hare, puma, land, nx, 
        ny, delta_t, r, a, k, b, m, l); 
    printf("Hare matrix after: \n");
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            printf("%f " , hare[i][j]);
        }
        printf("\n");
    }
}*/
