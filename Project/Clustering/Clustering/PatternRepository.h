#ifndef INCLUDE_PATTERNREPOSITORY
#define INCLUDE_PATTERNREPOSITORY

#include "ClassifierClass.h"
#include <boost/lockfree/queue.hpp>

//学習するパターンを貯めとくところ
class PatternRepository{
private:
	boost::lockfree::queue<double> *repository;
public:
	PatternRepository();

	void push(std::vector<double> data_list);

	Pattern pull(int dimension_num);
};

#endif