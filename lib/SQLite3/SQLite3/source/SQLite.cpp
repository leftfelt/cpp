#include <SQLite.h>

bool Row::isEmptySet(){
	std::map<std::string, std::string>::iterator itr;
	for(itr=this->begin(); itr!=this->end(); itr++){
		if(itr->second != "NULL") return false;
	}
	return true;
}

//================================================================================================

std::vector<Row> SQLite::result;

int SQLite::callback(void *arg ,int argc, char **argv, char **column){
	Row row;
	for(int i = 0 ; i < argc ; i++) {
		row[column[i]] = (argv[i] != NULL) ? argv[i] : "NULL";
	}
	SQLite::result.push_back(row);
	return SQLITE_OK;
}

SQLite::SQLite(){
	this->bindPos=1;
	this->statement = NULL;
	this->sqlite = NULL;
}

SQLite::~SQLite(){
}

void SQLite::open(std::string db_name){
	int status;
	status = sqlite3_open_v2(db_name.c_str(),&this->sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
	if(status != SQLITE_OK) throw std::exception(std::string("sqlite3 open failed '"+db_name+"'").c_str());
}


std::vector<Row> SQLite::execute(std::string query){
	int status;
	char *errmsg;
	//Œ‹‰Ê‚ðƒŠƒZƒbƒg‚·‚é
	SQLite::result.clear();
	status = sqlite3_exec(this->sqlite,query.c_str(),callback,NULL,&errmsg);
	if(status != SQLITE_OK) throw std::exception(std::string(query + "\n" + errmsg).c_str());
	return SQLite::result;
}

void SQLite::close(){
	int status;
	if(this->statement != NULL) sqlite3_finalize(this->statement);
	status = sqlite3_close_v2(this->sqlite);
	if(status != SQLITE_OK) throw std::exception(std::string("sqlite3 close failed").c_str());
}

void SQLite::startTransaction(){
	this->execute("BEGIN;");
}

void SQLite::commitTransaction(){
	this->execute("COMMIT;");
}

void SQLite::rollbackTransaction(){
	this->execute("ROLLBACK;");
}

void SQLite::truncate(std::string table_name){
	this->execute("DELETE FROM "+table_name);
	this->execute("UPDATE SQLITE_SEQUENCE SET SEQ=0 WHERE NAME = '"+table_name+"'");
}

void SQLite::prepare(std::string query){
	int status;
	char *errmsg;
	status = sqlite3_prepare_v2(this->sqlite, query.c_str(), query.size()+1, &this->statement, NULL);
	if(status != SQLITE_OK) throw std::exception(std::string(query + "\n" + errmsg).c_str());
}