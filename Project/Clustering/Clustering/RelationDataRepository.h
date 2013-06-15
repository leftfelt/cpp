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

class RelationData{
private:
	int label;
	std::string data;
public:
	RelationData(int label, std::string data);
	RelationData(Row row);
	int getLabel();
	std::string getData();
};

class RelationDataRepository{
private:
	static const std::string db_name;
	SQLite sqlite;
	std::string getTableName();
	std::string getRelationDataQuery(int label);
	std::string setRelationDataQuery(int label, std::string data);
public:
	RelationDataRepository();
	~RelationDataRepository();
	void setup();
	RelationData search(int label);
	void commit(RelationData relation_data);
};

#endif