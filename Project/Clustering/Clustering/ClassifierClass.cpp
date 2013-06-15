#include "ClassifierClass.h"

Classifier::Classifier(){}

Classifier::Classifier(ClassifierSetting setting){
	this->setting = setting;

	this->initialize();
}

void Classifier::initialize(){
	this->middle.resize(this->setting.cluster_size);
	srand((unsigned int)time(NULL));
	Pattern pattern(this->setting.dimension_num);
	
	//�����ŏ�����
	std::generate(this->middle.begin(), this->middle.end(),[&]()->Pattern{
		ClassifierSetting setting = this->setting;
		Pattern pattern(setting.dimension_num);
		std::generate(pattern.begin(), pattern.end(), [setting]()->double{
			return setting.data_min + rand() % (int)(setting.data_max - setting.data_min) + (double)rand() / RAND_MAX;
		});
		return pattern;
	});
}

//�w�K
std::vector<Pattern> Classifier::classify(std::vector<Pattern> pattern_list){
	Pattern result_pattern;
	double result = 0.000;
	int count = 0;

	result_pattern.resize(this->setting.dimension_num);
	this->initialize();
	
	//��������i�ړI�֐�����������܂ŌJ��Ԃ��j
	while(count < this->setting.learn_num){
		for(int i = 0 ; i < this->setting.cluster_size ; i++){
			std::fill(result_pattern.begin(), result_pattern.end(), 0.0);
			result = 0.0;
			std::for_each(pattern_list.begin(),pattern_list.end(),[&](Pattern pattern){
				result_pattern += this->getMembership(i,pattern) * this->getWeight(pattern) * pattern;
				result += this->getMembership(i,pattern) * this->getWeight(pattern);
			});
			middle[i] = (result != (double)0) ? (result_pattern / result) : middle[i];
		}
		count++;
	}
	//�����܂�
	return this->middle;
}

//�Q�̃p�^�[���̋���
double Classifier::getDistance(Pattern p1, Pattern p2, double power){
	double result = 0.0;
	for(int i = 0 ; i < this->setting.dimension_num ; i++){
		result += pow((p1[i] - p2[i]),power);
	}

	return result;
}

//�N���X�^�̒��S
Pattern Classifier::getMiddle(int num){
	return middle[num];
}

//=================================================================