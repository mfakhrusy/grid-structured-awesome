#include <fstream>
#include <sstream>
#include <numeric>
#include "../global.hpp"
#include "airfoil.hpp"

//constructor
Airfoil::Airfoil(Parameters &pars, Airfoil_Parameters &airfoil_pars) {

	//make local variables
	int &airfoil_nodes		=	airfoil_pars.airfoil_nodes;	//HAVE TO BE REFERENCE TO MAX_NODE BECAUSE AIRFOIL_READ IS INPUTING VALUE TO IT TNX
	std::vector<double> &x		=	airfoil_pars.x;
	std::vector<double> &y		=	airfoil_pars.y;
	std::vector<double> &s		=	airfoil_pars.s;
	std::vector<double> &eta	=	airfoil_pars.eta;

	//read the coordinate from databases
	airfoil_read(x, y, airfoil_nodes);
	
	//calculate length
	s	=	airfoil_element_length_calc(airfoil_pars, airfoil_nodes);

	//calculate eta
	eta	=	airfoil_eta_calc(airfoil_pars, airfoil_nodes);

	for (auto i = 0; i < x.size(); i++) {
		std::cout << i << " " << x[i] << " " << y[i] << " " << s[i] << " " << eta[i] << std::endl;
	}

}

void Airfoil::airfoil_read (std::vector<double> &x, std::vector<double> &y, int &airfoil_nodes) {

	std::string airfoil_type;
	std::cout << "--------- Start Reading Airfoil -----------" << std::endl;
	std::cout << "Please input NACA AIRFOIL (e.g 0015 0012): ";
	std::cin >> airfoil_type;
	std::cout << std::endl << "Checking from databases..." << std::endl;

	// check airfoil exist or not in the database
	if (airfoil_check_from_databases(airfoil_type)) {
		std::cout << "OK, airfoil data has been read successfully..." << std::endl;
		std::string airfoil_title, airfoil_coor;

		//read airfoil data from the databases
		std::string airfoil_path = "airfoil_databases/" + airfoil_type;
		std::ifstream airfoil_input(airfoil_path); 
		
		if (airfoil_input.is_open()) {
			//------- READ AIRFOIL DATA AND APPEND TO ARRAY -------
			std::string airfoil_title, airfoil_coordinate;
			std::getline(airfoil_input, airfoil_title); // read the first line (title!)

			while (std::getline(airfoil_input, airfoil_coordinate)) {
				std::istringstream iss(airfoil_coordinate);
				double a, b;
				iss >> a >> b;
				x.push_back(a);
				y.push_back(b);
			}	

			airfoil_nodes = x.size();
			
			// sharpen the airfoil's trailing edge, for kutta condition purpose.
			y[airfoil_nodes - 1]	= 	0;
			y[0] 		= 	0;

			//reverse the index
			std::vector<double> x_temp	=	x;
			std::vector<double> y_temp	=	y;
			for (auto i = 1; i <= airfoil_nodes; i++) {
				
				x[i - 1]	=	x_temp[airfoil_nodes - i];
				y[i - 1]	=	y_temp[airfoil_nodes - i];

			}

		} else {
			std::cout << "somehoew, the airfoil data is not exist!" << std::endl;
		}

	} else {
		std::cout << "Sorry, airfoil data is not found." << std::endl;
	}

}

bool Airfoil::airfoil_check_from_databases(std::string airfoil_input) {

	std::ifstream airfoil_databases;
	airfoil_databases.open("airfoil_databases/list_airfoil.dat");
	if (airfoil_databases.is_open()) {
		while (!airfoil_databases.eof()) {
			std::string read_per_line;
			airfoil_databases >> read_per_line;

			if (airfoil_input == read_per_line) {
				return true;
			} 
		}
		return false;
	}
	else {
		std::cout << "There isn't any file named 'list_airfoil.dat'" << std::endl;
		return false;
	}
}

std::vector<double> Airfoil::airfoil_element_length_calc(Airfoil_Parameters airfoil_pars, int airfoil_nodes) {
	
	std::vector<double> length(airfoil_nodes - 1); //the index is -1 from node index

	//make local variables
	std::vector<double> x	=	airfoil_pars.x;
	std::vector<double> y	=	airfoil_pars.y;

	for (auto i = 0; i < airfoil_nodes - 1; i++) {

		length[i]	= 	math_f.length_two_points(x[i+1], x[i], y[i+1], y[i]);
	}
		
	return length;
}

//function airfoil_eta_calc -> shape function calculations
std::vector<double> Airfoil::airfoil_eta_calc(Airfoil_Parameters airfoil_pars, int airfoil_nodes) {

	std::vector<double> eta(airfoil_nodes);

	//make local variables
	std::vector<double> s	=	airfoil_pars.s;

	//summation of the vector
	double sum_s	=	std::accumulate(s.begin(), s.end(), 0.0);

	//both edges //CHANGED (THIS IS CHANGED)
	//eta[airfoil_nodes-1]	=	1;
	//eta[0]		=	-1;
	//
	//INTO THIS ONE
	eta[airfoil_nodes-1]	=	1;
	eta[0]		=	eta[airfoil_nodes-1];
	eta[1]		=	-1;


	double temp;
	
	//for the rest of the nodes
	//for (auto i = 1; i < airfoil_nodes - 1; i++) { // CHANGED FROM i = 1 to i = 2 because of the above reason
	for (auto i = 2; i < airfoil_nodes - 1; i++) {
		temp 	=	temp + s[i-1];
		eta[i]	=	(2*temp/sum_s) - 1;
	}

	return eta;
}

