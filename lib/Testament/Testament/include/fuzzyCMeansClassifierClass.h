#ifndef INCLUDE_FUZZYCMEANSCLASSIFIER
#define INCLUDE_FUZZYCMEANSCLASSIFIER

#include <ClassifierClass.h>

class fuzzyCMeansClassifier : public Classifier{
private:
	int fuzzy;
public:
	fuzzyCMeansClassifier(ClassifierSetting setting, int fuzzy=2);

	double objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list);

	double getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list);

	double getWeight(Pattern pattern);
};

#endif