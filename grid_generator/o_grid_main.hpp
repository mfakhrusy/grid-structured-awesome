#ifndef O_GRID_MAIN_HPP
#define O_GRID_MAIN_HPP
#include "../math_libs/math_function.hpp"
#include "grid_process.hpp"

class Math_Function;
class O_Grid_Main: private Grid_Process {

	Math_Function math_f;

	//variables
	double o_grid_radius;
	const double o_grid_center_x = 0.5;
	const double o_grid_center_y = 0.0;
	Airfoil_Parameters airfoil_pars_new;

	//methods
	void o_grid_initialization();

	public:
		O_Grid_Main(Airfoil_Parameters airfoil_pars, Parameters pars);
		Grid_Parameters o_grid_boundary(Parameters pars);

};

#endif
