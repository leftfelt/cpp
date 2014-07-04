#include <Testament.h>

SearchResult::SearchResult(std::string data, int count){
	this->data = data;
	this->count = count;
}

int SearchResult::getCount(){
	return this->count;
}

std::string SearchResult::getData(){
	return this->data;
}

bool SearchResult::operator < (const SearchResult search_result) const{
	return this->count < search_result.count;
}
//======================================================================================================

LearnData::LearnData(const std::vector<Pattern> &pattern_list) : std::vector<Pattern>(pattern_list.size()){
	std::copy(pattern_list.begin(), pattern_list.end(), this->begin());
}

void LearnData::setData(std::string data){
	this->data = data;
}
std::string LearnData::getData(){
	return this->data;
}

//======================================================================================================
Testament::Testament(
		TestamentSetting setting,
		Classifier *classifier
	){
	this->setting = setting;
	//辞書リポジトリ
	this->dictionary_repository = new DictionaryRepository(this->setting.db_name,this->setting.data_type);
	this->dictionary_repository->setup();
	//関連データリポジトリ
	this->relation_data_repository = new RelationDataRepository(this->setting.db_name);
	this->relation_data_repository->setup();
	//特徴量リポジトリ
	this->feature_repository = new FeatureRepository(this->setting.db_name);
	this->feature_repository->setup();
	//分類器
	this->classifier = classifier;
	this->sqlite.open(this->setting.db_name);
}

Testament::~Testament(){
	this->sqlite.close();
}

std::vector<RelationData> Testament::getRelationData(std::vector<Pattern> pattern_list, int relation_range, int cluster_size, int dimension_num){
	std::vector<RelationData> data_list;
	std::vector<int> label_list = this->getRelationLabel(pattern_list, relation_range, cluster_size, dimension_num);

	std::for_each(label_list.begin(), label_list.end(), [&](int label){
		try{
			std::vector<RelationData> result = this->relation_data_repository->search(label);
			data_list.insert(data_list.end(), result.begin(), result.end());
		}catch(std::exception e){
			return;
		}
	});

	return data_list;
}

std::vector<int> Testament::getRelationLabel(std::vector<Pattern> pattern_list, int relation_range, int cluster_size, int dimension_num){
	Converter<Pattern> converter;
	std::vector<int> label_list;
	int id = 1;

	while((signed)label_list.size() < relation_range){
		Dictionary dictionary;
		
		dictionary = this->dictionary_map.get(id);
		if(dictionary.empty()){
			//辞書マップになければDBから取得する
			try{
				dictionary = this->dictionary_repository->find(id, cluster_size, dimension_num);
			}catch(std::exception e){
				 return label_list;
			}
			this->dictionary_map.set(id, dictionary);
		}
		if(dictionary.empty()) return label_list;

		Feature feature = this->getFeature(pattern_list, dictionary);
		
		for(int i = 0 ; i < this->setting.learn_num && (signed)label_list.size() < relation_range ; i++){
			int feature_id = (id-1)*this->setting.learn_num+i+1;
			//しきい値以下ならラベルを追加
			double similarity = Math::cosSimilarity(
				converter.convert(feature), 
				converter.convert(this->feature_repository->find(feature_id, cluster_size))
			);

			if(similarity >= this->setting.equivocation / (double)100){
				label_list.push_back(feature_id);
			}
		}

		id++;
	}

	return label_list;
}

void Testament::learn(){
	//toLinkしたデータがlearn_num以上になったら学習を開始する
	if(this->learn_data_list.size() < this->setting.learn_num) return;

	Dictionary dictionary = this->getDictionary(this->learn_data_list);
	std::vector<Feature> feature_list;

	std::for_each(this->learn_data_list.begin(), this->learn_data_list.end(), [&](LearnData learn_data){
		feature_list.push_back(this->getFeature(learn_data, dictionary));
	});
	
	int id = this->feature_repository->size(dictionary.size()) + 1;

	//featureとdataの関連を保存する処理
	for(int i = 0 ; i < this->learn_data_list.size() ; i++){
		this->relation_data_repository->commit(
			RelationData(id+i, this->learn_data_list[i].getData())
		);
	} 
	
	//cluster_listを保存する処理
	this->dictionary_repository->add(dictionary);

	//feature_listを保存する処理
	std::for_each(feature_list.begin(), feature_list.end(), [&](Feature feature){
		this->feature_repository->add(feature);
	});

	this->learn_data_list.clear();
}

void Testament::truncate(){
	this->dictionary_repository->truncate();
	this->relation_data_repository->truncate();
	this->feature_repository->truncate();
}

//クラスタリストと特徴量リストから投票して、特徴ベクトルを取得する
Feature Testament::getFeature(LearnData learn_data, const Dictionary &dictionary){
	Feature feature(dictionary.size());
	std::vector<int> vote(dictionary.size());
	
	std::for_each(learn_data.begin(), learn_data.end(), [&](Pattern pattern){
		int i;
		for(i=0 ; i<dictionary.size() ; i++){
			if(this->classifier->getMembership(i, pattern, dictionary) > 0.0) break;
		}
		vote[i] += 1;
	});

	std::copy(vote.begin(),vote.end(),feature.begin());

	return feature;
}

//学習データをクラスタリングしてクラスタリストを取得
Dictionary Testament::getDictionary(std::vector<LearnData> learn_data_list){
	std::vector<Pattern> data_list;
	std::for_each(learn_data_list.begin(), learn_data_list.end(), [&](LearnData learn_data){
		data_list.insert(data_list.begin(), learn_data.begin(), learn_data.end());
	});
	return this->classifier->classify(data_list);
}