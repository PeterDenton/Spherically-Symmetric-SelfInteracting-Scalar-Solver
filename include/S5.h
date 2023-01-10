#ifndef S5_H
#define S5_H

// hard sphere: r_s=1, n=1, g=1 (default)
enum object {HARD, SUN, EARTH};

double r_s(object obj);
//double phi0_Guess(double m, double lambda, double g, object obj);
double phi0_KG(double m, double lambda, double g, object obj);
void KG(double m, double lambda, double g, object obj, double phi0, double r_max, int n, double *rs, double *phis);

#endif
