#ifndef GRID_PROCESS_HPP
#define GRID_PROCESS_HPP
#include "../math_libs/spline.h"

class Grid_Process {

	std::vector<double> airfoil_x;
	std::vector<double> airfoil_y;

	protected:
		//variables
		struct Grid_Computation_Parameters {
			std::vector<std::vector<double>> xi;	//x computational domain
			std::vector<std::vector<double>> eta;	//y computational domain
		};

		class Grid_Algebra_Process {
			std::vector<double> xi_upper;
			std::vector<double> xi_lower;
			std::vector<double> eta_left;
			std::vector<double> eta_right;

			std::vector<std::vector<double>> grid_algebra_correction();

			public:
				Grid_Computation_Parameters grid_algebra_computation();	
				Grid_Algebra_Process();
				
		};

		//methods
		double grid_stretching_equal(double i, double N);
		double grid_stretching_cos(double i, double N);
		double grid_stretching_exp(double i, double N);
		double grid_stretching_invexp(double i, double N);
		Airfoil_Parameters grid_airfoil_interpolation(int airfoil_nodes, int xi_stretch_type);

	public:
		Grid_Process(Airfoil_Parameters airfoil_pars, Parameters pars);

};

#endif
