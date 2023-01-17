#include <stdio.h>
#include <cmath>

#include "Figures.h"
#include "S5.h"

void phi0_Plot()
{
	printf("Calculating phi(0) for different values of m and lambda...\n");

	double lambda, lambda_min, lambda_max, lambda_scale;
	double m, m_min, m_max, m_step;
	int n_m, n_lambda;
	object obj = HARD;

	n_lambda = 1e2; // 1e3 takes a bit, 1e2 doesn't take too long
	n_m = 2e0;

	m_min = 0;
	m_max = 2;
	m_step = (m_max - m_min) / n_m;

	lambda_min = 1e-1;
	lambda_max = 1e+5;
	lambda_scale = pow(lambda_max / lambda_min, 1. / n_lambda);

	FILE *data = fopen("data/phi0.txt", "w");
	for (int i = 0; i <= n_m; i++)
		fprintf(data, "%g ", m_min + i * m_step);
	fprintf(data, "\n");

	for (int i = 0; i <= n_lambda; i++)
	{
		if (i % 10 == 0) printf("Progress=%.2f\n", 1.0 * i / n_lambda);
		lambda = lambda_min * pow(lambda_scale, i);
		fprintf(data, "%g ", lambda);
		for (int j = 0; j <= n_m; j++)
		{
			m = m_min + j * m_step;
			fprintf(data, "%g ", phi0_KG(m, lambda, obj));
		} // j, n_m, m
		fprintf(data, "\n");
	} // i, n_lambda, lambda
	fclose(data);

	printf("Done.\n");
}

void phi_m_lambda_Plot()
{
	printf("Calculating phi at a range of radii for different discrete values of m and lambda...\n");
	const int n_lambda = 2, n_m = 2, n_r = 1e2;
	double lambda, lambdas[n_lambda] = {0, 1000};
	double m, ms[n_m] = {0, 1};
	double rs[n_r + 1], phis[n_m][n_lambda][n_r + 1];
	double phi0;
	object obj = HARD;

	// perform the calculations
	for (int i = 0; i < n_m; i++)
	{
		m = ms[i];
		for (int j = 0; j < n_lambda; j++)
		{
			lambda = lambdas[j];
			printf("m=%g, lambda=%g ...\n", m, lambda);
			phi0 = phi0_KG(m, lambda, obj);
			KG(m, lambda, obj, phi0, 2 * r_s(obj), n_r, rs, phis[i][j]);
		} // j, n_lambda, lambda
	} // i, n_m, m

	FILE *data = fopen("data/phi_m_lambda.txt", "w");
	// write the m and lambda values out
	for (int i = 0; i < n_m; i++)
		fprintf(data, "%g ", ms[i]);
	fprintf(data, "\n");
	for (int i = 0; i < n_lambda; i++)
		fprintf(data, "%g ", lambdas[i]);
	fprintf(data, "\n");

	// write the results out
	for (int i = 0; i <= n_r; i++)
	{
		fprintf(data, "%g ", rs[i]);
		for (int j = 0; j < n_m; j++)
		{
			for (int k = 0; k < n_lambda; k++)
			{
				fprintf(data, "%g ", phis[j][k][i]);
			} // k, n_lambda, lambda
		} // j, n_m, m
		fprintf(data, "\n");
	} // i, n_r, r
	fclose(data);

	printf("Done.\n");
}

