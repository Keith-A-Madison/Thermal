#include <boost/math/quadrature/gauss.hpp>

#include <functional>
#include <math.h>

double antennaResponse(double low, double high, std::function<double(double)> func){

	return boost::math::quadrature::gaus<double, 10>::integrate(func, low, high); 

}

double antennaResponse(double low, double high, int order){

	auto lowPass  = [&](double f){ 
		return sqrt(1 / (1 + pow(f / low, 2 * order)));
	};

	auto highPass = [&](double f){
		return sqrt(pow(f, order) / pow(f, order) + pow(high, order));
	};

	// Perform integration. Maybe include ability to set different high, low order

}
