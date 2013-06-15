#ifndef INCLUDE_TESTAMENT
#define INCLUDE_TESTAMENT

#include "ClassifierClass.h"
#include "fuzzyCMeansClassifierClass.h"
#include "KMeansClassifierClass.h"
#include "ClusterRepository.h"
#include "PatternRepository.h"

//学習装置
class Testament{
private:
	PatternRepository pattern_repository;
	ClusterRepository *cluster_repository;
	Classifier *classifier;
	ClassifierSetting setting;
	int dimension_num;
public:
	Testament(ClassifierSetting setting,ClusterRepository *cluster_repository);

	//各データ毎のスレッドから受け付ける感じ
	template<typename T> void accept(T data){
		Converter<T> converter;
		//入力があったらとりあえずバッチに積む
		this->pattern_repository.push(converter.convert(data));
	}

	//学習する（別スレッドで回し続ける感じ）
	void learn(double threshold);
};

#endif