#include "../global.hpp"
#include "grid_process.hpp"
#include "../pre_process/airfoil.hpp"
#include "../math_libs/spline.h"

Grid_Process::Grid_Process(Airfoil_Parameters airfoil_pars, Parameters pars) {

	//make local vars
	airfoil_x	=	airfoil_pars.x;
	airfoil_y	=	airfoil_pars.y;

}

//airfoil spline interpolation
Airfoil_Parameters Grid_Process::grid_airfoil_interpolation(int airfoil_nodes, int xi_stretch_type) {
	
	Airfoil_Parameters airfoil_pars_new;
	tk::spline lower_spline;		//from spline.h
	tk::spline upper_spline;		//from spline.h

	//divide x into upper and lower spline (in increasing way!)
	std::vector<double> upper_x((airfoil_x.size()-1)/2 + 1);
	std::vector<double> lower_x((airfoil_x.size()-1)/2 + 1);
	std::vector<double> upper_y((airfoil_y.size()-1)/2 + 1);
	std::vector<double> lower_y((airfoil_y.size()-1)/2 + 1);

	for (auto i = 0; i < (airfoil_x.size()-1)/2 + 1; i++) {
		lower_x[i]	=	airfoil_x[(airfoil_x.size()-1)/2 - i];
		upper_x[i]	=	airfoil_x[(airfoil_x.size()-1)/2 + i];
		lower_y[i]	=	airfoil_y[(airfoil_y.size()-1)/2 - i];
		upper_y[i]	=	airfoil_y[(airfoil_y.size()-1)/2 + i];
	}

	//interpolate process
	lower_spline.set_points(lower_x, lower_y);
	upper_spline.set_points(upper_x, upper_y);

	//looping through the airfoil nodes
	std::vector<double> temp_lower_x((airfoil_nodes+1)/2);
	std::vector<double> temp_upper_x((airfoil_nodes+1)/2);
	std::vector<double> temp_lower_y((airfoil_nodes+1)/2);
	std::vector<double> temp_upper_y((airfoil_nodes+1)/2);

	for (auto i = 0; i < (airfoil_nodes+1)/2; i++) {
		
		switch(xi_stretch_type) {
		case 0:
			temp_lower_x[i]	=	grid_stretching_equal(2*i, airfoil_nodes);
			temp_upper_x[i]	=	grid_stretching_equal(2*i, airfoil_nodes);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			std::cout << "ok" << std::endl;
		}

		
		//y
		temp_lower_y[i]	=	lower_spline(temp_lower_x[i]);
		temp_upper_y[i]	=	upper_spline(temp_upper_x[i]);
	}

	//combine the upper and lower in the same manner as before
	std::vector<double> temp_x(airfoil_nodes);
	std::vector<double> temp_y(airfoil_nodes);


	for (auto i = 0; i < airfoil_nodes; i++) {
		if (i < (airfoil_nodes+1)/2 - 1)  {
			temp_x[i]	=	temp_lower_x[temp_lower_x.size() - 1 - i];
			temp_y[i]	=	temp_lower_y[temp_lower_y.size() - 1 - i];
		} else if (i > (airfoil_nodes+1)/2 - 1) {
			temp_x[i]	=	temp_upper_x[i - (airfoil_nodes+1)/2 + 1];
			temp_y[i]	=	temp_upper_y[i - (airfoil_nodes+1)/2 + 1];
		} else {
			temp_x[i]	=	temp_lower_x[0];
			temp_y[i]	=	temp_lower_y[0];
		}

	}

	airfoil_pars_new.x	=	temp_x;
	airfoil_pars_new.y	=	temp_y;

	return airfoil_pars_new;
}

double Grid_Process::grid_stretching_equal(int i, int N) {

	return static_cast<double>(i)/(static_cast<double>(N) - 1);	

}

std::vector<std::vector<double>> Grid_Process::grid_algebra_correction(Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes) {
	
	std::vector<std::vector<double>> grid_correction_factor(max_xi_nodes, std::vector<double> (max_eta_nodes));

	//make local variables
	std::vector<double> xi_lower	=	grid_comp_pars.xi_lower;
	std::vector<double> xi_upper	=	grid_comp_pars.xi_upper;
	std::vector<double> eta_left	=	grid_comp_pars.eta_left;
	std::vector<double> eta_right	=	grid_comp_pars.eta_right;

	for (auto i = 0; i < max_xi_nodes; i++) {
		for (auto j = 0; j < max_eta_nodes; j++) {
			grid_correction_factor[i][j] = 1 - (xi_upper[i] - xi_lower[i])*(eta_right[j] - eta_left[j]);
		}
	}
	
	return grid_correction_factor;
}

//calculate the xi_outer xi_lower eta_left and eta_right
Grid_Computation_Parameters Grid_Process::grid_algebra_boundary_calc(Parameters pars) {
	
	//make local vars
	int xi_stretch_type	=	pars.xi_stretch_type;
	int eta_stretch_type	=	pars.eta_stretch_type;
	int max_xi_nodes	=	pars.max_xi_nodes;
	int max_eta_nodes	=	pars.max_eta_nodes;

	Grid_Computation_Parameters grid_comp_pars;

	//make local vars
	std::vector<double> &xi_lower	=	grid_comp_pars.xi_lower;
	std::vector<double> &xi_upper	=	grid_comp_pars.xi_upper;
	std::vector<double> &eta_left	=	grid_comp_pars.eta_left;
	std::vector<double> &eta_right	=	grid_comp_pars.eta_right;
	
	for (auto i = 0; i < max_xi_nodes; i++) {

		switch(xi_stretch_type) {
		case 0:
			xi_lower[i]	=	grid_stretching_equal(i, max_xi_nodes);
			xi_upper[i]	=	grid_stretching_equal(i, max_xi_nodes);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			std::cout << "ok" << std::endl;
		}
	}
	
	for (auto i = 0; i < max_eta_nodes; i++) {

		switch(eta_stretch_type) {
		case 0:
			eta_left[i]	=	grid_stretching_equal(i, max_eta_nodes);
			eta_right[i]	=	grid_stretching_equal(i, max_eta_nodes);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			std::cout << "ok" << std::endl;
		}
	}

	return grid_comp_pars;
}

Grid_Computation_Parameters Grid_Process::grid_algebra_calc(Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes) {

	//make local variables
	std::vector<double> xi_lower	=	grid_comp_pars.xi_lower;
	std::vector<double> xi_upper	=	grid_comp_pars.xi_upper;
	std::vector<double> eta_left	=	grid_comp_pars.eta_left;
	std::vector<double> eta_right	=	grid_comp_pars.eta_right;
	std::vector<std::vector<double>> grid_correction_factor	=	grid_comp_pars.grid_correction_factor;
	std::vector<std::vector<double>> &xi			=	grid_comp_pars.xi;
	std::vector<std::vector<double>> &eta			=	grid_comp_pars.eta;
	
	for (auto i = 0; i < max_xi_nodes; i++) {
		for (auto j = 0; j < max_eta_nodes; j++) {
			double temp	=	(1/grid_correction_factor[i][j]);
			xi[i][j]	=	temp*(xi_lower[i] + eta_left[j]*(xi_upper[i] - xi_lower[i]));
			eta[i][j]	=	temp*(eta_left[j] + xi_lower[i]*(eta_right[j] - eta_left[j]));
		}
	}

	return grid_comp_pars;
}

Grid_Parameters Grid_Process::grid_algebra_real_calc(Grid_Parameters grid_pars, Grid_Computation_Parameters grid_comp_pars, int max_xi_nodes, int max_eta_nodes) {

	//make local vars
	std::vector<std::vector<double>> xi			=	grid_comp_pars.xi;
	std::vector<std::vector<double>> eta			=	grid_comp_pars.eta;

	std::vector<double> x_xi_outer	=	grid_pars.x_xi_outer_boundary;
	std::vector<double> x_xi_inner	=	grid_pars.x_xi_inner_boundary;
	std::vector<double> x_eta_upper	=	grid_pars.x_eta_upper_boundary;
	std::vector<double> x_eta_lower	=	grid_pars.x_eta_lower_boundary;

	std::vector<double> y_xi_outer	=	grid_pars.y_xi_outer_boundary;
	std::vector<double> y_xi_inner	=	grid_pars.y_xi_inner_boundary;
	std::vector<double> y_eta_upper	=	grid_pars.y_eta_upper_boundary;
	std::vector<double> y_eta_lower	=	grid_pars.y_eta_lower_boundary;

	std::vector<std::vector<double>> &x	=	grid_pars.x;
	std::vector<std::vector<double>> &y	=	grid_pars.y;

	//calculation for x
	for (auto i = 0; i < max_xi_nodes; i++) {
		for (auto j = 0; j < max_eta_nodes; j++) {
			double temp_a	=	(1 - xi[i][j])*x_eta_lower[j] + xi[i][j]*x_eta_upper[j];
			double temp_b	=	(1 - eta[i][j])*x_xi_inner[i] + eta[i][j]*x_xi_outer[i];
			double corr_a	=	(1 - xi[i][j])*(1 - eta[i][j])*x_eta_lower[0] + (1 - xi[i][j])*eta[i][j]*x_eta_lower[max_eta_nodes];
			double corr_b	=	xi[i][j]*(1 - eta[i][j])*x_eta_upper[0] + xi[i][j]*eta[i][j]*x_eta_upper[max_eta_nodes];
			
			x[i][j]		=	temp_a + temp_b - corr_a - corr_b;
		}
	}
	
	//calculation for y
	for (auto i = 0; i < max_xi_nodes; i++) {
		for (auto j = 0; j < max_eta_nodes; j++) {
			double temp_a	=	(1 - xi[i][j])*y_eta_lower[j] + xi[i][j]*y_eta_upper[j];
			double temp_b	=	(1 - eta[i][j])*y_xi_inner[i] + eta[i][j]*y_xi_outer[i];
			double corr_a	=	(1 - xi[i][j])*(1 - eta[i][j])*y_eta_lower[0] + (1 - xi[i][j])*eta[i][j]*y_eta_lower[max_eta_nodes];
			double corr_b	=	xi[i][j]*(1 - eta[i][j])*y_eta_upper[0] + xi[i][j]*eta[i][j]*y_eta_upper[max_eta_nodes];
			
			y[i][j]		=	temp_a + temp_b - corr_a - corr_b;
		}
	}

	return grid_pars;
}
