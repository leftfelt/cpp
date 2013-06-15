#include "RelationDataRepository.h"

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

const std::string RelationDataRepository::db_name = "knowledge.db";

RelationDataRepository::RelationDataRepository(){
	this->sqlite.open(RelationDataRepository::db_name);
}
RelationDataRepository::~RelationDataRepository(){
	this->sqlite.close();
}

std::string RelationDataRepository::getRelationDataQuery(int label){
	std::ostringstream query;

	query << "SELECT *,count(data) AS count FROM " << this->getTableName()
		<< " WHERE label = " << label
		<< " GROUP BY data ORDER BY count DESC";

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
	this->sqlite.execute(
		"CREATE TABLE IF NOT EXISTS "+this->getTableName()+" ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"label INTEGER DEFAULT 0,"
		"data TEXT DEFAULT NULL"
		");"
	);
}
RelationData RelationDataRepository::search(int label){

	std::vector<Row> result = this->sqlite.execute(this->getRelationDataQuery(label));
	
	if(result.size() == 0) throw std::exception("not found");

	return RelationData(result[0]);

}

void RelationDataRepository::commit(RelationData relation_data){
	std::vector<Row> result = this->sqlite.execute(this->setRelationDataQuery(
		relation_data.getLabel(),
		relation_data.getData()
	));
}

