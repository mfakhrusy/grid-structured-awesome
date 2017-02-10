#include "global.hpp"
#include "pre_process/initialization.hpp"
#include "pre_process/airfoil.hpp"

int main() {
	Parameters pars;
	Airfoil_Parameters airfoil_pars;

	Initialization init(pars);
	Airfoil airfoil(pars, airfoil_pars);


}
