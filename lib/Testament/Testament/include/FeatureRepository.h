#ifndef INCLUDE_FEATURE_REPOSITORY
#define INCLUDE_FEATURE_REPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"sqlite3d.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"sqlite3.lib")
#endif

#include <SQLite.h>
#include <sstream>
#include <algorithm>
#include <PatternRepository.h>

//“Á’¥—Ê
typedef Pattern Feature;

class FeatureRepository{
private:
	SQLite sqlite;
	std::string getTableName();
	std::string getFindQuery(int id, int limit);
	std::string getAddQuery(Feature feature);
	std::string getFeatureSizeQuery();
	Feature getFeatureFromRowList(std::vector<Row> &row_list, int dimension_num);
public:
	FeatureRepository(std::string db_name);
	void setup();
	Feature find(int id, int dimension_num);
	void add(Feature feature);
	int size(int dimension_num);
	void truncate();
};

#endif