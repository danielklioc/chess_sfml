#pragma once
#include <boost/process/pipe.hpp>



void ConnectEngineToPipe()
{
	boost::process::pipe pipe_input;
	boost::process::ipstream is;

	std::vector<std::string> outline;
}

std::vector<std::string> read_outline(std::string & file)
{
	bp::ipstream is; //reading pipe-stream
	bp::child c(bp::search_patk("nm"), file, bp::std_out > is);

	std::vector<std::string> data;
	std::string line;

	while (c.running() && std::getline(is, line) && !line.empty())
		data.push_back(line);

	c.wait();

	return data;
}

std::vector<std::string> read_demangled_outline(const std::string & file)
{
	bp::pipe p;
	bp::ipstream is;

	std::vector<std::string> outline;

	//we just use the same pipe, so the 
	bp::child nm(bp::search_path("nm"), file, bp::std_out > p);
	bp::child filt(bp::search_path("c++filt"), bp::std_in < p, bp::std_out > is);

	std::string line;
	while (filt.running() && std::getline(is, line)) //when nm finished the pipe closes and c++filt exits
		outline.push_back(line);

	nm.wait();
	filt.wait();
}