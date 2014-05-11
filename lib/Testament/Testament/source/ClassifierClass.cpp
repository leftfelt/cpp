#include <ClassifierClass.h>

Classifier::Classifier(){}

Classifier::Classifier(ClassifierSetting setting){
	this->setting = setting;
}

std::vector<Pattern> Classifier::getRandomMiddleList(double min, double max, int size){

	if(size==0) size = this->setting.cluster_size;

	std::vector<Pattern> middle_list(size);
	std::random_device seed_gen;
	std::default_random_engine rng(seed_gen());
	//std::mt19937 rng;

	std::uniform_real_distribution<double> dist(min, max);

	std::function<double()> generateRandomNumber = [&]()->double{
		return dist(rng);
	};
	
	//乱数で初期化
	std::generate(middle_list.begin(), middle_list.end(),[&]()->Pattern{
		Pattern pattern(this->setting.dimension_num);
		std::generate(pattern.begin(), pattern.end(), generateRandomNumber);
		return pattern;
	});

	return middle_list;
}

std::vector<Pattern> Classifier::getRandomMiddleList(std::vector<Pattern> data_list){
	double min = DBL_MAX;
	double max = DBL_MIN;
	std::for_each(data_list.begin(), data_list.end(), [&](Pattern pattern){
		for(int i=0 ; i<(signed)pattern.size() ; i++){
			if(pattern[i] < min) min = pattern[i];
			if(max < pattern[i]) max = pattern[i];
		}
	});

	return this->getRandomMiddleList(min, max);
}

//現在のnum番目の中心を取得する
Pattern Classifier::getMiddle(int num, std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list){
	Pattern sum_pattern(this->setting.dimension_num);
	std::fill(sum_pattern.begin(), sum_pattern.end(), 0.000);
	double sum = 0.000;
	//i番目のクラスタの中心を再計算する
	std::for_each(pattern_list.begin(),pattern_list.end(),[&](Pattern pattern){
		double membership = this->getMembership(num,pattern, middle_list);
		double weight = this->getWeight(pattern);
		sum_pattern += membership * weight * pattern;
		sum += membership * weight;
	});
	return (sum != (double)0) ? (sum_pattern / sum) : middle_list[num];
}

//分類
std::vector<Pattern> Classifier::classify(std::vector<Pattern> pattern_list){
	//return this->classify(pattern_list, this->getRandomMiddleList(pattern_list));
	return this->classify(pattern_list, this->getRandomMiddleList(this->setting.data_min, this->setting.data_max));
}

std::vector<Pattern> Classifier::classify(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list){
	int count = 0;
	double error = DBL_MAX;
	double objective = DBL_MAX;
	
	//ここから（目的関数が収束するまで繰り返す）
	while(count < this->setting.classification_num && error >= this->setting.permissible_error){
		for(int i = 0 ; i < (signed)middle_list.size() ; i++){
			//クラスタの中心を再設定
			middle_list[i] = this->getMiddle(i, pattern_list, middle_list);
		}
		double current_objective = this->objective(pattern_list, middle_list);
		error = objective - current_objective;
		objective = current_objective;
		count++;
	}

	//ここまで
	return middle_list;
}

//２つのパターンの距離
double Classifier::getDistance(Pattern p1, Pattern p2, double power){
	double result = 0.0;
	for(int i = 0 ; i < this->setting.dimension_num ; i++){
		result += pow((p1[i] - p2[i]),power);
	}

	return result;
}

//設定を取得
ClassifierSetting Classifier::getSetting(){
	return this->setting;
}
//=================================================================