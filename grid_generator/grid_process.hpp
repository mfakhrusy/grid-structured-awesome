#ifndef GRID_PROCESS_HPP
#define GRID_PROCESS_HPP
#include "../math_libs/spline.h"

class Grid_Process {

	std::vector<double> airfoil_x;
	std::vector<double> airfoil_y;

	protected:
		//variables

		//methods
		std::vector<std::vector<double>> grid_algebra_correction(Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes);
		Grid_Computation_Parameters grid_algebra_boundary_calc(Parameters pars);
		Grid_Computation_Parameters grid_algebra_calc(Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes);
		Grid_Parameters grid_algebra_real_calc(Grid_Parameters grid_pars, Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes);

		double grid_stretching_equal(int i, int N);
		double grid_stretching_cos(double i, double N);
		double grid_stretching_exp(double i, double N);
		double grid_stretching_invexp(double i, double N);
		Airfoil_Parameters grid_airfoil_interpolation(int airfoil_nodes, int xi_stretch_type);

	public:
		Grid_Process(Airfoil_Parameters airfoil_pars, Parameters pars);

};

#endif
