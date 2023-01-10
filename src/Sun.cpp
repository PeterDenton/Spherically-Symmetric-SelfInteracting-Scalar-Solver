#include <fstream>
#include <sstream>
#include <vector>

#include "Sun.h"
#include "Interpolate.h"

#define sq(x) ((x)*(x))

// if you use this code, please also cite: https://arxiv.org/abs/astro-ph/0412440
// the data file can be found here: http://www.sns.ias.edu/~jnb/SNdata/Export/BS2005/bs05op.dat
// more information can be found here: http://www.sns.ias.edu/~jnb/SNdata/sndata.html

const double r_sun = 6.957e10; // cm

std::vector<std::pair<double, double>> solar_rhos; // g/cc

void Read_SSM()
{
	double r, tmp, rho;
	std::string line;
	std::string fname = "input/bs05op.dat";
	std::ifstream data(fname);
	while (std::getline(data, line))
	{
		std::istringstream ss(line);
		ss >> tmp >> r >> tmp >> rho;
		solar_rhos.push_back(std::make_pair(r, rho));
	}
	data.close();
}
// r in cm
// g/cc
double Solar_density(double r)
{
	r /= r_sun;
	if (r > solar_rhos.back().first) return 0;
	if (r < solar_rhos[0].first) return solar_rhos[0].second;
	return Interpolate(r, solar_rhos);
}

