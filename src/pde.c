#include <puma.h>

/* Intermediate  matrices used in compute function. */
//REAL hare_new[NX][NY] = {{0}}, puma_new[NX][NY] = {{0}};

/*
  Function to compute the Hare and Puma densities for each
  time step. The calculation uses a discretized for of the PDEs.
*/
void
compute(int map[NX][NY], REAL puma[NX][NY], REAL hare[NX][NY],
        REAL puma_new[NX][NY], REAL hare_new[NX][NY], int nx, int ny,
        EquationVariables *eq_val)
{
    int n, i, j;
    REAL delta_t, r, a, b, m, k, l;

    delta_t = eq_val -> delta_t;
    r = eq_val->prey_pop_inc_rate;
    a = eq_val->pred_rate_coeff;
    b = eq_val->rep_rate_pred;
    m = eq_val->pred_mort_rate;
    l = eq_val->diff_rate_pumas;
    k = eq_val->diff_rate_hares;

    /* Compute the densities for one iteration. */
    for (i = 1; i <= nx ; i++)
    {
        for (j = 1; j <= ny; j++)
        {
            /* If cell[i][j] is water, skip this cell. */
            if (map[i][j] == 0) continue;

            n = map[i - 1][j] + map[i + 1][j] + map[i][j - 1] + map[i][j + 1];

            hare_new[i][j] = hare[i][j] + delta_t * (r * hare[i][j] - a * hare[i][j] * puma[i][j] +
                    k * (hare[i - 1][j] + hare[i + 1][j] + hare[i][j - 1] + hare[i][j + 1] - n * hare[i][j]));

            puma_new[i][j] = puma[i][j] + delta_t * (b * hare[i][j] * puma[i][j] - m * puma[i][j] +
                    l * (puma[i - 1][j] + puma[i + 1][j] + puma[i][j - 1] + puma[i][j + 1] - n * puma[i][j]));
        }
    }

}
