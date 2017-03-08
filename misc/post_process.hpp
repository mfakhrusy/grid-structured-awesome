#ifndef POST_PROCESS_HPP
#define POST_PROCESS_HPP

class Post_Process {

	public:
		void grid_output(Grid_Parameters grid_pars, Parameters pars);
		Grid_Parameters aspect_ratio_computation(Grid_Parameters grid_pars, Parameters pars);

};
#endif
