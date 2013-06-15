#ifndef INCLUDE_CLUSTERREPOSITORY
#define INCLUDE_CLUSTERREPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
	#pragma comment(lib,"base.lib")
#endif

#include <ClassifierClass.h>
#include <Converter.h>
#include <SQLite.h>
#include <base/MathClass.h>
#include <base/NumberClass.h>
#include <sstream>

//クラスタ
class Cluster{
private:
	std::string label; //分類名
	double feature; //特徴量
public:
	Cluster(std::string label, double feature);
	Cluster(Row row);
	std::string getLabel();
	double getFeature();
};

//クラスタリポジトリ
class ClusterRepository{
private:
	std::string data_type; //データの種類
	SQLite sqlite;
public:
	ClusterRepository(std::string db_name, std::string data_type);
	~ClusterRepository();
	std::string getTableName(); //テーブル名を取得
	std::string getSelectClusterQuery(double feature, double threshold, int limit); //featureの差がthreshold以下のクラスタを取得するクエリを取得
	std::string getRaisePriorityQuery(std::vector<Row> clusters); //指定したクラスタのpriorityを上げるクエリを取得
	std::string getAddClusterQuery(double feature, int label = -1); //クラスタを追加するクエリを取得
	//テーブルが存在しない場合はテーブルを作る
	void setup(); 
	//特徴量の差がthreshold以下のクラスタを取得
	std::vector<Cluster> search(Pattern pattern,double threshold, int limit = 1);
	//特徴量の差がthreshold以下のクラスタは優先度を上げる、threshold以上ならクラスタを追加する
	void commit(std::vector<Pattern> pattern_list,  double threshold, int num = 3);
	//データを空にする
	void truncate();
	//featureの最も小さい差を取得する
	double getSmallestDiffByFeature();
	//featureの最も大きい差を取得する
	double getLargestDiffByFeature();
	//パターンの特徴量を取得
	double getFeature(Pattern pattern);
};

#endif