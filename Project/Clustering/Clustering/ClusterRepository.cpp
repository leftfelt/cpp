#include "ClusterRepository.h"

const std::string ClusterRepository::cluster_repository_db_name = "knowledge.db";

ClusterRepository::ClusterRepository(std::string data_type){
	this->data_type = data_type;
	this->sqlite.open(ClusterRepository::cluster_repository_db_name);
	
}

ClusterRepository::~ClusterRepository(){
	this->sqlite.close();
}

std::string ClusterRepository::getTableName(){
	return this->data_type+"_cluster_repository";
}

void ClusterRepository::setup(){
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"label INTEGER DEFAULT 0,"
		"feature REAL,"
		"priority INTEGER DEFAULT 0"
		");"
	);
}

std::string ClusterRepository::getSelectClusterQuery(double feature, double threshold){
	std::ostringstream query;

	//しきい値以下のデータ一覧を取得
	query << 
		"SELECT *,sum(priority) FROM "<<this->getTableName()<<" WHERE "
		"abs(feature-("<<feature<<")) < "<<threshold<<" GROUP BY label ORDER BY sum(priority) DESC LIMIT 3;";
	return query.str();
}
std::string ClusterRepository::getRaisePriorityQuery(std::vector<Row> result){
	std::ostringstream query;
	query << "UPDATE "<<this->getTableName()<<" SET priority = priority + 1 WHERE ";
		std::for_each(result.begin(), result.end()-1, [&](Row row){
			query << "id = " << row["id"] << " OR ";
		});
		query << "id = "<< result.back()["id"] << ";";
	return query.str();
}
std::string ClusterRepository::getAddClusterQuery(double feature, int label){
	std::ostringstream query;
	query << "INSERT INTO "<<this->getTableName()<<"(label,feature) VALUES(";
	if(label == -1){
		query << "ifnull((SELECT max(rowid) FROM "<<this->getTableName()<<")+1,0)";
	}else{
		query << label;
	}
	query << ","<<feature<<");";
	return query.str();
}

Row ClusterRepository::search(Pattern pattern, double threshold){
	std::vector<double> origin(pattern.size());
	Converter<Pattern> converter;

	std::fill(origin.begin(), origin.end(), (double)1);
	double feature = Math::cosSimilarity(origin, converter.convert(pattern));

	//しきい値以下のクラスタ
	std::vector<Row> result = this->sqlite.execute(this->getSelectClusterQuery(feature, threshold));
	
	if(result.empty()) throw std::exception("not found");

	return result[0];
}

void ClusterRepository::commit(std::vector<Pattern> pattern_list, double threshold){
	Converter<Pattern> converter;

	std::for_each(pattern_list.begin(), pattern_list.end(), [&](Pattern pattern){
		try{
			std::vector<double> origin(pattern.size());
		
			std::fill(origin.begin(), origin.end(), (double)1);
			double feature = Math::cosSimilarity(origin, converter.convert(pattern));
	
			std::vector<Row> result;
	
			//しきい値以下のクラスタがあるか
			result = this->sqlite.execute(this->getSelectClusterQuery(feature,threshold));
	
			if(result.empty()){
				//なかったら新しく追加する
				this->sqlite.execute(this->getAddClusterQuery(feature));
			}else{
				int id;
				std::istringstream converter;
				converter.str(result[0]["label"]);
				converter >> id;
				//重要度を上げて、同じラベルで追加する
				this->sqlite.execute(this->getRaisePriorityQuery(result));
				this->sqlite.execute(this->getAddClusterQuery(feature,id));
			}
		}catch(std::exception e){
		}
	});
}