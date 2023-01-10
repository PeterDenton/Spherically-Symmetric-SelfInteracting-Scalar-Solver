#include "Earth.h"

#define sq(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))

// if you use this code please also cite: https://doi.org/10.1016/0031-9201(81)90046-7

const double r_earth = 6371 * 1e5; // radius of earth in cm
const double r_earth_core = 3480 * 1e5; // radius of the outter core in cm

// r in cm
double Earth_density(double r)
{
	double x;
	x = r / r_earth;

	r /= 1e5; // now r is in km

	if (r < 1221.5)	return 13.0885 - 8.8381 * sq(x);
	if (r < 3480.0)	return 12.5815 - 1.2638 * x - 3.6426 * sq(x) - 5.5281 * cube(x);
	if (r < 5701.0)	return 7.9565 - 6.4761 * x + 5.5283 * sq(x) - 3.0807 * cube(x);
	if (r < 5771.0)	return 5.3197 - 1.4836 * x;
	if (r < 5971.0)	return 11.2494 - 8.0298 * x;
	if (r < 6151.0)	return 7.1089 - 3.8045 * x;
	if (r < 6346.6)	return 2.6910 + 0.6924 * x;
	if (r < 6356.0) return 2.9;
	if (r < 6368.0) return 2.6;
	if (r <= 6371.0) return 1.02;

	return 0;
}

