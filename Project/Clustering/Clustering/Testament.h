#ifndef INCLUDE_TESTAMENT
#define INCLUDE_TESTAMENT

#include "ClassifierClass.h"
#include "fuzzyCMeansClassifierClass.h"
#include "KMeansClassifierClass.h"
#include "ClusterRepository.h"
#include "PatternRepository.h"

//�w�K���u
class Testament{
private:
	PatternRepository pattern_repository;
	ClusterRepository *cluster_repository;
	Classifier *classifier;
	ClassifierSetting setting;
	int dimension_num;
public:
	Testament(ClassifierSetting setting,ClusterRepository *cluster_repository);

	//�e�f�[�^���̃X���b�h����󂯕t���銴��
	template<typename T> void accept(T data){
		Converter<T> converter;
		//���͂���������Ƃ肠�����o�b�`�ɐς�
		this->pattern_repository.push(converter.convert(data));
	}

	//�w�K����i�ʃX���b�h�ŉ񂵑����銴���j
	void learn(double threshold);
};

#endif