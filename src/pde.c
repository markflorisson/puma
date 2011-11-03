#include <puma.h>

/* Matrices used in compute function. */
Real hare_new[NX][NY], puma_new[NX][NY];

void
compute(Real hare[NX][NY], Real puma[NX][NY], int land[NX][NY], int nx, int ny, EquationVariables *eq_val) 
{
	int n, i, j;
	Real delta_t, r, a, b, m, k, l;

	delta_t = eq_val -> time_interval;
	r = eq_val -> prey_pop_inc_rate;
	a = eq_val -> pred_rate_coeff;
	b = eq_val -> rep_rate_pred;
	m = eq_val -> pred_mort_rate;
	l = eq_val -> diff_rate_pumas;
	k = eq_val -> diff_rate_hares; 

	/* Initialising new matrices for hares and pumas. */
	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < ny; j++)
		{
			hare_new[i][j] = puma_new[i][j] = 0;
		}
	}

	/* Compute the densities for one iteration. */
	for (i = 1; i < nx - 1; i++)
	{
		for (j = 1; j < ny - 1; j++)
		{
			/* If cell[i][j] is water, skip this cell. */
			if (land[i][j] == 0) continue;

			int n = land[i - 1][j] + land[i + 1][j] + land[i][j - 1] + land[i][j + 1];

			hare_new[i][j] = hare[i][j] + delta_t * (r * hare[i][j] - a * hare[i][j] * puma[i][j] + 
					k * (hare[i - 1][j] + hare[i + 1][j] + hare[i][j - 1] + hare[i][j + 1] - n * hare[i][j]));

			puma_new[i][j] = puma[i][j] + delta_t * (b * hare[i][j] * puma[i][j] - m * puma[i][j] + 
					l * (puma[i - 1][j] + puma[i + 1][j] + puma[i][j - 1] + puma[i][j + 1] - n * puma[i][j]));
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

/*
int main(void)
{

    Real r, a, k, b, m, l, delta_t;
    int i, j, iter, nx, ny;
    EquationVariables eqn_obj;
    delta_t = 0.4;
    nx = ny = 10;
    r = 0.08;
    a = 0.04;
    b = 0.02;
    m = 0.06;
    k = l = 0.2;
    eqn_obj.time_interval = delta_t;
    eqn_obj.prey_pop_inc_rate = .08;
    eqn_obj.pred_rate_coeff = .04;
    eqn_obj.rep_rate_pred = .02;
    eqn_obj.pred_mort_rate = .06;
    eqn_obj.diff_rate_hares = eqn_obj.diff_rate_pumas = .2;
    printf("NX: %d, NY: %d\n", NX, NY);
    
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            if (i == 0 || i == nx - 1 || j == 0 || j == ny - 1)
            {
                map[i][j] = 0;
                hare[i][j] = .0;
                puma[i][j] = .0;
            }
            else
            {
                map[i][j] = 1;
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
    for (iter = 0; iter < 10; iter++)
    {    
        compute(hare, puma, map, nx, 
            ny, &eqn_obj); 
        printf("Hare matrix after: \n");
        for (i = 0; i < nx; i++)
        {
            for (j = 0; j < ny; j++)
            {
                printf("%f " , hare[i][j]);
            }
            printf("\n");
        }
    }
}*/
