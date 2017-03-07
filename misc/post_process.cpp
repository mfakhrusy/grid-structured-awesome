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
