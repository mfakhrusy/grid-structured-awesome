#include "../global.hpp"
#include "grid_process.hpp"
#include "../pre_process/airfoil.hpp"
#include "../math_libs/spline.h"

Grid_Process::Grid_Process(Airfoil_Parameters airfoil_pars, Parameters pars) {

	//make local vars
	x	=	airfoil_pars.x;
	y	=	airfoil_pars.y;

}

//airfoil spline interpolation
Airfoil_Parameters Grid_Process::grid_airfoil_interpolation(int airfoil_nodes, int xi_stretch_type) {
	
	Airfoil_Parameters airfoil_pars_new;
	tk::spline lower_spline;		//from spline.h
	tk::spline upper_spline;		//from spline.h

	//divide x into upper and lower spline (in increasing way!)
	std::vector<double> upper_x((x.size()-1)/2 + 1);
	std::vector<double> lower_x((x.size()-1)/2 + 1);
	std::vector<double> upper_y((y.size()-1)/2 + 1);
	std::vector<double> lower_y((y.size()-1)/2 + 1);

	for (auto i = 0; i < (x.size()-1)/2 + 1; i++) {
		lower_x[i]	=	x[(x.size()-1)/2 - i];
		upper_x[i]	=	x[(x.size()-1)/2 + i];
		lower_y[i]	=	y[(y.size()-1)/2 - i];
		upper_y[i]	=	y[(y.size()-1)/2 + i];
		//std::cout << i << " " << upper_x[i] << " " << upper_y[i] << " " << lower_x[i] << " " << lower_y[i] << std::endl;
	}

	//interpolate process
	lower_spline.set_points(lower_x, lower_y);
	upper_spline.set_points(upper_x, upper_y);

	//looping through the airfoil nodes
	std::vector<double> temp_lower_x((airfoil_nodes+1)/2);
	std::vector<double> temp_upper_x((airfoil_nodes+1)/2);
	std::vector<double> temp_lower_y((airfoil_nodes+1)/2);
	std::vector<double> temp_upper_y((airfoil_nodes+1)/2);

	std::cout << "\n=======================\n";
	for (auto i = 0; i < (airfoil_nodes+1)/2; i++) {
		temp_lower_x[i]	=	static_cast<double>(2*i)/(airfoil_nodes - 1);
		temp_upper_x[i]	=	static_cast<double>(2*i)/(airfoil_nodes - 1);
		
		//y
		temp_lower_y[i]	=	lower_spline(temp_lower_x[i]);
		temp_upper_y[i]	=	upper_spline(temp_upper_x[i]);
		//std::cout << i << " " << temp_upper_x[i] << " " << temp_upper_y[i] << " " << temp_lower_x[i] << " " << temp_lower_y[i] << std::endl;
	}
	//combine the upper and lower in the same manner as before
	std::vector<double> temp_x(airfoil_nodes);
	std::vector<double> temp_y(airfoil_nodes);

	std::cout << "\n=======================\n";

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

		//std::cout << i << " " << temp_x[i] << " " << temp_y[i] << std::endl;
	}

	airfoil_pars_new.x	=	temp_x;
	airfoil_pars_new.y	=	temp_y;

	return airfoil_pars_new;
}
