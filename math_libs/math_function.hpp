#ifndef MATH_FUNCTION_HPP
#define MATH_FUNCTION_HPP

class Math_Function {

	double length_two_points(double x1, double x2, double y1, double y2);
	double circle_return_y(double x, double radius, double x_center, double y_center);
	
	//friend classes below can access the functions above (which are privates)
	friend class Airfoil;
	friend class O_Grid_Main;
};

#endif
