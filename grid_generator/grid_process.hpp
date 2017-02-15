#ifndef GRID_PROCESS_HPP
#define GRID_PROCESS_HPP
#include "../math_libs/spline.h"

class Grid_Process {
	std::vector<double> x;
	std::vector<double> y;


	protected:
		double grid_stretching_equal(double i, double N);

	public:
		Airfoil_Parameters grid_airfoil_interpolation(int airfoil_nodes, int xi_stretch_type);
		Grid_Process(Airfoil_Parameters airfoil_pars, Parameters pars);

};

#endif
