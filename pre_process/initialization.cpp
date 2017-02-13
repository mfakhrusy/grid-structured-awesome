#include <fstream>
#include <sstream>
#include "../global.hpp"
#include "initialization.hpp"

Initialization::Initialization(Parameters &pars) {

	read_input(pars);

}

void Initialization::read_input(Parameters &pars) {


	int &max_xi_nodes		=	pars.max_xi_nodes; 
	int &max_eta_nodes		=	pars.max_eta_nodes;
	int &xi_stretch_type		=	pars.xi_stretch_type;	
	int &eta_stretch_type		=	pars.eta_stretch_type;
	int &grid_type			=	pars.grid_type;

	std::ifstream read_file;
	read_file.open("input/input.dat");

	if(read_file.is_open()) {
		std::string a_line;
		int count = 0;
		int number;
		std::string alt_text;
		while (read_file >> number >> alt_text) {
			count++;
			switch(count) {
				case 1:
					max_xi_nodes = number;
					break;
				case 2:
					max_eta_nodes = number;
					break;
				case 3:
					xi_stretch_type = number;
					break;
				case 4:
					eta_stretch_type = number;
					break;
				case 5:
					grid_type = number;
					break;
				default:
					std::cout << "ok" << std::endl;
			}
		
		}
	} else {
		std::cout << "it is not opened!\n";
	}

}
