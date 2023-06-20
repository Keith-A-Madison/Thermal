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
	int triggerRate(double threshold, double temperature);

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

int Antenna::triggerRate(double threshold, double temperature){

	std::normal_distribution<double> dist(0, vrms = _vrms * sqrt(temperature));

	std::vector<boost::dynamic_bitset<>> 
		window(channels, boost::dynamic_bitset<>(windowSize));

	auto clear = [&](){
		for(auto &channel: window)
		        for(int i = 0; i < windowSize; ++i)
        	            channel.set(i, fabs(dist(gen)) > threshold);
	};

	clear();

	int numContributingChannels = 0, numTriggers = 0;

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
			channel.set(imod, fabs(dist(gen)) > threshold);
		
	}

	return numTriggers;

}

int main(int argc, char *argv[]){

	Antenna antenna(argv[4]);
	double temperature = atof(argv[1]), threshold = atof(argv[2]);

	std::cout << "V_{rms}: " << antenna.vrms << 
  " Trigger Rate: " << antenna.triggerRate(threshold, temperature) << "\n"; 

}
