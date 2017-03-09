#include <fstream>
#include <sstream>
#include <algorithm>	//std::max_element
#include "../global.hpp"
#include "post_process.hpp"

void Post_Process::grid_output(Grid_Parameters grid_pars, Parameters pars) {

	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
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
	//local vars
	std::vector<std::vector<double>> cell_AR	=	grid_pars.cell_aspect_ratio;
	std::vector<std::vector<double>> cell_area	=	grid_pars.cell_area;
	output_file.open("output/mesh_quality.csv");
	if (output_file.is_open()) {
		
		output_file << "AR,area" << std::endl;
		for (auto i = 0; i < max_xi_nodes - 1; i++) {
			for (auto j = 0; j < max_eta_nodes - 1; j++) {
				
				output_file << cell_AR[i][j] << ",";
				output_file << cell_area[i][j] << std::endl;
			}
		}
	}
	output_file.close();

}

Grid_Parameters Post_Process::cell_aspect_ratio_computation(Grid_Parameters grid_pars, Parameters pars) {
	
	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;
	
	//define new variable 
	std::vector<std::vector<double>> cell_aspect_ratio(max_xi_nodes-1, std::vector<double> (max_eta_nodes-1));

	//calculate delta_x and delta_y
	for (auto i = 0; i < max_xi_nodes-1; i++) {
		for (auto j = 0; j < max_eta_nodes-1; j++) {

			double delta_x_xi	=	x[i+1][j] - x[i][j];
			double delta_y_xi	=	y[i+1][j] - y[i][j];
			double delta_x_eta	=	x[i][j+1] - x[i][j];
			double delta_y_eta	=	y[i][j+1] - y[i][j];

			double s_xi		=	sqrt(pow(delta_x_xi,2) + pow(delta_y_xi,2));
			double s_eta		=	sqrt(pow(delta_x_eta,2) + pow(delta_y_eta,2));

			//check which one is larger
			if (s_eta > s_xi) {
				cell_aspect_ratio[i][j]	=	s_eta/s_xi;
			} else {
				cell_aspect_ratio[i][j]	=	s_xi/s_eta;
			}
		}
	}
	
	//copy the result to new object
	grid_pars.cell_aspect_ratio	=	cell_aspect_ratio;

	return grid_pars;
}

//calculate the area of each cell -> smoothness test
Grid_Parameters Post_Process::cell_area_computation(Grid_Parameters grid_pars, Parameters pars) {

	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;
	
	//define new variable
	std::vector<std::vector<double>> cell_area(max_xi_nodes-1, std::vector<double> (max_eta_nodes-1));

	//calculate delta_x and delta_y
	for (auto i = 0; i < max_xi_nodes-1; i++) {
		for (auto j = 0; j < max_eta_nodes-1; j++) {
		
			//perform more correct area calculation, with cross product method
			//first calculate each vector member
			double delta_x_xi_0	=	x[i+1][j] - x[i][j];
			double delta_y_xi_0	=	y[i+1][j] - y[i][j];
			double delta_x_eta_0	=	x[i][j+1] - x[i][j];
			double delta_y_eta_0	=	y[i][j+1] - y[i][j];

			double delta_x_xi_1	=	x[i+1][j+1] - x[i+1][j];
			double delta_y_xi_1	=	y[i+1][j+1] - y[i+1][j];
			double delta_x_eta_1	=	x[i+1][j+1] - x[i][j+1];
			double delta_y_eta_1	=	y[i+1][j+1] - y[i][j+1];

			//after that, do cross product for the first triangle	-> ONLY FOR 2D
			double temp_area_lower	=	0.5*std::abs((delta_x_xi_0*delta_y_eta_0) - (delta_y_xi_0*delta_x_eta_0));
			double temp_area_upper	=	0.5*std::abs((delta_x_xi_1*delta_y_eta_1) - (delta_y_xi_1*delta_x_eta_1));

			cell_area[i][j]	=	temp_area_lower + temp_area_upper;
		}
	}
	
	//copy the result to new object
	grid_pars.cell_area	=	cell_area;

	return grid_pars;
}

//calculate cell skewness
Grid_Parameters Post_Process::cell_skewness_computation(Grid_Parameters grid_pars, Parameters pars) {

	//make local vars
	std::vector<std::vector<double>> x	=	grid_pars.x;
	std::vector<std::vector<double>> y	=	grid_pars.y;
	double max_xi_nodes			=	pars.max_xi_nodes;
	double max_eta_nodes			=	pars.max_eta_nodes;
	
	//define new variable
	std::vector<std::vector<double>> cell_skewness(max_xi_nodes-1, std::vector<double> (max_eta_nodes-1));

	//calculate delta_x and delta_y
	for (auto i = 0; i < max_xi_nodes-1; i++) {
		for (auto j = 0; j < max_eta_nodes-1; j++) {
		
			//define 2 local vectors
			std::vector<double> temp_cross_mag(4);
			std::vector<double> temp_angle(4);
			std::vector<double> temp_skewness(4);
			//perform more correct area calculation, with cross product method
			//first calculate each vector member
			double delta_x_xi_0	=	x[i+1][j] - x[i][j];
			double delta_y_xi_0	=	y[i+1][j] - y[i][j];
			double delta_x_eta_0	=	x[i][j+1] - x[i][j];
			double delta_y_eta_0	=	y[i][j+1] - y[i][j];

			double delta_x_xi_1	=	x[i+1][j+1] - x[i+1][j];
			double delta_y_xi_1	=	y[i+1][j+1] - y[i+1][j];
			double delta_x_eta_1	=	x[i+1][j+1] - x[i][j+1];
			double delta_y_eta_1	=	y[i+1][j+1] - y[i][j+1];

			//calculate length of each side
			double s_xi_0		=	sqrt(pow(delta_x_xi_0,2) + pow(delta_y_xi_0,2));
			double s_eta_0		=	sqrt(pow(delta_x_eta_0,2) + pow(delta_y_eta_0,2));
			double s_xi_1		=	sqrt(pow(delta_x_xi_1,2) + pow(delta_y_xi_1,2));
			double s_eta_1		=	sqrt(pow(delta_x_eta_1,2) + pow(delta_y_eta_1,2));

			//after that, do cross product for the first triangle	-> ONLY FOR 2D
			//cell skewness calculation will be performed by using cross product approach
			//first one need to calculate cross product of each two respective sides
			//4 angles, 0 is from the angle from bottom nodes (node-wise), and increase in a clockwise manner
			//the cross product should be into negative k (negative z result)
			//0 and 2 -> xi cross eta
			//1 and 3 -> eta cross xi
			temp_cross_mag[0]	=	std::abs((delta_x_xi_0*delta_y_eta_0) - (delta_y_xi_0*delta_x_eta_0));
			temp_cross_mag[1]	=	std::abs((delta_x_eta_1*delta_y_xi_0) - (delta_y_eta_1*delta_x_xi_0));
			temp_cross_mag[2]	=	std::abs((delta_x_xi_1*delta_y_eta_1) - (delta_y_xi_1*delta_x_eta_1));
			temp_cross_mag[3]	=	std::abs((delta_x_eta_0*delta_y_xi_1) - (delta_y_eta_0*delta_x_xi_1));

			//calculate the angle in radians
			temp_angle[0]	=	std::asin(temp_cross_mag[0]/(s_xi_0*s_eta_0))*(180/M_PI);
			temp_angle[1]	=	std::asin(temp_cross_mag[1]/(s_eta_1*s_xi_0))*(180/M_PI);
			temp_angle[2]	=	std::asin(temp_cross_mag[2]/(s_xi_1*s_eta_1))*(180/M_PI);
			temp_angle[3]	=	std::asin(temp_cross_mag[3]/(s_eta_0*s_xi_1))*(180/M_PI);

			//calculate the skewness of each angle
			for (auto i = 0; i < temp_skewness.size(); i++) {
				temp_skewness[i]	=	(std::abs(std::abs(temp_angle[i]) - 90))/90;
			}


		}
	}
	
	//copy the result to new object
	grid_pars.cell_skewness	=	cell_skewness;

	return grid_pars;

}
