#include "Sun.h"
#include "Figures.h"

int main()
{
	Read_SSM(); // read in the solar density profile from file

	// do the calculations for the plots in the paper, the numbers are save in data/.
	// Then run the appropriate python script to generate the plot.
	phi0_Plot();
	phi_m_lambda_Plot();

	return 0;
}
