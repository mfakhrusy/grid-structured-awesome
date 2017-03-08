#include "global.hpp"
#include "pre_process/initialization.hpp"
#include "pre_process/airfoil.hpp"
#include "grid_generator/o_grid_main.hpp"
#include "grid_generator/c_grid_main.hpp"
#include "misc/post_process.hpp"

int main() {

	Parameters pars;
	Airfoil_Parameters airfoil_pars;
	Grid_Parameters grid_pars;

	Initialization init(pars);
	Airfoil airfoil(airfoil_pars);
	Post_Process post_process;

	//make local vars of grid

	//calculate the grid depends on the input.
	//grid_type = 0 -> O grid; grid_type = 1 -> C grid.
	if (pars.grid_type == 0) {

		O_Grid_Main o_grid(airfoil_pars, pars);
		grid_pars	=	o_grid.o_grid_boundary(pars);
		grid_pars	=	o_grid.o_grid_internal(grid_pars, pars);

		//post_process
		//post_process.aspect_ratio_computation(grid_pars, pars);
		grid_pars	=	post_process.aspect_ratio_computation(grid_pars, pars);
		//print the output
		post_process.grid_output(grid_pars, pars);
	
	} else if (pars.grid_type == 1) {
		//this is meant to be using the C grid type airfoil.
	
	} else {
		std::cout << "grid type is wrong, please change grid type and run again.";
	}
	
}
