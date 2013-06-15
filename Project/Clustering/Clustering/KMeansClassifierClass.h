
#ifndef INCLUDE_KMEANSCLASSIFIER
#define INCLUDE_KMEANSCLASSIFIER

#include "ClassifierClass.h"

class KMeansClassifier : public Classifier{
public:
	KMeansClassifier();
	KMeansClassifier(ClassifierSetting setting);

	double objective(std::vector<Pattern> pattern_list);

	double getMembership(int num, Pattern pattern);
	
	double getWeight(Pattern pattern);

};

#endif