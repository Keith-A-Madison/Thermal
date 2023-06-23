#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/dynamic_bitset.hpp>

#include <functional>
#include <iostream>
#include <vector>
#include <random>
#include <queue>

#include "antenna.h"

std::uint64_t Antenna::getTriggerRate(double threshold, double temperature){

	std::normal_distribution<double> dist(0, vrms = _vrms * sqrt(temperature));
	std::priority_queue<double, std::vector<double>, std::greater<double>> pq;

	std::vector<boost::dynamic_bitset<>> 
		window(channels, boost::dynamic_bitset<>(windowSize));

	auto takeSample = [&](int index, boost::dynamic_bitset<> &channel){

		double sample = fabs(dist(gen));
		channel.set(index, sample > threshold);

		if(pq.size() < channelThreshold){ pq.push(sample); }
		else if(sample > pq.top()){ pq.pop(); pq.push(sample); }

	};

	auto clear = [&](){
		for(auto &channel: window)
		        for(int i = 0; i < windowSize; ++i)
				takeSample(i, channel);	
	};

	clear();

	highestChanSNR = 0;
	std::uint64_t numContributingChannels = 0, numTriggers = 0;

	for(int i = 0, imod = 0; i < samplingRate; ++i, imod = (imod + 1) % windowSize){

		numContributingChannels = 0;

		for(auto &channel: window)
			 if(channel.any())
				++numContributingChannels;

		if(numContributingChannels > channelThreshold){

			highestChanSNR += pq.top() / (2 * vrms);

			++numTriggers;
			i += writeDelay + imod;

			clear();
			continue;

		}

		for(auto &channel: window)
			takeSample(imod, channel);	
		
	}

	highestChanSNR /= numTriggers;

	return numTriggers;

}