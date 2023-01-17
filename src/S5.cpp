#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_roots.h>

#include <cmath>
#include <cassert>
#include <stdexcept>

#include "S5.h"
#include "Earth.h"
#include "Sun.h"

#define sq(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))

// the radius of the source
double r_s(object obj)
{
	switch (obj)
	{
		case HARD:
			return 1;
		case SUN:
			return r_sun;
		case EARTH:
			return r_earth;
		default:
			throw std::domain_error("Object not in list in the radius function\n");
	} // switch object
}

// the coupling times the density of the source
double n(double r, object obj)
{
	switch (obj)
	{
		case HARD:
			if (r < 1)	return 1;
			else		return 0;
		case SUN:
			if (r < r_sun)	return Solar_density(r); // r in cm, density is in g/cc
			else			return 0;
		case EARTH:
			if (r < r_earth)	return Earth_density(r); // r in cm, density is in g/cc
			else				return 0;
		default:
			throw std::domain_error("Object not in list in the density function\n");
	} // switch object
}

// y[0] is phi and y[1] is dphi/dr
struct diff_eq_params {double m, lambda, phi0; object obj;};
int diff_eq(double r, const double y[], double f[], void *params)
{
	diff_eq_params p = *(diff_eq_params*)params;
	f[0] = y[1];

	// X is the term that is phi'/r which is 0/0 at the origin
	double X;
	if (r < 1e-5 * r_s(p.obj))
		X = (n(0, p.obj) / 3) + (p.lambda / 18) * cube(p.phi0) + (sq(p.m) / 3) * p.phi0;
	else
		X = y[1] / r;
	f[1] = n(r, p.obj) - 2. * X + sq(p.m) * y[0] + p.lambda * cube(y[0]) / 6;
	return GSL_SUCCESS;
}

// empty jacobian to make gsl happy
int jac_empty(double r, const double y[], double *dfdy, double dfdr[], void *params) { assert(false); }

// a guess for phi0, seems to be good to <~1% at m=0 and <~10% for m!=0 in the HARD case.
double phi0_Guess(double m, double lambda, object obj)
{
	double alpha, A, B, radius;
	radius = r_s(obj);

	alpha = 2.; // seems to be a good guess for the shape parameter

	// A: the low lambda limit
	if (m * radius < 1e-5)
		A = 0.5 * sq(radius) * n(0, obj);
	else
		A = n(0, obj) * (1 - exp(-m * radius) * (1 + m * radius)) / sq(m);
	if (lambda == 0) return -A;

	// B: the large lambda limit
	B = pow(6 * n(0, obj) / lambda, 1. / 3);

	return -pow(pow(A, -alpha) + pow(B, -alpha), -1. / alpha);
}

// helper function and parameters for the differential equation solver
struct phi0_KG_helper_params {double m, lambda, phi0_guess; object obj;};
double phi0_KG_helper(double phi0, void *params)
{
	phi0_KG_helper_params p = *(phi0_KG_helper_params*)params;

	double r1 = 1e3 * r_s(p.obj); // go out 1k times the radius
	const int n_repeat = 1e5; // 100 steps within the sphere

	diff_eq_params p2 = {p.m, p.lambda, phi0, p.obj};
	gsl_odeiv2_system sys = {diff_eq, jac_empty, 2, &p2};
	gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk8pd, 1e-10, 1e-10, 1e-10);
	double r = 0;
	double y[2] = {phi0, 0};

	for (int i = 0; i <= n_repeat; i++)
	{
		double ri = i * r1 / n_repeat;
		int status = gsl_odeiv2_driver_apply(d, &r, ri, y);

		if (y[1] < 0) // derivative is negative
			return log(ri / (2 * r1)); // < 0
		if (y[0] > 0) // R(0) < 0
			return -log(ri / (2 * r1)); // > 0

		if (status != GSL_SUCCESS)
			assert(false);
	}
	gsl_odeiv2_driver_free(d);

	return log(y[0] / p.phi0_guess) / 1e2; // < 0
}
double phi0_KG(double m, double lambda, object obj)
{
	int status;
	int iter = 0, max_iter = 100;
	const gsl_root_fsolver_type *T;
	double phi0_guess = phi0_Guess(m, lambda, obj);
	gsl_root_fsolver *s;
	phi0_KG_helper_params p = {m, lambda, phi0_guess, obj};
	double phi0_lo, phi0_hi;

	// may need to tweak this to get a viable window to find the correct value for phi0
	phi0_lo = phi0_guess * 1.8;
	phi0_hi = phi0_guess / 1.8;
	//printf("phi0: [%g, %g], f: [%g, %g]\n", phi0_lo, phi0_hi, phi0_KG_helper(phi0_lo, &p), phi0_KG_helper(phi0_hi, &p)); // useful print statement for diagnostics of the above window

	double r;
	gsl_function F;

	F.function = &phi0_KG_helper;
	F.params = &p;

	T = gsl_root_fsolver_brent;
	s = gsl_root_fsolver_alloc(T);
	gsl_root_fsolver_set(s, &F, phi0_lo, phi0_hi);

	do
	{
		iter++;
		status = gsl_root_fsolver_iterate(s);
		r = gsl_root_fsolver_root(s);
		phi0_lo = gsl_root_fsolver_x_lower(s);
		phi0_hi = gsl_root_fsolver_x_upper(s);
		status = gsl_root_test_interval(phi0_lo, phi0_hi, 0, 1e-5);
	}
	while (status == GSL_CONTINUE && iter < max_iter);

	gsl_root_fsolver_free(s);

	return r;
}

// takes as an input the correct value for phi0
// calculates the field at many values up to r_max
void KG(double m, double lambda, object obj, double phi0, double r_max, int n, double *rs, double *phis)
{
	diff_eq_params p = {m, lambda, phi0, obj};
	gsl_odeiv2_system sys = {diff_eq, jac_empty, 2, &p};
	gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk8pd, 1e-14, 1e-14, 1e-14);
	double r = 0;
	double y[2] = {phi0, 0};

	for (int i = 0; i <= n; i++)
	{
		double ri = i * r_max / n;
		int status = gsl_odeiv2_driver_apply(d, &r, ri, y);

		rs[i] = ri;

		if (status != GSL_SUCCESS)
		{
			for (int j = i; j <= n; j++)
			{
				rs[j] = j * r_max / n;
				phis[j] = -1e100;
			}
			break;
		}
		else
			phis[i] = y[0];
	}
	gsl_odeiv2_driver_free(d);
}

