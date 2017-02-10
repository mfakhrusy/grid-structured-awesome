#ifndef MATH_FUNCTION_HPP
#define MATH_FUNCTION_HPP

class Math_Function {

	double length_two_points(double x1, double x2, double y1, double y2);
	
	//friend classes below can access the functions above (which are privates)
	friend class Airfoil;
};

#endif
