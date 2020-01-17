#pragma once
#include <boost/process/pipe.hpp>



void ConnectEngineToPipe()
{
	boost::process::pipe pipe_input;
	boost::process::ipstream is;

	std::vector<std::string> outline;
}
