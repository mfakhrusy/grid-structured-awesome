#ifndef AIRFOIL_HPP
#define AIRFOIL_HPP
#include "../math_libs/math_function.hpp"

class Math_Function;
class Airfoil {

	Math_Function math_f;

	void airfoil_read(std::vector<double> &x, std::vector<double> &y, int &airfoil_node);	//max_node is initialized here
	bool airfoil_check_from_databases(std::string airfoil_input);
	std::vector<double> airfoil_element_length_calc(std::vector<double> x, std::vector<double> y, int max_node);
	std::vector<double> airfoil_sigma_calc(std::vector<double> s, int max_node);

	public:
		Airfoil(Airfoil_Parameters &airfoil_pars);
		Airfoil(Airfoil_Parameters &airfoil_pars, int airfoil_nodes); //this is for grid_process class, airfoil nodes is pars.max_xi_nodes

};

#endif
