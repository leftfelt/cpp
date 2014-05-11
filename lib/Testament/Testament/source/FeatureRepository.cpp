#include <FeatureRepository.h>

FeatureRepository::FeatureRepository(std::string db_name){
	this->sqlite.open(db_name);
}

 //テーブル名を取得
std::string FeatureRepository::getTableName(){
	return "features";
}

//id指定でdictionaryを取得するクエリ
std::string FeatureRepository::getFindQuery(int id, int limit){
	std::ostringstream query;
	int start = (id-1) * limit;
	query << "SELECT id, data FROM " << this->getTableName() << " LIMIT " << limit << " OFFSET " << start;

	return query.str();
}

//dictionaryを追加するクエリ
std::string FeatureRepository::getAddQuery(Feature feature){
	std::ostringstream query;

	query << "INSERT INTO " << this->getTableName() << "(data) VALUES";
	std::for_each(feature.begin(), feature.end(), [&](double data){
		query << "(" << data << "),";
	});

	std::string str = query.str();
	return str.substr(0,str.size()-1);
}

//dictionaryの数を取得するクエリ
std::string FeatureRepository::getFeatureSizeQuery(){
	std::ostringstream query;

	query << "SELECT count(id) as size FROM " << this->getTableName();

	return query.str();
}

void FeatureRepository::setup(){
	this->sqlite.startTransaction();
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"data DOUBLE"
		");"
	);
	this->sqlite.commitTransaction();
}


Feature FeatureRepository::getFeatureFromRowList(std::vector<Row> &row_list, int dimension_num){
	Feature feature(dimension_num);

	for(int i = 0 ; i < dimension_num ; i++){
		std::stringstream ss;
		ss << row_list[i]["data"];
		ss >> feature[i];
	}

	return feature;
}


Feature FeatureRepository::find(int id, int dimension_num){
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getFindQuery(id, dimension_num));
	this->sqlite.commitTransaction();
	if(result.empty()) throw std::exception("feature not found id:"+id);

	return this->getFeatureFromRowList(result, dimension_num);
}

void FeatureRepository::add(Feature feature){
	this->sqlite.startTransaction();
	this->sqlite.execute(this->getAddQuery(feature));
	this->sqlite.commitTransaction();
}

int FeatureRepository::size(int dimension_num){
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getFeatureSizeQuery());
	this->sqlite.commitTransaction();
	if(result.empty()){ return -1; }
	std::istringstream iss;
	iss.str(result[0]["size"]);
	int size;
	iss >> size;
	return size / dimension_num;
}

void FeatureRepository::truncate(){
	this->sqlite.startTransaction();
	this->sqlite.truncate(this->getTableName());
	this->sqlite.commitTransaction();
}