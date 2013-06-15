#include <fuzzyCMeansClassifierClass.h>

fuzzyCMeansClassifier::fuzzyCMeansClassifier(ClassifierSetting setting, int fuzzy) : Classifier(setting){
	this->fuzzy = fuzzy;
}

double fuzzyCMeansClassifier::objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list){
	double jfcm = 0.000;
	double result = 0.000;

	std::for_each(pattern_list.begin(),pattern_list.end(),[&](Pattern pattern){
		for(int i = 0 ; i < (signed)middle_list.size() ; i++){
			result += this->getMembership(i,pattern, middle_list)*this->getDistance(pattern,middle_list[i],2.0);
		}
		jfcm += result;
	});
	return jfcm;
}

double fuzzyCMeansClassifier::getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list){
	double result = 0.0;
	/*
	double dis = this->getDistance(pattern,this->getMiddle(num), 2.0);
	for(int i = 0 ; i < this->cluster_size ; i++){
		result += 1 / pow(dis / this->getDistance(pattern,this->getMiddle(i), 2.0) , -2/(this->fuzzy-1));
	}*/

	double dis = this->getDistance(pattern,middle_list[num], -2/(this->fuzzy-1));
	for(int i = 0 ; i < (signed)middle_list.size() ; i++){
		result += this->getDistance(pattern,middle_list[i], -2/(this->fuzzy-1));
	}

	result = dis / result;

	if (result < (double)0) return (double)0;

	return result;
}

double fuzzyCMeansClassifier::getWeight(Pattern pattern){
	return 1.0;
}