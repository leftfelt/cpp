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

//TODO ���O���
class TestamentSetting{
public:
	int equivocation; //�����܂��x(0-100)
	int learn_num; //�w�K��
	int relation_range; //�֘A����͈�
	std::string db_name; //db��
	std::string data_type; //�f�[�^�`��
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

//�w�K���u
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

	//�ݒ�Ŏw�肵���������w�K����i�ʃX���b�h�ŉ񂵑����銴���j
	void learn();

	//�f�[�^��S�č폜
	void truncate();

	//data��link_data���֘A�t���� 
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

		//�֘A�f�[�^���擾
		std::vector<RelationData> relation_data_list = this->getRelationData(data_list,this->setting.relation_range,classifier_setting.cluster_size, classifier_setting.dimension_num);
		if(relation_data_list.empty()) return results;
		for(int i = 0 ; i < (signed)relation_data_list.size() ; i++){
			data_map[relation_data_list[i].getData()]++;
		}

		if(data_map.empty()) return results;

		//�J�E���g�̑傫�����ɕ��בւ�
		std::priority_queue<SearchResult> queue;
		std::map<std::string, int>::iterator itr;
		for(itr = data_map.begin() ; itr != data_map.end(); itr++){
			queue.push(SearchResult(itr->first, itr->second));
		}
		//limit�������擾
		for(int i = 0 ; i < limit && !queue.empty(); i++){
			results.push_back(queue.top().getData());
			queue.pop();
		}

		return results;
	}
};

#endif