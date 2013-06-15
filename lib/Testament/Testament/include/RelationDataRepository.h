#ifndef INCLUDE_RELATIONDATAREPOSITORY
#define INCLUDE_RELATIONDATAREPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
	#pragma comment(lib,"base.lib")
#endif

#include <SQLite.h>
#include <sstream>
#include <string>
#include <algorithm>

class RelationData{
private:
	int label;
	std::string data;
public:
	RelationData();
	RelationData(int label, std::string data);
	RelationData(Row row);
	int getLabel();
	std::string getData();
};

class RelationDataRepository{
private:
	SQLite sqlite;
public:
	RelationDataRepository(std::string db_name);
	~RelationDataRepository();
	std::string getTableName();
	std::string getRelationDataQuery(int label, int limit);
	std::string setRelationDataQuery(int label, std::string data);
	void setup();
	std::vector<RelationData> search(int label, int limit=1);
	void commit(RelationData relation_data);
	void truncate();
};

#endif