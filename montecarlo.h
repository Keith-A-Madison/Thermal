#include <boost/numeric/odeint.hpp>

#ifndef montecarlo_H
#define montecarlo_H

class MonteCarlo{

public:
	MonteCarlo();
	void rayTrace(const stateType& initState);

private:
	void stochStep(const stateType& state, stateType& ds, const double /*t*/);
	double uniformRand();

	typedef std::array<double, 6> stateType;

	stateType prevState;

	std::uniform_real_distribution<double> dist(0.0, 1.0);
	std::random_device rd;
	std::mt19937 gen; 

};

#endif
