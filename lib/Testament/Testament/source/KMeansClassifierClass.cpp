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

//num番目のクラスタとの距離が最短なら1を返す
double KMeansClassifier::getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list){
	double min = DBL_MAX;
	double dis;
	int min_index = -1;
	//各クラスタの最短距離と取得
	for(int i = 0 ; i < middle_list.size() ; i++){
		dis = this->getDistance(pattern,middle_list[i],2.0);
		if(dis < min){
			min = dis;
			min_index = i;
		}
	}

	return (min_index == num) ? 1.0 : 0.0;
//	return (this->getDistance(pattern,middle_list[num],2.0) == min) ? 1 : 0;
}

double KMeansClassifier::getWeight(Pattern pattern){
	return 1.0;
}
