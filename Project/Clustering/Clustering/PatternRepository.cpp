#include "PatternRepository.h"

PatternRepository::PatternRepository(){
	this->repository = new boost::lockfree::queue<double>(1024);
}

void PatternRepository::push(std::vector<double> data_list){
	//data_list‚ÌÅŒã‚Éƒ}[ƒW
	std::for_each(data_list.begin(), data_list.end(),[&](double data){
		repository->push(data);
	});
}

Pattern PatternRepository::pull(int dimension_num){
	Pattern pattern(dimension_num);
	//data_list‚ÌÅ‰‚©‚çdimension_num‚¾‚¯Žæ‚Á‚Ä‚­‚é
	for(int i = 0 ; !repository->empty() && i < dimension_num ; i++){
		repository->pop(pattern[i]);
	}
	return pattern;
}