#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "o_grid_main.hpp"

//constructor
O_Grid_Main::O_Grid_Main(Airfoil_Parameters airfoil_pars, Parameters pars): Grid_Process(airfoil_pars, pars) {


	//make local vars
	int xi_nodes		=	pars.max_xi_nodes;
	int xi_stretch_type	=	pars.xi_stretch_type;

	//main process for o-grid
	o_grid_initialization();
	airfoil_pars_new = grid_airfoil_interpolation(xi_nodes, xi_stretch_type);
}

void O_Grid_Main::o_grid_initialization() {

	std::ifstream read_file;
	read_file.open("input/o_grid_parameters.dat");
	if(read_file.is_open()) {
		std::string a_line;
		int count = 0;
		int number;
		std::string alt_text;
		while (read_file >> number >> alt_text) {
			count++;
			switch(count) {
				case 1:
					o_grid_radius = number;
					break;
				default:
					std::cout << "Wrong Input!!!" << std::endl;
			}
		}

	} else {
		std::cout << "it is not opened!\n";
	}

}

Grid_Parameters O_Grid_Main::o_grid_boundary(Parameters pars) {

	Grid_Parameters grid_pars;

	//make local vars
	int max_xi_nodes	=	pars.max_xi_nodes;
	int max_eta_nodes	=	pars.max_eta_nodes;
	int xi_stretch_type	=	pars.xi_stretch_type;
	int eta_stretch_type	=	pars.eta_stretch_type;

	std::vector<double> &x_xi_outer_boundary	=	grid_pars.x_xi_outer_boundary;
	std::vector<double> &y_xi_outer_boundary	=	grid_pars.y_xi_outer_boundary;
	std::vector<double> &x_xi_inner_boundary	=	grid_pars.x_xi_inner_boundary;
	std::vector<double> &y_xi_inner_boundary	=	grid_pars.y_xi_inner_boundary;

	std::vector<double> &x_eta_upper_boundary	=	grid_pars.x_eta_upper_boundary;
	std::vector<double> &y_eta_upper_boundary	=	grid_pars.y_eta_upper_boundary;
	std::vector<double> &x_eta_lower_boundary	=	grid_pars.x_eta_lower_boundary;
	std::vector<double> &y_eta_lower_boundary	=	grid_pars.y_eta_lower_boundary;

	//x and y xi inner boundary
	x_xi_inner_boundary	=	airfoil_pars_new.x;
	y_xi_inner_boundary	=	airfoil_pars_new.y;
	
	//x and y xi outer boundary
	//assign the vector with number of grids
	x_xi_outer_boundary.resize(max_xi_nodes);
	y_xi_outer_boundary.resize(max_xi_nodes);

	for (auto i = 0; i < max_xi_nodes; i++) {
		//compute x and y outer value 
		if (i < (max_xi_nodes+1)/2 - 1) {

			double temp_x_outer	=	1 - grid_stretching_equal(i, (max_xi_nodes+1)/2);	
			x_xi_outer_boundary[i]	=	(o_grid_center_x - o_grid_radius) + 2*o_grid_radius*temp_x_outer;
			y_xi_outer_boundary[i]	=	o_grid_center_y + -1*math_f.circle_return_y(x_xi_outer_boundary[i], o_grid_radius, o_grid_center_x, 0);

		} else if (i > (max_xi_nodes+1)/2 - 1){

			double temp_x_outer	=	grid_stretching_equal(i + 1 - (max_xi_nodes+1)/2, (max_xi_nodes+1)/2);	
			x_xi_outer_boundary[i]	=	(o_grid_center_x - o_grid_radius) + 2*o_grid_radius*temp_x_outer;
			y_xi_outer_boundary[i]	=	o_grid_center_y + math_f.circle_return_y(x_xi_outer_boundary[i], o_grid_radius, o_grid_center_x, 0);

		} else {

			x_xi_outer_boundary[i]	=	o_grid_center_x - o_grid_radius;
			y_xi_outer_boundary[i]	=	o_grid_center_y;

		}

	}

	//x and y eta lower and upper boundary
	x_eta_upper_boundary.resize(max_eta_nodes);
	y_eta_upper_boundary.resize(max_eta_nodes);
	x_eta_lower_boundary.resize(max_eta_nodes);
	y_eta_lower_boundary.resize(max_eta_nodes);

	double x_eta_min	=	x_xi_inner_boundary[0];
	double y_eta_min	=	y_xi_inner_boundary[0];
	double x_eta_max	=	x_xi_outer_boundary[0];
	double y_eta_max	=	y_xi_outer_boundary[0];

	for (auto i = 0; i < max_eta_nodes; i++) {

		double temp_x		=	grid_stretching_equal(i, max_eta_nodes);
		
		//x and y eta upper boundary
		x_eta_upper_boundary[i]	=	x_eta_min + temp_x*(x_eta_max - x_eta_min);
		y_eta_upper_boundary[i]	=	o_grid_center_y;

		//x and y eta lower boundary
		x_eta_lower_boundary[i]	=	x_eta_min + temp_x*(x_eta_max - x_eta_min);
		y_eta_lower_boundary[i]	=	o_grid_center_y;

	}

	return grid_pars;
}

Grid_Parameters O_Grid_Main::o_grid_internal(Grid_Parameters grid_pars, Parameters pars) {
	
	//make local var
	std::vector<std::vector<double>> &x	=	grid_pars.x;
	std::vector<std::vector<double>> &y	=	grid_pars.y;
	int max_xi_nodes	=	pars.max_xi_nodes;
	int max_eta_nodes	=	pars.max_eta_nodes;
	
	//make Grid_Computation_Parameters struct
	Grid_Computation_Parameters grid_comp_pars;
	grid_comp_pars	=	grid_algebra_boundary_calc(pars);
	grid_comp_pars	=	grid_algebra_calc(grid_comp_pars, max_xi_nodes, max_eta_nodes);
	grid_pars	=	grid_algebra_real_calc(grid_pars, grid_comp_pars, max_xi_nodes, max_eta_nodes);
	return grid_pars;

}
