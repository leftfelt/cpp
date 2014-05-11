#include <base/LogClass.h>

Log::Log(std::string filename){
	this->ofs.open(filename, std::ios::out | std::ios::app);
}

Log::~Log(){
	this->ofs.close();
}

void Log::err(std::string message){
	std::string prefix = " [err] ";
	prefix += message;
	this->log(prefix);
}
void Log::info(std::string message){
	std::string prefix = " [info] ";
	prefix += message;
	this->log(prefix);
}

void Log::log(std::string message){
	time_t now;
	struct tm *date;
	std::string current_time;
	
	time(&now);
	date = localtime(&now);
	current_time = asctime(date);

	current_time.erase(current_time.end()-1); //––”ö‚Ì‰üs‚ğíœ
	ofs << current_time.c_str() << message.c_str() << std::endl;
}