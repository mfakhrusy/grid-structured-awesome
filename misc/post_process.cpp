#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "post_process.hpp"

void Post_Process::grid_output(Grid_Parameters grid_pars, Parameters pars) {

	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	std::vector<std::vector<double>> AR	=	grid_pars.cell_aspect_ratio;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;

	double z_value	=	0;
	//print grid
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
	output_file.close();

	//print mesh quality
	output_file.open("output/mesh_quality.csv");
	if (output_file.is_open()) {
		
		output_file << "AR" << std::endl;
		for (auto i = 0; i < max_xi_nodes - 1; i++) {
			for (auto j = 0; j < max_eta_nodes - 1; j++) {
				
				output_file << AR[i][j] << std::endl;
			}
		}
	}
	output_file.close();

}

Grid_Parameters Post_Process::aspect_ratio_computation(Grid_Parameters grid_pars, Parameters pars) {
	
	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;
	
	//define new variable of delta_x and delta_y with each respective length
	std::vector<std::vector<double>> cell_aspect_ratio(max_xi_nodes-1, std::vector<double> (max_eta_nodes-1));

	//calculate delta_x and delta_y
	for (auto i = 0; i < max_xi_nodes-1; i++) {
		for (auto j = 0; j < max_eta_nodes-1; j++) {

			double delta_x_eta	=	x[i][j+1] - x[i][j];
			double delta_y_eta	=	y[i][j+1] - y[i][j];
			double delta_x_xi	=	x[i+1][j] - x[i][j];
			double delta_y_xi	=	y[i+1][j] - y[i][j];

			double s_eta		=	sqrt(pow(delta_x_eta,2) + pow(delta_y_eta,2));
			double s_xi		=	sqrt(pow(delta_x_xi,2) + pow(delta_y_xi,2));

			//check which one is larger
			if (s_eta > s_xi) {
				cell_aspect_ratio[i][j]	=	s_eta/s_xi;
			} else {
				cell_aspect_ratio[i][j]	=	s_xi/s_eta;
			}

//			std::cout << i << " " << j << " " << s_eta << " " << s_xi << " " << cell_aspect_ratio[i][j] << std::endl;
		}
	}
	
	//copy the result to new object
	grid_pars.cell_aspect_ratio	=	cell_aspect_ratio;

	return grid_pars;
}
