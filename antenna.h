#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/dynamic_bitset.hpp>

#include <functional>
#include <iostream>
#include <vector>
#include <random>
#include <queue>

#ifndef antenna_H
#define antenna_H

class Antenna{

public:
	Antenna(const char* descriptor,
		std::function<double(double)> freqResponse = nullptr);

	std::uint64_t getTriggerRate(double thresh, double temp);
	
	double vrms, highestChanSNR;

private:
	double _vrms;
        int windowSize, channels,
                channelThreshold, writeDelay;

        long int samplingRate;

        std::random_device rd;
        std::mt19937 gen;

};

#endif
