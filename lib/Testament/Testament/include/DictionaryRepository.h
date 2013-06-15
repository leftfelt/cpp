#ifndef INCLUDE_DICTIONARY_REPOSITORY
#define INCLUDE_DICTIONARY_REPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"sqlite3d.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"sqlite3.lib")
#endif

#include <SQLite.h>
#include <PatternRepository.h>

class DictionaryPattern : public Pattern{
private:
	int id;
public:
	DictionaryPattern(Pattern pattern);
	void setId(int id);
	int getId(void);
};

//é´èë
typedef std::vector<Pattern> Dictionary;

class DictionaryRepository{
private:
	SQLite sqlite;
	std::string data_type;
	std::string getTableName();
	std::string getFindQuery(int id, int limit);
	std::string getAddQuery(Pattern pattern);
	std::string getDictionarySizeQuery();
	std::string getFindOrderByUpdateTime(int limit, int offset);
	DictionaryPattern getPatternFromRowList(std::vector<Row> &row_list, int pattern_size, int offset=0);
	std::vector<DictionaryPattern> getPatternListFromRowList(std::vector<Row> &row_list, int pattern_size);
	Dictionary getDictionaryFromPatternList(std::vector<DictionaryPattern> pattern_list, int dictionary_size, int pattern_size, int offset=0);
public:
	DictionaryRepository(std::string db_name, std::string data_type);
	void setup();
	Dictionary find(int id, int dictionary_size, int pattern_size);
	Dictionary findOrderByUpdateTime(int num, int dictionary_size, int pattern_size);
	void add(Dictionary dictionary);
	int size(int dimension_num = 1);
	void truncate();
};

class DictionaryMap{
private:
	std::map<int, Dictionary> dictionary_map;
public:
	Dictionary get(int key);
	void set(int key, Dictionary dictionary_list);
};

#endif