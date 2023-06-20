#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/dynamic_bitset.hpp>

#include <vector>
#include <random>
#include <queue>

#include <iostream>

class Antenna{

	double _vrms;
	int windowSize, channels, 
		channelThreshold, writeDelay;

	long int samplingRate;

    	std::random_device rd;
	std::mt19937 gen;

public:
	Antenna(const char *filename);
	std::uint64_t triggerRate(double threshold, double temperature);

	double vrms;

};

Antenna::Antenna(const char *filename){

	gen.seed(rd());

	boost::property_tree::ptree root;
	boost::property_tree::read_json(filename,root);

	_vrms = sqrt(5.52E-23 * root.get<double>("resistance")  * 
			       root.get<double>("bandwidth"));

	_vrms *= root.get<double>("gain") * 
			pow(10, root.get<double>("noiseFig") / 10);

	channelThreshold = root.get<double>("channelThreshold");

	samplingRate = root.get<int>("samplingRate")  * 1E+9;
	windowSize   = root.get<double>("windowSize") * samplingRate;
	writeDelay   = root.get<double>("writeDelay") * samplingRate;
	channels     = root.get<int>("channels");

}

std::uint64_t Antenna::triggerRate(double threshold, double temperature){

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

	std::uint64_t numContributingChannels = 0, numTriggers = 0;

	for(int i = 0, imod = 0; i < samplingRate/1000; ++i, imod = (imod + 1) % windowSize){

		numContributingChannels = 0;

		for(auto &channel: window)
			 if(channel.any())
				++numContributingChannels;

		if(numContributingChannels > channelThreshold){

			++numTriggers;
			i += writeDelay + imod;

			clear();
			continue;

		}

		for(auto &channel: window)
			takeSample(imod, channel);	
		
	}

	return numTriggers;

}

int main(int argc, char *argv[]){

	Antenna antenna(argv[1]);
	double temperature = atof(argv[2]), threshold = atof(argv[3]);

	std::cout << "V_{rms}: " << antenna.vrms << " Trigger Rate: " << antenna.triggerRate(threshold, temperature) << "\n"; 

}
