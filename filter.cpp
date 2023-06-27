#include <algorithm>

#include "filter.h"

Butterworth::Butterworth(int ordLow, int ordHigh, 
			 double fLow, double fHigh): 

			 _ordLow(ordLow), _fLow(fLow), 
			 _ordHigh(ordHigh), _fHigh(fHigh) {};

double Butterworth::transferFunction(double freq){

	return std::min(std::sqrt(1 / (1 + pow(freq/_fLow, 2 * _ordLow))),	// Low-pass filter
			std::sqrt(pow(freq, _ordHigh) / 
			(pow(freq, _ordHigh) + pow(_fHigh, _ordHigh))));	// High-pass filter

}
