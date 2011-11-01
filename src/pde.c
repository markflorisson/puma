#include <puma.h>

/* To reduce access to memory, the compute function assumes the population densities on water cells are initialised to -1 */
void
compute(double hare[NX][NY], double puma[NX][NY], int land[NX][NY], int nx, int ny, int delta_t, int total_time, int r, 
    int a, int k, int b, int m, int l) 
{
    int n, i, j, max_iter;
    double hare_new[NX][NY], puma_new[NX][NY];
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            hare_new[i][j] = puma_new[i][j] = 0;
        }
    }
    max_iter = total_time / delta_t;
    for (n = 0; k < max_iter; n++)
    {
        for (i = 1; i < nx - 1; i++)
        {
            for (j = 1; j < ny - 1; j++)
            {
                /* If cell[i][j] is land, compute the new value of hare and puma.*/
                if (land[i][j] == 1)
                {
                    int n = land[i - 1][j] + land[i + 1][j] + land[i][j - 1] + land[i][j + 1];
                    hare_new[i][j] = hare[i][j] + delta_t * (r * hare[i][j] - a * hare[i][j] * puma[i][j] + 
                        k * (hare[i - 1][j] + hare[i + 1][j] + hare[i][j - 1] + hare[i][j + 1] - n * hare[i][j]));
                    puma_new[i][j] = puma[i][j] + delta_t * (b * hare[i][j] * puma[i][j] - m * puma[i][j] + 
                        l * (puma[i - 1][j] + puma[i + 1][j] + puma[i][j - 1] + puma[i][j + 1] - n * puma[i][j]));
                    

                }
                /* If cell[i][j] is water, do nothing. */

            }
        }
        /* Replace the old hare and puma matrices by the new two. */
        for (i = 1; i < nx - 1; i++)
        {
            for (j = 1; j < ny - 1; j++)
            {
                hare[i][j] = hare_new[i][j];
                puma[i][j] = puma_new[i][j];
            }
        }
    }

}
