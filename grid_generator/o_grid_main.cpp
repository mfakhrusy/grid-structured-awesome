#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "o_grid_main.hpp"
#include "grid_process.hpp"

//constructor
O_Grid_Main::O_Grid_Main(Airfoil_Parameters airfoil_pars, Parameters pars) {


	//make local vars
	int xi_nodes		=	pars.max_xi_nodes;
	int xi_stretch_type	=	pars.xi_stretch_type;

	o_grid_initialization();

	Grid_Process grids(airfoil_pars, pars);
	
	Airfoil_Parameters airfoil_pars_new = grids.grid_airfoil_interpolation(xi_nodes, xi_stretch_type);

	for (auto i = 0; i < airfoil_pars_new.x.size(); i++) {
		std::cout << i << " " << airfoil_pars_new.x[i] << " " << airfoil_pars_new.y[i] << std::endl;
	}
 
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
