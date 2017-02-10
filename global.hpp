#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include <cmath>
#include <vector>

struct Parameters {
	int max_xi_nodes; //xi means x in computational domain
	int max_eta_nodes;	//eta means y in computational domain
	int xi_stretch_type;
	int eta_stretch_type;
};

struct Airfoil_Parameters {
	
	int airfoil_nodes;
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> s;
	std::vector<double> eta;	//parameterized s
};

#endif
