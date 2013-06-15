#include <RelationDataRepository.h>

RelationData::RelationData(){
	this->label = -1;
	this->data = "";
}

RelationData::RelationData(int label, std::string data){
	this->label = label;
	this->data = data;
}

RelationData::RelationData(Row row){
	std::istringstream converter;
	converter.str(row["label"]);
	converter >> this->label;
	this->data = row["data"];
}

int RelationData::getLabel(){
	return this->label;
}

std::string RelationData::getData(){
	return this->data;
}

//RelationDataRepository--------------------------------------------------------------------------------

RelationDataRepository::RelationDataRepository(std::string db_name){
	this->sqlite.open(db_name);
}
RelationDataRepository::~RelationDataRepository(){
	this->sqlite.close();
}

std::string RelationDataRepository::getRelationDataQuery(int label, int limit){
	std::ostringstream query;

	query << "SELECT *,count(data) AS count FROM " << this->getTableName()
		<< " WHERE label = " << label
		<< " GROUP BY data ORDER BY count DESC LIMIT " << limit;

	return query.str();
}
std::string RelationDataRepository::setRelationDataQuery(int label, std::string data){
	std::ostringstream query;

	query << "INSERT INTO " << this->getTableName() << "(label,data) VALUES(" << label << ",'" << data << "')";

	return query.str();
}

std::string RelationDataRepository::getTableName(){
	return "relation_data_repository";
}
void RelationDataRepository::setup(){
	this->sqlite.startTransaction();
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"label INTEGER DEFAULT 0,"
		"data TEXT DEFAULT NULL,"
		"UNIQUE(label, data)"
		");"
	);
	this->sqlite.commitTransaction();
}

std::vector<RelationData> RelationDataRepository::search(int label, int limit){
	this->sqlite.startTransaction();
	std::vector<Row> result = this->sqlite.execute(this->getRelationDataQuery(label, limit));
	this->sqlite.commitTransaction();

	if(result.empty()) {
		throw std::exception("relation data search not found");
	}

	std::vector<RelationData> relation_data_list(result.size());

	for(int i = 0 ; i < result.size() ; i++){
		relation_data_list[i] = RelationData(result[i]);
	}

	return relation_data_list;
}

void RelationDataRepository::commit(RelationData relation_data){
	this->sqlite.startTransaction();
	try{
		std::vector<Row> result = this->sqlite.execute(this->setRelationDataQuery(
			relation_data.getLabel(),
			relation_data.getData()
		));
	}catch(std::exception e){
		//nice catch!!
	}
	this->sqlite.commitTransaction();
}

void RelationDataRepository::truncate(){
	this->sqlite.startTransaction();
	this->sqlite.truncate(this->getTableName());
	this->sqlite.commitTransaction();
}