#include <boost/numeric/odeint.hpp>
#include "montecarlo.h"

using namespace boost::numeric::odeint;

MonteCarlo::MonteCarlo(): gen(rd()){}

double MonteCarlo::uniformRand(){

	return dist(gen);

}

void MonteCarlo::stochStep(const stateType& state, stateType& ds, const double /*t*/){

	ds[0] = state[3]; ds[3] = 0;
	ds[1] = state[4]; ds[4] = 0;
	ds[2] = state[5]; ds[5] = -0.006832 * exp(0.016 * ds[2]);

	// The interaction probability is dependent upon this step size
	double stepSize = sqrt(pow(ds[0] - prevState[0], 2) + 
			       pow(ds[1] - prevState[1], 2) + 
			       pow(ds[2] - prevState[2], 2));

	if(uniformRand() < (1 - exp(-n * sigma * stepSize))
		// Interaction occurs, compute new direction, intensity
}

void MonteCarlo::rayTrace(const stateType& initState){

	stateType state = initState;
	prevState = initState;

	typedef rkDopri5<stateType> stepperType;

	auto observer = [&](const stateType& state, double t){
		// Do things...
	};

	integrate_const(make_dense_output(1.0e-6, 1.0e-6, stepperType()),
			stochStep, state, 0.0, maxDepth, stepSize, observer);

}
