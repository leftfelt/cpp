#ifndef INCLUDE_TESTAMENT
#define INCLUDE_TESTAMENT

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
	#pragma comment(lib,"base.lib")
#endif

#include <map>
#include <SQLite.h>
#include <base/MathClass.h>
#include <base/NumberClass.h>
#include <ClassifierClass.h>
#include <fuzzyCMeansClassifierClass.h>
#include <KMeansClassifierClass.h>
#include <ClusterRepository.h>
#include <PatternRepository.h>
#include <RelationDataRepository.h>
#include <DictionaryRepository.h>
#include <FeatureRepository.h>

//TODO 名前空間
class TestamentSetting{
public:
	int equivocation; //あいまい度(0-100)
	int learn_num; //学習回数
	int relation_range; //関連する範囲
	std::string db_name; //db名
	std::string data_type; //データ形式
	ConverterSetting converter_setting;
};

class SearchResult{
	std::string data;
	int count;
public:
	SearchResult(std::string data, int count);
	std::string getData();
	int getCount();
	bool operator < (const SearchResult search_result) const;
};

class LearnData : public std::vector<Pattern>{
private:
	std::string data;
public:
	LearnData(const std::vector<Pattern> &pattern_list);
	void setData(std::string data);
	std::string getData();
};

//学習装置
class Testament{
private:
	DictionaryMap dictionary_map;
	DictionaryRepository *dictionary_repository;
	FeatureRepository *feature_repository;
	RelationDataRepository *relation_data_repository;
	Classifier *classifier;
	TestamentSetting setting;
	SQLite sqlite;
	template<typename T> PatternRepository setDataToPatternRepository(PatternRepository repository, T data){
		Converter<T> converter;
		converter.setSetting(this->setting.converter_setting);
		repository.push(converter.convert(data));
		return repository;
	}
	std::vector<LearnData> learn_data_list;
	Feature getFeature(LearnData learn_data, const Dictionary &dictionary);
	Dictionary getDictionary(std::vector<LearnData> learn_data_list);

	std::vector<RelationData> getRelationData(std::vector<Pattern> pattern_list, int relation_range, int cluster_size, int dimension_num);
	std::vector<int> getRelationLabel(std::vector<Pattern> pattern_list, int relation_range, int cluster_size, int dimension_num);
public:
	Testament(
		TestamentSetting setting,
		Classifier *classifier
	);
	~Testament();

	//設定で指定した数だけ学習する（別スレッドで回し続ける感じ）
	void learn();

	//データを全て削除
	void truncate();

	//dataにlink_dataを関連付ける 
	template<typename T> void toLink(T data, std::string link_data){
		PatternRepository repository;
		repository = this->setDataToPatternRepository(repository, data);
		LearnData learn_data(repository.pullAll(this->classifier->getSetting().dimension_num));
		learn_data.setData(link_data);
		this->learn_data_list.push_back(learn_data);
	}

	template<typename T> std::vector<std::string> search(T data, int limit){
		std::map<std::string, int> data_map;
		ClassifierSetting classifier_setting = this->classifier->getSetting();
		PatternRepository repository;
		repository = this->setDataToPatternRepository(repository, data);
		std::vector<Pattern> data_list = repository.pullAll(classifier_setting.dimension_num);
		std::vector<std::string> results;

		//関連データを取得
		std::vector<RelationData> relation_data_list = this->getRelationData(data_list,this->setting.relation_range,classifier_setting.cluster_size, classifier_setting.dimension_num);
		if(relation_data_list.empty()) return results;
		for(int i = 0 ; i < (signed)relation_data_list.size() ; i++){
			data_map[relation_data_list[i].getData()]++;
		}

		if(data_map.empty()) return results;

		//カウントの大きい順に並べ替え
		std::priority_queue<SearchResult> queue;
		std::map<std::string, int>::iterator itr;
		for(itr = data_map.begin() ; itr != data_map.end(); itr++){
			queue.push(SearchResult(itr->first, itr->second));
		}
		//limit数だけ取得
		for(int i = 0 ; i < limit && !queue.empty(); i++){
			results.push_back(queue.top().getData());
			queue.pop();
		}

		return results;
	}
};

#endif