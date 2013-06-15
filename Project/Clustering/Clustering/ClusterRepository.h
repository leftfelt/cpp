#ifndef INCLUDE_CLUSTERREPOSITORY
#define INCLUDE_CLUSTERREPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
	#pragma comment(lib,"base.lib")
#endif

#include "ClassifierClass.h"
#include "Converter.h"
#include <SQLite.h>
#include <base/MathClass.h>
#include <base/NumberClass.h>
#include <sstream>

//ƒNƒ‰ƒXƒ^
class Cluster{
private:
	std::string label; //•ª—Þ–¼
	double feature; //“Á’¥—Ê
public:
	Cluster(std::string label, double feature);
	std::string getLabel();
	double getFeature();
};

class ClusterRepository{
private:
	static const std::string cluster_repository_db_name;
	std::string data_type;
	SQLite sqlite;
	std::string getTableName();
	std::string getSelectClusterQuery(double feature, double threshold);
	std::string getRaisePriorityQuery(std::vector<Row> result);
	std::string getAddClusterQuery(double feature, int label = -1);
public:
	ClusterRepository(std::string data_type);
	~ClusterRepository();
	void setup();
	void adjust();
	Row search(Pattern pattern,double threshold);
	void commit(std::vector<Pattern> pattern_list,  double threshold);
};

#endif