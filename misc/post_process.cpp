#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "post_process.hpp"

void Post_Process::grid_output(Grid_Parameters grid_pars, Parameters pars) {

	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;

	double z_value	=	0;
	std::ofstream output_file ("output/grid_output.csv");
	if (output_file.is_open()) {

		output_file << "x_coord,y_coord,z_coord" << std::endl;
		for (auto i = 0; i < max_xi_nodes; i++) {
			for (auto j = 0; j < max_eta_nodes; j++) {

				output_file << x[i][j] << ",";
				output_file << y[i][j] << ",";
				output_file << z_value << std::endl;

			}
		}

	} else {
		std::cout << "output file cannot be made thank you\n";
	}	

}

void Post_Process::aspect_ratio_computation(Grid_Parameters grid_pars, Parameters pars) {
	
	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;
	
	//define new variable of delta_x and delta_y with each respective length
	std::vector<std::vector<double>> delta_x(max_xi_nodes-1, std::vector<double>(max_eta_nodes-1));
	std::vector<std::vector<double>> delta_y(max_xi_nodes-1, std::vector<double>(max_eta_nodes-1));

	//calculate delta_x and delta_y
	for (auto i = 0; i < max_xi_nodes-1; i++) {
		for (auto j = 0; j < max_eta_nodes-1; j++) {
			delta_x[i][j]	=	std::abs(x[i+1][j] - x[i][j]);
			delta_y[i][j]	=	std::abs(y[i][j+1] - y[i][j]);
			std::cout << i << " " << j << " " << delta_x[i][j] << " " << delta_y[i][j] << std::endl;
		}
	}
}
