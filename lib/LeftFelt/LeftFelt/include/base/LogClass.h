#ifndef LOG_INCLUDE
#define LOG_INCLUDE

#include <fstream>
#include <time.h>
#include <math.h>

class Log{
private:
	std::ofstream ofs;
	void log(std::string message);
public:
	Log(std::string filename);
	~Log();
	void err(std::string message);
	void info(std::string message);
};

#endif