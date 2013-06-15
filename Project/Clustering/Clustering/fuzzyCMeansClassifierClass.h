#ifndef INCLUDE_FUZZYCMEANSCLASSIFIER
#define INCLUDE_FUZZYCMEANSCLASSIFIER

#include "ClassifierClass.h"

class fuzzyCMeansClassifier : public Classifier{
private:
	int fuzzy;
public:
	fuzzyCMeansClassifier(ClassifierSetting setting);

	double objective(std::vector<Pattern> pattern_list);

	double getMembership(int num, Pattern pattern);

	double getWeight(Pattern pattern);

	int getClusterNum(Pattern pattern);
};

#endif