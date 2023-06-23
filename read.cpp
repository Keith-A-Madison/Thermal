#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/dynamic_bitset.hpp>

#include <functional>
#include <iostream>
#include <vector>
#include <random>
#include <queue>

#include "antenna.h"

Antenna::Antenna(const char* descriptor, std::function<double(double)> freqResponse = nullptr){

	gen.seed(rd());

try{

	boost::property_tree::ptree json;
	boost::property_tree::read_json(descriptor, json);

	double fHigh = json.get<double>("filter.fHigh");
	double fLow  = json.get<double>("filter.fLow");
	
	double bandwidth;

	// Read filter type, and use to compute bandwidth
	std::string filterType = json.get<std::string>("filter.type");

	if(filterType == "simple")
		bandwidth = fHigh - fLow;
	else if (filterType == "butterworth")
		bandwidth = fHigh - fLow;
	else if (filterType == "custom")
		bandwidth = fHigh - fLow;
	else
		throw std::runtime_error("Invalid filter type");

	_vrms  = std::sqrt(5.52E-23 * bandwidth * json.get<double>("resistance"));
	_vrms *= json.get<double>("gain") * pow(10, json.get<double>("noiseFig") / 10);

	channelThreshold = json.get<double>("channelThreshold");
	
	samplingRate = json.get<int>("samplingRate")  * 1E+9;
	windowSize   = json.get<double>("windowSize") * samplingRate;
	writeDelay   = json.get<double>("writeDelay") * samplingRate;
	channels     = json.get<int>("channels");
	

} catch (const boost::property_tree::json_parser::json_parser_error& ex){

		std::cout << "JSON parsing error: " << ex.what() << std::endl;
		throw;

}
	
}
