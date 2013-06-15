#include "KMeansClassifierClass.h"

KMeansClassifier::KMeansClassifier(){}

KMeansClassifier::KMeansClassifier(ClassifierSetting setting):Classifier(setting){}

double KMeansClassifier::objective(std::vector<Pattern> pattern_list){
	double jkmeans = 0.000;
	double result = 0.000;
	std::for_each(pattern_list.begin(),pattern_list.end(),[&](Pattern pattern){
		for(int i = 0 ; i < this->setting.cluster_size ; i++){
			jkmeans += this->getDistance(pattern,this->getMiddle(i),2.0);
		}
		result += jkmeans;
	});

	return result;
}

double KMeansClassifier::getMembership(int num, Pattern pattern){
	double min = DBL_MAX;
	double dis;
	for(int i = 0 ; i < this->setting.cluster_size ; i++){
		dis = this->getDistance(pattern,this->getMiddle(i),2.0);
		if(dis < min){
			min = dis;
		}
	}

	return (this->getDistance(pattern,this->getMiddle(num),2.0) == min) ? 1 : 0;
}

double KMeansClassifier::getWeight(Pattern pattern){
	return 1.0;
}
