#include <PatternRepository.h>

PatternRepository::PatternRepository(){
	this->repository = new boost::lockfree::queue<double>(1024);
	this->size = 0;
}

int PatternRepository::getSize(){
	return this->size;
}

void PatternRepository::push(std::vector<double> data_list){
	//data_list‚ÌÅŒã‚Éƒ}[ƒW
	std::for_each(data_list.begin(), data_list.end(),[&](double data){
		repository->push(data);
		this->size++;
	});
}

Pattern PatternRepository::pull(int dimension_num){
	Pattern pattern(dimension_num);
	//data_list‚ÌÅ‰‚©‚çdimension_num‚¾‚¯Žæ‚Á‚Ä‚­‚é
	//expire
	for(int i = 0 ; !repository->empty() && i < dimension_num ; i++){
		repository->pop(pattern[i]);
		this->size--;
	}
	return pattern;
}

std::vector<Pattern> PatternRepository::pullAll(int dimension_num){
	std::vector<Pattern> pattern_list;
	while(dimension_num <= this->size){
		pattern_list.push_back(this->pull(dimension_num));
	}
	this->clear();
	return pattern_list;
}

void PatternRepository::clear(){
	while(this->size != 0){
		this->pull(1);
	}
}