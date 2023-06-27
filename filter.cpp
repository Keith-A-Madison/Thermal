#include <algorithm>

#include "filter.h"

Butterworth::Butterworth(int ordLow, int ordHigh, 
			 double fLow, double fHigh): 

			 _ordLow(ordLow), _fLow(fLow), 
			 _ordHigh(ordHigh), _fHigh(fHigh) {};

double Butterworth::transferFunction(double freq){

	return std::min(std::sqrt(1 / (1 + pow(freq/_fLow, 2 * _ordLow))),	// H(f)ₗₒ ₚₐₛₛ = √1/(1 + (f/fₗₒ)²ⁿ)
			std::sqrt(pow(freq, _ordHigh) / 
			(pow(freq, _ordHigh) + pow(_fHigh, _ordHigh))));	// H(f)ₕᵢ ₚₐₛₛ = √ fⁿ / (fⁿ + fₕᵢⁿ)

}
