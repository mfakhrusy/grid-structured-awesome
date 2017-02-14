#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "o_grid_main.hpp"

//constructor
O_Grid_Main::O_Grid_Main(Airfoil_Parameters airfoil_pars, Parameters pars) {

	o_grid_initialization();
	std::cout << o_grid_radius << std::endl;
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
