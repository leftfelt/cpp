#include <ClusterRepository.h>

Cluster::Cluster(std::string label, double feature){
	this->label = label;
	this->feature = feature;
}

Cluster::Cluster(Row row){
	if(row.isEmptySet()){
		throw std::exception("[Cluster] invalid argument exception");
	}
	std::istringstream iss;
	iss.str(row["feature"]);
	iss >> this->feature;
	this->label = row["label"];
}

std::string Cluster::getLabel(){
	return this->label;
}
double Cluster::getFeature(){
	return this->feature;
}

//ClusterRepository=================================================================

ClusterRepository::ClusterRepository(std::string db_name, std::string data_type){
	this->data_type = data_type;
	this->sqlite.open(db_name);	
}

ClusterRepository::~ClusterRepository(){
	this->sqlite.close();
}

 //テーブル名を取得
std::string ClusterRepository::getTableName(){
	return this->data_type+"_cluster_repository";
}

//テーブルが存在しない場合はテーブルを作る indexがないなら作る
void ClusterRepository::setup(){
	this->sqlite.startTransaction();
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"label INTEGER DEFAULT 0,"
		"feature REAL,"
		"priority INTEGER DEFAULT 0"
		");"
	);
	this->sqlite.execute(
		"CREATE INDEX IF NOT EXISTS featureindex ON "+this->getTableName()+"(feature);"
	);
	this->sqlite.commitTransaction();
}

//featureの差がthreshold以下のクラスタを取得するクエリを取得
std::string ClusterRepository::getSelectClusterQuery(double feature, double threshold, int limit){
	std::ostringstream query;

	//しきい値以下のデータ一覧を取得
	query << 
		"SELECT id,label,feature,abs(feature-("<<feature<<")) AS diff, priority FROM "<<this->getTableName()<<" WHERE "
		"diff < "<<threshold <<" ORDER BY diff, priority DESC LIMIT " << limit;
	return query.str();
}

//指定したクラスタのpriorityを上げるクエリを取得
std::string ClusterRepository::getRaisePriorityQuery(std::vector<Row> clusters){
	std::ostringstream query;
	query << "UPDATE "<<this->getTableName()<<" SET priority = priority + 1 WHERE ";
		std::for_each(clusters.begin(), clusters.end()-1, [&](Row row){
			query << "id = " << row["id"] << " OR ";
		});
		query << "id = "<< clusters.back()["id"];
	return query.str();
}

//クラスタを追加するクエリを取得
std::string ClusterRepository::getAddClusterQuery(double feature, int label){
	std::ostringstream query;
	query << "INSERT INTO "<<this->getTableName()<<"(label,feature) VALUES(";
	if(label == -1){
		//新しくラベルを付ける
		query << "ifnull((SELECT max(rowid) FROM "<<this->getTableName()<<")+1,0)";
	}else{
		query << label;
	}
	query << ","<<feature<<")";
	return query.str();
}

//特徴量の差がthreshold以下のクラスタを取得
std::vector<Cluster> ClusterRepository::search(Pattern pattern, double threshold, int limit){
	double feature = this->getFeature(pattern);

	//しきい値以下のクラスタ
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getSelectClusterQuery(feature, threshold, limit));
	this->sqlite.commitTransaction();
	
	if(result.empty()) throw std::exception("[ClusterRepository] cluster not found");
	std::vector<Cluster> clusters;
	std::for_each(result.begin(), result.end(), [&](Row row){
		clusters.push_back(Cluster(row));
	});

	return clusters;
}

//特徴量の差がthreshold以下のクラスタは優先度を上げる、threshold以上ならクラスタを追加する
void ClusterRepository::commit(std::vector<Pattern> pattern_list, double threshold, int num){

	this->sqlite.startTransaction();
	std::for_each(pattern_list.begin(), pattern_list.end(), [&](Pattern pattern){
		try{
			double feature = this->getFeature(pattern);
	
			//しきい値以下のクラスタがあるか
			std::vector<Row> subthreshold_clusters = this->sqlite.execute(this->getSelectClusterQuery(feature,threshold, num));
	
			if(subthreshold_clusters.empty() || subthreshold_clusters[0].isEmptySet()){
				//なかったら新しく追加する
				this->sqlite.execute(this->getAddClusterQuery(feature));
			}else{
				int id;
				std::istringstream converter;
				converter.str(subthreshold_clusters[0]["label"]);
				converter >> id;
				//重要度を上げて、同じラベルで追加する
				this->sqlite.execute(this->getRaisePriorityQuery(subthreshold_clusters));
				this->sqlite.execute(this->getAddClusterQuery(feature,id));
			}
		}catch(std::exception e){
			//this->sqlite.rollbackTransaction();
			std::string message("[ClusterRepository] can't cluster repository commit\n");
			message += e.what();
			this->getFeature(pattern);
			//throw std::exception(message.c_str());
		}
	});
	this->sqlite.commitTransaction();
}

void ClusterRepository::truncate(){
	this->sqlite.startTransaction();
	this->sqlite.truncate(this->getTableName());
	this->sqlite.commitTransaction();
}


//featureの最も小さい差を取得する
double ClusterRepository::getSmallestDiffByFeature(){
	this->sqlite.startTransaction();
	std::vector<Row> min_features = this->sqlite.execute(
		"SELECT S1.feature - S2.feature AS diff FROM "+this->getTableName()+" S1, "+this->getTableName()+" S2 "
		"WHERE S2.feature = (SELECT MAX(feature) FROM "+this->getTableName()+" S3 WHERE S1.feature > S3.feature) "
		"ORDER BY diff ASC "
		"LIMIT 1"
	);
	this->sqlite.commitTransaction();
	
	if(min_features.empty()){
		throw std::exception("[ClusterRepository] smallest diff not found");
	}

	double diff;
	std::istringstream iss;
	iss.str(min_features[0]["diff"]);
	iss >> diff;
	
	return diff;
}

//featureの最も大きい差を取得する
double ClusterRepository::getLargestDiffByFeature(){
	this->sqlite.startTransaction();
	std::vector<Row> min_features = this->sqlite.execute(
		"SELECT S1.feature - S2.feature AS diff FROM "+this->getTableName()+" S1, "+this->getTableName()+" S2 "
		"WHERE S2.feature = (SELECT MAX(feature) FROM "+this->getTableName()+" S3 WHERE S1.feature > S3.feature) "
		"ORDER BY diff DESC "
		"LIMIT 1"
	);
	this->sqlite.commitTransaction();

	if(min_features.empty()){
		throw std::exception("[ClusterRepository] lagest diff not found");
	}

	double diff;
	std::istringstream iss;
	iss.str(min_features[0]["diff"]);
	iss >> diff;
	
	return diff;
}

//パターンの特徴量を取得
double ClusterRepository::getFeature(Pattern pattern){
	std::vector<double> origin(pattern.size());
	Converter<Pattern> converter;

	std::fill(origin.begin(), origin.end(), (double)1);
	double feature = Math::cosSimilarity(origin, converter.convert(pattern));
	return feature;
}