
#ifndef INCLUDE_KMEANSCLASSIFIER
#define INCLUDE_KMEANSCLASSIFIER

#include <ClassifierClass.h>

class KMeansClassifier : public Classifier{
public:
	KMeansClassifier();
	KMeansClassifier(ClassifierSetting setting);

	double objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list);

	double getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list);
	
	double getWeight(Pattern pattern);

};

#endif