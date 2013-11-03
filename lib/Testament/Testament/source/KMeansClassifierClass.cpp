#include <KMeansClassifierClass.h>

KMeansClassifier::KMeansClassifier(){}

KMeansClassifier::KMeansClassifier(ClassifierSetting setting):Classifier(setting){}

double KMeansClassifier::objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list){
	double jkmeans = 0.000;
	double result = 0.000;
	std::for_each(pattern_list.begin(),pattern_list.end(),[&](Pattern pattern){
		for(int i = 0 ; i < middle_list.size() ; i++){
			jkmeans += this->getDistance(pattern,middle_list[i],2.0);
		}
		result += jkmeans;
	});

	return result;
}

//num�Ԗڂ̃N���X�^�Ƃ̋������ŒZ�Ȃ�1��Ԃ�
double KMeansClassifier::getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list){
	double min = DBL_MAX;
	double dis;
	int min_index = -1;
	//�e�N���X�^�̍ŒZ�����Ǝ擾
	for(int i = 0 ; i < middle_list.size() ; i++){
		dis = this->getDistance(pattern,middle_list[i],2.0);
		if(dis < min){
			min = dis;
			min_index = i;
		}
	}

	return (min_index == num);
//	return (this->getDistance(pattern,middle_list[num],2.0) == min) ? 1 : 0;
}

double KMeansClassifier::getWeight(Pattern pattern){
	return 1.0;
}
