#ifndef antenna_H
#define antenna_H

class Antenna{

public:
	Antenna(const char* descriptor,
		std::function<double(double)> freqResponse);
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
