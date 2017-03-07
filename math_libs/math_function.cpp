#include "../global.hpp"
#include "math_function.hpp"

double Math_Function::length_two_points(double x1, double x2, double y1, double y2) {

	double delta_x	=	x2 - x1;
	double delta_y	=	y2 - y1;
	double length	=	sqrt(pow(delta_x,2) + pow(delta_y,2));

	return length;
}

double Math_Function::circle_return_y(double x, double radius, double x_center, double y_center) {
	return sqrt(pow(radius,2) - pow((x - x_center), 2)) + y_center;
}
