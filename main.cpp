#include "global.hpp"
#include "pre_process/initialization.hpp"
#include "pre_process/airfoil.hpp"
#include "grid_generator/o_grid_main.hpp"
#include "grid_generator/c_grid_main.hpp"

int main() {

	Parameters pars;
	Airfoil_Parameters airfoil_pars;
	Grid_Parameters grid_pars;

	Initialization init(pars);
	Airfoil airfoil(airfoil_pars);

	//make local vars of grid

	//calculate the grid depends on the input.
	//grid_type = 0 -> O grid; grid_type = 1 -> C grid.
	if (pars.grid_type == 0) {

		O_Grid_Main o_grid(airfoil_pars, pars);
		grid_pars	=	o_grid.o_grid_boundary(pars);
		grid_pars	=	o_grid.o_grid_internal(grid_pars, pars);
	
	} else if (pars.grid_type == 1) {
	
	} else {
		std::cout << "grid type is wrong, please change grid type and run again.";
	}
	
}
