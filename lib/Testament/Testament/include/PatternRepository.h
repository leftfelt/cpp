#ifndef INCLUDE_PATTERNREPOSITORY
#define INCLUDE_PATTERNREPOSITORY

#include <boost/numeric/ublas/vector.hpp>
#include <boost/assign.hpp>
#include <boost/lockfree/queue.hpp>

//�p�^�[��
typedef boost::numeric::ublas::vector<double> Pattern;

//�w�K����p�^�[���𒙂߂Ƃ��Ƃ���
class PatternRepository{
private:
	boost::lockfree::queue<double> *repository;
	int size;
public:
	PatternRepository();
	
	int getSize();

	void push(std::vector<double> data_list);

	Pattern pull(int dimension_num);

	std::vector<Pattern> pullAll(int dimension_num);

	void clear();

};

#endif