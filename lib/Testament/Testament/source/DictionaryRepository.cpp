#include <DictionaryRepository.h>

DictionaryPattern::DictionaryPattern(Pattern pattern) : Pattern(pattern){
}

void DictionaryPattern::setId(int id){
	this->id = id;
}

int DictionaryPattern::getId(void){
	return this->id;
}

//============================================================================================

DictionaryRepository::DictionaryRepository(std::string db_name, std::string data_type){
	this->data_type = data_type;
	this->sqlite.open(db_name);
}

 //テーブル名を取得
std::string DictionaryRepository::getTableName(){
	return this->data_type+"_dictionaries";
}

//id指定でdictionaryを取得するクエリ
std::string DictionaryRepository::getFindQuery(int id, int limit){
	std::ostringstream query;
	int start = (id-1) * limit;
	query << "SELECT id, data FROM " << this->getTableName() << " LIMIT " << limit << " OFFSET " << start;

	return query.str();
}

//dictionaryを追加するクエリ
std::string DictionaryRepository::getAddQuery(Pattern pattern){
	std::ostringstream query;

	query << "INSERT INTO " << this->getTableName() << "(data) VALUES";
	std::for_each(pattern.begin(), pattern.end(), [&](double data){
		query << "(" << data << "),";
	});

	std::string str = query.str();
	return str.substr(0,str.size()-1);
}

//dictionaryの数を取得するクエリ
std::string DictionaryRepository::getDictionarySizeQuery(){
	std::ostringstream query;

	query << "SELECT count(id) as size FROM " << this->getTableName();

	return query.str();
}

//更新日時が近い順でdictionaryを取得するクエリ
std::string DictionaryRepository::getFindOrderByUpdateTime(int limit, int offset){
	std::ostringstream query;

	query << "SELECT * FROM " << this->getTableName() << " LIMIT " << limit << " OFFSET " << offset;

	return query.str();
}

//テーブルが存在しない場合はテーブルを作る
void DictionaryRepository::setup(){
	this->sqlite.startTransaction();
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"data DOUBLE,"
		"update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
		");"
	);
	this->sqlite.execute("CREATE INDEX IF NOT EXISTS idindex ON "+this->getTableName()+"(id);");
	this->sqlite.execute("CREATE INDEX IF NOT EXISTS update_timeindex ON "+this->getTableName()+"(update_time);");
	this->sqlite.commitTransaction();
}


DictionaryPattern DictionaryRepository::getPatternFromRowList(std::vector<Row> &row_list, int pattern_size, int offset){
	Pattern pattern(pattern_size);

	std::istringstream iss;
	for(int j = offset ; j < offset+pattern_size ; j++){
		iss.str(row_list[j]["data"]);
		iss >> pattern[j-offset];
		iss.clear();
	}

	DictionaryPattern dictionary_pattern = pattern;
	int id;
	iss.str(row_list[offset]["id"]);
	iss >> id;
	dictionary_pattern.setId((int)(id/pattern_size));

	return dictionary_pattern;
}

std::vector<DictionaryPattern> DictionaryRepository::getPatternListFromRowList(std::vector<Row> &row_list, int pattern_size){
	std::vector<DictionaryPattern> pattern_list;

	for(int i = 0 ; i < (signed)row_list.size() ; i+= pattern_size){
		pattern_list.push_back(this->getPatternFromRowList(row_list, pattern_size, i));
	}

	return pattern_list;
}

Dictionary DictionaryRepository::getDictionaryFromPatternList(std::vector<DictionaryPattern> pattern_list, int dictionary_size, int pattern_size, int offset){
	Dictionary dictionary;
	for(int i = offset ; i < offset+dictionary_size ; i++){
		dictionary.push_back(pattern_list[i]);
	}
	return dictionary;
}
/**
 * 辞書を取得する
 * @param int id
 * @param int dictionary_size
 * @param int pattern_size
 * @return Dictionary
 */
Dictionary DictionaryRepository::find(int id, int dictionary_size, int pattern_size){
	Dictionary dictionary;
	for(int i = 0 ; i < dictionary_size ; i++){
		this->sqlite.startTransaction();
		std::vector<Row> result = this->sqlite.execute(this->getFindQuery(id+i, pattern_size));
		this->sqlite.commitTransaction();
		if(result.empty()) throw std::exception("dictionary not found id:"+id);
		dictionary.push_back(this->getPatternFromRowList(result, pattern_size));
	}
	
	return dictionary;
}

Dictionary DictionaryRepository::findOrderByUpdateTime(int id, int dictionary_size, int pattern_size){
	Dictionary dictionary;
	int count = this->size();
	int limit = dictionary_size*pattern_size;
	int offset = count - id*limit;

	if(offset < 0) return dictionary;
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getFindOrderByUpdateTime(limit, offset));
	this->sqlite.commitTransaction();

	if(result.empty()) return dictionary;

	std::vector<DictionaryPattern> pattern_list = this->getPatternListFromRowList(result,pattern_size);

	return this->getDictionaryFromPatternList(pattern_list, dictionary_size, pattern_size);
}

void  DictionaryRepository::add(Dictionary dictionary){
	std::for_each(dictionary.begin(), dictionary.end(), [&](Pattern pattern){
		this->sqlite.startTransaction();
		this->sqlite.execute(this->getAddQuery(pattern));
		this->sqlite.commitTransaction();
	});
}

int DictionaryRepository::size(int dimension_num){
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getDictionarySizeQuery());
	this->sqlite.commitTransaction();
	if(result.empty()){ return -1; }
	std::istringstream iss;
	iss.str(result[0]["size"]);
	int size;
	iss >> size;
	return size / dimension_num;
}

void DictionaryRepository::truncate(){
	this->sqlite.startTransaction();
	this->sqlite.truncate(this->getTableName());
	this->sqlite.commitTransaction();
}

//===========================================================
Dictionary DictionaryMap::get(int key){
	return this->dictionary_map[key];
}

void DictionaryMap::set(int key, Dictionary dictionary){
	this->dictionary_map[key] = dictionary;
}