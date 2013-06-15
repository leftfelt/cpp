#include "Testament.h"

Testament::Testament(ClassifierSetting setting,ClusterRepository *cluster_repository){
	this->setting = setting;
	this->cluster_repository = cluster_repository;
	this->cluster_repository->setup();
	this->classifier = new KMeansClassifier(setting);
}

//学習する（別スレッドで回し続ける感じ）
void Testament::learn(double threshold){
	int learn_pattern_num = 10;
	std::vector<Pattern> pattern_list;
	for(int i = 0 ; i < learn_pattern_num ; i++) 
		pattern_list.push_back(pattern_repository.pull(this->setting.dimension_num));
	pattern_list = this->classifier->classify(pattern_list);
	this->cluster_repository->commit(pattern_list,threshold);
}